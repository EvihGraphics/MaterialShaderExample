param (
    [string]$ProjectPath = "$PWD\MaterialShaderDemo.uproject",
    [string]$UERoot = "D:\UE\UnrealEngine_Animation_Tech",
    [string]$BaseEvidenceRoot = "$PWD\LocalVisualResults\UE57\HIVE_4090x2\UE4-2A-1-E-Real-Editor-Smoke",
    [int]$TimeoutSeconds = 600,
    [switch]$SkipBuild
)

$ErrorActionPreference = "Stop"

$RunId = [guid]::NewGuid().ToString()
$EvidenceRoot = "$BaseEvidenceRoot\$RunId"

if (Test-Path $EvidenceRoot) {
    Remove-Item -Recurse -Force $EvidenceRoot
}

New-Item -ItemType Directory -Force -Path "$EvidenceRoot" | Out-Null
New-Item -ItemType Directory -Force -Path "$EvidenceRoot\13_Logs" | Out-Null

$StartTime = Get-Date
$ExitCode = 0

try {
    # 1. Build
    if (-not $SkipBuild) {
        Write-Host "Building Editor..."
        $BuildProcess = Start-Process -FilePath "$UERoot\Engine\Build\BatchFiles\Build.bat" -ArgumentList "MaterialShaderDemoEditor Win64 Development `"$ProjectPath`" -WaitMutex" -NoNewWindow -Wait -PassThru
        if ($BuildProcess.ExitCode -ne 0) {
            Write-Host "Build failed."
            $ExitCode = 6
            return
        }
    }

    # 2. Editor PIE Closeout
    Write-Host "Running Editor PIE Closeout..."
    $LogPath = "$EvidenceRoot\13_Logs\AVBOITEditorPIE.log"
    
    # Launch UnrealEditor.exe (with UI), run map /Game/NewMap, and execute AVBOIT.RunEditorPIECloseout
    $Args = "`"$ProjectPath`" /Game/NewMap -unattended -nop4 -nosplash -NoSound -ExecCmds=`"AVBOIT.RunEditorPIECloseout root=$EvidenceRoot screenshots=1`" -abslog=`"$LogPath`""
    
    $Args | Out-File "$EvidenceRoot\RunCommands.txt"

    $EditorProcess = Start-Process -FilePath "$UERoot\Engine\Binaries\Win64\UnrealEditor.exe" -ArgumentList $Args -PassThru

    $EditorProcess | Wait-Process -Timeout $TimeoutSeconds -ErrorAction SilentlyContinue

    if (-not $EditorProcess.HasExited) {
        Write-Host "Timeout! Killing process tree..."
        taskkill /PID $EditorProcess.Id /T /F
        $ExitCode = 4
    } else {
        $ExitCode = $EditorProcess.ExitCode
        if ($ExitCode -ne 0) {
            Write-Host "Editor failed with exit code $ExitCode"
            return
        }
        
        # Check Screenshots
        $Screenshots = @("Editor/01_Mode1_Deterministic.png", "PIE/01_Mode1.png")
        foreach ($s in $Screenshots) {
            $SPath = "$EvidenceRoot\$s"
            if (-not (Test-Path $SPath)) { Write-Host "Missing screenshot $SPath"; $ExitCode = 5; return }
        }

        # Runtime Error Scan
        $LogContent = Get-Content $LogPath -Raw
        if ($LogContent -match "(?i)Fatal error|Assertion failed|Ensure condition failed|LogRHI: Error|LogD3D12RHI: Error|LogRenderer: Error|LogRenderGraph: Error|RDG validation|UAV overlap|GPU crash|Device removed|Access violation|\bNaN\b|\bInf\b") {
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
    Write-Host "Validation finished with exit code $ExitCode"
    exit $ExitCode
}
