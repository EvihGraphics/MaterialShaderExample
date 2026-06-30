# UE-4.2E Actual State Audit

Generated UTC: 2026-06-30T19:02:06Z

## Preflight

- Branch: `AVBOIT开发`
- Local HEAD before implementation: `a661bc4f69e5159e996f06dd14c4f754c2208948`
- Remote HEAD before implementation: `a661bc4f69e5159e996f06dd14c4f754c2208948`
- `git fetch --all --prune`, `git checkout AVBOIT开发`, and `git pull --ff-only` completed with the branch already up to date.
- UE Build.version: UE 5.7.4, BranchName `UE5`, CompatibleChangelist `47537391`.
- Dirty-tree condition: untracked `LocalVisualResults/UE57/` existed before implementation and must remain untracked, unstaged, and excluded from acceptance evidence.

## Current Technical Boundary

- Policy: Plugin-first. No UE5.7 engine source edits are applied.
- Epic `TestSpriteMap1.umap` and its dependent assets are not modified.
- The existing UE-4.2D code captures Niagara particle counts and renderer metadata, but it does not own a legal public hook to the real Niagara sprite `FMeshBatch`, `FVertexFactory`, or `FMaterialRenderProxy`.
- `AVBOITNiagaraDebug.usf` and the current scene extension allocate named RDG scratch resources and PassId evidence only; those resources do not prove a real AVBOIT composite into SceneColor.

## Reused Code

- Runtime camera and Engine Unlit verification from UE-4.2C remain valid.
- Transient runtime conversion from `UNiagaraSpriteRendererProperties` to `UNiagaraAVBOITSpriteRendererProperties` remains useful for scanning and metadata capture.
- Overlay mode selection and tint controls remain useful for manual verification, but tint acceptance must move to the Identity/AVBOIT shader path.
- Existing backend/raster AVBOIT shader folders remain the intended shared core source of contracts and tests.

## Replaced Or Isolated Code

- Default `FNiagaraRendererSprites::GetDynamicMeshElements` fallback is not accepted as PluginIdentity or PluginAVBOIT evidence.
- MID/material-parameter tint is considered a debug aid only and not 4.2E tint proof.
- Foundation probe passes are labeled as blocked-local evidence until a real Niagara sprite draw bridge and SceneColor composite exist.

## Blocking APIs

The following required pieces are implemented in UE Niagara private source and are not exposed as a plugin-safe draw packet:

- `FNiagaraDynamicDataSprites`
- `PrepareParticleSpriteRenderData`
- `PrepareParticleRenderBuffers`
- `SetupVertexFactory`
- `CreateViewUniformBuffer`
- `FNiagaraRendererSprites::CreateMeshBatchForView`

Patch proposal:

```text
Patches/UE57/NiagaraAVBOITMinimalHook.patch
```

This patch is not applied. It documents the minimum UE source hook needed to expose a real Niagara sprite draw packet to the plugin.

## Status

- StageStatus: `blocked-local`
- OverallProjectStatus: `partial`
- KeyResults promotion: forbidden for UE-4.2E until real VF/material/particle-attribute hash/SceneColor composite/GPU readback/GPU capture gates pass.
