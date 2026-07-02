# CHECKPOINT 0049 - UE-4.2G.2 Foundation Ground-Truth Forensics

UTC: 2026-07-02T04:34:39Z

## Status

partial

## Heads

- Starting Local HEAD: `10883e4743f78d2e69c2f01ff37d567cec7b21b2`
- Starting Remote HEAD: `10883e4743f78d2e69c2f01ff37d567cec7b21b2`
- Final Local HEAD: `10883e4743f78d2e69c2f01ff37d567cec7b21b2`
- Final Remote HEAD: `10883e4743f78d2e69c2f01ff37d567cec7b21b2`
- UE Source HEAD: `260bb2e1c5610b31c63a36206eedd289409c5f11`
- The Forge HEAD: `6d4f4a207bf3a7bef2bc5c4768001298b5bfffba`

## Build

```powershell
D:\UE\UnrealEngine_Animation_Tech\Engine\Build\BatchFiles\Build.bat MaterialShaderDemoEditor Win64 Development -Project="D:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT\MaterialShaderDemo.uproject" -WaitMutex -NoHotReloadFromIDE
```

Exit Code: 0

```powershell
D:\UE\UnrealEngine_Animation_Tech\Engine\Build\BatchFiles\Build.bat ContentExamplesEditor Win64 Development -Project="D:\Users\l3d\Documents\Unreal Projects\ContentExamples\ContentExamples.uproject" -WaitMutex -NoHotReloadFromIDE
```

Exit Code: 0

## Validation

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File Scripts\Run-AVBOITFoundationQuadValidation.ps1 -ProjectPath "D:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT\MaterialShaderDemo.uproject" -SkipBuild -CaptureTool None -RunRandomOrders -TimeoutSeconds 600
```

Exit Code: 5

- TempResults: `LocalVisualResults\TempResults\UE57\HIVE_4090x2\UE4-2G2-AVBOIT-Transparent-Sorting-Forensics\20260702T043159Z`
- KeyResults: none
- Status: `partial`
- PromotionEligible: false
- LogScan: `TotalErrors=0`
- Markers: `SUITE_FINISHED.marker`, `FAILED.marker`

## Actual Draw Orders

- PluginIdentity AB: `A_Green,B_Cyan`
- PluginIdentity BA: `B_Cyan,A_Green`
- PluginAVBOIT AB: `A_Green,B_Cyan`
- PluginAVBOIT BA: `B_Cyan,A_Green`
- PluginAVBOIT RandomSeed1: `B_Cyan,A_Green`
- PluginAVBOIT RandomSeed2: `A_Green,B_Cyan`
- PluginAVBOIT RandomSeed3: `B_Cyan,A_Green`

## Ground Truth

- Source: `GPULayerReadbackCPUResolve`
- AFrontPixelCount: 505440
- BFrontPixelCount: 505440
- SameSliceAmbiguityPixelCount: 102492
- ValidExactComparisonPixelCount: 904176
- Old analytic debug A/B front counts: 583200 / 583200

## Earliest Divergence

- EarliestDivergentPass: `Splat.Extinction`
- Extinction AB hash: `ddeed0d869973ae2`
- Extinction BA hash: `595674f18ffe70c2`
- Extinction DifferentElementCount: 29192
- Extinction FirstDifferentIndex: 491550

## Metrics

- AVBOIT vs GPU Exact RGB MAE: 0.245376729
- AVBOIT vs GPU Exact MaxAbs: 0.768627451
- AVBOIT vs GPU Exact P95Abs: 0.277124183
- AB vs BA RGB MAE: 0.287461855
- AB vs BA MaxAbs: 0.768627451
- AB vs BA P95Abs: 0.287581699

## Blocking Reasons

- Splat Extinction AB/BA differs.
- AVBOIT vs GPU Exact parity fails.
- AB/BA order-independence fails.
- Forward accumulation and Composite remain divergent after Splat.
- GPU capture was not requested.
- Native OIT and lifecycle gates were not run because the core gate failed.

## Resume Entry

Start from `EarliestDivergence.json` and fix the Splat/extinction order dependency before spending time on EngineDefault, Native OIT, BufferOverview, RenderDoc/PIX, or lifecycle promotion.

Do not enter UE-4.2H while this checkpoint remains `partial`.
