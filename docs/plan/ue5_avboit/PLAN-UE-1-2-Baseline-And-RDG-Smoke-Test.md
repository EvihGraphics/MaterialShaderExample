# PLAN-UE-1-2: Baseline And RDG Smoke Test

## Summary

This plan advances `AVBOIT开发` from the documentation bootstrap into the first
verified UE5.7 runtime loop:

1. UE-1: build and run the original MaterialShaderExample baseline with local
   screenshot, logs, and GPU evidence.
2. UE-2: add an isolated AVBOIT RDG smoke test that reads real SceneDepth,
   creates plugin-owned RDG resources, and composites debug output through a
   legal post-process SceneColor path.

Repository state and machine-local UE state are tracked separately. The project
requires UE 5.7, but each machine must rediscover and record its own `UE57_ROOT`
before producing build/run evidence.

## Portable Requirements

- Repository: `EvihGraphics/MaterialShaderExample`.
- Development worktree: `D:\HTC\avboit\AVBOIT开发`.
- Development branch: `AVBOIT开发`.
- Project file: `MaterialShaderDemo.uproject`.
- Portable engine requirement: `EngineAssociation = 5.7`.
- Build target: `MaterialShaderDemoEditor Win64 Development`.
- Runtime edits are allowed only after the preflight checkpoint is archived.

## UE-1 Baseline Gate

- Verify `UE57_ROOT` on the current machine and record `Build.version`.
- Build `MaterialShaderDemoEditor` with the current machine's UE 5.7 install.
- Launch the editor and verify the original MaterialShaderExample actor,
  material shader, and SceneViewExtension path.
- Archive baseline logs, screenshot, and GPU pass evidence under
  `LocalVisualResults/UE57/Baseline`.
- Do not modify the original `D:\HTC\avboit\MaterialShaderExample` worktree.

## UE-2 RDG Smoke Gate

- Keep the existing MaterialShaderExample classes and shader path intact.
- Add isolated AVBOIT code under plugin `AVBOIT` subdirectories.
- Use UE5.7 public post-process SceneViewExtension subscription:
  `SubscribeToPostProcessingPass(EPostProcessingPass, const FSceneView&, FPostProcessingPassDelegateArray&, bool)`.
- Use `FPostProcessMaterialInputs` for SceneColor input/output and SceneTextures;
  do not use RDG string lookup such as `FindTexture(GraphBuilder, "Nanite.*")`
  for AVBOIT SceneColor or SceneDepth.
- Provide CVars:
  - `r.AVBOIT.Smoke.Enable = 0`
  - `r.AVBOIT.Smoke.DebugMode = 1`
  - `r.AVBOIT.Smoke.Opacity = 0.45`
- Implement GPU events:
  - `AVBOIT.Smoke.Clear`
  - `AVBOIT.Smoke.Fill`
  - `AVBOIT.Smoke.Composite`
- Debug modes:
  - `1`: deterministic gradient/checkerboard with ViewRect boundary marker.
  - `2`: real SceneDepth visualization using UE reverse-Z DeviceZ.
  - `3`: SceneColor overlay.

## Evidence And Checkpoints

- Preflight checkpoint: `CHECKPOINT-0008-<UTC>-UE1-2-preflight.md`.
- Result checkpoint: `CHECKPOINT-0009-<UTC>-UE1-2-result.md`.
- Toolchain discovery: `docs/knowledge/ue5_avboit/UE57_TOOLCHAIN_DISCOVERY.md`.
- Runtime evidence: `docs/knowledge/ue5_avboit/MATERIALSHADEREXAMPLE_BASELINE_RUNTIME_EVIDENCE.md`.
- Entry decision: `docs/knowledge/ue5_avboit/UE57_SCENE_RESOURCE_ENTRY_DECISION.md`.
- RDG contract: `docs/knowledge/ue5_avboit/AVBOIT_RDG_SMOKE_TEST_CONTRACT.md`.
- Session report: `docs/reports/PLAN-UE-1-2-SESSION-<UTC>.md`.

## Completion Rules

- `passed` requires all UE-1 and UE-2 gates to pass.
- `partial` is required when any runtime validation is incomplete.
- `blocked` is required when the current machine lacks a valid UE 5.7 toolchain.
- Do not push automatically.
