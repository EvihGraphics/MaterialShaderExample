param (
    [string]$UERoot = "D:\UE\UnrealEngine_Animation_Tech",
    [string]$Reference = "All",
    [switch]$Clean,
    [switch]$SkipProjectGeneration,
    [switch]$KeepStaging
)

$ErrorActionPreference = "Stop"
$RepoRoot = (git rev-parse --show-toplevel).Trim()
$StagingRoot = Join-Path $RepoRoot "Saved/UERP_RefBuild/Staging"
$HarnessRoot = Join-Path $RepoRoot "Saved/UERP_RefBuild/Harness"
$EvidenceDir = Join-Path $RepoRoot "LocalVisualResults/UE57/HIVE_4090x2/UERP_Ref"

if ($Clean) {
    if (Test-Path $StagingRoot) { Remove-Item -Recurse -Force $StagingRoot }
    if (Test-Path $HarnessRoot) { Remove-Item -Recurse -Force $HarnessRoot }
}

$BuildBat = "$UERoot\Engine\Build\BatchFiles\Build.bat"
if (-not (Test-Path $BuildBat)) {
    Write-Error "UE Build script not found at $BuildBat"
}

$Timestamp = (Get-Date).ToUniversalTime().ToString("yyyyMMddTHHmmssZ")
$RefsToBuild = @()

$ManifestPath = Join-Path $RepoRoot "UERP_Ref/REFERENCE_MANIFEST.json"
$Manifest = Get-Content $ManifestPath | ConvertFrom-Json

if ($Reference -eq "All") {
    $RefsToBuild = $Manifest
} else {
    $RefsToBuild = $Manifest | Where-Object { $_.Name -eq $Reference }
}

foreach ($Ref in $RefsToBuild) {
    Write-Host "Building reference: $($Ref.Name)" -ForegroundColor Cyan
    $RefEvidence = Join-Path $EvidenceDir "$($Ref.Name)/$Timestamp"
    New-Item -ItemType Directory -Force -Path $RefEvidence | Out-Null
    
    $RefSourceDir = Join-Path $RepoRoot "UERP_Ref/$($Ref.Name)"
    $LogFile = Join-Path $RefEvidence "BuildOutput.log"
    $ExitCode = 0
    
    # 1. Gather info
    $SelectedCommit = (git -C $RefSourceDir rev-parse HEAD)
    $SelectedCommit | Out-File (Join-Path $RefEvidence "SelectedCommit.json")
    
    @{ UERoot=$UERoot; Time=$Timestamp; Machine=$env:COMPUTERNAME } | ConvertTo-Json | Out-File (Join-Path $RefEvidence "Environment.json")
    
    if ($Ref.RepositoryType -eq "Project") {
        # Build Project
        $ProjStaging = Join-Path $StagingRoot $Ref.Name
        New-Item -ItemType Directory -Force -Path $ProjStaging | Out-Null
        Copy-Item -Recurse -Force "$RefSourceDir\*" $ProjStaging
        
        # Apply patches if exist
        $PatchDir = Join-Path $RepoRoot "UERP_Ref/Patches/$($Ref.Name)"
        if (Test-Path $PatchDir) {
            Copy-Item -Recurse -Force "$PatchDir\*" $ProjStaging
            "Applied patches from $PatchDir" | Out-File (Join-Path $RefEvidence "CompatibilityChanges.md") -Append
        } else {
            "No patches applied." | Out-File (Join-Path $RefEvidence "CompatibilityChanges.md")
        }
        
        $UProjectFile = Get-ChildItem -Path $ProjStaging -Filter "*.uproject" | Select-Object -First 1
        if ($UProjectFile) {
            $ProjectName = $UProjectFile.BaseName
            $CmdArgs = "$ProjectName`Editor", "Win64", "Development", "-Project=`"$($UProjectFile.FullName)`"", "-WaitMutex", "-NoHotReloadFromIDE"
            $CmdArgs -join " " | Out-File (Join-Path $RefEvidence "BuildCommand.txt")
            
            Write-Host "Running: $BuildBat $CmdArgs"
            $process = Start-Process -FilePath $BuildBat -ArgumentList $CmdArgs -Wait -NoNewWindow -PassThru -RedirectStandardOutput $LogFile -RedirectStandardError $LogFile
            $ExitCode = $process.ExitCode
        } else {
            Write-Error "No uproject found in $ProjStaging"
            $ExitCode = -1
        }
    } elseif ($Ref.RepositoryType -eq "Plugin") {
        # Build Plugin via dummy project harness
        $ProjHarness = Join-Path $HarnessRoot $Ref.Name
        New-Item -ItemType Directory -Force -Path $ProjHarness | Out-Null
        
        # Create minimal uproject
        $UProjectContent = @{
            FileVersion = 3
            EngineAssociation = "5.7"
            Category = ""
            Description = ""
            Plugins = @(
                @{ Name = $Ref.Name; Enabled = $true }
            )
        }
        $UProjectContent | ConvertTo-Json | Out-File (Join-Path $ProjHarness "Harness.uproject")
        
        $PluginDir = Join-Path $ProjHarness "Plugins/$($Ref.Name)"
        New-Item -ItemType Directory -Force -Path $PluginDir | Out-Null
        Copy-Item -Recurse -Force "$RefSourceDir\*" $PluginDir
        
        # Apply patches
        $PatchDir = Join-Path $RepoRoot "UERP_Ref/Patches/$($Ref.Name)"
        if (Test-Path $PatchDir) {
            Copy-Item -Recurse -Force "$PatchDir\*" $PluginDir
            "Applied patches from $PatchDir" | Out-File (Join-Path $RefEvidence "CompatibilityChanges.md") -Append
        } else {
            "No patches applied." | Out-File (Join-Path $RefEvidence "CompatibilityChanges.md")
        }
        
        $UProjectFile = Join-Path $ProjHarness "Harness.uproject"
        $CmdArgs = "HarnessEditor", "Win64", "Development", "-Project=`"$UProjectFile`"", "-WaitMutex", "-NoHotReloadFromIDE"
        $CmdArgs -join " " | Out-File (Join-Path $RefEvidence "BuildCommand.txt")
        
        Write-Host "Running: $BuildBat $CmdArgs"
        $process = Start-Process -FilePath $BuildBat -ArgumentList $CmdArgs -Wait -NoNewWindow -PassThru -RedirectStandardOutput $LogFile
        $ExitCode = $process.ExitCode
    }
    
    # Save result
    $BuildResult = @{ ExitCode = $ExitCode; Passed = ($ExitCode -eq 0) }
    $BuildResult | ConvertTo-Json | Out-File (Join-Path $RefEvidence "BuildResult.json")
    
    # Simple log scan
    $LogContent = Get-Content $LogFile -ErrorAction SilentlyContinue
    $Errors = $LogContent | Where-Object { $_ -match "error" -and $_ -notmatch "0 error" }
    $Warnings = $LogContent | Where-Object { $_ -match "warning" -and $_ -notmatch "0 warning" }
    @{ ErrorCount = @($Errors).Count; WarningCount = @($Warnings).Count; Snippets = ($Errors | Select-Object -First 5) } | ConvertTo-Json | Out-File (Join-Path $RefEvidence "LogScan.json")
    
    if ($ExitCode -eq 0) {
        Write-Host "$($Ref.Name) built successfully." -ForegroundColor Green
    } else {
        Write-Host "$($Ref.Name) build FAILED (Code $ExitCode)." -ForegroundColor Red
    }
}
