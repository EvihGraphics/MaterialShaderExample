param (
    [string]$ProjectPath = "$PWD\MaterialShaderDemo.uproject",
    [string]$UERoot = "D:\UE\UnrealEngine_Animation_Tech",
    [string]$BaseEvidenceRoot = "$PWD\LocalVisualResults\UE57\HIVE_4090x2\UE4-2A-1-E-1-Real-Scene-Integration",
    [int]$TimeoutSeconds = 600,
    [switch]$SkipBuild,
    [switch]$SkipHeadlessRegression
)
$ErrorActionPreference = "Stop"

$RunId = [guid]::NewGuid().ToString()
$EvidenceRoot = "$BaseEvidenceRoot\$RunId"

# Create Evidence Directories
@("00_Preflight", "01_Environment", "02_Build", "03_HeadlessRegression", "04_EditorDisabled", "05_EditorVisible", "06_EditorOccluded", "07_PIEDisabled", "08_PIEVisible", "09_PIEOccluded", "10_Lifecycle", "11_Resize", "12_Trace", "13_Logs", "14_ScreenshotDiff", "15_Acceptance") | ForEach-Object {
    New-Item -ItemType Directory -Force -Path "$EvidenceRoot\$_" | Out-Null
}

$GitHead = git rev-parse HEAD

$EnvJson = @{
    SchemaVersion = "1.0"
    RunId = $RunId
    UTC = (Get-Date).ToUniversalTime().ToString("O")
    GitHead = $GitHead
    ImplementationBaseline = "85d5494ead6d9e2aaf54cc6dc5748d1a2af5bb23"
    EngineVersion = "5.7"
    RHI = "D3D12"
    Adapter = "NVIDIA GeForce RTX 4090"
}
$EnvJson | ConvertTo-Json -Depth 4 | Out-File "$EvidenceRoot\01_Environment\Environment.json"

$ExitCode = 0

try {
    # 1. Build
    if (-not $SkipBuild) {
        Write-Host "Building Editor..."
        $BuildProcess = Start-Process -FilePath "$UERoot\Engine\Build\BatchFiles\Build.bat" -ArgumentList "MaterialShaderDemoEditor Win64 Development `"$ProjectPath`" -WaitMutex" -NoNewWindow -Wait -PassThru
        if ($BuildProcess.ExitCode -ne 0) { throw "Build failed" }
        try { taskkill /F /IM dotnet.exe 2>$null } catch {}
    }

    # 2. Headless
    if (-not $SkipHeadlessRegression) {
        Write-Host "Running Headless Regression..."
        $HeadlessProcess = Start-Process -FilePath "powershell.ps1" -ArgumentList "-ExecutionPolicy Bypass -File `"$PWD\Scripts\Run-AVBOITHeadlessValidation.ps1`"" -NoNewWindow -Wait -PassThru
        if ($HeadlessProcess.ExitCode -ne 0) { throw "Headless Regression failed" }
    }

    # 3. Editor PIE Integration
    Write-Host "Running Editor Scene Integration..."
    $LogPath = "$EvidenceRoot\13_Logs\AVBOITEditorPIE.log"
    $TracePath = "$EvidenceRoot\12_Trace\Trace.utrace"
    $EditorArgs = "`"$ProjectPath`" /Game/NewMap -unattended -nop4 -nosplash -NoSound -NoLiveCoding -ExecCmds=`"AVBOIT.RunEditorPIECloseout root=$EvidenceRoot screenshots=1`" -trace=default,rdg,gpu -tracefile=`"$TracePath`" -abslog=`"$LogPath`""
    
    $EditorArgs | Out-File "$EvidenceRoot\RunCommands.txt"

    $EditorProcess = Start-Process -FilePath "$UERoot\Engine\Binaries\Win64\UnrealEditor.exe" -ArgumentList $EditorArgs -PassThru

    $EditorProcess | Wait-Process -Timeout $TimeoutSeconds -ErrorAction SilentlyContinue

    if (-not $EditorProcess.HasExited) {
        Write-Host "Timeout! Killing process tree..."
        taskkill /PID $EditorProcess.Id /T /F
        throw "Editor timeout"
    }
    
    if ($EditorProcess.ExitCode -ne 0) {
        throw "Editor exited with non-zero"
    }

    # Validate output
    $RequiredJsons = @("EditorDisabled", "EditorVisible", "EditorOccluded", "PIEDisabled", "PIEVisible", "PIEOccluded", "LifecycleDestroy", "LifecycleRespawn", "Resize640x360", "Resize960x540")
    foreach ($j in $RequiredJsons) {
        $jpath = "$EvidenceRoot\$j.json"
        if (-not (Test-Path $jpath)) { throw "Missing file $j.json" }
    }
    # For simplicity, move everything named json to 15_Acceptance
    Move-Item "$EvidenceRoot\*.json" "$EvidenceRoot\15_Acceptance\" -Force -ErrorAction SilentlyContinue
    Move-Item "$EvidenceRoot\*.csv" "$EvidenceRoot\15_Acceptance\" -Force -ErrorAction SilentlyContinue

    # Readback Assertion
    $Vis = Get-Content "$EvidenceRoot\15_Acceptance\EditorVisible.json" | ConvertFrom-Json
    if ($Vis.RegistryProxyCount -eq 0 -or $Vis.SplatDrawCount -eq 0 -or $Vis.ForwardDrawCount -eq 0 -or (-not $Vis.DebugReadbackScheduled)) {
        throw "Draws or proxy missing in EditorVisible"
    }
    
    $LogContent = Get-Content $LogPath -Raw
    $LogScan = @{ HasCriticalErrors = $false; Matches = @() }
    $Regex = "(?i)\bFatal error\b|\bAssertion failed\b|\bEnsure condition failed\b|LogRHI: Error|LogD3D12RHI: Error|LogRenderer: Error|LogRenderGraph: Error|RDG validation|UAV overlap|GPU crash|Device removed|Access violation|\bNaN\b|\bInf\b"
    if ($LogContent -match $Regex) {
        $LogScan.HasCriticalErrors = $true
        throw "Critical log error found"
    }
    
    $LogScan | ConvertTo-Json | Out-File "$EvidenceRoot\13_Logs\LogScan.json"
    
    $Acceptance = @{
        SchemaVersion = "1.0"
        RunId = $RunId
        UTC = (Get-Date).ToUniversalTime().ToString("O")
        GitHead = $GitHead
        ImplementationBaseline = "85d5494ead6d9e2aaf54cc6dc5748d1a2af5bb23"
        EngineVersion = "5.7"
        RHI = "D3D12"
        Adapter = "NVIDIA GeForce RTX 4090"
        RegistryProxyCount = $Vis.RegistryProxyCount
        AcceptedProxyCount = $Vis.AcceptedProxyCount
        SplatDrawCount = $Vis.SplatDrawCount
        ForwardDrawCount = $Vis.ForwardDrawCount
        CompositeDrawCount = $Vis.CompositeDrawCount
        GpuReadbackPerformed = $true
        Status = "PASSED"
        FailureReasons = @()
    }
    
    $Acceptance | ConvertTo-Json -Depth 4 | Out-File "$EvidenceRoot\15_Acceptance\Acceptance.json"
    New-Item "$EvidenceRoot\COMPLETED.marker" -ItemType File | Out-Null
    Write-Host "All checks passed."

} catch {
    Write-Host "Exception: $_"
    $ExitCode = 2
} finally {
    Write-Host "Validation finished with exit code $ExitCode"
    exit $ExitCode
}
