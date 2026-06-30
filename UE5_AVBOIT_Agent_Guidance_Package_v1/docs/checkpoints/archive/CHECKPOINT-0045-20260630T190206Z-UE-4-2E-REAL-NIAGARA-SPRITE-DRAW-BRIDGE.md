# CHECKPOINT-0045 - UE-4.2E Real Niagara Sprite Draw Bridge and Shared AVBOIT Core

UTC Time: 2026-06-30T19:02:06Z

Status: blocked-local

## Scope

- Advanced from UE-4.2D foundation to UE-4.2E real Niagara Sprite draw bridge scope.
- Preserved Plugin-first policy and did not modify UE5.7 engine source.
- Added shared AVBOIT core contract types for runtime mode, stage status, depth/slice, frame graph, primitive draw packet, readback, and promotion gates.
- Expanded runtime modes to `EngineDefault`, `UESortedPixelsOIT`, `PluginIdentity`, `PluginAVBOIT`, and `BufferOverview`.
- Added hard gate CVars for real Niagara vertex factory, material proxy, particle attribute hash, SceneColor composite, and shared core buffer overview/debug slice.

## Implementation Notes

- Existing Niagara adapter now separates `RendererMetadataHash` from `ParticleAttributeHash`.
- Current plugin-only draw packet is explicitly metadata-only and records private UE API blockers.
- `Patches/UE57/NiagaraAVBOITMinimalHook.patch` documents the minimal engine hook needed to expose real sprite draw packets, but it is not applied.
- Runner supports the UE-4.2E phase name `UE4-2E-Real-Niagara-Sprite-AVBOIT` and writes contract manifests while withholding KeyResults promotion.

## Hard Gates Not Yet Passed

- Real Niagara sprite `FVertexFactory` is not proven.
- Real `FMaterialRenderProxy` is not proven.
- Real `FMeshBatch` and SceneColor composite are not proven.
- Particle attribute hash for Position/Size/Rotation/Facing/SubUV/Color/Alpha/Material/Sim state is incomplete.
- GPU readback and GPU capture are not produced.
- Shader tint is contracted but not proven through Identity/AVBOIT shader execution.

## Local Validation

- `ContentExamplesEditor Win64 Development` build completed with exit code 0.
- A short `UnrealEditor.exe -game` status smoke entered `PluginIdentity`, wrote `Status.json`, verified Engine `Unlit`, and exited with code 0.
- Smoke evidence root: `LocalVisualResults/TempResults/UE57/HIVE_4090x2/UE4-2E-Real-Niagara-Sprite-AVBOIT/20260630T190816Z-status-smoke`
- Smoke hard-fail log scan found no fatal/assert/RDG/RHI/shader binding/GPU crash/Invalid socket handle/duplicate draw/no AVBOIT sprite draws matches.
- Smoke status remained `blocked-local` with `PromotionEligible=false`.

## Recovery Pointer

Use:

```text
docs/plan/CURRENT.md
```

Do not use UE-4.2C or UE-4.2D partial evidence as UE-4.2E acceptance evidence.
