# UE-4.2G.2 Foundation Ground-Truth Forensics Audit

Generated UTC: 2026-07-02T04:34:39Z

## Scope

This pass continues UE-4.2G only. It does not start UE-4.2H, does not implement the Niagara Sprite bridge, and does not save or modify `TestSpriteMap1.umap`.

The focus is transparent sorting forensics for the Foundation `TwoIntersectingQuads` scene.

## Preflight

- Repository: `D:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT`
- Branch: current upstream-tracked AVBOIT development branch
- Starting local HEAD: `10883e4743f78d2e69c2f01ff37d567cec7b21b2`
- Starting upstream HEAD: `10883e4743f78d2e69c2f01ff37d567cec7b21b2`
- UE source HEAD: `260bb2e1c5610b31c63a36206eedd289409c5f11`
- UE Build.version: 5.7.4, CompatibleChangelist 47537391, BranchName `UE5`
- The Forge HEAD: `6d4f4a207bf3a7bef2bc5c4768001298b5bfffba`
- The Forge state: pre-existing dirty/deleted tree, used readonly.
- RHI in latest validation: D3D12
- GPU in latest validation: NVIDIA GeForce RTX 4090
- MSVC: 14.38.33144
- Windows SDK: 10.0.22621.0

`CURRENT.md` was stale for UE-4.2G.1 and pointed at checkpoint 0048 before this pass.

## Implemented

- Replaced the G.1 ROI/analytic ExactReference path for G.2 metrics with GPU layer coverage readback plus CPU resolve.
- Added per-layer transparent sorting evidence for Layer A and Layer B:
  - `LayerA_Coverage.png`
  - `LayerB_Coverage.png`
  - `LayerA_LinearDepth.png`
  - `LayerB_LinearDepth.png`
  - `LayerA_PremultipliedColor.png`
  - `LayerB_PremultipliedColor.png`
- Kept the old analytic reference only as `Derived/AnalyticReference_DebugOnly.png`.
- Added masks:
  - `CoverageUnionMask.png`
  - `OverlapCoverageMask.png`
  - `SameSliceAmbiguityMask.png`
  - `ValidExactComparisonMask.png`
  - `FrontLayerClassification.png`
- Added Extinction buffer readback:
  - `Readback/ExtinctionVolume_AB.raw`
  - `Readback/ExtinctionVolume_BA.raw`
- Added `Metrics/GroundTruthMetrics.json` and `Metrics/EarliestDivergence.json`.
- Added `ManualRepro/ManualReproManifest.json`.
- Updated the runner to use the UE-4.2G.2 output directory and to gate on ground-truth, earliest-divergence, and manual repro evidence.

## Latest Evidence

- TempResults: `LocalVisualResults\TempResults\UE57\HIVE_4090x2\UE4-2G2-AVBOIT-Transparent-Sorting-Forensics\20260702T043159Z`
- KeyResults: none
- Status: `partial`
- PromotionEligible: false
- LogScan: `TotalErrors=0`
- Markers: `SUITE_FINISHED.marker`, `FAILED.marker`
- Deprecated `COMPLETED.marker`: absent
- `PASSED.marker`: absent

## Ground Truth Metrics

- GroundTruthSource: `GPULayerReadbackCPUResolve`
- LayerA covered pixels: 1010880
- LayerB covered pixels: 1010880
- CoverageUnion pixels: 1010880
- OverlapCoverage pixels: 1010880
- SameSliceAmbiguity pixels: 102492
- ValidExactComparison pixels: 904176
- AFrontPixelCount: 505440
- BFrontPixelCount: 505440
- Analytic debug AFrontPixelCount: 583200
- Analytic debug BFrontPixelCount: 583200

The old analytic reference was materially different from the GPU-covered truth and must remain debug-only.

## Earliest Divergence

- EarliestDivergentPass: `Splat.Extinction`
- Extinction AB hash: `ddeed0d869973ae2`
- Extinction BA hash: `595674f18ffe70c2`
- Extinction DifferentElementCount: 29192
- Extinction MaxDifference: 7936
- Extinction FirstDifferentIndex: 491550
- Transmittance proxy: ResolvedAlpha far-slice proxy, AB/BA identical in this run.
- Forward ColorAccumulation AB/BA RGB MAE: 0.287461855
- Composite AB/BA RGB MAE: 0.287461855

Because Splat already diverges, later visual parity work must remain blocked until the Splat/extinction order dependency is fixed.

## AVBOIT Metrics

- AVBOIT vs GPU Exact RGB MAE: 0.245376729
- AVBOIT vs GPU Exact MaxAbs: 0.768627451
- AVBOIT vs GPU Exact P95Abs: 0.277124183
- AB vs BA RGB MAE: 0.287461855
- AB vs BA MaxAbs: 0.768627451
- AB vs BA P95Abs: 0.287581699

## Builds

```powershell
D:\UE\UnrealEngine_Animation_Tech\Engine\Build\BatchFiles\Build.bat MaterialShaderDemoEditor Win64 Development -Project="D:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT\MaterialShaderDemo.uproject" -WaitMutex -NoHotReloadFromIDE
```

Exit Code: 0

```powershell
D:\UE\UnrealEngine_Animation_Tech\Engine\Build\BatchFiles\Build.bat ContentExamplesEditor Win64 Development -Project="D:\Users\l3d\Documents\Unreal Projects\ContentExamples\ContentExamples.uproject" -WaitMutex -NoHotReloadFromIDE
```

Exit Code: 0

## Blocking Reasons

- PluginAVBOIT vs GPU Exact RGB MAE exceeds 0.03.
- PluginAVBOIT vs GPU Exact P95Abs exceeds 0.08.
- PluginAVBOIT AB vs BA RGB MAE exceeds 1/255.
- PluginAVBOIT AB vs BA MaxAbs exceeds 2/255.
- Splat Extinction AB/BA differs.
- Forward accumulation AB/BA MaxAbs exceeds 2/255.
- Composite AB/BA MaxAbs exceeds 2/255.
- GPU capture was not requested.
- Native OIT proof and lifecycle matrix were not run because the core Splat/order gate failed.

## Decision

UE-4.2G.2 remains `partial`. Do not promote KeyResults. Do not mark `passed-local`. Do not proceed to UE-4.2H.
