param (
    [string]$ProjectPath = "$PWD\MaterialShaderDemo.uproject",
    [string]$UERoot = "D:\UE\UnrealEngine_Animation_Tech",
    [string]$BaseEvidenceRoot = "$PWD\LocalVisualResults\UE57\HIVE_4090x2\UE4-2A-1-H-1-Real-Headless-GPU",
    [int]$TimeoutSeconds = 600,
    [switch]$SkipBuild
)

$ErrorActionPreference = "Stop"

$RunId = [guid]::NewGuid().ToString()
$EvidenceRoot = "$BaseEvidenceRoot\$RunId"

if (Test-Path $EvidenceRoot) {
    Remove-Item -Recurse -Force $EvidenceRoot
}

New-Item -ItemType Directory -Force -Path "$EvidenceRoot\00_Preflight" | Out-Null
New-Item -ItemType Directory -Force -Path "$EvidenceRoot\01_Environment" | Out-Null
New-Item -ItemType Directory -Force -Path "$EvidenceRoot\02_PluginDescriptorCheck" | Out-Null
New-Item -ItemType Directory -Force -Path "$EvidenceRoot\03_Build" | Out-Null
New-Item -ItemType Directory -Force -Path "$EvidenceRoot\04_ShaderCompile" | Out-Null
New-Item -ItemType Directory -Force -Path "$EvidenceRoot\05_Synthetic" | Out-Null
New-Item -ItemType Directory -Force -Path "$EvidenceRoot\06_VisibleWhite" | Out-Null
New-Item -ItemType Directory -Force -Path "$EvidenceRoot\07_Occlusion" | Out-Null
New-Item -ItemType Directory -Force -Path "$EvidenceRoot\08_NonBlackComposite" | Out-Null
New-Item -ItemType Directory -Force -Path "$EvidenceRoot\09_DepthNear" | Out-Null
New-Item -ItemType Directory -Force -Path "$EvidenceRoot\10_DepthMiddle" | Out-Null
New-Item -ItemType Directory -Force -Path "$EvidenceRoot\11_DepthFar" | Out-Null
New-Item -ItemType Directory -Force -Path "$EvidenceRoot\12_Trace" | Out-Null
New-Item -ItemType Directory -Force -Path "$EvidenceRoot\13_Logs" | Out-Null
New-Item -ItemType Directory -Force -Path "$EvidenceRoot\14_Acceptance" | Out-Null
New-Item -ItemType Directory -Force -Path "$EvidenceRoot\16_LogScan" | Out-Null
New-Item -ItemType Directory -Force -Path "$EvidenceRoot\17_Acceptance" | Out-Null

$StartTime = Get-Date
$ExitCode = 0
$GitHead = (git rev-parse HEAD).Trim()

try {
    # 1. Environment & Git State
    $GitState = @{ HEAD = $GitHead }
    $GitState | ConvertTo-Json | Out-File "$EvidenceRoot\01_Environment\GitState.json" -Force

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

    # 4. Raster Headless Suite via Commandlet
    Write-Host "Running Commandlet..."
    $LogPath = "$EvidenceRoot\13_Logs\AVBOITHeadless.log"
    $TracePath = "$EvidenceRoot\12_Trace\AVBOITDirectGPU.utrace"
    $Args = "`"$ProjectPath`" -run=AVBOITHeadless -AllowCommandletRendering -RenderOffscreen -dx12 -unattended -nop4 -nosplash -NoSound -NoLiveCoding -AVBOITSuite=All -AVBOITRunId=`"$RunId`" -AVBOITEvidenceRoot=`"$EvidenceRoot`" -trace=default,rdg,gpu -tracefile=`"$TracePath`" -abslog=`"$LogPath`""
    
    $Args | Out-File "$EvidenceRoot\RunCommands.txt"

    $RasterProcess = Start-Process -FilePath "$UERoot\Engine\Binaries\Win64\UnrealEditor-Cmd.exe" -ArgumentList $Args -NoNewWindow -PassThru

    $RasterProcess | Wait-Process -Timeout $TimeoutSeconds -ErrorAction SilentlyContinue

    if (-not $RasterProcess.HasExited) {
        Write-Host "Timeout! Killing process tree..."
        taskkill /PID $RasterProcess.Id /T /F
        $ExitCode = 4
    } else {
        $ExitCode = $RasterProcess.ExitCode
        if ($ExitCode -ne 0) {
            Write-Host "Commandlet failed with exit code $ExitCode"
            return
        }
        
        # Verify Output
        if (Test-Path "$EvidenceRoot\RUNNING.marker") {
            Write-Host "RUNNING.marker still exists!"
            $ExitCode = 5
            return
        }

        # Verify JSON
        $RasterJsonPath = "$EvidenceRoot\RasterDirectSummary.json"
        if (-not (Test-Path $RasterJsonPath)) {
            Write-Host "Missing RasterDirectSummary.json"
            $ExitCode = 5
            return
        }
        
        $JsonContent = Get-Content $RasterJsonPath | ConvertFrom-Json
        if ($JsonContent.RunId -ne $RunId -or $JsonContent.GpuReadbackPerformed -ne $true -or $JsonContent.RealRasterDrawPerformed -ne $true) {
            Write-Host "JSON verification failed"
            $ExitCode = 5
            return
        }
        
        # Verify Trace
        if (-not (Test-Path $TracePath) -or (Get-Item $TracePath).Length -lt 65536) {
            Write-Host "Trace file missing or too small"
            $ExitCode = 5
            return
        }

        # Log Scanning
        $LogContent = Get-Content $LogPath -Raw
        if ($LogContent -match "Fatal error|Assertion failed|Ensure condition failed|LogRHI: Error|LogD3D12RHI: Error|LogRenderer: Error|LogRenderGraph: Error|GPU crash|Device removed|Access violation") {
            Write-Host "Critical errors found in log!"
            $ExitCode = 5
            return
        }
        
        Write-Host "All checks passed."
    }

} catch {
    Write-Host "Exception: $_"
    $ExitCode = 2
} finally {
    # Restore plugins
    .\Scripts\Test-UEPluginDescriptors.ps1 -UERoot $UERoot -EvidenceRoot $EvidenceRoot -Restore
    if ($LASTEXITCODE -eq 8) {
        $ExitCode = 8
    }
    
    $Acceptance = @{
        ExitCode = $ExitCode
        Duration = (Get-Date) - $StartTime
    }
    $Acceptance | ConvertTo-Json | Out-File "$EvidenceRoot\17_Acceptance\Acceptance.json" -Force

    Write-Host "Validation finished with exit code $ExitCode"
    exit $ExitCode
}
