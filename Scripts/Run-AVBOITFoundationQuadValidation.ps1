param (
    [string]$UERoot = "D:\UE\UnrealEngine_Animation_Tech",
    [string]$ProjectPath = "D:\Users\l3d\Documents\Unreal Projects\ContentExamples\ContentExamples.uproject",
    [string]$RepositoryRoot = "",
    [string]$TheForgeRoot = "",
    [int]$ResolutionX = 1920,
    [int]$ResolutionY = 1080,
    [ValidateSet("SingleLayerIdentity", "TwoIntersectingQuads", "ThreeLayerStress")]
    [string]$Scene = "TwoIntersectingQuads",
    [int]$DownsampleFactor = 8,
    [int]$NumSlices = 64,
    [switch]$SkipBuild,
    [switch]$RunNativeOIT,
    [switch]$RunRandomOrders,
    [ValidateSet("None", "RenderDoc", "PIX")]
    [string]$CaptureTool = "None",
    [int]$TimeoutSeconds = 900
)

$ErrorActionPreference = "Stop"

function Resolve-FullPath([string]$Path) {
    $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($Path)
}

function Invoke-LoggedProcess {
    param(
        [string]$FilePath,
        [string[]]$ArgumentList,
        [string]$StdOutPath,
        [string]$StdErrPath,
        [int]$TimeoutSeconds = 900
    )

    $process = Start-Process -FilePath $FilePath -ArgumentList $ArgumentList -NoNewWindow -PassThru -RedirectStandardOutput $StdOutPath -RedirectStandardError $StdErrPath
    $process | Wait-Process -Timeout $TimeoutSeconds -ErrorAction SilentlyContinue
    if (-not $process.HasExited) {
        taskkill /PID $process.Id /T /F | Out-Null
        return 124
    }
    return $process.ExitCode
}

function Save-Json($Path, $Object) {
    $dir = Split-Path -Parent $Path
    if (-not (Test-Path $dir)) {
        New-Item -ItemType Directory -Force -Path $dir | Out-Null
    }
    $Object | ConvertTo-Json -Depth 32 | Set-Content -Path $Path -Encoding UTF8
}

if ([string]::IsNullOrWhiteSpace($RepositoryRoot)) {
    $RepositoryRoot = Resolve-FullPath (Join-Path $PSScriptRoot "..")
} else {
    $RepositoryRoot = Resolve-FullPath $RepositoryRoot
}

if ([string]::IsNullOrWhiteSpace($TheForgeRoot)) {
    $TheForgeRoot = Resolve-FullPath (Join-Path $RepositoryRoot "..\The-Forge")
} elseif (Test-Path $TheForgeRoot) {
    $TheForgeRoot = Resolve-FullPath $TheForgeRoot
}

$Timestamp = (Get-Date).ToUniversalTime().ToString("yyyyMMddTHHmmssZ")
$MachineId = "HIVE_4090x2"
$StageName = "UE4-2G-AVBOIT-Quad-Foundation"
$TempRoot = Join-Path $RepositoryRoot "LocalVisualResults\TempResults\UE57\$MachineId\$StageName\$Timestamp"
$KeyRoot = Join-Path $RepositoryRoot "LocalVisualResults\KeyResults\UE57\$MachineId\$StageName\$Timestamp"
$Dirs = @("Raw", "Derived", "Metrics", "Capture", "Logs", "Readback", "Preflight")
foreach ($d in $Dirs) {
    New-Item -ItemType Directory -Force -Path (Join-Path $TempRoot $d) | Out-Null
}

$BuildVersionPath = Join-Path $UERoot "Engine\Build\Build.version"
$BuildVersion = $null
if (Test-Path $BuildVersionPath) {
    $BuildVersion = Get-Content -Raw $BuildVersionPath | ConvertFrom-Json
}

$GitState = [ordered]@{
    GeneratedUtc = (Get-Date).ToUniversalTime().ToString("o")
    RepositoryRoot = $RepositoryRoot
    ProjectPath = $ProjectPath
    UERoot = $UERoot
    MaterialShaderExampleHead = (& git -C $RepositoryRoot rev-parse HEAD 2>$null).Trim()
    MaterialShaderExampleRemoteHead = (& git -C $RepositoryRoot rev-parse origin/AVBOIT开发 2>$null).Trim()
    MaterialShaderExampleStatus = @(& git -C $RepositoryRoot status --short)
    UEHead = (& git -C $UERoot rev-parse HEAD 2>$null).Trim()
    UEStatusShort = @(& git -C $UERoot status --short 2>$null)
    TheForgeRoot = $TheForgeRoot
    TheForgeHead = if (Test-Path $TheForgeRoot) { (& git -C $TheForgeRoot rev-parse HEAD 2>$null).Trim() } else { "" }
    TheForgeStatusShort = if (Test-Path $TheForgeRoot) { @(& git -C $TheForgeRoot status --short --untracked-files=no 2>$null) } else { @("missing") }
    UEBuildVersion = $BuildVersion
    Requested = [ordered]@{
        ResolutionX = $ResolutionX
        ResolutionY = $ResolutionY
        Scene = $Scene
        DownsampleFactor = $DownsampleFactor
        NumSlices = $NumSlices
        RunNativeOIT = [bool]$RunNativeOIT
        RunRandomOrders = [bool]$RunRandomOrders
        CaptureTool = $CaptureTool
    }
}
Save-Json (Join-Path $TempRoot "Preflight\GitState.json") $GitState

$ExitCode = 0
$BlockingReasons = New-Object System.Collections.Generic.List[string]
$CommandRecords = New-Object System.Collections.Generic.List[object]

try {
    if ($ProjectPath -like "*ContentExamples.uproject") {
        $DeployScript = Join-Path $RepositoryRoot "Scripts\Deploy-AVBOITPluginToContentExamples.ps1"
        if (Test-Path $DeployScript) {
            & powershell -NoProfile -ExecutionPolicy Bypass -File $DeployScript -SourceRepoRoot $RepositoryRoot -HostProjectRoot (Split-Path -Parent $ProjectPath) -Mode Copy
            $CommandRecords.Add([ordered]@{ Name = "DeployPlugins"; ExitCode = $LASTEXITCODE })
        }
    }

    $ProjectName = [IO.Path]::GetFileNameWithoutExtension($ProjectPath)
    $EditorTarget = "$($ProjectName)Editor"
    if (-not $SkipBuild) {
        $BuildBat = Join-Path $UERoot "Engine\Build\BatchFiles\Build.bat"
        $BuildArgs = @($EditorTarget, "Win64", "Development", "-Project=`"$ProjectPath`"", "-WaitMutex", "-NoHotReloadFromIDE")
        $BuildLog = Join-Path $TempRoot "Logs\Build.log"
        $BuildErr = Join-Path $TempRoot "Logs\Build.err.log"
        $BuildExit = Invoke-LoggedProcess -FilePath $BuildBat -ArgumentList $BuildArgs -StdOutPath $BuildLog -StdErrPath $BuildErr -TimeoutSeconds $TimeoutSeconds
        $CommandRecords.Add([ordered]@{ Name = "Build"; File = $BuildBat; Args = $BuildArgs; ExitCode = $BuildExit; Log = $BuildLog; ErrorLog = $BuildErr })
        if ($BuildExit -ne 0) {
            $BlockingReasons.Add("Build failed with exit code $BuildExit")
            $ExitCode = $BuildExit
            throw "Build failed"
        }
    }

    $RunId = [guid]::NewGuid().ToString()
    $EditorCmd = Join-Path $UERoot "Engine\Binaries\Win64\UnrealEditor-Cmd.exe"
    $TracePath = Join-Path $TempRoot "Capture\FoundationQuad.utrace"
    $CommandletLog = Join-Path $TempRoot "Logs\FoundationQuadCommandlet.log"
    $CommandletErr = Join-Path $TempRoot "Logs\FoundationQuadCommandlet.err.log"
    $CommandletArgs = @(
        "`"$ProjectPath`"",
        "-run=AVBOITHeadless",
        "-AllowCommandletRendering",
        "-RenderOffscreen",
        "-dx12",
        "-unattended",
        "-nop4",
        "-nosplash",
        "-NoSound",
        "-NoLiveCoding",
        "-AVBOITSuite=FoundationQuad",
        "-AVBOITRunId=`"$RunId`"",
        "-AVBOITEvidenceRoot=`"$TempRoot`"",
        "-ExecCmds=`"r.AVBOIT.Foundation.Enable 1,r.AVBOIT.Foundation.Mode 3,r.AVBOIT.Foundation.Scene 1,r.AVBOIT.Foundation.DownsampleFactor $DownsampleFactor,r.AVBOIT.Foundation.NumSlices $NumSlices`"",
        "-trace=default,rdg,gpu",
        "-tracefile=`"$TracePath`"",
        "-abslog=`"$CommandletLog`"",
        "-resx=$ResolutionX",
        "-resy=$ResolutionY"
    )

    $CommandletExit = Invoke-LoggedProcess -FilePath $EditorCmd -ArgumentList $CommandletArgs -StdOutPath $CommandletLog -StdErrPath $CommandletErr -TimeoutSeconds $TimeoutSeconds
    $CommandRecords.Add([ordered]@{ Name = "FoundationCommandlet"; File = $EditorCmd; Args = $CommandletArgs; ExitCode = $CommandletExit; Log = $CommandletLog; ErrorLog = $CommandletErr; Trace = $TracePath })
    if ($CommandletExit -ne 0) {
        $BlockingReasons.Add("Foundation commandlet failed with exit code $CommandletExit")
        $ExitCode = $CommandletExit
    }

    $RequiredImages = @(
        "00_ExactReference_AB.png", "01_EngineDefault_AB.png", "02_EngineDefault_BA.png",
        "03_UESortedPixelsOIT_AB.png", "04_UESortedPixelsOIT_BA.png",
        "05_PluginIdentity_AB.png", "06_PluginIdentity_BA.png",
        "07_PluginAVBOIT_AB.png", "08_PluginAVBOIT_BA.png",
        "09_PluginAVBOIT_RandomSeed1.png", "10_PluginAVBOIT_RandomSeed2.png", "11_PluginAVBOIT_RandomSeed3.png",
        "12_ExtinctionOverview.png", "13_TransmittanceOverview.png", "14_ColorAccumulationOverview.png",
        "15_AlphaAccumulationOverview.png", "16_SliceOverview.png",
        "17_AVBOIT_vs_Exact_Difference.png", "18_AVBOIT_vs_Exact_Heatmap.png",
        "19_OrderAB_vs_BA_Difference.png", "20_SideBySide.png", "21_AnnotatedGoal.png"
    )
    $MissingImages = @($RequiredImages | Where-Object { -not (Test-Path (Join-Path $TempRoot "Raw\$_")) -and -not (Test-Path (Join-Path $TempRoot "Derived\$_")) })
    if ($MissingImages.Count -gt 0) {
        $BlockingReasons.Add("Foundation screenshot matrix is incomplete: $($MissingImages.Count) files missing")
    }

    if ($CaptureTool -eq "None") {
        $BlockingReasons.Add("GPU capture was not requested")
    } else {
        $CaptureFiles = @(Get-ChildItem -Path (Join-Path $TempRoot "Capture") -File -ErrorAction SilentlyContinue | Where-Object { $_.Extension -match "\.(rdc|wpix|pix3)$" })
        if ($CaptureFiles.Count -eq 0) {
            $BlockingReasons.Add("$CaptureTool capture file was not produced")
        }
    }

    $CombinedLogs = Get-ChildItem -Path (Join-Path $TempRoot "Logs") -File -ErrorAction SilentlyContinue | ForEach-Object { Get-Content -Raw $_.FullName -ErrorAction SilentlyContinue }
    $CriticalLogPattern = "Fatal error|Assertion failed|Ensure condition failed|LogRHI: Error|LogD3D12RHI: Error|LogRenderer: Error|LogRenderGraph: Error|RDG validation|UAV overlap|GPU crash|Device removed|Access violation|Shader Binding Error"
    if (($CombinedLogs -join "`n") -match $CriticalLogPattern) {
        $BlockingReasons.Add("Critical render/build error pattern found in logs")
    }
}
catch {
    if ($ExitCode -eq 0) {
        $ExitCode = 1
    }
    $BlockingReasons.Add($_.Exception.Message)
}
finally {
    $Status = if ($BlockingReasons.Count -eq 0 -and $ExitCode -eq 0) { "passed-local" } else { "partial" }
    $KeyResultsValue = ""
    if ($Status -eq "passed-local") {
        $KeyResultsValue = $KeyRoot
    }
    $CommandArray = @()
    foreach ($record in $CommandRecords) {
        $CommandArray += $record
    }
    $BlockingArray = @()
    foreach ($reason in $BlockingReasons) {
        $BlockingArray += [string]$reason
    }
    $PromotionDecision = @{
        SchemaVersion = 1
        Stage = "UE-4.2G AVBOIT Quad Foundation"
        GeneratedUtc = (Get-Date).ToUniversalTime().ToString("o")
        Status = $Status
        PromotionEligible = ($Status -eq "passed-local")
        TempResultsPath = $TempRoot
        KeyResultsPath = $KeyResultsValue
        Commands = $CommandArray
        BlockingReasons = $BlockingArray
    }
    Save-Json (Join-Path $TempRoot "Metrics\PromotionDecision.json") $PromotionDecision

    if ($Status -eq "passed-local") {
        New-Item -ItemType Directory -Force -Path $KeyRoot | Out-Null
        Copy-Item -Path (Join-Path $TempRoot "*") -Destination $KeyRoot -Recurse -Force
        (Get-Date).ToUniversalTime().ToString("o") | Set-Content -Path (Join-Path (Split-Path -Parent $KeyRoot) "LATEST.txt") -Encoding UTF8
    } elseif ($ExitCode -eq 0) {
        $ExitCode = 5
    }

    Save-Json (Join-Path $TempRoot "Metrics\Acceptance.json") ([ordered]@{
        ExitCode = $ExitCode
        Status = $Status
        PromotionEligible = ($Status -eq "passed-local")
        BlockingReasons = $BlockingArray
    })
    Write-Host "AVBOIT Foundation Quad validation status: $Status"
    Write-Host "TempResults: $TempRoot"
    exit $ExitCode
}
