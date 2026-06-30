# UE5.7 Native Sorted Pixels OIT Source Map

Generated UTC: 20260630T171030Z

## Project Setting And CVar

- Project setting display name: Enable Order Independent Transparency (Experimental)
- Source path: `Engine/Source/Runtime/Engine/Classes/Engine/RendererSettings.h`
- CVar: `r.OIT.SortedPixels`
- Important note: no verified `r.OIT.SortedPixels.Enable` CVar was found in the local UE5.7 source map.

## Shader Compile Defines

- Source path: `Engine/Source/Developer/ShaderCompilerCommon/Private/ShaderCompiler.cpp`
- Observed behavior: reads `r.OIT.SortedPixels` and sets `PROJECT_OIT`.
- Source path: `Engine/Source/Developer/ShaderCompilerCommon/Private/ShaderGenerationUtil.cpp`
- Observed behavior: fetches compile bool `PROJECT_OIT`.

## Shader Files

- `Engine/Shaders/Private/OITCommon.ush`
- `Engine/Shaders/Private/OITCombine.usf`
- `Engine/Shaders/Private/OIT/OITSorting.usf`
- `Engine/Shaders/Private/BasePassPixelShader.usf`

## Base Pass Integration

- `BasePassPixelShader.usf` uses `PROJECT_OIT` / `OIT_ENABLED` and calls `AddOITSample(...)`.
- This indicates native UE Sorted Pixels OIT is integrated at base-pass material shading time, not as a plugin-only post-process replacement.

## Renderer Integration Points

- `Engine/Source/Runtime/Renderer/Private/MeshDrawCommands.cpp`
  - Uses `OIT::IsSortedPixelsEnabled(...)` to influence transparent sorting.
- `Engine/Source/Runtime/Renderer/Private/DeferredShadingRenderer.cpp`
  - Mentions OIT sorted triangles path and pass scheduling.

## UE-4.2D Policy

- Runtime mode `UESortedPixelsOIT` only marks available if `r.OIT.SortedPixels` is registered.
- Plugin code may toggle `r.OIT.SortedPixels`, but must not claim native OIT parity unless the engine reports the CVar and rendered evidence proves it.
