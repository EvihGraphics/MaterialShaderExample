# CURRENT - UE-4.2E Real Niagara Sprite Draw Bridge and Shared AVBOIT Core

Generated UTC: 2026-06-30T19:02:06Z

## Active Stage

- Stage: UE-4.2E Real Niagara Sprite Draw Bridge and Shared AVBOIT Core
- StageStatus: blocked-local
- OverallProjectStatus: partial
- Branch: AVBOIT开发
- Local HEAD at stage start: `a661bc4f69e5159e996f06dd14c4f754c2208948`
- Remote HEAD at stage start: `a661bc4f69e5159e996f06dd14c4f754c2208948`
- Dirty-tree condition: untracked `LocalVisualResults/UE57/` is preserved and must not be staged as acceptance evidence.

## Implementation Boundary

- Plugin-first only. No UE5.7 engine source changes are applied.
- If a real Niagara Sprite VF/material draw bridge is required, use `Patches/UE57/NiagaraAVBOITMinimalHook.patch` as a proposal only.
- Epic `TestSpriteMap1.umap` and dependent assets must remain unmodified.
- KeyResults promotion is milestone-only and forbidden for blocked-local/foundation/probe evidence.

## Landed This Stage

- Added shared AVBOIT core public contracts in `MaterialShaderPlugin/Public/AVBOIT/Core/AVBOITCoreTypes.h`.
- Expanded runtime modes to `EngineDefault`, `UESortedPixelsOIT`, `PluginIdentity`, `PluginAVBOIT`, and `BufferOverview`; old `AVBOITUnlit` remains an alias.
- Added 4.2E CVars for core identity/fixed-slice modes, shared buffer overview/debug slice, and real VF/material/particle-hash/SceneColor gates.
- Split Niagara renderer metadata hash from particle attribute hash.
- Status and renderer manifests now report `NiagaraDrawFeasibility`, depth/slice contract, frame graph contract, blocking private UE symbols, and patch proposal.
- Runner supports UE-4.2E phase switches and writes required TempResults contract JSON placeholders without promoting KeyResults.

## Current Blocking APIs / Gaps

- Real Niagara sprite VF/material/mesh batch construction remains inside UE private `NiagaraRendererSprites.cpp`.
- Current plugin-only adapter can observe particle counts and renderer metadata, but cannot prove a real `FMeshBatch`, `FVertexFactory`, or `FMaterialRenderProxy`.
- `PluginIdentity` and `PluginAVBOIT` therefore remain blocked-local until the minimal hook or another legal draw-packet API exists.
- The current scene extension still records foundation RDG scratch resources; it does not prove final SceneColor composite.
- GPU readback, RenderDoc/PIX capture, comparison images, and shader tint gates are contracted but not passed.

## Local Validation

- Build: `ContentExamplesEditor Win64 Development` completed with exit code 0 on 2026-06-30T19:07Z.
- Status smoke: `UnrealEditor.exe -game` entered `PluginIdentity`, wrote `Status.json`, verified Engine `Unlit`, and exited with code 0.
- Smoke evidence root: `LocalVisualResults/TempResults/UE57/HIVE_4090x2/UE4-2E-Real-Niagara-Sprite-AVBOIT/20260630T190816Z-status-smoke`
- Smoke log hard-fail scan: no fatal/assert/RDG/RHI/shader binding/GPU crash/Invalid socket handle/duplicate draw/no AVBOIT sprite draws matches.
- Smoke status: `StageStatus=blocked-local`, `PromotionEligible=false`.

## New Knowledge / Patch Files

- `docs/knowledge/ue5_avboit/UE42E_ACTUAL_STATE_AUDIT_20260630T190206Z.md`
- `docs/knowledge/ue5_avboit/NIAGARA_SPRITE_DRAW_FEASIBILITY_UE57.md`
- `docs/knowledge/ue5_avboit/UE57_SORTED_PIXELS_OIT_CALL_GRAPH_UE42E.md`
- `docs/knowledge/ue5_avboit/THE_FORGE_TO_UE42E_AVBOIT_CONTRACT.md`
- `docs/knowledge/ue5_avboit/UERP_REF_UE42E_SOURCE_MAPPING.md`
- `Patches/UE57/NiagaraAVBOITMinimalHook.patch`

## Validation Command

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File Scripts\Run-AVBOITNiagaraSpriteUnlitParity.ps1 `
  -ContentExamplesProject "D:\Users\l3d\Documents\Unreal Projects\ContentExamples\ContentExamples.uproject" `
  -RunCoreRefactorValidation -RunIdentityBringup -RunTestSpriteMap1 `
  -RequireRealVertexFactory -RequireRealMaterial -RequireParticleAttributeHash `
  -RequireRealAVBOITDraw -RequireSceneColorComposite
```

Expected current result: `blocked-local`; not `SUCCESS`, `COMPLETED`, or `passed-local`.
