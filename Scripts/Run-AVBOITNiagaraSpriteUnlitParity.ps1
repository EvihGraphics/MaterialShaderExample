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
    [switch]$RunTintVisualGate,
    [object[]]$TintColor = @(1.0, 0.0, 0.0, 1.0),
    [string]$TintROI = "",
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

function Get-RoiPixelRect($Roi, [int]$Width, [int]$Height) {
    $left = [Math]::Max(0.0, [Math]::Min(1.0, [double]$Roi.Left))
    $top = [Math]::Max(0.0, [Math]::Min(1.0, [double]$Roi.Top))
    $right = [Math]::Max(0.0, [Math]::Min(1.0, [double]$Roi.Right))
    $bottom = [Math]::Max(0.0, [Math]::Min(1.0, [double]$Roi.Bottom))
    if ($right -le $left -or $bottom -le $top) {
        throw "Invalid normalized ROI: Left=$left Top=$top Right=$right Bottom=$bottom"
    }

    $x = [Math]::Max(0, [Math]::Min($Width - 1, [int][Math]::Floor($left * $Width)))
    $y = [Math]::Max(0, [Math]::Min($Height - 1, [int][Math]::Floor($top * $Height)))
    $r = [Math]::Max($x + 1, [Math]::Min($Width, [int][Math]::Ceiling($right * $Width)))
    $b = [Math]::Max($y + 1, [Math]::Min($Height, [int][Math]::Ceiling($bottom * $Height)))

    [pscustomobject]@{
        X = $x
        Y = $y
        Width = $r - $x
        Height = $b - $y
        Right = $r
        Bottom = $b
    }
}

function Save-BitmapPng($Bitmap, [string]$Path) {
    New-Item -ItemType Directory -Force -Path (Split-Path -Parent $Path) | Out-Null
    $Bitmap.Save($Path, [System.Drawing.Imaging.ImageFormat]::Png)
}

function Save-RoiCrop($Bitmap, $Rect, [string]$Path) {
    $cropRect = [System.Drawing.Rectangle]::new($Rect.X, $Rect.Y, $Rect.Width, $Rect.Height)
    $crop = $Bitmap.Clone($cropRect, $Bitmap.PixelFormat)
    try {
        Save-BitmapPng $crop $Path
    }
    finally {
        $crop.Dispose()
    }
}

function Copy-ImageInto($Graphics, $Bitmap, [int]$X, [string]$Label) {
    $Graphics.DrawImage($Bitmap, $X, 0, $Bitmap.Width, $Bitmap.Height)
    $font = [System.Drawing.Font]::new("Arial", 18, [System.Drawing.FontStyle]::Bold)
    $brush = [System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(230, 255, 255, 255))
    $shadow = [System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(180, 0, 0, 0))
    try {
        $Graphics.DrawString($Label, $font, $shadow, $X + 15, 16)
        $Graphics.DrawString($Label, $font, $brush, $X + 14, 15)
    }
    finally {
        $font.Dispose()
        $brush.Dispose()
        $shadow.Dispose()
    }
}

function Measure-TintVisualGate(
    [string]$EngineBeforePath,
    [string]$PluginTintPath,
    [string]$EngineAfterPath,
    $Roi,
    [double[]]$TintColor,
    [string]$OutputRoot,
    [string]$Token
) {
    Add-Type -AssemblyName System.Drawing
    New-Item -ItemType Directory -Force -Path $OutputRoot | Out-Null
    $cropRoot = Join-Path $OutputRoot "ROICrops"
    $comparisonRoot = Join-Path $OutputRoot "Comparisons"
    New-Item -ItemType Directory -Force -Path $cropRoot, $comparisonRoot | Out-Null

    $engineBefore = [System.Drawing.Bitmap]::new($EngineBeforePath)
    $pluginTint = [System.Drawing.Bitmap]::new($PluginTintPath)
    $engineAfter = [System.Drawing.Bitmap]::new($EngineAfterPath)
    $heatmap = $null
    $coverage = $null
    $difference = $null
    $sideBySide = $null
    $annotated = $null
    try {
        if ($engineBefore.Width -ne $pluginTint.Width -or $engineBefore.Height -ne $pluginTint.Height -or
            $engineBefore.Width -ne $engineAfter.Width -or $engineBefore.Height -ne $engineAfter.Height) {
            throw "Tint visual images have mismatched dimensions."
        }

        $width = $engineBefore.Width
        $height = $engineBefore.Height
        $rect = Get-RoiPixelRect $Roi $width $height
        $roiPixels = [int64]$rect.Width * [int64]$rect.Height

        [double]$engineBeforeAfterAbs = 0.0
        [double]$pluginDiffAbs = 0.0
        [double]$redAdvantageSum = 0.0
        [int64]$changedPixels = 0
        [int64]$redDominantPixels = 0
        [int64]$redDominantAndChangedPixels = 0

        $heatmap = [System.Drawing.Bitmap]::new($width, $height, [System.Drawing.Imaging.PixelFormat]::Format32bppArgb)
        $coverage = [System.Drawing.Bitmap]::new($width, $height, [System.Drawing.Imaging.PixelFormat]::Format32bppArgb)
        $difference = [System.Drawing.Bitmap]::new($width, $height, [System.Drawing.Imaging.PixelFormat]::Format32bppArgb)

        for ($y = $rect.Y; $y -lt $rect.Bottom; $y++) {
            for ($x = $rect.X; $x -lt $rect.Right; $x++) {
                $before = $engineBefore.GetPixel($x, $y)
                $plugin = $pluginTint.GetPixel($x, $y)
                $after = $engineAfter.GetPixel($x, $y)

                $engineBeforeAfterAbs += [Math]::Abs([int]$before.R - [int]$after.R) + [Math]::Abs([int]$before.G - [int]$after.G) + [Math]::Abs([int]$before.B - [int]$after.B)

                $dr = [Math]::Abs([int]$plugin.R - [int]$before.R)
                $dg = [Math]::Abs([int]$plugin.G - [int]$before.G)
                $db = [Math]::Abs([int]$plugin.B - [int]$before.B)
                $sumDiff = $dr + $dg + $db
                $pluginDiffAbs += $sumDiff

                $redAdvantage = ([int]$plugin.R - [Math]::Max([int]$plugin.G, [int]$plugin.B))
                $isChanged = $sumDiff -ge 24
                $isRedDominant = $redAdvantage -ge 32
                if ($isChanged) {
                    $changedPixels++
                    $redAdvantageSum += ($redAdvantage / 255.0)
                    $coverage.SetPixel($x, $y, [System.Drawing.Color]::FromArgb(255, 255, 255, 255))
                }
                if ($isRedDominant) {
                    $redDominantPixels++
                }
                if ($isChanged -and $isRedDominant) {
                    $redDominantAndChangedPixels++
                    $coverage.SetPixel($x, $y, [System.Drawing.Color]::FromArgb(255, 255, 0, 0))
                }

                $heat = [Math]::Min(255, $sumDiff)
                $heatmap.SetPixel($x, $y, [System.Drawing.Color]::FromArgb(255, $heat, 0, 255 - $heat))
                $difference.SetPixel($x, $y, [System.Drawing.Color]::FromArgb(255, $dr, $dg, $db))
            }
        }

        $engineBeforeAfterMAE = $engineBeforeAfterAbs / ([double]$roiPixels * 3.0 * 255.0)
        $pluginVsEngineMAE = $pluginDiffAbs / ([double]$roiPixels * 3.0 * 255.0)
        $redCoverageRatio = if ($changedPixels -gt 0) { [double]$redDominantAndChangedPixels / [double]$changedPixels } else { 0.0 }
        $meanRedAdvantage = if ($changedPixels -gt 0) { $redAdvantageSum / [double]$changedPixels } else { 0.0 }
        $changedPixelRatio = [double]$changedPixels / [double]$roiPixels
        $minChangedPixelCount = [Math]::Max(500, [int]($roiPixels * 0.002))

        $engineStablePass = $engineBeforeAfterMAE -le (1.0 / 255.0)
        $redCoveragePass = $redCoverageRatio -ge 0.98
        $redAdvantagePass = $meanRedAdvantage -ge 0.12
        $changedCoveragePass = $changedPixels -ge $minChangedPixelCount
        $gatePass = $engineStablePass -and $redCoveragePass -and $redAdvantagePass -and $changedCoveragePass

        $engineBeforeCrop = Join-Path $cropRoot "EngineDefaultBefore_${Token}_ROI.png"
        $pluginCrop = Join-Path $cropRoot "PluginAVBOIT_RedTint_${Token}_ROI.png"
        $engineAfterCrop = Join-Path $cropRoot "EngineDefaultAfter_${Token}_ROI.png"
        Save-RoiCrop $engineBefore $rect $engineBeforeCrop
        Save-RoiCrop $pluginTint $rect $pluginCrop
        Save-RoiCrop $engineAfter $rect $engineAfterCrop

        $sideBySidePath = Join-Path $comparisonRoot "SideBySide_${Token}.png"
        $sideBySide = [System.Drawing.Bitmap]::new($width * 3, $height, [System.Drawing.Imaging.PixelFormat]::Format32bppArgb)
        $g = [System.Drawing.Graphics]::FromImage($sideBySide)
        try {
            Copy-ImageInto $g $engineBefore 0 "Engine Default Before"
            Copy-ImageInto $g $pluginTint $width "Plugin AVBOIT Red Tint"
            Copy-ImageInto $g $engineAfter ($width * 2) "Engine Default After"
        }
        finally {
            $g.Dispose()
        }
        Save-BitmapPng $sideBySide $sideBySidePath

        $heatmapPath = Join-Path $comparisonRoot "Heatmap_${Token}.png"
        $coveragePath = Join-Path $comparisonRoot "CoverageMask_${Token}.png"
        $differencePath = Join-Path $comparisonRoot "Difference_${Token}.png"
        Save-BitmapPng $heatmap $heatmapPath
        Save-BitmapPng $coverage $coveragePath
        Save-BitmapPng $difference $differencePath

        $annotatedPath = Join-Path $comparisonRoot "AnnotatedGoal_${Token}.png"
        $annotated = [System.Drawing.Bitmap]::new($pluginTint)
        $graphics = [System.Drawing.Graphics]::FromImage($annotated)
        $pen = [System.Drawing.Pen]::new([System.Drawing.Color]::Red, 3)
        $font = [System.Drawing.Font]::new("Arial", 12, [System.Drawing.FontStyle]::Bold)
        $brush = [System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(235, 255, 255, 255))
        $shadow = [System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(180, 0, 0, 0))
        try {
            $graphics.DrawRectangle($pen, $rect.X, $rect.Y, $rect.Width, $rect.Height)
            $label1 = "UE-4.2F ROI Red Tint | Target: PluginAVBOIT red particles, EngineDefault unchanged"
            $label2 = "Mode=PluginAVBOIT | Tint=({0:0.###},{1:0.###},{2:0.###},{3:0.###}) | Gate={4}" -f $TintColor[0], $TintColor[1], $TintColor[2], $TintColor[3], ($(if ($gatePass) { "PASS" } else { "FAIL" }))
            $label3 = "RedCoverage={0:P2} | MeanRedAdv={1:0.###} | EngineMAE={2:0.#####}" -f $redCoverageRatio, $meanRedAdvantage, $engineBeforeAfterMAE
            $graphics.DrawString($label1, $font, $shadow, 15, 13)
            $graphics.DrawString($label1, $font, $brush, 14, 12)
            $graphics.DrawString($label2, $font, $shadow, 15, 33)
            $graphics.DrawString($label2, $font, $brush, 14, 32)
            $graphics.DrawString($label3, $font, $shadow, 15, 53)
            $graphics.DrawString($label3, $font, $brush, 14, 52)
        }
        finally {
            $graphics.Dispose()
            $pen.Dispose()
            $font.Dispose()
            $brush.Dispose()
            $shadow.Dispose()
        }
        Save-BitmapPng $annotated $annotatedPath

        $metrics = [pscustomobject]@{
            GeneratedUtc = (Get-Date).ToUniversalTime().ToString("o")
            CaptureToken = $Token
            ScreenshotSource = "UE.FScreenshotRequest"
            DesktopScreenshotUsed = $false
            Width = $width
            Height = $height
            ROI = [pscustomobject]@{
                Normalized = $Roi
                PixelRect = [pscustomobject]@{ X = $rect.X; Y = $rect.Y; Width = $rect.Width; Height = $rect.Height; Right = $rect.Right; Bottom = $rect.Bottom }
                PixelCount = $roiPixels
            }
            TintColorLinear = [pscustomobject]@{ R = $TintColor[0]; G = $TintColor[1]; B = $TintColor[2]; A = $TintColor[3] }
            EngineDefaultBeforeAfterRoiMAE = $engineBeforeAfterMAE
            PluginVsEngineDefaultRoiMAE = $pluginVsEngineMAE
            ChangedPixelCount = $changedPixels
            ChangedPixelRatio = $changedPixelRatio
            MinChangedPixelCount = $minChangedPixelCount
            RedDominantPixelCount = $redDominantPixels
            RedDominantChangedPixelCount = $redDominantAndChangedPixels
            RedCoverageRatio = $redCoverageRatio
            MeanRedAdvantage = $meanRedAdvantage
            EngineStablePass = $engineStablePass
            ChangedCoveragePass = $changedCoveragePass
            RedCoveragePass = $redCoveragePass
            RedAdvantagePass = $redAdvantagePass
            GatePass = $gatePass
            Files = [pscustomobject]@{
                EngineDefaultBefore = $EngineBeforePath
                PluginAVBOITRedTint = $PluginTintPath
                EngineDefaultAfter = $EngineAfterPath
                EngineDefaultBeforeROICrop = $engineBeforeCrop
                PluginAVBOITRedTintROICrop = $pluginCrop
                EngineDefaultAfterROICrop = $engineAfterCrop
                SideBySide = $sideBySidePath
                Heatmap = $heatmapPath
                CoverageMask = $coveragePath
                Difference = $differencePath
                AnnotatedGoal = $annotatedPath
            }
        }

        $metricsPath = Join-Path $OutputRoot "TintVisualMetrics_${Token}.json"
        $metrics | ConvertTo-Json -Depth 12 | Set-Content -Path $metricsPath -Encoding UTF8
        Add-Member -InputObject $metrics -MemberType NoteProperty -Name MetricsPath -Value $metricsPath
        return $metrics
    }
    finally {
        if ($engineBefore) { $engineBefore.Dispose() }
        if ($pluginTint) { $pluginTint.Dispose() }
        if ($engineAfter) { $engineAfter.Dispose() }
        if ($heatmap) { $heatmap.Dispose() }
        if ($coverage) { $coverage.Dispose() }
        if ($difference) { $difference.Dispose() }
        if ($sideBySide) { $sideBySide.Dispose() }
        if ($annotated) { $annotated.Dispose() }
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
$isUE42F = [bool]$RunTintVisualGate
$isUE42E = (-not $isUE42F) -and ($RunCoreRefactorValidation -or $RunIdentityBringup -or $RunCoreQuad -or $CompareUESortedPixelsOIT -or $DumpAllBuffers -or $CaptureGPU -or $GPUCapture -or $RequireRealVertexFactory -or $RequireRealMaterial -or $RequireParticleAttributeHash -or $RequireIdentityParity -or $RequireRealAVBOITDraw -or $RequireNonZeroBuffers -or $RequireSceneColorComposite -or $RequireComparisonImages -or $PromoteMilestone)
$isUE42D = $RunNativeOITStudy -or $RunCoreBufferBringup -or $RunSingleNiagaraSprite -or $RunTestSpriteMap1 -or $RunTintMatrix -or $CaptureBufferOverview
$isUE42D = $isUE42D -and (-not $isUE42E) -and (-not $isUE42F)
$phaseName = if ($isUE42F) { "UE4-2F-ROI-RedTint-VisualGate" } elseif ($isUE42E) { "UE4-2E-Real-Niagara-Sprite-AVBOIT" } elseif ($isUE42D) { "UE4-2D-NativeOIT-Guided-AVBOIT" } else { "UE4-2C-NiagaraSprite-UnlitParity" }
if ([string]::IsNullOrWhiteSpace($TintROI)) {
    $TintROI = Join-Path $MaterialShaderRepo "Config\AVBOIT\TestSpriteMap1TintROI.json"
}
$TintROI = Resolve-FullPath $TintROI
$tintColorValues = @()
foreach ($value in @($TintColor)) {
    foreach ($part in ([string]$value -split '[,|;]')) {
        if (-not [string]::IsNullOrWhiteSpace($part)) {
            $tintColorValues += [double]::Parse($part.Trim(), [Globalization.CultureInfo]::InvariantCulture)
        }
    }
}
if ($tintColorValues.Count -lt 3) {
    throw "-TintColor expects at least three values: r,g,b[,a]."
}
if ($tintColorValues.Count -eq 3) {
    $TintColor = @($tintColorValues[0], $tintColorValues[1], $tintColorValues[2], 1.0)
} else {
    $TintColor = @($tintColorValues[0], $tintColorValues[1], $tintColorValues[2], $tintColorValues[3])
}
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
$tintColorArg = ($TintColor | Select-Object -First 4 | ForEach-Object { $_.ToString([Globalization.CultureInfo]::InvariantCulture) }) -join ","
$tintRoiArg = $TintROI -replace '\\', '/'
$interactiveMode = if ($CaptureBufferOverview -or $BufferOverview) { "BufferOverview" } elseif ($RunIdentityBringup) { "PluginIdentity" } elseif ($isUE42E -or $isUE42F) { "PluginAVBOIT" } else { "AVBOITUnlit" }
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
    if ($RunTintVisualGate) {
        "AVBOIT.Niagara.CaptureTintComparison root=$evidenceRoot times=$timesArg color=$tintColorArg roi=$tintRoiArg"
    } else {
        "AVBOIT.Niagara.CaptureParity root=$evidenceRoot times=$timesArg map=$Map"
    }
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
if ($RunTintVisualGate) {
    $expectedTintScreenshotCount = $CaptureTimes.Count * 3
    if ($pngs.Count -ne $expectedTintScreenshotCount) {
        $failures += "expected $expectedTintScreenshotCount tint visual gate screenshots, found $($pngs.Count)"
    }

    $roiContract = Get-Content -Raw $TintROI | ConvertFrom-Json
    Copy-Item -Path $TintROI -Destination (Join-Path $evidenceRoot "TestSpriteMap1TintROI.json") -Force

    $tintMetricsRoot = Join-Path $evidenceRoot "TintVisualGate"
    New-Item -ItemType Directory -Force -Path $tintMetricsRoot | Out-Null
    $allTintMetrics = @()

    foreach ($time in ($CaptureTimes | Sort-Object)) {
        $captureMs = [Math]::Round($time * 1000.0)
        $token = "{0:D4}ms" -f [int]$captureMs
        $engineBefore = Get-ChildItem -Path $evidenceRoot -Filter "*_EngineDefaultBefore_$token.png" -File -ErrorAction SilentlyContinue | Select-Object -First 1
        $pluginRed = Get-ChildItem -Path $evidenceRoot -Filter "*_PluginAVBOIT_RedTint_$token.png" -File -ErrorAction SilentlyContinue | Select-Object -First 1
        $engineAfter = Get-ChildItem -Path $evidenceRoot -Filter "*_EngineDefaultAfter_$token.png" -File -ErrorAction SilentlyContinue | Select-Object -First 1

        if (-not $engineBefore -or -not $pluginRed -or -not $engineAfter) {
            $failures += "missing tint visual gate triplet for $token"
            continue
        }

        $metrics = Measure-TintVisualGate `
            -EngineBeforePath $engineBefore.FullName `
            -PluginTintPath $pluginRed.FullName `
            -EngineAfterPath $engineAfter.FullName `
            -Roi $roiContract `
            -TintColor $TintColor `
            -OutputRoot $tintMetricsRoot `
            -Token $token
        $allTintMetrics += $metrics
    }

    $lastCaptureMs = [Math]::Round((($CaptureTimes | Sort-Object -Descending | Select-Object -First 1) * 1000.0))
    $lastCaptureToken = "{0:D4}ms" -f [int]$lastCaptureMs
    $finalMetrics = @($allTintMetrics | Where-Object { $_.CaptureToken -eq $lastCaptureToken } | Select-Object -First 1)
    if (-not $finalMetrics) {
        $failures += "missing final tint visual metrics for $lastCaptureToken"
    }

    $captureSourceOk = $false
    $overlayClean = $false
    if ($captureManifest) {
        $badScreenshotSources = @($captureManifest.Captures | Where-Object { $_.ScreenshotSource -ne "UE.FScreenshotRequest" -or $_.DesktopScreenshotUsed -ne $false })
        $overlayContaminated = @($captureManifest.Captures | Where-Object { $_.OverlayHidden -ne $true })
        $captureSourceOk = $badScreenshotSources.Count -eq 0
        $overlayClean = $overlayContaminated.Count -eq 0
        if (-not $captureSourceOk) {
            $failures += "one or more tint screenshots were not produced by UE.FScreenshotRequest"
        }
        if (-not $overlayClean) {
            $failures += "one or more raw tint screenshots had overlay contamination risk"
        }
    } else {
        $failures += "missing CaptureManifest.json"
    }

    $convertedRendererCount = 0
    $rendererConversionPath = Join-Path $evidenceRoot "RendererConversionManifest.json"
    if (Test-Path $rendererConversionPath) {
        $rendererConversion = Get-Content -Raw $rendererConversionPath | ConvertFrom-Json
        $convertedRendererCount = [int]$rendererConversion.ConvertedRendererCount
        if ($convertedRendererCount -le 0) {
            $failures += "no converted AVBOIT Niagara sprite renderers for tint visual gate"
        }
    } else {
        $failures += "missing RendererConversionManifest.json"
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

    $finalGatePass = ($process.ExitCode -eq 0 -and $failures.Count -eq 0 -and $finalMetrics -and $finalMetrics.GatePass -eq $true)
    if ($finalMetrics -and $finalMetrics.GatePass -ne $true) {
        $failures += "final ROI red tint visual gate failed"
    }

    $roiRuntime = [pscustomobject]@{
        GeneratedUtc = (Get-Date).ToUniversalTime().ToString("o")
        Phase = $phaseName
        SourceConfig = $TintROI
        Normalized = $roiContract
        FinalCaptureToken = $lastCaptureToken
        FinalPixelRect = if ($finalMetrics) { $finalMetrics.ROI.PixelRect } else { $null }
        TintColorLinear = [pscustomobject]@{ R = $TintColor[0]; G = $TintColor[1]; B = $TintColor[2]; A = $TintColor[3] }
        ScreenshotSource = "UE.FScreenshotRequest"
        DesktopScreenshotAllowed = $false
    }
    $roiRuntime | ConvertTo-Json -Depth 12 | Set-Content -Path (Join-Path $evidenceRoot "TintROIContractRuntime.json") -Encoding UTF8

    $tintVisualMetrics = [pscustomobject]@{
        GeneratedUtc = (Get-Date).ToUniversalTime().ToString("o")
        Phase = $phaseName
        EvidenceRoot = $evidenceRoot
        RequiredTarget = "PluginAVBOIT red tint inside ROI; EngineDefault before/after unchanged"
        ScreenshotSource = "UE.FScreenshotRequest"
        DesktopScreenshotUsed = $false
        FinalCaptureToken = $lastCaptureToken
        FinalGatePass = [bool]$finalGatePass
        FinalMetrics = $finalMetrics
        AllCaptureMetrics = $allTintMetrics
    }
    $tintVisualMetrics | ConvertTo-Json -Depth 14 | Set-Content -Path (Join-Path $evidenceRoot "TintVisualMetrics.json") -Encoding UTF8

    $keyResultsPromoted = $false
    $promotionEligible = [bool]($finalGatePass -and (-not $NoKeyResultsPromotion))
    if ($promotionEligible) {
        New-Item -ItemType Directory -Force -Path $keyResultsRoot | Out-Null

        $filesToPromote = @(
            $finalMetrics.Files.EngineDefaultBefore,
            $finalMetrics.Files.PluginAVBOITRedTint,
            $finalMetrics.Files.EngineDefaultAfter,
            $finalMetrics.Files.EngineDefaultBeforeROICrop,
            $finalMetrics.Files.PluginAVBOITRedTintROICrop,
            $finalMetrics.Files.EngineDefaultAfterROICrop,
            $finalMetrics.Files.SideBySide,
            $finalMetrics.Files.Heatmap,
            $finalMetrics.Files.CoverageMask,
            $finalMetrics.Files.Difference,
            $finalMetrics.Files.AnnotatedGoal
        )
        foreach ($file in $filesToPromote) {
            if ($file -and (Test-Path $file)) {
                Copy-Item -Path $file -Destination (Join-Path $keyResultsRoot (Split-Path -Leaf $file)) -Force
            }
        }

        foreach ($name in @(
            "Acceptance.json",
            "CameraContractRuntime.json",
            "TestSpriteMap1Camera.json",
            "FinalCameraPOV.json",
            "ViewModeContractRuntime.json",
            "CaptureManifest.json",
            "RendererBindingManifest.json",
            "RendererConversionManifest.json",
            "FeatureManifest.json",
            "FeatureManifestAfterConversion.json",
            "TintVisualMetrics.json",
            "TintROIContractRuntime.json",
            "TestSpriteMap1TintROI.json",
            "GitState.txt",
            "UEBuild.version.json"
        )) {
            $source = Join-Path $evidenceRoot $name
            if (Test-Path $source) {
                Copy-Item -Path $source -Destination (Join-Path $keyResultsRoot $name) -Force
            }
        }

        $keyIndex = [pscustomobject]@{
            GeneratedUtc = (Get-Date).ToUniversalTime().ToString("o")
            Phase = $phaseName
            TempEvidenceRoot = $evidenceRoot
            KeyResultsRoot = $keyResultsRoot
            SelectedCaptureTimeSeconds = $lastCaptureMs / 1000.0
            SelectedGoal = "ROI Red Tint Visual Gate"
            SelectedMode = "PluginAVBOIT"
            SelectedTintColorLinear = [pscustomobject]@{ R = $TintColor[0]; G = $TintColor[1]; B = $TintColor[2]; A = $TintColor[3] }
            SelectedImages = @($filesToPromote | Where-Object { $_ } | ForEach-Object { Split-Path -Leaf $_ })
            VisualGateStatus = "passed"
            OverallProjectStatus = "partial"
            Note = "KeyResults promotion here means the UE-4.2F visual red-tint target passed. It does not claim the UE-4.2E real Niagara draw bridge hard gates are complete."
        }
        $keyIndex | ConvertTo-Json -Depth 10 | Set-Content -Path (Join-Path $keyResultsRoot "KeyResultsIndex.json") -Encoding UTF8
        Set-Content -Path (Join-Path $keyResultsPhaseRoot "LATEST.txt") -Value $stamp -Encoding UTF8
        $keyResultsPromoted = $true
    }

    $tempManifest = [pscustomobject]@{
        GeneratedUtc = (Get-Date).ToUniversalTime().ToString("o")
        Phase = $phaseName
        EvidenceRoot = $evidenceRoot
        RunTintVisualGate = $true
        TintColorLinear = [pscustomobject]@{ R = $TintColor[0]; G = $TintColor[1]; B = $TintColor[2]; A = $TintColor[3] }
        TintROI = $TintROI
        ScreenshotCount = $pngs.Count
        ScreenshotSource = "UE.FScreenshotRequest"
        DesktopScreenshotUsed = $false
        Status = if ($process.ExitCode -eq 0 -and $failures.Count -eq 0) { "partial" } else { "blocked-local" }
    }
    $tempManifest | ConvertTo-Json -Depth 10 | Set-Content -Path (Join-Path $evidenceRoot "TempResultsManifest.json") -Encoding UTF8

    $gpuCaptureManifest = [pscustomobject]@{
        GeneratedUtc = (Get-Date).ToUniversalTime().ToString("o")
        Phase = $phaseName
        CaptureProduced = $false
        RequiredForRealBridge = $true
        Note = "UE-4.2F visual gate uses UE.FScreenshotRequest image evidence. GPU capture remains required for real Niagara draw bridge acceptance."
    }
    $gpuCaptureManifest | ConvertTo-Json -Depth 8 | Set-Content -Path (Join-Path $evidenceRoot "GPUCaptureManifest.json") -Encoding UTF8

    $promotionDecision = [pscustomobject]@{
        GeneratedUtc = (Get-Date).ToUniversalTime().ToString("o")
        Phase = $phaseName
        PromotionEligible = [bool]$promotionEligible
        KeyResultsPromoted = [bool]$keyResultsPromoted
        FinalVisualGatePass = [bool]$finalGatePass
        PolicyBlocked = [bool]$NoKeyResultsPromotion
        Failures = $failures
        OverallProjectStatus = "partial"
        Reason = if ($finalGatePass) { "UE-4.2F ROI red tint visual gate passed for the final time point; real Niagara draw bridge remains separately blocked until UE-4.2E hard gates pass." } else { "UE-4.2F ROI red tint visual gate failed or evidence was incomplete; KeyResults promotion is forbidden." }
    }
    $promotionDecision | ConvertTo-Json -Depth 10 | Set-Content -Path (Join-Path $evidenceRoot "PromotionDecision.json") -Encoding UTF8

    $keyResultsIndex = [pscustomobject]@{
        GeneratedUtc = (Get-Date).ToUniversalTime().ToString("o")
        Phase = $phaseName
        TempEvidenceRoot = $evidenceRoot
        KeyResultsRoot = if ($keyResultsPromoted) { $keyResultsRoot } else { $null }
        KeyResultsPromoted = [bool]$keyResultsPromoted
        LatestUpdated = [bool]$keyResultsPromoted
    }
    $keyResultsIndex | ConvertTo-Json -Depth 8 | Set-Content -Path (Join-Path $evidenceRoot "KeyResultsIndex.json") -Encoding UTF8

    $summary = [pscustomobject]@{
        GeneratedUtc = (Get-Date).ToUniversalTime().ToString("o")
        Phase = $phaseName
        EvidenceRoot = $evidenceRoot
        KeyResultsRoot = if ($keyResultsPromoted) { $keyResultsRoot } else { $null }
        UnrealExitCode = $process.ExitCode
        HardFailPatterns = $failures
        ViewModeVerifiedUnlit = $viewModeVerified
        ScreenshotCount = $pngs.Count
        ScreenshotSource = "UE.FScreenshotRequest"
        KeyResultsPromoted = [bool]$keyResultsPromoted
        Status = if ($process.ExitCode -eq 0 -and $failures.Count -eq 0) { "partial" } else { "blocked-local" }
        Note = "UE-4.2F requires PluginAVBOIT red tint in the configured ROI and unchanged EngineDefault before/after. Real Niagara draw bridge acceptance remains separate."
    }
    $summary | ConvertTo-Json -Depth 10 | Set-Content -Path (Join-Path $evidenceRoot "RunSummary.json") -Encoding UTF8
    if ($keyResultsPromoted) {
        Copy-Item -Path (Join-Path $evidenceRoot "RunSummary.json") -Destination (Join-Path $keyResultsRoot "RunSummary.json") -Force
        Copy-Item -Path (Join-Path $evidenceRoot "PromotionDecision.json") -Destination (Join-Path $keyResultsRoot "PromotionDecision.json") -Force
        Copy-Item -Path (Join-Path $evidenceRoot "TempResultsManifest.json") -Destination (Join-Path $keyResultsRoot "TempResultsManifest.json") -Force
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
    return
}

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
