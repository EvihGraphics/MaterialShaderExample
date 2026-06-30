[CmdletBinding()]
param(
    [string]$SourceRepoRoot = "",
    [string]$HostProjectRoot = (Join-Path $env:USERPROFILE "Documents\Unreal Projects\ContentExamples"),
    [ValidateSet("Copy", "Junction", "Remove", "Validate")]
    [string]$Mode = "Copy",
    [switch]$Clean
)

$ErrorActionPreference = "Stop"

function Resolve-FullPath([string]$Path) {
    $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($Path)
}

function Assert-InDirectory([string]$Candidate, [string]$Root) {
    $candidateFull = Resolve-FullPath $Candidate
    $rootFull = Resolve-FullPath $Root
    if (-not $candidateFull.StartsWith($rootFull, [System.StringComparison]::OrdinalIgnoreCase)) {
        throw "Refusing to operate outside expected root. Candidate='$candidateFull' Root='$rootFull'"
    }
}

function Update-ProjectPluginState([string]$ProjectFile) {
    if (-not (Test-Path $ProjectFile)) {
        throw "ContentExamples project file not found: $ProjectFile"
    }

    $json = Get-Content -Raw $ProjectFile | ConvertFrom-Json
    if (-not $json.PSObject.Properties.Name.Contains("Plugins") -or $null -eq $json.Plugins) {
        $json | Add-Member -MemberType NoteProperty -Name Plugins -Value @()
    }

    $required = @("MaterialShaderExample", "MaterialShaderExampleNiagara")
    foreach ($pluginName in $required) {
        $entry = $json.Plugins | Where-Object { $_.Name -eq $pluginName } | Select-Object -First 1
        if ($entry) {
            $entry.Enabled = $true
        } else {
            $json.Plugins += [pscustomobject]@{
                Name = $pluginName
                Enabled = $true
            }
        }
    }

    $json | ConvertTo-Json -Depth 32 | Set-Content -Path $ProjectFile -Encoding UTF8
}

function Copy-Plugin([string]$Source, [string]$Destination) {
    if (-not (Test-Path $Source)) {
        throw "Source plugin missing: $Source"
    }

    if ($Clean -and (Test-Path $Destination)) {
        Assert-InDirectory $Destination (Join-Path $HostProjectRoot "Plugins")
        Remove-Item -LiteralPath $Destination -Recurse -Force
    }

    New-Item -ItemType Directory -Force -Path $Destination | Out-Null
    $excludedDirs = @("Binaries", "Intermediate", "Saved", "DerivedDataCache", "LocalVisualResults", ".git")
    $excludedFiles = @("*.sln", "*.opensdf", "*.sdf", "*.user", "*.VC.db")
    $args = @(
        "`"$Source`"",
        "`"$Destination`"",
        "/MIR",
        "/XD"
    ) + ($excludedDirs | ForEach-Object { "`"$_`"" }) + @(
        "/XF"
    ) + ($excludedFiles | ForEach-Object { "`"$_`"" }) + @(
        "/R:2",
        "/W:1",
        "/NFL",
        "/NDL",
        "/NP"
    )

    $process = Start-Process -FilePath "robocopy.exe" -ArgumentList $args -NoNewWindow -Wait -PassThru
    if ($process.ExitCode -ge 8) {
        throw "robocopy failed from '$Source' to '$Destination' with exit code $($process.ExitCode)"
    }
}

function New-PluginJunction([string]$Source, [string]$Destination) {
    if (-not (Test-Path $Source)) {
        throw "Source plugin missing: $Source"
    }
    if (Test-Path $Destination) {
        Assert-InDirectory $Destination (Join-Path $HostProjectRoot "Plugins")
        Remove-Item -LiteralPath $Destination -Recurse -Force
    }
    New-Item -ItemType Junction -Path $Destination -Target $Source | Out-Null
}

if ([string]::IsNullOrWhiteSpace($SourceRepoRoot)) {
    $SourceRepoRoot = Join-Path $PSScriptRoot ".."
}

$SourceRepoRoot = Resolve-FullPath $SourceRepoRoot
$HostProjectRoot = Resolve-FullPath $HostProjectRoot
$hostPluginsRoot = Join-Path $HostProjectRoot "Plugins"
$manifestRoot = Join-Path $HostProjectRoot "Saved\AVBOITValidation"
$manifestPath = Join-Path $manifestRoot "DeployManifest.json"

$pluginPairs = @(
    [pscustomobject]@{
        Name = "MaterialShaderExample"
        Source = Join-Path $SourceRepoRoot "Plugins\MaterialShaderPlugin"
        Destination = Join-Path $hostPluginsRoot "MaterialShaderPlugin"
    },
    [pscustomobject]@{
        Name = "MaterialShaderExampleNiagara"
        Source = Join-Path $SourceRepoRoot "Plugins\MaterialShaderPluginNiagara"
        Destination = Join-Path $hostPluginsRoot "MaterialShaderPluginNiagara"
    }
)

New-Item -ItemType Directory -Force -Path $hostPluginsRoot | Out-Null

switch ($Mode) {
    "Copy" {
        foreach ($pair in $pluginPairs) {
            Copy-Plugin -Source $pair.Source -Destination $pair.Destination
        }
        Update-ProjectPluginState -ProjectFile (Join-Path $HostProjectRoot "ContentExamples.uproject")
    }
    "Junction" {
        foreach ($pair in $pluginPairs) {
            New-PluginJunction -Source $pair.Source -Destination $pair.Destination
        }
        Update-ProjectPluginState -ProjectFile (Join-Path $HostProjectRoot "ContentExamples.uproject")
    }
    "Remove" {
        foreach ($pair in $pluginPairs) {
            if (Test-Path $pair.Destination) {
                Assert-InDirectory $pair.Destination $hostPluginsRoot
                Remove-Item -LiteralPath $pair.Destination -Recurse -Force
            }
        }
    }
    "Validate" {
        foreach ($pair in $pluginPairs) {
            if (-not (Test-Path $pair.Destination)) {
                throw "Missing deployed plugin: $($pair.Destination)"
            }
        }
    }
}

New-Item -ItemType Directory -Force -Path $manifestRoot | Out-Null
$manifest = [pscustomobject]@{
    GeneratedUtc = (Get-Date).ToUniversalTime().ToString("o")
    Mode = $Mode
    SourceRepoRoot = $SourceRepoRoot
    HostProjectRoot = $HostProjectRoot
    Plugins = $pluginPairs | ForEach-Object {
        [pscustomobject]@{
            Name = $_.Name
            Source = $_.Source
            Destination = $_.Destination
            Deployed = (Test-Path $_.Destination)
        }
    }
}
$manifest | ConvertTo-Json -Depth 8 | Set-Content -Path $manifestPath -Encoding UTF8
Write-Host "AVBOIT deploy manifest: $manifestPath"
