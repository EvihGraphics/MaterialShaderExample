# CHECKPOINT 0048 - UE-4.2G Foundation Visual Closeout

UTC: 2026-07-01T14:56:42Z

## Status

partial

## Heads

- Local HEAD: `25d8e3bee4b38701c66b62e7ada8423d8e0200ed`
- Upstream HEAD: `25d8e3bee4b38701c66b62e7ada8423d8e0200ed`
- UE Source HEAD: `260bb2e1c5610b31c63a36206eedd289409c5f11`
- The Forge HEAD: `6d4f4a207bf3a7bef2bc5c4768001298b5bfffba`

## Environment

- UE Build.version: 5.7.4, CompatibleChangelist 47537391, BranchName `UE5`
- MSVC toolchain observed in build: 14.38.33144
- Windows SDK observed in build: 10.0.22621.0
- RHI used by validation commandlet: D3D12
- GPU observed in commandlet log: NVIDIA GeForce RTX 4090

## Build

```powershell
D:\UE\UnrealEngine_Animation_Tech\Engine\Build\BatchFiles\Build.bat MaterialShaderDemoEditor Win64 Development -Project="D:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT\MaterialShaderDemo.uproject" -WaitMutex -NoHotReloadFromIDE
```

Exit Code: 0

```powershell
D:\UE\UnrealEngine_Animation_Tech\Engine\Build\BatchFiles\Build.bat ContentExamplesEditor Win64 Development -Project="D:\Users\l3d\Documents\Unreal Projects\ContentExamples\ContentExamples.uproject" -WaitMutex -NoHotReloadFromIDE
```

Exit Code: 0

## Landed In This Closeout

- Added direct Foundation visual suite to the headless commandlet.
- Added procedural two-quad scene generation with recorded actual draw order.
- Added independent CPU exact reference output and readback-backed plugin identity/plugin AVBOIT outputs.
- Added full-image GPU texture readback helper and per-image manifests.
- Added plugin identity raster shaders and a resolved-alpha compute shader/pass.
- Added runner-owned marker semantics and real log scanning.
- Added metrics-based promotion blocking in `Run-AVBOITFoundationQuadValidation.ps1`.
- Preserved Niagara bridge and `TestSpriteMap1.umap`.

## Validation Run

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File Scripts\Run-AVBOITFoundationQuadValidation.ps1 -ProjectPath "D:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT\MaterialShaderDemo.uproject" -SkipBuild -CaptureTool None -RunRandomOrders -TimeoutSeconds 420
```

Exit Code: 5

- TempResults: `LocalVisualResults\TempResults\UE57\HIVE_4090x2\UE4-2G-AVBOIT-Quad-Foundation\20260701T145304Z`
- KeyResults: none
- Status: `partial`
- PromotionEligible: false
- Markers: `SUITE_FINISHED.marker`, `FAILED.marker`
- `PASSED.marker`: absent
- `COMPLETED.marker`: absent

## Metrics

- Log scan: `TotalErrors=0`
- Direct raster: 6 / 6 passed
- Synthetic: 12 / 12 passed
- A-front pixels: 583200
- B-front pixels: 583200
- AVBOIT vs Exact RGB MAE: 0.279496436
- AVBOIT vs Exact P95 abs: 0.533333333
- Order AB vs BA RGB MAE: 0.249121070
- Order AB vs BA MaxAbs: 0.768627451

## Blocking Reasons

- Foundation screenshot matrix is incomplete: 7 files missing.
- `PluginAVBOIT` vs Exact RGB MAE exceeds 0.03.
- `PluginAVBOIT` vs Exact P95 abs exceeds 0.08.
- `PluginAVBOIT` AB vs BA RGB MAE exceeds 1/255.
- `PluginAVBOIT` AB vs BA MaxAbs exceeds 2/255.
- RenderDoc/PIX capture was not produced.
- Native UE Sorted Pixels OIT was not executed/proven.
- Lifecycle matrix was not executed.

## Resume Entry

Continue from:

```text
LocalVisualResults\TempResults\UE57\HIVE_4090x2\UE4-2G-AVBOIT-Quad-Foundation\20260701T145304Z
```

Recommended next work:

1. Implement missing EngineDefault, Native OIT, extinction/transmittance/slice overview, side-by-side, and annotated outputs.
2. Fix AVBOIT composition/order behavior until Exact parity and AB/BA gates pass.
3. Produce a RenderDoc or PIX capture that proves the five Foundation passes and resource contents.
4. Run lifecycle and nonzero `ViewRect.Min` cases.
5. Promote KeyResults only if all gates pass.

Do not enter UE-4.2H while this checkpoint remains `partial`.
