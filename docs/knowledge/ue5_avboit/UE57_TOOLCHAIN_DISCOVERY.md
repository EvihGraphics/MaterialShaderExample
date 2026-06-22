# UE57 Toolchain Discovery

UTC: 2026-06-22T12:59:02Z

## Portable Requirement

- Project: `MaterialShaderDemo.uproject`
- Engine requirement: `EngineAssociation = 5.7`
- Target: `MaterialShaderDemoEditor`
- Platform: `Win64`
- Configuration: `Development`
- Required current-machine binaries:
  - `Engine/Binaries/Win64/UnrealEditor.exe`
  - `Engine/Build/BatchFiles/Build.bat`
  - `Engine/Build/Build.version`
  - `Engine/Binaries/DotNET/UnrealBuildTool/UnrealBuildTool.dll` or `.exe`

This section is portable. It does not prescribe an absolute `UE57_ROOT`.

## Current Machine Observation

- Machine: `DESKTOP-14SQPSC`
- Local time: `2026-06-22T20:59:02.8620593+08:00`
- UTC: `2026-06-22T12:59:02Z`
- Observed `UE57_ROOT`: `E:\tools\Epic Games\UE_5.7`
- `UnrealEditor.exe`: present
- `Build.bat`: present
- `UnrealBuildTool.dll`: present
- `UnrealBuildTool.exe`: present
- `GenerateProjectFiles.bat`: not present in this Launcher install
- RenderDoc:
  - `C:\Program Files\RenderDoc\renderdoccmd.exe`: present
  - `C:\Program Files\RenderDoc\qrenderdoc.exe`: present
  - not on `PATH`
- PIX: not found

`Build.version`:

```json
{
  "MajorVersion": 5,
  "MinorVersion": 7,
  "PatchVersion": 4,
  "Changelist": 51494982,
  "CompatibleChangelist": 47537391,
  "IsLicenseeVersion": 0,
  "IsPromotedBuild": 1,
  "BranchName": "++UE5+Release-5.7"
}
```

## Candidate Paths Checked

- `E:\tools\Epic Games\UE_5.7`: valid UE 5.7.4
- `C:\Program Files\Epic Games\UE_5.7`: not present
- `D:\Epic Games\UE_5.7`: not present

## Resume Rule

Do not assume this absolute path on another machine. A resumed session must
rediscover `UE57_ROOT`, verify `Build.version`, and record the machine-local
observation before build/run evidence is accepted.

