# UE5.7 Native Sorted Pixels OIT Reaudit for UE-4.2G

Generated UTC: 2026-07-01T13:37:00Z

## Environment

- UE root: `D:\UE\UnrealEngine_Animation_Tech`
- UE Build.version: 5.7.4, CompatibleChangelist 47537391, BranchName `UE5`
- UE source HEAD: `260bb2e1c5610b31c63a36206eedd289409c5f11`
- MaterialShaderExample starting HEAD: `bed155c57cf48656267b7c6c8c8ee80498649e11`

## Confirmed Native OIT Facts

- Project compile switch: `r.OIT.SortedPixels` is read in `Engine/Source/Runtime/Engine/Private/ShaderCompiler/ShaderCompiler.cpp` and emits `PROJECT_OIT`.
- Runtime switch: `r.OIT.SortedPixels.Enable` participates in `OIT::InternalIsSortedPixelsEnabled`, together with project support, ROV support, and no MSAA.
- Additional runtime CVars: `r.OIT.SortedPixels.PassType`, `r.OIT.SortedPixels.MaxSampleCount`, `r.OIT.SortedPixels.Debug`, `r.OIT.SortedPixels.Method`, and `r.OIT.SortedPixels.TransmittanceThreshold`.
- Base pass shader gate: `BasePassPixelShader.usf` defines `OIT_ENABLED` from `PROJECT_OIT`, material translucency support, platform ROV support, and permutation support.
- Sample insertion: `AddOITSample(...)` is called from the translucent base pass path in `BasePassPixelShader.usf`.
- Buffer creation: `OIT::CreateOITData` creates per-view sample data and sample count textures in `Engine/Source/Runtime/Renderer/Private/OIT/OIT.cpp`.
- Resolve/composition: `OIT::AddOITComposePass` runs from translucency rendering after the matching translucency pass, and `OITCombine.usf` sorts per-pixel samples before composing.
- Mesh draw order: `MeshDrawCommands.cpp` checks `OIT::IsSortedPixelsEnabled` and inverts translucent sort for OIT-enabled passes.

## UE-4.2G Interpretation

- The plugin must not copy native Sorted Pixels OIT internals; it should mimic the engineering pattern: view-local resources, raster sample insertion, and a resolve/composite into the active `SceneColor`.
- `r.OIT.SortedPixels` is a project/shader compile setting, so runtime toggling alone cannot prove `PROJECT_OIT` was compiled into test materials.
- Different UE translucency passes cannot be treated as one sorted set; UE creates/uses OIT data per eligible translucency pass.
- Renderer-private data structures remain read-only reference material for this stage.
