# UE-4.2G Foundation Actual State Audit

Generated UTC: 2026-07-01T13:37:00Z

## Starting State

- Repository HEAD: `bed155c57cf48656267b7c6c8c8ee80498649e11`
- Branch: `AVBOIT开发`
- Pre-existing dirty state: untracked `LocalVisualResults/UE57/` is preserved.
- The-Forge HEAD: `6d4f4a207bf3a7bef2bc5c4768001298b5bfffba`; working tree has pre-existing deletions and is treated as readonly reference.

## Confirmed Raster Issues Before UE-4.2G Edits

- Depth domain was split between `100/400000` in scene input and `10/1000` in shader parameter binding.
- Raster shaders used low-resolution extinction/transmittance resources but Forward read transmittance using full-resolution `SV_Position.xy`.
- Composite hardcoded the far slice as `63`.
- `SubmissionOrder` existed on the test component/proxy but was not copied into draw data or used to sort the actual draw calls.
- The only active runtime switch was the legacy `r.AVBOIT.Raster.Enable`.
- RDG event names still used `AVBOIT.Raster.*`, so GPU captures could not identify the UE-4.2G Foundation pass contract.

## Edits Landed In This Working Tree

- Added `FAVBOITFrameConfig` and `FAVBOITPrimitivePacket`.
- Added Foundation CVars under `r.AVBOIT.Foundation.*`, with `r.AVBOIT.Raster.Enable` retained as a deprecated alias.
- Rewired `AVBOITRasterRenderer` to build a single frame config, sort draw data by `SubmissionOrder`, use config-driven depth/slice/downsample values, and emit `AVBOIT.Foundation.*` RDG events.
- Updated raster shaders so Splat/Forward/Composite use `AbsolutePixel -> LocalPixel -> VolumePixel` and use `NumSlices - 1` instead of hardcoded slice 63.
- Added overflow and out-of-bounds counters to the Foundation raster path and debug readback.
- Preserved the existing direct headless raster test by setting its config to downsample 1.

## Current Status

- `MaterialShaderDemoEditor` build passed locally after the code changes.
- Full UE-4.2G visual screenshot matrix, exact-reference parity, Native OIT runtime proof, and RenderDoc/PIX capture are not yet promoted; status remains `partial`.
