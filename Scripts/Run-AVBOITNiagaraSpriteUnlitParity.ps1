[CmdletBinding()]
param(
    [string]$UERoot = "D:\UE\UnrealEngine_Animation_Tech",
    [string]$MaterialShaderRepo = "",
    [string]$ContentExamplesProject = (Join-Path $env:USERPROFILE "Documents\Unreal Projects\ContentExamples\ContentExamples.uproject"),
    [string]$Map = "/Game/NiagaraExamples/Utilities/SpriteGeneration/TestSpriteMap1",
    [int]$Width = 1920,
    [int]$Height = 1080,
    [int]$ResolutionX = 0,
    [int]$ResolutionY = 0,
    [string]$TheForgeRoot = "",
    [double[]]$CaptureTimes = @(0.5, 1.0, 2.0, 4.0),
    [switch]$Clean,
    [switch]$SkipBuild,
    [switch]$Interactive,
    [switch]$RunNativeOITStudy,
    [switch]$RunCoreBufferBringup,
    [switch]$RunCoreRefactorValidation,
    [switch]$RunIdentityBringup,
    [switch]$RunCoreQuad,
    [switch]$RunSingleNiagaraSprite,
    [switch]$RunTestSpriteMap1,
    [switch]$RunTintMatrix,
    [switch]$CompareUESortedPixelsOIT,
    [switch]$CaptureBufferOverview,
    [switch]$BufferOverview,
    [switch]$DumpAllBuffers,
    [switch]$CaptureGPU,
    [switch]$GPUCapture,
    [switch]$RequireRealVertexFactory,
    [switch]$RequireRealMaterial,
    [switch]$RequireParticleAttributeHash,
    [switch]$RequireIdentityParity,
    [switch]$RequireRealAVBOITDraw,
    [switch]$RequireNonZeroBuffers,
    [switch]$RequireSceneColorComposite,
    [switch]$RequireComparisonImages,
    [switch]$PromoteMilestone,
    [string]$MilestoneName = "",
    [Alias("TempResultsRoot")]
    [string]$TempResultsRootOverride = "",
    [Alias("KeyResultsRoot")]
    [string]$KeyResultsRootOverride = "",
    [switch]$SkipInteractiveOverlay,
    [switch]$NoKeyResultsPromotion,
    [int]$TimeoutSeconds = 900
)

$ErrorActionPreference = "Stop"

function Resolve-FullPath([string]$Path) {
    $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($Path)
}

function Quote-ProcessArgument([string]$Value) {
    if ($Value -match '[\s;"]') {
        return '"' + ($Value -replace '"', '\"') + '"'
    }
    return $Value
}

function Compare-PngPixels([string]$ReferencePath, [string]$CandidatePath) {
    Add-Type -AssemblyName System.Drawing

    $reference = [System.Drawing.Bitmap]::new($ReferencePath)
    $candidate = [System.Drawing.Bitmap]::new($CandidatePath)
    try {
        if ($reference.Width -ne $candidate.Width -or $reference.Height -ne $candidate.Height) {
            return [pscustomobject]@{
                Reference = $ReferencePath
                Candidate = $CandidatePath
                PixelIdentical = $false
                Width = $reference.Width
                Height = $reference.Height
                CandidateWidth = $candidate.Width
                CandidateHeight = $candidate.Height
                MAE = 1.0
                MaxError = 1.0
                DifferingPixelCount = -1
                Reason = "Image dimensions differ."
            }
        }

        [double]$absoluteError = 0.0
        [int]$maxChannelError = 0
        [int64]$differingPixels = 0
        for ($y = 0; $y -lt $reference.Height; $y++) {
            for ($x = 0; $x -lt $reference.Width; $x++) {
                $a = $reference.GetPixel($x, $y)
                $b = $candidate.GetPixel($x, $y)
                $dr = [Math]::Abs([int]$a.R - [int]$b.R)
                $dg = [Math]::Abs([int]$a.G - [int]$b.G)
                $db = [Math]::Abs([int]$a.B - [int]$b.B)
                if ($dr -ne 0 -or $dg -ne 0 -or $db -ne 0) {
                    $differingPixels++
                }
                $absoluteError += $dr + $dg + $db
                $maxChannelError = [Math]::Max($maxChannelError, [Math]::Max($dr, [Math]::Max($dg, $db)))
            }
        }

        $channelCount = [double]($reference.Width * $reference.Height * 3)
        $mae = $absoluteError / ($channelCount * 255.0)
        return [pscustomobject]@{
            Reference = $ReferencePath
            Candidate = $CandidatePath
            PixelIdentical = ($differingPixels -eq 0)
            Width = $reference.Width
            Height = $reference.Height
            MAE = $mae
            MaxError = ($maxChannelError / 255.0)
            DifferingPixelCount = $differingPixels
            Reason = if ($differingPixels -eq 0) { "Pixel-identical." } else { "Images differ; UE-4.2C remains partial until material/pixel equivalence work is complete." }
        }
    }
    finally {
        $reference.Dispose()
        $candidate.Dispose()
    }
}

if ([string]::IsNullOrWhiteSpace($MaterialShaderRepo)) {
    $MaterialShaderRepo = Join-Path $PSScriptRoot ".."
}

$UERoot = Resolve-FullPath $UERoot
$MaterialShaderRepo = Resolve-FullPath $MaterialShaderRepo
$ContentExamplesProject = Resolve-FullPath $ContentExamplesProject
if ($ResolutionX -gt 0) { $Width = $ResolutionX }
if ($ResolutionY -gt 0) { $Height = $ResolutionY }
$hostProjectRoot = Split-Path -Parent $ContentExamplesProject
$engineExe = Join-Path $UERoot "Engine\Binaries\Win64\UnrealEditor.exe"
$buildBat = Join-Path $UERoot "Engine\Build\BatchFiles\Build.bat"

if (-not (Test-Path $engineExe)) { throw "UnrealEditor.exe not found: $engineExe" }
if (-not (Test-Path $ContentExamplesProject)) { throw "ContentExamples project missing: $ContentExamplesProject" }

$stamp = (Get-Date).ToUniversalTime().ToString("yyyyMMddTHHmmssZ")
$isUE42E = $RunCoreRefactorValidation -or $RunIdentityBringup -or $RunCoreQuad -or $CompareUESortedPixelsOIT -or $DumpAllBuffers -or $CaptureGPU -or $GPUCapture -or $RequireRealVertexFactory -or $RequireRealMaterial -or $RequireParticleAttributeHash -or $RequireIdentityParity -or $RequireRealAVBOITDraw -or $RequireNonZeroBuffers -or $RequireSceneColorComposite -or $RequireComparisonImages -or $PromoteMilestone
$isUE42D = $RunNativeOITStudy -or $RunCoreBufferBringup -or $RunSingleNiagaraSprite -or $RunTestSpriteMap1 -or $RunTintMatrix -or $CaptureBufferOverview
$isUE42D = $isUE42D -and (-not $isUE42E)
$phaseName = if ($isUE42E) { "UE4-2E-Real-Niagara-Sprite-AVBOIT" } elseif ($isUE42D) { "UE4-2D-NativeOIT-Guided-AVBOIT" } else { "UE4-2C-NiagaraSprite-UnlitParity" }
$tempPhaseRoot = if ([string]::IsNullOrWhiteSpace($TempResultsRootOverride)) { Join-Path $MaterialShaderRepo "LocalVisualResults\TempResults\UE57\HIVE_4090x2\$phaseName" } else { Resolve-FullPath $TempResultsRootOverride }
$keyResultsPhaseRoot = if ([string]::IsNullOrWhiteSpace($KeyResultsRootOverride)) { Join-Path $MaterialShaderRepo "LocalVisualResults\KeyResults\UE57\HIVE_4090x2\$phaseName" } else { Resolve-FullPath $KeyResultsRootOverride }
$evidenceRoot = Join-Path $tempPhaseRoot $stamp
$keyResultsRoot = Join-Path $keyResultsPhaseRoot $stamp
if ($Clean -and (Test-Path $evidenceRoot)) {
    Remove-Item -LiteralPath $evidenceRoot -Recurse -Force
}
New-Item -ItemType Directory -Force -Path $evidenceRoot | Out-Null

git -C $MaterialShaderRepo status --short --branch | Set-Content -Path (Join-Path $evidenceRoot "GitState.txt") -Encoding UTF8
Copy-Item -Path (Join-Path $UERoot "Engine\Build\Build.version") -Destination (Join-Path $evidenceRoot "UEBuild.version.json") -Force

$deployScript = Join-Path $MaterialShaderRepo "Scripts\Deploy-AVBOITPluginToContentExamples.ps1"
$deployClean = $Clean -and (-not $SkipBuild)
if ($Clean -and $SkipBuild) {
    Write-Warning "Ignoring -Clean for plugin deployment because -SkipBuild requires existing plugin binaries."
}
& $deployScript -SourceRepoRoot $MaterialShaderRepo -HostProjectRoot $hostProjectRoot -Mode Copy -Clean:$deployClean

if (-not $SkipBuild) {
    & $buildBat "ContentExamplesEditor" "Win64" "Development" "-Project=$ContentExamplesProject" "-WaitMutex" "-NoHotReloadFromIDE"
    if ($LASTEXITCODE -ne 0) {
        throw "ContentExamplesEditor build failed with exit code $LASTEXITCODE"
    }
}

$timesArg = ($CaptureTimes | ForEach-Object { $_.ToString([Globalization.CultureInfo]::InvariantCulture) }) -join "|"
$interactiveMode = if ($CaptureBufferOverview -or $BufferOverview) { "BufferOverview" } elseif ($RunIdentityBringup) { "PluginIdentity" } elseif ($isUE42E) { "PluginAVBOIT" } else { "AVBOITUnlit" }
$preExecCmds = @()
if ($RequireRealVertexFactory) { $preExecCmds += "r.AVBOIT.Niagara.RequireRealVertexFactory 1" }
if ($RequireRealMaterial) { $preExecCmds += "r.AVBOIT.Niagara.RequireRealMaterial 1" }
if ($RequireParticleAttributeHash) { $preExecCmds += "r.AVBOIT.Niagara.RequireParticleAttributeHash 1" }
if ($RequireRealAVBOITDraw) { $preExecCmds += "r.AVBOIT.Niagara.RequireRealDraw 1" }
if ($RequireSceneColorComposite) { $preExecCmds += "r.AVBOIT.Niagara.RequireSceneColorComposite 1" }
if ($BufferOverview) { $preExecCmds += "r.AVBOIT.BufferOverview 1" }
if ($DumpAllBuffers) { $preExecCmds += "r.AVBOIT.Niagara.CaptureInputs 1" }
$mainExecCmd = if ($Interactive) {
    if ($SkipInteractiveOverlay) {
        "AVBOIT.Niagara.Interactive root=$evidenceRoot mode=$interactiveMode,AVBOIT.Niagara.HideOverlay,AVBOIT.Niagara.Status"
    } else {
        "AVBOIT.Niagara.Interactive root=$evidenceRoot mode=$interactiveMode,AVBOIT.Niagara.Status"
    }
} else {
    "AVBOIT.Niagara.CaptureParity root=$evidenceRoot times=$timesArg map=$Map"
}
$execCmds = (@($preExecCmds) + @($mainExecCmd)) -join ","
$logPath = Join-Path $evidenceRoot "UnrealEditor.log"
$args = @(
    $ContentExamplesProject,
    $Map,
    "-game",
    "-log",
    "-stdout",
    "-FullStdOutLogOutput",
    "-unattended",
    "-nosplash",
    "-dx12",
    "-ResX=$Width",
    "-ResY=$Height",
    "-ForceRes",
    "-windowed",
    "-NoLoadingScreen",
    "-execcmds=$execCmds",
    "-abslog=$logPath"
)

$argumentLine = ($args | ForEach-Object { Quote-ProcessArgument $_ }) -join " "
$windowStyle = if ($Interactive) { "Normal" } else { "Hidden" }
$process = Start-Process -FilePath $engineExe -ArgumentList $argumentLine -PassThru -WindowStyle $windowStyle
if ($Interactive) {
    Write-Host "$phaseName interactive session launched. Evidence/status root: $evidenceRoot"
    Write-Host "Use console commands: AVBOIT.Niagara.Mode EngineDefault|UESortedPixelsOIT|PluginIdentity|PluginAVBOIT|BufferOverview, AVBOIT.Niagara.Status"
    return
}
if (-not $process.WaitForExit($TimeoutSeconds * 1000)) {
    $process.Kill()
    throw "UnrealEditor timed out after $TimeoutSeconds seconds. Evidence root: $evidenceRoot"
}

$hardFailPatterns = @(
    "Fatal error",
    "Assertion failed",
    "RDG.*(error|failed|fatal|assert|ensure)",
    "RHI.*(error|failed|fatal|assert|ensure|crash)",
    "shader binding",
    "GPU crash",
    "Invalid socket handle",
    "duplicate draw",
    "no AVBOIT sprite draws"
)

$failures = @()
if (Test-Path $logPath) {
    $logText = Get-Content -Raw $logPath
    foreach ($pattern in $hardFailPatterns) {
        if ($logText -match $pattern) {
            $failures += $pattern
        }
    }
} else {
    $failures += "missing log"
}

$viewModeVerified = $false
$viewModeContractPath = Join-Path $evidenceRoot "ViewModeContractRuntime.json"
if (Test-Path $viewModeContractPath) {
    $viewModeContract = Get-Content -Raw $viewModeContractPath | ConvertFrom-Json
    $viewModeVerified = ($viewModeContract.RequestedViewMode -eq "Unlit" -and $viewModeContract.VerifiedViewMode -eq "Unlit" -and $viewModeContract.VerifiedViewModeIsUnlit -eq $true)
    if (-not $viewModeVerified) {
        $failures += "Engine ViewMode Unlit was not verified"
    }
} else {
    $failures += "missing ViewModeContractRuntime.json"
}

$captureManifestPath = Join-Path $evidenceRoot "CaptureManifest.json"
if (Test-Path $captureManifestPath) {
    $captureManifest = Get-Content -Raw $captureManifestPath | ConvertFrom-Json
    $badCaptures = @($captureManifest.Captures | Where-Object { $_.VerifiedViewMode -ne "Unlit" -or $_.VerifiedViewModeIsUnlit -ne $true })
    if ($badCaptures.Count -gt 0) {
        $failures += "one or more captures were not verified as Engine Unlit"
    }
} else {
    $failures += "missing CaptureManifest.json"
}

$pngs = @(Get-ChildItem -Path $evidenceRoot -Filter "*.png" -File -ErrorAction SilentlyContinue)
if ($pngs.Count -ne 16) {
    $failures += "expected 16 screenshots, found $($pngs.Count)"
}

$lastCaptureMs = [Math]::Round((($CaptureTimes | Sort-Object -Descending | Select-Object -First 1) * 1000.0))
$lastCaptureToken = "{0:D4}" -f [int]$lastCaptureMs
$engineFinal = Get-ChildItem -Path $evidenceRoot -Filter "*_EngineDefault_${lastCaptureToken}ms.png" -File -ErrorAction SilentlyContinue | Select-Object -First 1
$bypassFinal = Get-ChildItem -Path $evidenceRoot -Filter "*_PluginBypass_${lastCaptureToken}ms.png" -File -ErrorAction SilentlyContinue | Select-Object -First 1
$avboitFinal = Get-ChildItem -Path $evidenceRoot -Filter "*_AVBOITNiagaraUnlit_${lastCaptureToken}ms.png" -File -ErrorAction SilentlyContinue | Select-Object -First 1
$debugFinal = Get-ChildItem -Path $evidenceRoot -Filter "*_DebugBuffers_${lastCaptureToken}ms.png" -File -ErrorAction SilentlyContinue | Select-Object -First 1

$imageMetrics = $null
if ($engineFinal -and $bypassFinal) {
    $imageMetrics = [pscustomobject]@{
        GeneratedUtc = (Get-Date).ToUniversalTime().ToString("o")
        BypassVsEngineDefaultFinal = Compare-PngPixels $engineFinal.FullName $bypassFinal.FullName
        Status = "partial"
        SuccessWithheld = $true
    }
    $imageMetrics | ConvertTo-Json -Depth 8 | Set-Content -Path (Join-Path $evidenceRoot "ImageMetrics.json") -Encoding UTF8
} else {
    $failures += "missing final EngineDefault or PluginBypass screenshot for image metrics"
}

$requiredPasses = @(
    "AVBOIT.Niagara.Clear",
    "AVBOIT.Niagara.SpriteSplat",
    "AVBOIT.Niagara.Integrate",
    "AVBOIT.Niagara.ForwardUnlit",
    "AVBOIT.Niagara.Composite"
)
$rendererBindingPath = Join-Path $evidenceRoot "RendererBindingManifest.json"
if (Test-Path $rendererBindingPath) {
    $rendererBinding = Get-Content -Raw $rendererBindingPath | ConvertFrom-Json
    if ([int]$rendererBinding.SpriteDrawCount -le 0) {
        $failures += "no AVBOIT sprite draws"
    }
    foreach ($pass in $requiredPasses) {
        if (@($rendererBinding.ScheduledRDGPasses) -notcontains $pass) {
            $failures += "missing RDG pass $pass"
        }
    }
    if ($isUE42D) {
        if ($rendererBinding.RealAVBOITDraw -ne $true) {
            $failures += "UE-4.2D real AVBOIT draw packet is not proven"
        }
        if ($rendererBinding.CompositeWritesSceneColor -ne $true) {
            $failures += "UE-4.2D composite does not write SceneColor"
        }
        if ($rendererBinding.DefaultNiagaraFallbackUsed -eq $true) {
            $failures += "UE-4.2D default Niagara fallback draw was used"
        }
    }
} else {
    $failures += "missing RendererBindingManifest.json"
}

if ($isUE42E) {
    $materialContract = [pscustomobject]@{
        GeneratedUtc = (Get-Date).ToUniversalTime().ToString("o")
        Phase = $phaseName
        RealMaterialRenderProxyRequired = [bool]$RequireRealMaterial
        RealMaterialRenderProxyProven = if ($rendererBinding) { @($rendererBinding.Draws | Where-Object { $_.HasMaterialRenderProxy -eq $true }).Count -gt 0 } else { $false }
        EnginePatchRequired = $true
        PatchProposal = "Patches/UE57/NiagaraAVBOITMinimalHook.patch"
        Status = "blocked-local"
    }
    $materialContract | ConvertTo-Json -Depth 8 | Set-Content -Path (Join-Path $evidenceRoot "MaterialContract.json") -Encoding UTF8

    $sceneContract = [pscustomobject]@{
        GeneratedUtc = (Get-Date).ToUniversalTime().ToString("o")
        Phase = $phaseName
        Map = $Map
        RunMode = if ($Interactive) { "Interactive" } else { "Game" }
        Resolution = [pscustomobject]@{ Width = $Width; Height = $Height }
        EngineViewModeRequired = "Unlit"
        FixedCaptureTimesSeconds = $CaptureTimes
        RuntimeModes = @("EngineDefault", "UESortedPixelsOIT", "PluginIdentity", "PluginAVBOIT", "BufferOverview")
        KeyResultsPromotionPolicy = "Milestone gates only; no promotion for blocked-local/foundation/probe evidence."
    }
    $sceneContract | ConvertTo-Json -Depth 8 | Set-Content -Path (Join-Path $evidenceRoot "SceneContract.json") -Encoding UTF8

    $playerReference = [pscustomobject]@{
        GeneratedUtc = (Get-Date).ToUniversalTime().ToString("o")
        Source = "User-provided PlayerCameraManager0 reference"
        RelativeLocation = [pscustomobject]@{ X = -2798.789173; Y = 36.626050; Z = 457.510006 }
        RelativeRotation = [pscustomobject]@{ Pitch = 17.254463; Yaw = -8.130192; Roll = 0.0 }
        RelativeScale3D = [pscustomobject]@{ X = 1.0; Y = 1.0; Z = 1.0 }
        Mobility = "Movable"
    }
    $playerReference | ConvertTo-Json -Depth 8 | Set-Content -Path (Join-Path $evidenceRoot "TestSpriteMap1PlayerReference.json") -Encoding UTF8

    $cameraContractRuntime = Join-Path $evidenceRoot "CameraContractRuntime.json"
    if (Test-Path $cameraContractRuntime) {
        Copy-Item -Path $cameraContractRuntime -Destination (Join-Path $evidenceRoot "TestSpriteMap1Camera.json") -Force
        Copy-Item -Path $cameraContractRuntime -Destination (Join-Path $evidenceRoot "FinalCameraPOV.json") -Force
    } else {
        $playerReference | ConvertTo-Json -Depth 8 | Set-Content -Path (Join-Path $evidenceRoot "TestSpriteMap1Camera.json") -Encoding UTF8
        $playerReference | ConvertTo-Json -Depth 8 | Set-Content -Path (Join-Path $evidenceRoot "FinalCameraPOV.json") -Encoding UTF8
    }

    $keyResultsIndex = [pscustomobject]@{
        GeneratedUtc = (Get-Date).ToUniversalTime().ToString("o")
        Phase = $phaseName
        TempEvidenceRoot = $evidenceRoot
        KeyResultsPromoted = $false
        LatestUpdated = $false
        PromotionPolicy = "PromotionDecision must be eligible before LATEST.txt is updated."
    }
    $keyResultsIndex | ConvertTo-Json -Depth 8 | Set-Content -Path (Join-Path $evidenceRoot "KeyResultsIndex.json") -Encoding UTF8
}

$acceptancePath = Join-Path $evidenceRoot "Acceptance.json"
if (Test-Path $acceptancePath) {
    $acceptance = Get-Content -Raw $acceptancePath | ConvertFrom-Json
    if ($acceptance.Status -in @("SUCCESS", "COMPLETED", "passed-local")) {
        $failures += "forbidden acceptance status $($acceptance.Status)"
    }
} else {
    $failures += "missing Acceptance.json"
}

$keyResultsPromoted = $false
$promotionBlockedByPolicy = $NoKeyResultsPromotion -or $isUE42D -or ($isUE42E -and (-not $PromoteMilestone))
if ($process.ExitCode -eq 0 -and $failures.Count -eq 0 -and (-not $promotionBlockedByPolicy)) {
    New-Item -ItemType Directory -Force -Path $keyResultsRoot | Out-Null
    foreach ($item in @($engineFinal, $bypassFinal, $avboitFinal, $debugFinal)) {
        if ($item) {
            Copy-Item -Path $item.FullName -Destination (Join-Path $keyResultsRoot $item.Name) -Force
        }
    }

    foreach ($name in @(
        "RunSummary.json",
        "Acceptance.json",
        "CameraContractRuntime.json",
        "ViewModeContractRuntime.json",
        "CaptureManifest.json",
        "RendererBindingManifest.json",
        "RendererConversionManifest.json",
        "FeatureManifest.json",
        "FeatureManifestAfterConversion.json",
        "ImageMetrics.json",
        "GitState.txt",
        "UEBuild.version.json"
    )) {
        $source = Join-Path $evidenceRoot $name
        if (Test-Path $source) {
            Copy-Item -Path $source -Destination (Join-Path $keyResultsRoot $name) -Force
        }
    }

    $selectedImages = @()
    foreach ($item in @($engineFinal, $bypassFinal, $avboitFinal, $debugFinal)) {
        if ($item) {
            $selectedImages += $item.Name
        }
    }

    $keyIndex = [pscustomobject]@{
        GeneratedUtc = (Get-Date).ToUniversalTime().ToString("o")
        TempEvidenceRoot = $evidenceRoot
        KeyResultsRoot = $keyResultsRoot
        SelectedCaptureTimeSeconds = $lastCaptureMs / 1000.0
        SelectedImages = $selectedImages
        Status = "partial"
        SuccessWithheld = $true
    }
    $keyIndex | ConvertTo-Json -Depth 8 | Set-Content -Path (Join-Path $keyResultsRoot "KeyResultsIndex.json") -Encoding UTF8
    Set-Content -Path (Join-Path $keyResultsPhaseRoot "LATEST.txt") -Value $stamp -Encoding UTF8
    $keyResultsPromoted = $true
}

$tempManifest = [pscustomobject]@{
    GeneratedUtc = (Get-Date).ToUniversalTime().ToString("o")
    Phase = $phaseName
    EvidenceRoot = $evidenceRoot
    RunNativeOITStudy = [bool]$RunNativeOITStudy
    RunCoreBufferBringup = [bool]$RunCoreBufferBringup
    RunCoreRefactorValidation = [bool]$RunCoreRefactorValidation
    RunIdentityBringup = [bool]$RunIdentityBringup
    RunCoreQuad = [bool]$RunCoreQuad
    RunSingleNiagaraSprite = [bool]$RunSingleNiagaraSprite
    RunTestSpriteMap1 = [bool]$RunTestSpriteMap1
    RunTintMatrix = [bool]$RunTintMatrix
    CompareUESortedPixelsOIT = [bool]$CompareUESortedPixelsOIT
    CaptureBufferOverview = [bool]$CaptureBufferOverview
    DumpAllBuffers = [bool]$DumpAllBuffers
    CaptureGPU = [bool]($CaptureGPU -or $GPUCapture)
    RequireRealVertexFactory = [bool]$RequireRealVertexFactory
    RequireRealMaterial = [bool]$RequireRealMaterial
    RequireParticleAttributeHash = [bool]$RequireParticleAttributeHash
    RequireRealAVBOITDraw = [bool]$RequireRealAVBOITDraw
    RequireSceneColorComposite = [bool]$RequireSceneColorComposite
    ScreenshotCount = $pngs.Count
    Status = if ($process.ExitCode -eq 0 -and $failures.Count -eq 0) { "partial" } else { "blocked-local" }
}
$tempManifest | ConvertTo-Json -Depth 8 | Set-Content -Path (Join-Path $evidenceRoot "TempResultsManifest.json") -Encoding UTF8

$gpuRequiredPasses = $requiredPasses
$gpuPatchProposal = $null
$gpuCaptureNote = "UE-4.2D foundation records required pass names, but no RenderDoc/PIX capture is produced by this script yet."
if ($isUE42E) {
    $gpuRequiredPasses = @("AVBOIT.Identity.Draw", "AVBOIT.Identity.Composite", "AVBOIT.Clear", "AVBOIT.SpriteSplat", "AVBOIT.Integrate", "AVBOIT.ForwardUnlit", "AVBOIT.Composite")
    $gpuPatchProposal = "Patches/UE57/NiagaraAVBOITMinimalHook.patch"
    $gpuCaptureNote = "UE-4.2E records the required real pass names, but PluginIdentity/PluginAVBOIT are blocked by the Niagara private draw hook gap."
}
$gpuCaptureManifest = [pscustomobject]@{
    GeneratedUtc = (Get-Date).ToUniversalTime().ToString("o")
    Phase = $phaseName
    RenderDocOrPIXCaptureRequired = $true
    CaptureProduced = $false
    RequiredPasses = $gpuRequiredPasses
    PatchProposal = $gpuPatchProposal
    Note = $gpuCaptureNote
}
$gpuCaptureManifest | ConvertTo-Json -Depth 8 | Set-Content -Path (Join-Path $evidenceRoot "GPUCaptureManifest.json") -Encoding UTF8

$promotionDecision = [pscustomobject]@{
    GeneratedUtc = (Get-Date).ToUniversalTime().ToString("o")
    Phase = $phaseName
    PromotionEligible = $false
    KeyResultsPromoted = $keyResultsPromoted
    PolicyBlocked = [bool]$promotionBlockedByPolicy
    Failures = $failures
    Reason = if ($isUE42E) { "UE-4.2E hard gates require real Niagara sprite VF/material draw, particle attribute hash, SceneColor composite, nonzero readback, comparison images, and GPU capture proof before KeyResults promotion." } elseif ($isUE42D) { "UE-4.2D hard gates require real Niagara AVBOIT draw, SceneColor composite, and GPU readback proof before KeyResults promotion." } else { "Promotion withheld only when failures or -NoKeyResultsPromotion are present." }
}
$promotionDecision | ConvertTo-Json -Depth 8 | Set-Content -Path (Join-Path $evidenceRoot "PromotionDecision.json") -Encoding UTF8

$summary = [pscustomobject]@{
    GeneratedUtc = (Get-Date).ToUniversalTime().ToString("o")
    Phase = $phaseName
    EvidenceRoot = $evidenceRoot
    KeyResultsRoot = if ($keyResultsPromoted) { $keyResultsRoot } else { $null }
    UnrealExitCode = $process.ExitCode
    HardFailPatterns = $failures
    ViewModeVerifiedUnlit = $viewModeVerified
    ScreenshotCount = $pngs.Count
    KeyResultsPromoted = $keyResultsPromoted
    Status = if ($process.ExitCode -eq 0 -and $failures.Count -eq 0) { "partial" } else { "blocked-local" }
    Note = if ($isUE42E) { "UE-4.2E KeyResults promotion is intentionally blocked until the real Niagara draw bridge, shared AVBOIT resources, SceneColor composite, GPU readback, and comparison gates pass." } elseif ($isUE42D) { "UE-4.2D KeyResults promotion is intentionally blocked until real AVBOIT draw, SceneColor composite, and GPU readback gates pass." } else { "The runtime command writes Acceptance.json. SUCCESS/passed-local is intentionally withheld until all UE-4.2C gates pass." }
}
$summary | ConvertTo-Json -Depth 8 | Set-Content -Path (Join-Path $evidenceRoot "RunSummary.json") -Encoding UTF8
if ($keyResultsPromoted) {
    Copy-Item -Path (Join-Path $evidenceRoot "RunSummary.json") -Destination (Join-Path $keyResultsRoot "RunSummary.json") -Force
}

if ($process.ExitCode -ne 0 -or $failures.Count -gt 0) {
    throw "$phaseName run failed. Evidence root: $evidenceRoot"
}

Write-Host "$phaseName evidence root: $evidenceRoot"
if ($keyResultsPromoted) {
    Write-Host "$phaseName key results root: $keyResultsRoot"
} else {
    Write-Host "$phaseName key results promotion withheld."
}
