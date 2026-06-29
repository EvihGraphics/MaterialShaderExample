param (
    [string]$ProjectPath = "$PWD\MaterialShaderDemo.uproject",
    [string]$UERoot = "D:\UE\UnrealEngine_Animation_Tech",
    [string]$BaseEvidenceRoot = "$PWD\LocalVisualResults\TempResults\UE57\HIVE_4090x2\UE4-2A-1-H-1-Real-Headless-GPU",
    [string]$KeyEvidenceRoot = "$PWD\LocalVisualResults\KeyResults\UE57\HIVE_4090x2\UE4-2A-1-H-1-Real-Headless-GPU",
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
New-Item -ItemType Directory -Force -Path "$EvidenceRoot\12_Trace" | Out-Null
New-Item -ItemType Directory -Force -Path "$EvidenceRoot\13_Logs" | Out-Null
New-Item -ItemType Directory -Force -Path "$EvidenceRoot\14_Acceptance" | Out-Null
New-Item -ItemType Directory -Force -Path "$EvidenceRoot\16_LogScan" | Out-Null
New-Item -ItemType Directory -Force -Path "$EvidenceRoot\17_Acceptance" | Out-Null

$StartTime = Get-Date
$ExitCode = 0
$GitHead = (git rev-parse HEAD).Trim()

function Get-SHA256($Path) {
    return (Get-FileHash $Path -Algorithm SHA256).Hash
}

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
        
        # Check Markers
        if (Test-Path "$EvidenceRoot\RUNNING.marker") { Write-Host "RUNNING.marker still exists!"; $ExitCode = 5; return }
        if (-not (Test-Path "$EvidenceRoot\COMPLETED.marker")) { Write-Host "COMPLETED.marker missing!"; $ExitCode = 5; return }

        # Check Synthetic
        $SynthJsonPath = "$EvidenceRoot\05_Synthetic\SyntheticSummary.json"
        if (-not (Test-Path $SynthJsonPath)) { Write-Host "Missing SyntheticSummary.json"; $ExitCode = 5; return }
        $SynthJson = Get-Content $SynthJsonPath -Raw | ConvertFrom-Json
        if ($SynthJson.FailedCases -ne 0 -or $SynthJson.Status -ne "PASSED" -or $SynthJson.RunId -ne $RunId) { Write-Host "Synthetic Failed"; $ExitCode = 5; return }

        # Check Raster Summary
        $RasterJsonPath = "$EvidenceRoot\RasterDirectSummary.json"
        if (-not (Test-Path $RasterJsonPath)) { Write-Host "Missing RasterDirectSummary.json"; $ExitCode = 5; return }
        $RasterJson = Get-Content $RasterJsonPath -Raw | ConvertFrom-Json
        if ($RasterJson.RunId -ne $RunId -or $RasterJson.GpuReadbackPerformed -ne $true -or $RasterJson.RealRasterDrawPerformed -ne $true -or $RasterJson.FailedCases -ne 0 -or $RasterJson.Status -ne "PASSED") {
            Write-Host "Raster Summary Failed"
            $ExitCode = 5
            return
        }

        # Check 6 Individual Cases
        $Cases = @("VisibleWhite", "Occlusion", "NonBlackComposite", "DepthNear", "DepthMiddle", "DepthFar")
        $Hashes = @{}
        foreach ($c in $Cases) {
            $CPath = "$EvidenceRoot\$c.json"
            if (-not (Test-Path $CPath)) { Write-Host "Missing case json $CPath"; $ExitCode = 5; return }
            $H = Get-SHA256 $CPath
            if ($Hashes.ContainsKey($H)) { Write-Host "Case JSONs are identical! Forgery detected."; $ExitCode = 5; return }
            $Hashes[$H] = $true

            $CJson = Get-Content $CPath -Raw | ConvertFrom-Json
            if ($CJson.Status -ne "PASSED" -or $CJson.Name -ne $c) { Write-Host "Case $c failed"; $ExitCode = 5; return }
        }

        # Check CSVs
        $CSVs = @("VisibleWhite", "Occlusion", "DepthNear", "DepthMiddle", "DepthFar")
        foreach ($c in $CSVs) {
            $CsvPath = "$EvidenceRoot\${c}_ExtinctionTransmittance.csv"
            if (-not (Test-Path $CsvPath)) { Write-Host "Missing CSV $CsvPath"; $ExitCode = 5; return }
            $CsvLines = Get-Content $CsvPath
            if ($CsvLines.Count -ne 65) { Write-Host "CSV line count != 65"; $ExitCode = 5; return }
            if ($CsvLines[1] -match "1,2,3") { Write-Host "CSV contains fake 1,2,3 data"; $ExitCode = 5; return }
        }

        # Check Trace
        if (-not (Test-Path $TracePath) -or (Get-Item $TracePath).Length -lt 65536) { Write-Host "Trace file missing or too small"; $ExitCode = 5; return }

        # Runtime Error Scan
        $LogContent = Get-Content $LogPath -Raw
        if ($LogContent -match "Fatal error|Assertion failed|Ensure condition failed|LogRHI: Error|LogD3D12RHI: Error|LogRenderer: Error|LogRenderGraph: Error|RDG validation|UAV overlap|GPU crash|Device removed|Access violation|Readback Timeout|\bNaN\b|\bInf\b") {
            Write-Host "Critical errors found in log!"
            $ExitCode = 5
            return
        }

        # Evidence Provenance Scan
        $DirScanList = @(
            "D:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT\Plugins\MaterialShaderPlugin\Source",
            $EvidenceRoot
        )
        foreach ($Dir in $DirScanList) {
            $Files = Get-ChildItem -Path $Dir -File -Recurse
            foreach ($F in $Files) {
                # skip checking our own scanner script if it accidentally matches
                if ($F.Extension -eq ".ps1") { continue }
                $FContent = Get-Content $F.FullName -Raw
                if ($FContent -match "Dummy evidence|stubbed evidence|placeholder result|fixed pass") {
                    Write-Host "Provenance failed! Found forged evidence in $($F.FullName)"
                    $ExitCode = 5
                    return
                }
            }
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

    if ($ExitCode -eq 0) {
        # Copy Key Results to KeyEvidenceRoot
        $KeyRunRoot = "$KeyEvidenceRoot\$RunId"
        New-Item -ItemType Directory -Force -Path $KeyRunRoot | Out-Null
        Copy-Item -Path "$EvidenceRoot\17_Acceptance\Acceptance.json" -Destination "$KeyRunRoot\" -Force
        
        $Screenshots = Get-ChildItem -Path $EvidenceRoot -Recurse -Filter "*.png"
        foreach ($S in $Screenshots) {
            $Rel = $S.FullName.Substring($EvidenceRoot.Length + 1)
            $Dest = "$KeyRunRoot\$Rel"
            $DestDir = Split-Path $Dest -Parent
            if (-not (Test-Path $DestDir)) {
                New-Item -ItemType Directory -Force -Path $DestDir | Out-Null
            }
            Copy-Item -Path $S.FullName -Destination $Dest -Force
        }
        Write-Host "Key results successfully copied to $KeyRunRoot"
    }

    Write-Host "Validation finished with exit code $ExitCode"
    exit $ExitCode
}
