# Current Work State

## Repository Pointer

Primary plan:
`docs/plan/ue5_avboit/PLAN-UE-4-Engine-Mod.md` (To be created)

Latest checkpoint:
`docs/checkpoints/archive/CHECKPOINT-0015-20260623T125333Z-UE3-done.md`

Status: `passed`

| Repository | Branch | HEAD | Stage Status |
|---|---|---|---|
| `MaterialShaderExample_AVBOIT` | `AVBOIT开发` tracking `origin/AVBOIT开发` | `42fbeca` (0014 base was `522c790`) | UE-3 Test Content Completed |
| `MaterialShaderExample` | `master` | `a80b000f589e9953fcb5efbfd3eccbf247d62c4a` | reference worktree, left unmodified |
| `The-Forge` | `baseline/theforge-1.58-windows-vs-dx12` | `39046051cef37bff0e52e497ca7df07ba9aebe65` | reference worktree, left unmodified |

## Project UE Contract

Required UE:
5.7

EngineAssociation:
5.7

Build Target:
MaterialShaderDemoEditor Win64 Development

Project Source Compatibility:
PASSED

Latest Portable Verified Commit:
`42fbeca`

The project contract is portable and does not prescribe an absolute UE install
path.

## Latest Machine Verification

Machine ID:
`HIVE-4090X2`

Commit:
`42fbeca`

UE Version:
5.7.4

Build:
PASS

Editor:
PASS

PIE:
PASS

Standalone:
PASS

Resize:
PASS

Determinism:
PASS

Accepted Log Scan:
PASS

GPU Capture:
PASS

Evidence Root:
`LocalVisualResults/UE57/HIVE-4090X2/UE3-RGB-TestContent`

Machine-local UE details are recorded in:

```text
Saved/AVBOIT/LocalMachine/ue57-environment.json
```

## Current Gate

- Remote state synchronized: PASS
- Project / Machine UE separation: PASS
- Machine-scoped evidence namespace: PASS
- UE-3 Subsystem & Assets: PASS
- UE-3 RGB Suite execution: PASS
- Ground Truth validation data: PASS

UE-3 has `PASSED` and generated valid evidence for SingleWhite, DoubleRedBlue, and TripleRGB variants in `LocalVisualResults/UE57/HIVE-4090X2/UE3-RGB-TestContent`.

## Resume

1. Generate Plan for UE-4: Engine Modification
2. Setup the custom Unreal Engine fork for MaterialShaderPlugin compatibility.
3. Hook into the Transparent Rendering passes.
