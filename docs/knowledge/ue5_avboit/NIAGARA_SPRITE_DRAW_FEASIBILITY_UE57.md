# Niagara Sprite Draw Feasibility - UE5.7 Plugin-First

Generated UTC: 2026-06-30T19:02:06Z

## Public Surface Available To Plugin

- `FNiagaraRenderer::GetDynamicData()`
- `FNiagaraDynamicDataBase::GetParticleDataToRender(...)`
- `UNiagaraSpriteRendererProperties` material and renderer settings
- Component/system/emitter names and runtime world scans

This is enough for component/renderer discovery, particle counts, and renderer metadata hashing.

## Private Surface Needed For Real Draw

The real UE sprite draw path constructs the usable draw packet in `NiagaraRendererSprites.cpp`:

- sprite dynamic data type
- sorted particle view data
- vertex factory setup
- material render proxy selection
- per-view uniform buffers
- final `FMeshBatch`

These are not exposed as a stable public adapter API.

## UE-4.2E Conclusion

- `PluginIdentity` and `PluginAVBOIT` must not call default `FNiagaraRendererSprites` as fallback proof.
- Current plugin-only implementation must report `PluginOnlyRealDrawPossible=false`.
- `RendererMetadataHash` may be populated.
- `ParticleAttributeHash` must remain incomplete until real Position/Size/Rotation/Facing/SubUV/Color/Alpha/Material/Sim state is available.
- `Patches/UE57/NiagaraAVBOITMinimalHook.patch` is the documented blocker escape hatch.

## Required Hook Shape

A minimal hook must expose, per view and per sprite renderer:

- `FMeshBatch`
- `FVertexFactory`
- `FMaterialRenderProxy`
- `FNiagaraDataBuffer`
- particle count
- view index
- material unlit/opacity contract

The plugin can then build Identity and Fixed-Slice AVBOIT passes without copying UE private Niagara code.
