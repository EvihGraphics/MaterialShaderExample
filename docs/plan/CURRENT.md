# Current Work State

## Repository Pointer

Primary plan:
`docs/plan/ue5_avboit/PLAN-UE-1-2-Baseline-And-RDG-Smoke-Test.md`

Latest checkpoint:
`docs/checkpoints/archive/CHECKPOINT-0009-20260622T134419Z-UE1-2-rdg-smoke-result.md`

Status: `implemented-with-test-gaps`

| Repository | Branch | HEAD | Stage Status |
|---|---|---|---|
| `D:\HTC\avboit\AVBOIT开发` | `AVBOIT开发` tracking `origin/AVBOIT开发` | `d0ec701dd2abc79170f5dcf99df5451f78f2fd55` | UE-1 and UE-2 standalone evidence captured; Editor/PIE and external GPU capture remain |
| `D:\HTC\avboit\MaterialShaderExample` | `master` | `a80b000f589e9953fcb5efbfd3eccbf247d62c4a` | reference worktree, left unmodified |
| `D:\HTC\avboit\The-Forge` | `baseline/theforge-1.58-windows-vs-dx12` | `39046051cef37bff0e52e497ca7df07ba9aebe65` | reference worktree, left unmodified |

Working tree contains generated evidence under `LocalVisualResults/UE57`, docs
updates, and the isolated AVBOIT smoke-test runtime files. No commit has been
made by this checkpoint.

## Portable UE Requirement

The project requires UE 5.7 through `MaterialShaderDemo.uproject`
`EngineAssociation = 5.7`.

This is the portable requirement. It does not prescribe an absolute UE install
path.

Build target: `MaterialShaderDemoEditor Win64 Development`.

Required current-machine binaries after rediscovery:

- `Engine/Binaries/Win64/UnrealEditor.exe`
- `Engine/Build/BatchFiles/Build.bat`
- `Engine/Build/Build.version`
- UnrealBuildTool `.dll` or `.exe`

## Current Machine UE Observation

Machine-local evidence from `DESKTOP-14SQPSC`; stale on other machines until
rediscovered and checkpointed:

| Item | Value |
|---|---|
| Observed `UE57_ROOT` | `E:\tools\Epic Games\UE_5.7` |
| Version | 5.7.4 |
| Build branch | `++UE5+Release-5.7` |
| UnrealEditor.exe | present |
| Build.bat | present |
| UBT | present |
| RenderDoc | present at `C:\Program Files\RenderDoc`, not on `PATH` |
| PIX | not found |

## Current Gate

UE-1 baseline and UE-2 isolated RDG smoke test have passed standalone evidence:

- build succeeded with `W:\MaterialShaderDemo.uproject`
- baseline viewport screenshot captured
- disabled parity screenshot captured
- DebugMode 1/2/3 screenshots captured
- 960x540 resize screenshots captured for disabled and Mode 1
- accepted build/run logs have zero shader/RDG/fatal/assertion error matches

Remaining validation before a full `passed` checkpoint:

- Editor mode
- PIE mode
- external RenderDoc or PIX capture

## Resume

1. Rediscover the current machine's `UE57_ROOT`; do not assume the path above is
   valid on another machine.
2. If the local checkout path contains non-ASCII characters, verify an ASCII
   alias such as `W:` before UBT/editor commands.
3. Open
   `docs/checkpoints/archive/CHECKPOINT-0009-20260622T134419Z-UE1-2-rdg-smoke-result.md`.
4. Continue with Editor/PIE validation or GPU capture. Record a new checkpoint
   if additional evidence is produced.
