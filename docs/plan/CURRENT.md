# Current Work State

## Repository Pointer

Primary plan:
`docs/plan/ue5_avboit/PLAN-UE-3-Deterministic-RGB-Test-Content.md`

Latest checkpoint:
`docs/checkpoints/archive/CHECKPOINT-0014-20260623T123858Z-UE3-preflight.md`

Status: `partial`

| Repository | Branch | HEAD | Stage Status |
|---|---|---|---|
| `MaterialShaderExample_AVBOIT` | `AVBOIT开发` tracking `origin/AVBOIT开发` | `90f9514c7744f0da343e480acc91e3a235c52220` (0011 base was `59d8c84da73b6d585629978a5e7e85c7d0845a8c`) | UE-3 Test Content in progress |
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
`90f9514c7744f0da343e480acc91e3a235c52220`

The project contract is portable and does not prescribe an absolute UE install
path.

## Latest Machine Verification

Machine ID:
`HIVE-4090X2`

Commit:
`90f9514c7744f0da343e480acc91e3a235c52220`

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
- External GPU Capture: PASS

UE-2 has `PASSED` and generated valid evidence for `AVBOIT.Smoke.Clear`, `AVBOIT.Smoke.Fill`, and `AVBOIT.Smoke.Composite` in `LocalVisualResults/UE57/HIVE-4090X2/UE2-Closeout/GPUCapture`.

## Resume

1. Follow PLAN-UE-3 to implement `FAVBOITTestPrimitiveDesc` and the fixed test scene.
2. Setup the fixed Camera and Black Background.
3. Validate SingleWhite, DoubleRedBlue, and TripleRGB presets.
