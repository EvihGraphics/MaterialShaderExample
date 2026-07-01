# CURRENT - UE-4.2G.1 AVBOIT Foundation Visual Closeout

Generated UTC: 2026-07-01T14:56:42Z

## Active Stage

- Stage: UE-4.2G.1 AVBOIT Foundation Quad Validation Closeout
- StageStatus: partial
- OverallProjectStatus: partial
- Branch: current upstream-tracked AVBOIT development branch
- Local HEAD: `25d8e3bee4b38701c66b62e7ada8423d8e0200ed`
- Upstream HEAD: `25d8e3bee4b38701c66b62e7ada8423d8e0200ed`
- UE Source HEAD: `260bb2e1c5610b31c63a36206eedd289409c5f11`
- The Forge HEAD: `6d4f4a207bf3a7bef2bc5c4768001298b5bfffba`
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
- Added UE-4.2G.1 direct Foundation visual suite in the headless commandlet.
- Added `PluginIdentity` raster shaders and `ResolvedAlpha` compute shader/pass.
- Added CPU exact reference image generation, full-image GPU readback helpers, per-image manifests, and actual draw-order capture for the direct Foundation suite.
- Replaced fake/fixed log scan with real pattern scanning into `Metrics/LogScan.json`.
- Replaced deprecated/pass-early marker behavior with runner-owned `PASSED.marker` / `FAILED.marker` after all gates are evaluated.

## Verified So Far

```powershell
D:\UE\UnrealEngine_Animation_Tech\Engine\Build\BatchFiles\Build.bat MaterialShaderDemoEditor Win64 Development -Project="D:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT\MaterialShaderDemo.uproject" -WaitMutex -NoHotReloadFromIDE
```

Exit Code: 0.

```powershell
D:\UE\UnrealEngine_Animation_Tech\Engine\Build\BatchFiles\Build.bat ContentExamplesEditor Win64 Development -Project="D:\Users\l3d\Documents\Unreal Projects\ContentExamples\ContentExamples.uproject" -WaitMutex -NoHotReloadFromIDE
```

Exit Code: 0.

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File Scripts\Run-AVBOITFoundationQuadValidation.ps1 -ProjectPath "D:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT\MaterialShaderDemo.uproject" -SkipBuild -CaptureTool None -RunRandomOrders -TimeoutSeconds 420
```

Exit Code: 5.

TempResults: `LocalVisualResults\TempResults\UE57\HIVE_4090x2\UE4-2G-AVBOIT-Quad-Foundation\20260701T145304Z`

KeyResults: none.

Latest metrics:

- Log scan: `TotalErrors=0`
- Direct raster: 6 / 6 passed
- Synthetic: 12 / 12 passed
- A-front pixels: 583200
- B-front pixels: 583200
- AVBOIT vs Exact RGB MAE: 0.279496436
- AVBOIT vs Exact P95 abs: 0.533333333
- Order AB vs BA RGB MAE: 0.249121070
- Order AB vs BA MaxAbs: 0.768627451
- Markers: `SUITE_FINISHED.marker`, `FAILED.marker`
- `PASSED.marker`: absent
- Deprecated `COMPLETED.marker`: absent

## Remaining Hard Gates

- Full Foundation screenshot matrix is incomplete: missing `01`, `02`, `12`, `13`, `16`, `20`, and `21`.
- PluginAVBOIT vs Exact parity fails current thresholds.
- PluginAVBOIT AB vs BA order independence fails current thresholds.
- Native UE Sorted Pixels OIT runtime comparison is not yet captured/proven.
- RenderDoc or PIX capture is not yet produced.
- Editor/PIE/Standalone lifecycle matrix is not yet complete.
- KeyResults promotion is not allowed.

Latest closeout audit:

```text
docs/knowledge/ue5_avboit/UE42G1_REMOTE_AND_EXECUTION_AUDIT_20260701T145642Z.md
```

Latest checkpoint:

```text
UE5_AVBOIT_Agent_Guidance_Package_v1/docs/checkpoints/archive/CHECKPOINT-0048-20260701T145642Z-UE-4-2G-FOUNDATION-VISUAL-CLOSEOUT.md
```

Do not mark `SUCCESS`, `COMPLETED`, or `passed-local`, and do not proceed to UE-4.2H.
