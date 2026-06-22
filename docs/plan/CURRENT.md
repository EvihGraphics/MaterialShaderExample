# Current Work State

## Route

Primary plan: `docs/plan/ue5_avboit/PLAN-UE-2-TheForge-Scene-And-AVBOIT-Port.md`

Latest checkpoint: `docs/checkpoints/archive/CHECKPOINT-0007-20260622T081940Z-UE-bootstrap-result.md`

Status: `partial`

## Repositories

| Repository | Branch | HEAD | Status |
|---|---|---|---|
| `D:\HTC\avboit\AVBOIT开发` | `AVBOIT开发` tracking `origin/master` | `a80b000f589e9953fcb5efbfd3eccbf247d62c4a` | docs generated; build blocked |
| `D:\HTC\avboit\MaterialShaderExample` | `master` | `a80b000f589e9953fcb5efbfd3eccbf247d62c4a` | baseline, left unmodified |
| `D:\HTC\avboit\The-Forge` | `baseline/theforge-1.58-windows-vs-dx12` | `39046051cef37bff0e52e497ca7df07ba9aebe65` | reference, left unmodified |

## Current Gate

UE build/run is blocked by `ue57-path-missing`. Lightweight discovery did not find `UnrealEditor.exe` or `UnrealBuildTool.exe`.

## Resume

1. Open `docs/checkpoints/archive/CHECKPOINT-0007-20260622T081940Z-UE-bootstrap-result.md`.
2. Provide or discover the UE 5.7 root path.
3. Run MaterialShaderExample baseline build/run from `D:\HTC\avboit\AVBOIT开发` and archive the result in a new checkpoint.

