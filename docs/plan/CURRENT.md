# Current Work State

## Repository Pointer

Primary plan:
`docs/plan/ue5_avboit/PLAN-UE-4-1-Plugin-Owned-AVBOIT-Core-Vertical-Slice.md` (To be created)

Latest checkpoint:
`docs/checkpoints/archive/CHECKPOINT-0017-20260623T070000Z-UE4-1-preflight.md` (To be created)

Status: `partial`

| Repository | Branch | Remote Head Observed At Session Start | Stage Status |
|---|---|---|---|
| `MaterialShaderExample_AVBOIT` | `AVBOIT开发` | `2bb1d77e88aa41759336c01f85b554f57f1143bf` | UE-4.1 ACTIVE |
| `MaterialShaderExample` | `master` | `a80b000f589e9953fcb5efbfd3eccbf247d62c4a` | reference worktree |
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

Latest Runtime Verified Commit:
`ddc68ec0565e54a7195a3f00ae94c251e462e1bb`

Latest Documentation Base Commit:
`2bb1d77e88aa41759336c01f85b554f57f1143bf`

Latest UE-2 Runtime Verified Commit:
`90f9514c7744f0da343e480acc91e3a235c52220`

The project contract is portable and does not prescribe an absolute UE install
path.

## Latest Machine Verification

Machine ID:
`HIVE-4090X2`

Commit:
`ddc68ec0565e54a7195a3f00ae94c251e462e1bb`

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
`LocalVisualResults/UE57/HIVE-4090X2/UE3-1-RGB-Validation`

Machine-local UE details are recorded in:

```text
Saved/AVBOIT/LocalMachine/ue57-environment.json
```

## Current Gate

- Remote state synchronized: PASS
- Project / Machine UE separation: PASS
- Machine-scoped evidence namespace: PASS
- UE-3 Subsystem & Assets: PASS
- UE-3.1 RGB Suite execution: PASS
- Ground Truth validation data: PASS
- UE-3.1 Background Contract Repair: PASS
- UE-4.1 Vertical Slice Execution: FAIL

Stage Status:
- UE-3.1 PASSED
- UE-3 Acceptance: ACCEPTED THROUGH UE-3.1 VALIDATION
- UE-4.1 ACTIVE
- UE-4.2 NOT STARTED

## Resume

The project is currently establishing the **UE-4.1 Plugin-Owned AVBOIT Core Math & RDG Vertical Slice**.
We have fixed the UE-3.1 background contract and removed the engine fork plan.
Next: generate Checkpoint A (0017), freeze the Forge math specs, and implement the AVBOIT backend.
