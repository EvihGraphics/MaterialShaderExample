# CHECKPOINT 0047 - UE-4.2G AVBOIT Quad Foundation

UTC: 2026-07-01T13:37:00Z

## Status

partial

## Heads

- Starting Local HEAD: `bed155c57cf48656267b7c6c8c8ee80498649e11`
- Starting Remote HEAD: `bed155c57cf48656267b7c6c8c8ee80498649e11`
- UE Source HEAD: `260bb2e1c5610b31c63a36206eedd289409c5f11`
- The Forge HEAD: `6d4f4a207bf3a7bef2bc5c4768001298b5bfffba`

## Environment

- UE Build.version: 5.7.4, CompatibleChangelist 47537391, BranchName `UE5`
- MSVC toolchain observed in build: 14.38.33144
- Windows SDK observed in build: 10.0.22621.0

## Build

```powershell
D:\UE\UnrealEngine_Animation_Tech\Engine\Build\BatchFiles\Build.bat MaterialShaderDemoEditor Win64 Development -Project="D:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT\MaterialShaderDemo.uproject" -WaitMutex -NoHotReloadFromIDE
```

Exit Code: 0

## Landed

- Added Foundation frame config and primitive packet core types.
- Added Foundation CVars and deprecated raster alias.
- Rewired the existing Raster renderer to consume one frame config, sort actual draw calls by submission order, emit Foundation RDG event names, and use config-driven depth/slice/downsample values.
- Updated raster shaders for ViewRect-local low-resolution volume addressing and `NumSlices` based far-slice access.
- Added overflow/out-of-bounds counters and debug readback fields.
- Added `Scripts/Run-AVBOITFoundationQuadValidation.ps1`.
- Added native OIT and actual-state audit docs.

## Known Gaps

- Full screenshot matrix is not complete.
- ExactReference render path and parity metrics are not yet promoted.
- Native OIT runtime comparison is not yet captured.
- RenderDoc/PIX capture is not yet produced.
- Status must remain `partial`; do not enter UE-4.2H.

## Resume Entry

Run:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File Scripts\Run-AVBOITFoundationQuadValidation.ps1 -SkipBuild -CaptureTool None
```

Then complete the remaining visual/exact-reference/capture gates before promotion.
