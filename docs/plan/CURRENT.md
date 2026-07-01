# CURRENT - UE-4.2G AVBOIT Quad Foundation

Generated UTC: 2026-07-01T13:37:00Z

## Active Stage

- Stage: UE-4.2G Native-OIT-Guided AVBOIT Foundation Quad Unit Test
- StageStatus: partial
- OverallProjectStatus: partial
- Branch: `AVBOIT开发`
- Starting HEAD: `bed155c57cf48656267b7c6c8c8ee80498649e11`
- Dirty-tree condition: pre-existing untracked `LocalVisualResults/UE57/` is preserved and must not be staged as acceptance evidence.

## Landed This Stage

- Added `FAVBOITFrameConfig` and `FAVBOITPrimitivePacket` under `MaterialShaderPlugin` core public headers.
- Added Foundation CVars:
  `r.AVBOIT.Foundation.Enable`,
  `Mode`,
  `Scene`,
  `SubmissionOrder`,
  `RandomSeed`,
  `DebugPixelX/Y`,
  `DownsampleFactor`,
  `NumSlices`,
  `NearDepthCm`,
  `FarDepthCm`.
- Kept `r.AVBOIT.Raster.Enable` as a deprecated alias for the Foundation path.
- Rewired the existing Raster renderer to use one frame config, sort actual draw calls by `SubmissionOrder`, and emit `AVBOIT.Foundation.*` RDG events.
- Updated raster/backend shaders to support active `NumSlices` and removed the Foundation path's hardcoded far slice `63`.
- Fixed low-resolution volume addressing so Splat, Forward, Composite, and Debug readback use ViewRect-local `VolumePixel`.
- Added overflow and out-of-bounds counters to the Foundation raster path.
- Added `Scripts/Run-AVBOITFoundationQuadValidation.ps1`.
- Added UE5.7 Native OIT reaudit and Foundation actual-state audit docs.

## Verified So Far

```powershell
D:\UE\UnrealEngine_Animation_Tech\Engine\Build\BatchFiles\Build.bat MaterialShaderDemoEditor Win64 Development -Project="D:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT\MaterialShaderDemo.uproject" -WaitMutex -NoHotReloadFromIDE
```

Exit Code: 0.

The build included `MaterialShaderExample`, `MaterialShaderExampleTests`, and `MaterialShaderExampleNiagara`.

## Remaining Hard Gates

- Full Foundation screenshot matrix is not complete.
- Exact per-pixel sorted reference render path and metrics are not yet promoted.
- Native UE Sorted Pixels OIT runtime comparison is not yet captured.
- RenderDoc or PIX capture is not yet produced.
- Editor/PIE/Standalone lifecycle matrix is not yet complete.
- KeyResults promotion is not allowed yet.

Do not mark `SUCCESS`, `COMPLETED`, or `passed-local`, and do not proceed to UE-4.2H.
