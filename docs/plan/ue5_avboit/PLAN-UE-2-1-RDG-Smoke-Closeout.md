# PLAN-UE-2.1: RDG Smoke Closeout

## Summary

This plan closes UE-2 for the isolated AVBOIT RDG smoke test. It does not
implement the production AVBOIT backend.

The required flow is:

1. Synchronize and verify repository state.
2. Separate portable project UE state from current-machine UE state.
3. Make Smoke DebugMode 1 deterministic.
4. Capture fresh current-machine evidence for Editor, PIE, Standalone, Resize,
   lifecycle, original sample regression, and external GPU capture.
5. Promote UE-2 only if every gate passes.

## Project UE Contract

- Required UE: 5.7
- EngineAssociation: 5.7
- Target: `MaterialShaderDemoEditor`
- Platform: `Win64`
- Configuration: `Development`
- Portable build target: `MaterialShaderDemoEditor Win64 Development`

Project-level documents must not contain a required absolute UE install path.
Each machine records its own UE root in ignored local state and checkpoints.

## Current Machine Contract

- Machine ID: `HIVE-4090X2`
- UE root source: user-provided
- UE version observed: 5.7.4
- Evidence root: `LocalVisualResults/UE57/HIVE-4090X2/UE2-Closeout`
- Repository build alias: `NONE` because the repository path is ASCII.

## Implementation Changes

- Replace the DebugMode 1 frame-dependent wave in `AVBOITSmoke.usf` with a
  static UV gradient, checkerboard, and ViewRect boundary marker.
- Keep DebugMode 2 reading `SceneTexturesStruct.SceneDepthTexture` and using
  `ConvertFromDeviceZ`.
- Clarify DebugMode 3 as a UV/checker SceneColor overlay.
- Update CVar help text to match the actual modes.
- Add only minimal validation support if needed to prove Editor and PIE
  lifecycle behavior. Do not add AVBOIT volume, splat, integrate, shade, or
  final resolve backend code.

## Evidence Requirements

Fresh current-machine evidence must be written under:

```text
LocalVisualResults/UE57/HIVE-4090X2/UE2-Closeout/
```

Required subdirectories:

- `Determinism`
- `Editor`
- `PIE`
- `Standalone`
- `Resize`
- `GPUCapture`
- `Logs`

Existing historical evidence under `LocalVisualResults/UE57/Baseline` and
`LocalVisualResults/UE57/UE2` must not be moved, renamed, overwritten, or used
as current-machine PASS evidence.

## Validation Gates

- Remote state synchronized: PASS
- Project / Machine UE separation: PASS
- Machine-scoped evidence: PASS
- Deterministic Mode 1: PASS
- SceneDepth Mode: PASS
- SceneColor Composite: PASS
- Editor: PASS
- PIE: PASS
- Standalone: PASS
- Resize 960x540: PASS
- Resize 1280x720: PASS
- Resize 1920x1080: PASS
- Enable / Disable lifecycle: PASS
- Original MaterialShaderExample regression: PASS
- External GPU Capture: PASS
- Shader compile errors: 0
- RDG validation errors: 0
- Fatal / Assertion errors: 0

UE-2 may be marked `PASSED` only when all gates pass. Otherwise the stage remains
`PARTIAL`, `BLOCKED`, or `FAILED` according to the missing gate.

## Forbidden Scope

Do not implement The Forge scene replication, plugin-owned RGB planes, 64-slice
AVBOIT volume, extinction atomics, transmittance integration, AVBOIT transparent
shade, final resolve, Niagara/Substrate/common translucency integration, engine
fork changes, mobile support, or performance optimization.

Do not push automatically.
