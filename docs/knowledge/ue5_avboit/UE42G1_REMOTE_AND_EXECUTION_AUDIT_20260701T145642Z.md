# UE-4.2G.1 Remote and Execution Audit

Generated UTC: 2026-07-01T14:56:42Z

## Scope

This audit closes the current UE-4.2G.1 Foundation Quad validation attempt. It records what was actually executed locally and why the stage remains `partial`.

No UE-4.2H work was started. No Niagara Sprite bridge work was added. `TestSpriteMap1.umap` was not saved or edited.

## Repository State

- Repository: `D:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT`
- Branch: `AVBOIT开发`
- Local HEAD: `25d8e3bee4b38701c66b62e7ada8423d8e0200ed`
- Upstream HEAD: `25d8e3bee4b38701c66b62e7ada8423d8e0200ed`
- UE source HEAD: `260bb2e1c5610b31c63a36206eedd289409c5f11`
- UE Build.version: 5.7.4, CompatibleChangelist 47537391, BranchName `UE5`
- The Forge HEAD: `6d4f4a207bf3a7bef2bc5c4768001298b5bfffba`
- The Forge status: pre-existing dirty/deleted tree, used as dirty-readonly reference only.

## Local Changes Made

- Added direct Foundation quad validation to `AVBOITHeadlessCommandlet.cpp`:
  - procedural `SingleLayerIdentity` and `TwoIntersectingQuads` scene construction;
  - actual draw-order capture from the draw data used by `DrawIndexedPrimitive`;
  - CPU exact sorted reference image generation;
  - full-image GPU readback helper and per-image manifests;
  - readback-backed `PluginIdentity`, `PluginAVBOIT`, and partial buffer overview outputs;
  - metrics JSON for AVBOIT vs Exact and AB vs BA order comparisons.
- Added `AVBOITRasterIdentity.usf` and matching VS/PS classes for plugin source-over identity.
- Added `AVBOITRasterResolvedAlpha.usf` and a Foundation `ResolvedAlpha` compute pass. `AlphaAccumulation` now stores resolved alpha semantics as `1 - far transmittance`.
- Refactored `Run-AVBOITFoundationQuadValidation.ps1`:
  - real log scanning into `Metrics/LogScan.json`;
  - marker lifecycle: `RUNNING.marker`, `SUITE_FINISHED.marker`, final runner-owned `PASSED.marker` or `FAILED.marker`;
  - no deprecated `COMPLETED.marker`;
  - metrics-based partial/promotion decision;
  - upstream HEAD recorded through the current branch upstream to avoid Unicode branch-name corruption.
- Explicitly initialized direct raster case result fields including `AbsoluteError` and `Tolerance`.

## Executed Commands

```powershell
D:\UE\UnrealEngine_Animation_Tech\Engine\Build\BatchFiles\Build.bat MaterialShaderDemoEditor Win64 Development -Project="D:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT\MaterialShaderDemo.uproject" -WaitMutex -NoHotReloadFromIDE
```

Exit Code: 0

```powershell
D:\UE\UnrealEngine_Animation_Tech\Engine\Build\BatchFiles\Build.bat ContentExamplesEditor Win64 Development -Project="D:\Users\l3d\Documents\Unreal Projects\ContentExamples\ContentExamples.uproject" -WaitMutex -NoHotReloadFromIDE
```

Exit Code: 0

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File Scripts\Run-AVBOITFoundationQuadValidation.ps1 -ProjectPath "D:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT\MaterialShaderDemo.uproject" -SkipBuild -CaptureTool None -RunRandomOrders -TimeoutSeconds 420
```

Exit Code: 5

## Evidence

- TempResults: `LocalVisualResults\TempResults\UE57\HIVE_4090x2\UE4-2G-AVBOIT-Quad-Foundation\20260701T145304Z`
- KeyResults: none
- `Metrics\Acceptance.json`: `Status=partial`, `PromotionEligible=false`
- Markers: `SUITE_FINISHED.marker`, `FAILED.marker`
- `PASSED.marker`: absent
- Deprecated `COMPLETED.marker`: absent
- `Metrics\LogScan.json`: `TotalErrors=0`
- `RasterDirectSummary.json`: 6 / 6 passed
- `05_Synthetic\SyntheticSummary.json`: 12 / 12 passed
- GPU capture: not requested in this run

## Visual Metrics

- `AFrontPixelCount`: 583200
- `BFrontPixelCount`: 583200
- `AVBOIT_vs_Exact.RGB_MAE`: 0.279496436
- `AVBOIT_vs_Exact.MaxAbs`: 0.874509804
- `AVBOIT_vs_Exact.P95Abs`: 0.533333333
- `OrderAB_vs_BA.RGB_MAE`: 0.249121070
- `OrderAB_vs_BA.MaxAbs`: 0.768627451
- `OrderAB_vs_BA.P95Abs`: 0.287581699

## Blocking Reasons

- Foundation screenshot matrix is incomplete: 7 files missing.
- Missing current matrix outputs: `01_EngineDefault_AB.png`, `02_EngineDefault_BA.png`, `12_ExtinctionOverview.png`, `13_TransmittanceOverview.png`, `16_SliceOverview.png`, `20_SideBySide.png`, `21_AnnotatedGoal.png`.
- `PluginAVBOIT` vs Exact RGB MAE is greater than 0.03.
- `PluginAVBOIT` vs Exact P95 absolute error is greater than 0.08.
- `PluginAVBOIT` AB vs BA RGB MAE is greater than 1/255.
- `PluginAVBOIT` AB vs BA MaxAbs is greater than 2/255.
- RenderDoc/PIX capture was not produced.
- Native UE Sorted Pixels OIT was not executed or proven in this closeout run.
- Lifecycle matrix was not executed.

## Gap Status Against UE-4.2G.1 Prompt

- Fixed: fake `LogScan` was replaced by real log scanning.
- Fixed: unconditional/premature pass marker was removed; final pass/fail marker is runner-owned.
- Fixed: exact reference now exists as an independent CPU sorted image path for the direct Foundation suite.
- Partial: full-image readback exists for direct-RDG outputs, but not all requested mode outputs exist.
- Partial: BufferOverview currently records color accumulation and resolved alpha readbacks; extinction, transmittance, slice, composite and AVBOIT-vs-Exact overview panes are still incomplete.
- Partial: actual draw order is captured for direct Foundation draw data, but EngineDefault and Native OIT execution paths are not implemented in the matrix.
- Failed gate: current AVBOIT output is not order-independent against AB/BA.
- Failed gate: current AVBOIT output is not close enough to ExactReference.

## TestSpriteMap1

The located asset is:

```text
D:\Users\l3d\Documents\Unreal Projects\ContentExamples\Content\NiagaraExamples\Utilities\SpriteGeneration\TestSpriteMap1.umap
```

Observed `LastWriteTimeUtc`: 2026-06-29T13:44:07.6307636Z. It was not modified by this closeout attempt.

## Decision

UE-4.2G.1 remains `partial`. Do not promote KeyResults. Do not mark `passed-local`. Do not proceed to UE-4.2H.
