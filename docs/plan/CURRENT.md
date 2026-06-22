# Current Work State

## Repository Pointer

Primary plan:
`docs/plan/ue5_avboit/PLAN-UE-2-1-RDG-Smoke-Closeout.md`

Latest checkpoint:
`docs/checkpoints/archive/CHECKPOINT-0011-20260622T163417Z-UE2-closeout-result.md`

Status: `partial`

| Repository | Branch | HEAD | Stage Status |
|---|---|---|---|
| `MaterialShaderExample_AVBOIT` | `AVBOIT开发` tracking `origin/AVBOIT开发` | `59d8c84da73b6d585629978a5e7e85c7d0845a8c` | UE-2.1 local closeout evidence mostly complete; external GPU capture blocked |
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
PARTIAL

Latest Portable Verified Commit:
`59d8c84da73b6d585629978a5e7e85c7d0845a8c`

The project contract is portable and does not prescribe an absolute UE install
path.

## Latest Machine Verification

Machine ID:
`HIVE-4090X2`

Commit:
`59d8c84da73b6d585629978a5e7e85c7d0845a8c`

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
BLOCKED

Evidence Root:
`LocalVisualResults/UE57/HIVE-4090X2/UE2-Closeout`

Machine-local UE details are recorded in:

```text
Saved/AVBOIT/LocalMachine/ue57-environment.json
```

## Current Gate

- Remote state synchronized: PASS
- Project / Machine UE separation: PASS
- Machine-scoped evidence namespace: PASS
- Deterministic Mode 1: PASS
- Editor: PASS
- PIE: PASS
- Standalone: PASS
- Resize: PASS
- Enable / Disable lifecycle: PASS
- Original MaterialShaderExample regression: PASS
- Accepted fatal/assert/shader/RDG/device log scan: PASS
- External GPU Capture: BLOCKED

UE-2 remains `PARTIAL` until external GPU capture produces accepted event
evidence for `AVBOIT.Smoke.Clear`, `AVBOIT.Smoke.Fill`, and
`AVBOIT.Smoke.Composite`.

## Resume

1. Read `docs/checkpoints/archive/CHECKPOINT-0011-20260622T163417Z-UE2-closeout-result.md`.
2. Resolve the external GPU capture gate using RenderDoc or PIX.
3. Export and scan the accepted GPU event list under
   `LocalVisualResults/UE57/HIVE-4090X2/UE2-Closeout/GPUCapture`.
4. Only then promote UE-2 from `PARTIAL` to `PASSED`.
