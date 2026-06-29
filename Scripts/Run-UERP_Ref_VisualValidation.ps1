param (
    [string]$UERoot = "D:\UE\UnrealEngine_Animation_Tech",
    [string]$UprojectPath = "",
    [string]$EvidenceRoot = "",
    [int]$ResolutionX = 1280,
    [int]$ResolutionY = 720,
    [switch]$SkipBuild
)

$ErrorActionPreference = "Stop"

$ProjectName = [System.IO.Path]::GetFileNameWithoutExtension($UprojectPath)

$Timestamp = (Get-Date).ToUniversalTime().ToString("yyyyMMddTHHmmssZ")
$CurrentEvidenceDir = Join-Path $EvidenceRoot $Timestamp
New-Item -ItemType Directory -Force -Path $CurrentEvidenceDir | Out-Null
$KeyResultsDir = Join-Path $EvidenceRoot "..\KeyResults\$ProjectName"
New-Item -ItemType Directory -Force -Path $KeyResultsDir | Out-Null

if (-not $SkipBuild) {
    Write-Host "Building $($ProjectName)Editor..."
    $BuildBat = "$UERoot\Engine\Build\BatchFiles\Build.bat"
    $CmdArgs = "$($ProjectName)Editor", "Win64", "Development", "-Project=`"$UprojectPath`"", "-WaitMutex", "-NoHotReloadFromIDE", "-NoSharedDependencies"
    $process = Start-Process -FilePath $BuildBat -ArgumentList $CmdArgs -Wait -NoNewWindow -PassThru
    if ($process.ExitCode -ne 0) { throw "Project Build Failed" }
}

Write-Host "Running Visual Validation..."
$EditorCmd = "$UERoot\Engine\Binaries\Win64\UnrealEditor.exe"
$ExecArgs = "`"$UprojectPath`" -game -windowed -resx=$ResolutionX -resy=$ResolutionY -unattended -nopause -NoUI -nosound -ExecCmds=`"HighResShot 1`""

$TestLog = Join-Path $CurrentEvidenceDir "$($ProjectName)_Run.log"
$TestLogErr = Join-Path $CurrentEvidenceDir "$($ProjectName)_Run.err.log"

$process = Start-Process -FilePath $EditorCmd -ArgumentList $ExecArgs -PassThru
Write-Host "Waiting up to 5 minutes for screenshot to be generated..."
$WaitTime = 0
$ProjectDir = Split-Path -Parent $UprojectPath
$ScreenshotsDir = Join-Path $ProjectDir "Saved\Screenshots\Windows"

while ($WaitTime -lt 300) {
    if (Test-Path $ScreenshotsDir) {
        $Screenshots = Get-ChildItem -Path $ScreenshotsDir -Filter "*.png" | Sort-Object LastWriteTime -Descending
        if ($Screenshots.Count -gt 0) {
            Write-Host "Screenshot found after $WaitTime seconds."
            break
        }
    }
    Start-Sleep -Seconds 5
    $WaitTime += 5
}

if (-not $process.HasExited) {
    Write-Host "Terminating game process..."
    Stop-Process -Id $process.Id -Force
}

# Move screenshot from Saved/Screenshots/Windows to KeyResults
$ProjectDir = Split-Path -Parent $UprojectPath
$ScreenshotsDir = Join-Path $ProjectDir "Saved\Screenshots\Windows"

if (Test-Path $ScreenshotsDir) {
    $Screenshots = Get-ChildItem -Path $ScreenshotsDir -Filter "*.png" | Sort-Object LastWriteTime -Descending
    if ($Screenshots.Count -gt 0) {
        $LatestScreenshot = $Screenshots[0].FullName
        $TargetScreenshot = Join-Path $KeyResultsDir "$($ProjectName)_Screenshot.png"
        Copy-Item -Path $LatestScreenshot -Destination $TargetScreenshot -Force
        Write-Host "Screenshot saved to $TargetScreenshot"
    } else {
        Write-Host "No screenshot found in $ScreenshotsDir" -ForegroundColor Yellow
    }
} else {
    Write-Host "Screenshot directory not found: $ScreenshotsDir" -ForegroundColor Yellow
}

Write-Host "Done!"
