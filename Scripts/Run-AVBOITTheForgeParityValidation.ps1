param (
    [string]$UERoot = "D:\UE\UnrealEngine_Animation_Tech",
    [string]$ProjectPath = "",
    [string]$TheForgeRoot = "",
    [string]$EvidenceRoot = "",
    [int]$ResolutionX = 1920,
    [int]$ResolutionY = 1080,
    [switch]$SkipBuild,
    [switch]$SkipReferenceBuild,
    [string]$CaptureTool = "RenderDoc",
    [int]$TimeoutSeconds = 600
)

$ErrorActionPreference = "Stop"

if (-not $ProjectPath) {
    $ProjectPath = Join-Path (git rev-parse --show-toplevel).Trim() "MaterialShaderDemo.uproject"
}
if (-not $TheForgeRoot) {
    $TheForgeRoot = Join-Path (git rev-parse --show-toplevel).Trim() "../The-Forge"
}
if (-not $EvidenceRoot) {
    $EvidenceRoot = Join-Path (git rev-parse --show-toplevel).Trim() "LocalVisualResults/UE57/HIVE_4090x2/UE4-2B-TheForgeParity"
}

$Timestamp = (Get-Date).ToUniversalTime().ToString("yyyyMMddTHHmmssZ")
$CurrentEvidenceDir = Join-Path $EvidenceRoot $Timestamp
New-Item -ItemType Directory -Force -Path $CurrentEvidenceDir | Out-Null

$Result = @{ Passed = $false; Steps = @{} }

try {
    # 1. Git State Record
    $Result.Steps.GitState = @{
        UEHead = (git -C $UERoot rev-parse HEAD 2>$null)
        ProjectHead = (git rev-parse HEAD 2>$null)
        TheForgeHead = (git -C $TheForgeRoot rev-parse HEAD 2>$null)
    }
    
    # 2. UE Build Version
    if (Test-Path "$UERoot\Engine\Build\Build.version") {
        $Result.Steps.UEBuildVersion = Get-Content "$UERoot\Engine\Build\Build.version" | ConvertFrom-Json
    }
    
    # 3 & 4. UERP_Ref Build
    if (-not $SkipReferenceBuild) {
        Write-Host "Building References..."
        $RefScript = Join-Path (git rev-parse --show-toplevel).Trim() "Scripts/Build-UERPRefs.ps1"
        if (Test-Path $RefScript) {
            & $RefScript -UERoot $UERoot
        }
    }
    
    # 5. Build Project
    if (-not $SkipBuild) {
        Write-Host "Building MaterialShaderDemoEditor..."
        $BuildBat = "$UERoot\Engine\Build\BatchFiles\Build.bat"
        $CmdArgs = "MaterialShaderDemoEditor", "Win64", "Development", "-Project=`"$ProjectPath`"", "-WaitMutex", "-NoHotReloadFromIDE"
        $process = Start-Process -FilePath $BuildBat -ArgumentList $CmdArgs -Wait -NoNewWindow -PassThru
        if ($process.ExitCode -ne 0) { throw "Project Build Failed" }
    }
    
    # 6. Execute Parity Automation
    Write-Host "Running Parity Validation (This requires PIE and Commandlets)..."
    $EditorCmd = "$UERoot\Engine\Binaries\Win64\UnrealEditor-Cmd.exe"
    $ExecArgs = "`"$ProjectPath`" -ExecCmds=`"Automation RunTests AVBOIT.Parity; Quit`" -unattended -nopause -NoUI -nosound -windowed -resx=$ResolutionX -resy=$ResolutionY"
    
    $TestLog = Join-Path $CurrentEvidenceDir "Automation.log"
    $process = Start-Process -FilePath $EditorCmd -ArgumentList $ExecArgs -Wait -NoNewWindow -PassThru -RedirectStandardOutput $TestLog -RedirectStandardError $TestLog
    
    if ($process.ExitCode -ne 0) { throw "Automation Execution Failed" }
    
    # 13. Log Scan
    $Errors = Get-Content $TestLog | Where-Object { $_ -match "Error:" }
    if ($Errors) { throw "Found errors in log: $($Errors[0])" }
    
    $Result.Passed = $true
} catch {
    Write-Host "Validation Failed: $_" -ForegroundColor Red
    $Result.Error = $_.Exception.Message
    $Result.Passed = $false
} finally {
    $Result | ConvertTo-Json -Depth 5 | Out-File (Join-Path $CurrentEvidenceDir "Acceptance.json")
    if ($Result.Passed) { exit 0 } else { exit 1 }
}
