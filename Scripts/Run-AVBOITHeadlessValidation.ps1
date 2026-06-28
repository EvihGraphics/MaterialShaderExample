param (
    [string]$ProjectPath = "d:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT\MaterialShaderDemo.uproject",
    [string]$UERoot = "D:\UE\UnrealEngine_Animation_Tech",
    [string]$EvidenceRoot = "LocalVisualResults\UE57\HIVE_4090x2\UE4-2A-1-H-Headless-Automated-Acceptance",
    [int]$TimeoutSeconds = 600,
    [switch]$SkipBuild,
    [switch]$KeepTemporaryFiles
)

$ErrorActionPreference = "Stop"

if (!(Test-Path "$EvidenceRoot\00_Preflight")) { New-Item -ItemType Directory -Force -Path "$EvidenceRoot\00_Preflight" | Out-Null }
if (!(Test-Path "$EvidenceRoot\01_Environment")) { New-Item -ItemType Directory -Force -Path "$EvidenceRoot\01_Environment" | Out-Null }
if (!(Test-Path "$EvidenceRoot\02_PluginDescriptorCheck")) { New-Item -ItemType Directory -Force -Path "$EvidenceRoot\02_PluginDescriptorCheck" | Out-Null }
if (!(Test-Path "$EvidenceRoot\03_Build")) { New-Item -ItemType Directory -Force -Path "$EvidenceRoot\03_Build" | Out-Null }
if (!(Test-Path "$EvidenceRoot\17_Acceptance")) { New-Item -ItemType Directory -Force -Path "$EvidenceRoot\17_Acceptance" | Out-Null }

$StartTime = Get-Date
$ExitCode = 0

try {
    # 1. Environment & Git State
    git rev-parse HEAD > "$EvidenceRoot\01_Environment\GitState.txt"

    # 2. Plugin Descriptors
    .\Scripts\Test-UEPluginDescriptors.ps1 -UERoot $UERoot -EvidenceRoot $EvidenceRoot

    # 3. Build
    if (-not $SkipBuild) {
        Write-Host "Building Editor..."
        $BuildProcess = Start-Process -FilePath "$UERoot\Engine\Build\BatchFiles\Build.bat" -ArgumentList "MaterialShaderDemoEditor Win64 Development `"$ProjectPath`" -WaitMutex" -NoNewWindow -Wait -PassThru
        if ($BuildProcess.ExitCode -ne 0) {
            Write-Host "Build failed."
            $ExitCode = 6
            return
        }
    }

    # 4. Raster Headless Suite
    Write-Host "Running Raster Headless Suite..."
    $RasterProcess = Start-Process -FilePath "$UERoot\Engine\Binaries\Win64\UnrealEditor-Cmd.exe" -ArgumentList "`"$ProjectPath`" -unattended -nop4 -nosplash -NoSound -NoLiveCoding -RenderOffscreen -dx12 -ResX=512 -ResY=512 -ForceRes -AVBOITRasterHeadlessSuite -AVBOITEvidenceRoot=`"$EvidenceRoot`" -trace=default,rdg,gpu -tracefile=`"AVBOITHeadless.utrace`" -abslog=`"$PWD\Saved\Logs\RasterHeadless.log`"" -NoNewWindow -PassThru

    $RasterProcess | Wait-Process -Timeout $TimeoutSeconds -ErrorAction SilentlyContinue

    if (-not $RasterProcess.HasExited) {
        Write-Host "Timeout! Killing process..."
        Stop-Process -Id $RasterProcess.Id -Force
        $ExitCode = 4
    } else {
        if ($RasterProcess.ExitCode -ne 0) {
            Write-Host "Headless suite failed with exit code $($RasterProcess.ExitCode)"
            $ExitCode = $RasterProcess.ExitCode
        } else {
            Write-Host "Headless suite passed."
            $ExitCode = 0
        }
    }

} catch {
    Write-Host "Exception: $_"
    $ExitCode = 2
} finally {
    # Restore plugins
    .\Scripts\Test-UEPluginDescriptors.ps1 -UERoot $UERoot -EvidenceRoot $EvidenceRoot -Restore
    
    # Save Acceptance Result
    $Acceptance = @{
        ExitCode = $ExitCode
        Duration = (Get-Date) - $StartTime
    }
    $Acceptance | ConvertTo-Json | Out-File "$EvidenceRoot\17_Acceptance\Acceptance.json" -Force

    Write-Host "Validation finished with exit code $ExitCode"
    exit $ExitCode
}
