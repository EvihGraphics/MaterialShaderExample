param (
    [string]$UERoot = "D:\UE\UnrealEngine_Animation_Tech",
    [string]$Reference = "All",
    [switch]$Clean,
    [switch]$SkipProjectGeneration,
    [switch]$KeepStaging
)

$ErrorActionPreference = "Stop"
$RepoRoot = (git rev-parse --show-toplevel).Trim()
$BuildScratchRoot = Join-Path (Split-Path -Path $RepoRoot -Qualifier) "_UERPRefBuild"
$StagingRoot = Join-Path $BuildScratchRoot "S"
$HarnessRoot = Join-Path $BuildScratchRoot "H"
$EvidenceDir = Join-Path $RepoRoot "LocalVisualResults/UE57/HIVE_4090x2/UERP_Ref"
$Utf8NoBom = New-Object System.Text.UTF8Encoding($false)

function Write-JsonFileUtf8NoBom {
    param (
        [Parameter(Mandatory=$true)] [object]$Value,
        [Parameter(Mandatory=$true)] [string]$Path
    )

    [System.IO.File]::WriteAllText($Path, ($Value | ConvertTo-Json -Depth 16), $Utf8NoBom)
}

function Write-TextFileUtf8NoBom {
    param (
        [Parameter(Mandatory=$true)] [string]$Value,
        [Parameter(Mandatory=$true)] [string]$Path
    )

    [System.IO.File]::WriteAllText($Path, $Value, $Utf8NoBom)
}

if ($Clean) {
    if (Test-Path $StagingRoot) { Remove-Item -Recurse -Force $StagingRoot }
    if (Test-Path $HarnessRoot) { Remove-Item -Recurse -Force $HarnessRoot }
}

$BuildBat = "$UERoot\Engine\Build\BatchFiles\Build.bat"
if (-not (Test-Path $BuildBat)) {
    Write-Error "UE Build script not found at $BuildBat"
}

$Timestamp = (Get-Date).ToUniversalTime().ToString("yyyyMMddTHHmmssZ")
$RefsToBuild = @()

$ManifestPath = Join-Path $RepoRoot "UERP_Ref/REFERENCE_MANIFEST.json"
$Manifest = Get-Content $ManifestPath | ConvertFrom-Json

if ($Reference -eq "All") {
    $RefsToBuild = $Manifest
} else {
    $RefsToBuild = $Manifest | Where-Object { $_.Name -eq $Reference }
}

foreach ($Ref in $RefsToBuild) {
    Write-Host "Building reference: $($Ref.Name)" -ForegroundColor Cyan
    $RefEvidence = Join-Path $EvidenceDir "$($Ref.Name)/$Timestamp"
    New-Item -ItemType Directory -Force -Path $RefEvidence | Out-Null
    
    $RefSourceDir = Join-Path $RepoRoot "UERP_Ref/$($Ref.Name)"
    $LogFile = Join-Path $RefEvidence "BuildOutput.log"
    $ExitCode = 0
    $EffectiveRepositoryType = $Ref.RepositoryType
    $ProjectDescriptor = Get-ChildItem -Path $RefSourceDir -Filter "*.uproject" -Recurse | Select-Object -First 1
    $PluginDescriptor = Get-ChildItem -Path $RefSourceDir -Filter "*.uplugin" -Recurse | Select-Object -First 1

    if ($EffectiveRepositoryType -eq "Project" -and -not $ProjectDescriptor -and $PluginDescriptor) {
        $EffectiveRepositoryType = "Plugin"
        "Manifest marks this reference as Project, but no .uproject exists. Built as plugin harness." | Out-File (Join-Path $RefEvidence "CompatibilityChanges.md")
    }
    
    # 1. Gather info
    $SelectedCommit = (git -C $RefSourceDir rev-parse HEAD)
    $SelectedCommit | Out-File (Join-Path $RefEvidence "SelectedCommit.json")
    
    @{ UERoot=$UERoot; Time=$Timestamp; Machine=$env:COMPUTERNAME } | ConvertTo-Json | Out-File (Join-Path $RefEvidence "Environment.json")
    
    if ($EffectiveRepositoryType -eq "Project") {
        # Build Project
        $ProjStaging = Join-Path $StagingRoot $Ref.Name
        New-Item -ItemType Directory -Force -Path $ProjStaging | Out-Null
        Copy-Item -Recurse -Force "$RefSourceDir\*" $ProjStaging
        
        # Apply patches if exist
        $PatchDir = Join-Path $RepoRoot "UERP_Ref/Patches/$($Ref.Name)"
        if (Test-Path $PatchDir) {
            Copy-Item -Recurse -Force "$PatchDir\*" $ProjStaging
            "Applied patches from $PatchDir" | Out-File (Join-Path $RefEvidence "CompatibilityChanges.md") -Append
        } else {
            "No patches applied." | Out-File (Join-Path $RefEvidence "CompatibilityChanges.md")
        }
        
        $UProjectFile = Get-ChildItem -Path $ProjStaging -Filter "*.uproject" | Select-Object -First 1
        if ($UProjectFile) {
            $ProjectName = $UProjectFile.BaseName
            $CmdArgs = "$ProjectName`Editor", "Win64", "Development", "-Project=`"$($UProjectFile.FullName)`"", "-WaitMutex", "-NoHotReloadFromIDE"
            $CmdArgs -join " " | Out-File (Join-Path $RefEvidence "BuildCommand.txt")
            
            Write-Host "Running: $BuildBat $CmdArgs"
            $ErrorLogFile = Join-Path $RefEvidence "BuildError.log"
            $process = Start-Process -FilePath $BuildBat -ArgumentList $CmdArgs -Wait -NoNewWindow -PassThru -RedirectStandardOutput $LogFile -RedirectStandardError $ErrorLogFile
            $ExitCode = $process.ExitCode
        } else {
            Write-Error "No uproject found in $ProjStaging"
            $ExitCode = -1
        }
    } elseif ($EffectiveRepositoryType -eq "Plugin") {
        # Build Plugin via dummy project harness
        $ProjHarness = Join-Path $HarnessRoot $Ref.Name
        New-Item -ItemType Directory -Force -Path $ProjHarness | Out-Null

        $SourcePluginDescriptor = $PluginDescriptor
        if (-not $SourcePluginDescriptor) {
            Write-Error "No uplugin found in $RefSourceDir"
            $ExitCode = -1
            continue
        }
        $PluginName = $SourcePluginDescriptor.BaseName

        $SourceDir = Join-Path $ProjHarness "Source/Harness"
        New-Item -ItemType Directory -Force -Path $SourceDir | Out-Null
        Write-TextFileUtf8NoBom -Path (Join-Path $ProjHarness "Source/Harness.Target.cs") -Value @'
using UnrealBuildTool;
using System.Collections.Generic;

public class HarnessTarget : TargetRules
{
    public HarnessTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("Harness");
    }
}
'@
        Write-TextFileUtf8NoBom -Path (Join-Path $ProjHarness "Source/HarnessEditor.Target.cs") -Value @'
using UnrealBuildTool;
using System.Collections.Generic;

public class HarnessEditorTarget : TargetRules
{
    public HarnessEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("Harness");
    }
}
'@
        Write-TextFileUtf8NoBom -Path (Join-Path $SourceDir "Harness.Build.cs") -Value @'
using UnrealBuildTool;

public class Harness : ModuleRules
{
    public Harness(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });
    }
}
'@
        Write-TextFileUtf8NoBom -Path (Join-Path $SourceDir "Harness.cpp") -Value @'
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, Harness, "Harness");
'@
        
        # Create minimal uproject
        $UProjectContent = @{
            FileVersion = 3
            EngineAssociation = "5.7"
            Category = ""
            Description = ""
            Modules = @(
                @{ Name = "Harness"; Type = "Runtime"; LoadingPhase = "Default" }
            )
            Plugins = @(
                @{ Name = $PluginName; Enabled = $true }
            )
        }
        Write-JsonFileUtf8NoBom -Value $UProjectContent -Path (Join-Path $ProjHarness "Harness.uproject")
        
        $PluginDir = Join-Path $ProjHarness "Plugins/$PluginName"
        New-Item -ItemType Directory -Force -Path $PluginDir | Out-Null
        Copy-Item -Recurse -Force "$RefSourceDir\*" $PluginDir
        
        # Apply patches
        $PatchDir = Join-Path $RepoRoot "UERP_Ref/Patches/$($Ref.Name)"
        if (Test-Path $PatchDir) {
            Copy-Item -Recurse -Force "$PatchDir\*" $PluginDir
            "Applied patches from $PatchDir" | Out-File (Join-Path $RefEvidence "CompatibilityChanges.md") -Append
        } else {
            "No patches applied." | Out-File (Join-Path $RefEvidence "CompatibilityChanges.md")
        }
        
        $UProjectFile = Join-Path $ProjHarness "Harness.uproject"
        $CmdArgs = "HarnessEditor", "Win64", "Development", "-Project=`"$UProjectFile`"", "-WaitMutex", "-NoHotReloadFromIDE"
        $CmdArgs -join " " | Out-File (Join-Path $RefEvidence "BuildCommand.txt")
        
        Write-Host "Running: $BuildBat $CmdArgs"
        $ErrorLogFile = Join-Path $RefEvidence "BuildError.log"
        $process = Start-Process -FilePath $BuildBat -ArgumentList $CmdArgs -Wait -NoNewWindow -PassThru -RedirectStandardOutput $LogFile -RedirectStandardError $ErrorLogFile
        $ExitCode = $process.ExitCode
    }
    
    # Save result
    $BuildResult = @{ ExitCode = $ExitCode; Passed = ($ExitCode -eq 0) }
    $BuildResult | ConvertTo-Json | Out-File (Join-Path $RefEvidence "BuildResult.json")
    
    # Simple log scan
    $LogContent = @()
    $LogContent += Get-Content $LogFile -ErrorAction SilentlyContinue
    $LogContent += Get-Content (Join-Path $RefEvidence "BuildError.log") -ErrorAction SilentlyContinue
    $Errors = $LogContent | Where-Object { $_ -match "error" -and $_ -notmatch "0 error" }
    $Warnings = $LogContent | Where-Object { $_ -match "warning" -and $_ -notmatch "0 warning" }
    @{ ErrorCount = @($Errors).Count; WarningCount = @($Warnings).Count; Snippets = ($Errors | Select-Object -First 5) } | ConvertTo-Json | Out-File (Join-Path $RefEvidence "LogScan.json")
    
    if ($ExitCode -eq 0) {
        Write-Host "$($Ref.Name) built successfully." -ForegroundColor Green
    } else {
        Write-Host "$($Ref.Name) build FAILED (Code $ExitCode)." -ForegroundColor Red
    }
}
