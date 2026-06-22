# CHECKPOINT-0010: UE2 Closeout Preflight

UTC: 2026-06-22T15:24:26Z

Status: partial

Previous Checkpoint:
`docs/checkpoints/archive/CHECKPOINT-0009-20260622T134419Z-UE1-2-rdg-smoke-result.md`

Supersedes:
`docs/plan/CURRENT.md`

## Repository

| Item | Value |
|---|---|
| Repository | `EvihGraphics/MaterialShaderExample` |
| Repository root | `D:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT` |
| Branch | `AVBOIT开发` tracking `origin/AVBOIT开发` |
| Local HEAD | `59d8c84da73b6d585629978a5e7e85c7d0845a8c` |
| Remote HEAD | `59d8c84da73b6d585629978a5e7e85c7d0845a8c` |
| Ahead / Behind | `0 / 0` |
| Working tree | clean |

Worktrees:

| Path | Branch | HEAD | Role |
|---|---|---|---|
| `D:\Users\l3d\Documents\AVBOIT\MaterialShaderExample` | `master` | `a80b000f589e9953fcb5efbfd3eccbf247d62c4a` | reference worktree |
| `D:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT` | `AVBOIT开发` | `59d8c84da73b6d585629978a5e7e85c7d0845a8c` | active closeout worktree |

## Project UE Contract

| Item | Value |
|---|---|
| Required UE | 5.7 |
| EngineAssociation | 5.7 |
| Build Target | `MaterialShaderDemoEditor` |
| Platform | `Win64` |
| Configuration | `Development` |
| Portable target | `MaterialShaderDemoEditor Win64 Development` |

## Current Machine UE Environment

| Item | Value |
|---|---|
| Machine ID | `HIVE-4090X2` |
| Computer Name | `HIVE-4090X2` |
| UE57_ROOT | `D:\UE\UnrealEngine_Animation_Tech` |
| UE root source | user-provided |
| Build.version | `5.7.4`, changelist `0`, compatible changelist `47537391`, branch `UE5` |
| UnrealEditor.exe | PASS |
| Build.bat | PASS |
| UnrealBuildTool.dll | PASS |
| UnrealBuildTool.exe | PASS |
| RenderDoc | present at `C:\Program Files\RenderDoc` |
| PIX | present at `C:\Program Files\Microsoft PIX\2603.25`, version `2603.25` |

## Path And Evidence

| Item | Value |
|---|---|
| Current machine repository path | `D:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT` |
| ASCII build alias | `NONE` |
| Historical evidence root | `LocalVisualResults/UE57/Baseline`, `LocalVisualResults/UE57/UE2` |
| New machine-scoped evidence root | `LocalVisualResults/UE57/HIVE-4090X2/UE2-Closeout` |
| Local machine JSON | `Saved/AVBOIT/LocalMachine/ue57-environment.json` |
| Local JSON ignore status | ignored by `.gitignore` via `**/Saved/` |

## Current UE-2 Implementation Files

- `Plugins/MaterialShaderPlugin/Source/MaterialShaderPlugin/Private/AVBOIT/AVBOITSmokeSettings.h`
- `Plugins/MaterialShaderPlugin/Source/MaterialShaderPlugin/Private/AVBOIT/AVBOITSmokeSettings.cpp`
- `Plugins/MaterialShaderPlugin/Source/MaterialShaderPlugin/Private/AVBOIT/AVBOITSmokeSubsystem.h`
- `Plugins/MaterialShaderPlugin/Source/MaterialShaderPlugin/Private/AVBOIT/AVBOITSmokeSubsystem.cpp`
- `Plugins/MaterialShaderPlugin/Source/MaterialShaderPlugin/Private/AVBOIT/AVBOITSceneViewExtension.h`
- `Plugins/MaterialShaderPlugin/Source/MaterialShaderPlugin/Private/AVBOIT/AVBOITSceneViewExtension.cpp`
- `Plugins/MaterialShaderPlugin/Shaders/AVBOIT/AVBOITSmoke.usf`
- `Plugins/MaterialShaderPlugin/Source/MaterialShaderPlugin/Private/MaterialShaderExampleModule.cpp`

## Known Test Gaps

- DebugMode 1 is not deterministic because it uses `View.StateFrameIndexMod8`.
- Editor viewport validation is not yet covered on this machine.
- PIE validation and repeated PIE lifecycle checks are not yet covered.
- External RenderDoc or PIX capture is not yet accepted.
- Resize coverage must be refreshed for 960x540, 1280x720, and 1920x1080.
- Original MaterialShaderExample regression must be refreshed at the closeout
  commit.

## HEAD Drift Record

Checkpoint 0009 recorded pre-commit HEAD:

```text
d0ec701dd2abc79170f5dcf99df5451f78f2fd55
```

Actual remote commit containing Checkpoint 0009:

```text
59d8c84da73b6d585629978a5e7e85c7d0845a8c
```

Old checkpoints are not modified.

## First Planned Runtime Change

Change DebugMode 1 in `AVBOITSmoke.usf` from frame-dependent coverage wave to a
static UV gradient, checkerboard, and ViewRect boundary marker.
