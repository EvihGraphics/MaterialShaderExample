# CURRENT - UE-4.2G.3 Manual Transparent Sorting Entry

Generated UTC: 2026-07-02T06:52:42Z

## Active Stage

- Stage: UE-4.2G.3 Foundation manual transparent sorting viewport entry
- StageStatus: partial
- OverallProjectStatus: partial
- Branch: current upstream-tracked AVBOIT development branch
- Local HEAD: `10883e4743f78d2e69c2f01ff37d567cec7b21b2`
- Upstream HEAD: `10883e4743f78d2e69c2f01ff37d567cec7b21b2`

## Landed This Stage

- Fixed the invalid manual assumption that `r.AVBOIT.Foundation.Scene 1` creates viewport geometry.
- Added `AVBOIT.Foundation.SpawnTransparentSortingScene order=AB mode=PluginAVBOIT`.
- Added transient `TwoIntersectingQuads` proxy scene and transient camera alignment.
- Added `AVBOIT.Foundation.SetOrder`, `SetMode`, `Status`, and `CleanupTransparentSortingScene`.
- Added renderer probe draw-order/status fields.
- Routed `r.AVBOIT.Foundation.Mode 2` to a real PluginIdentity source-over pass.
- Updated manual reproduction docs and checkpoint 0050.
- Updated `Scripts/Run-AVBOITFoundationQuadValidation.ps1` to use a G.3 stage and standalone manual repro manifest gate.

## Manual Repro

Close the currently running Unreal Editor, rebuild, reopen `MaterialShaderDemo.uproject`, then run:

```text
AVBOIT.Foundation.SpawnTransparentSortingScene order=AB mode=PluginAVBOIT
AVBOIT.Foundation.Status
```

Expected visual: two intersecting transparent quads in the viewport, with left side A/Green in front and right side B/Cyan in front.

## Verified

Attempted:

```powershell
D:\UE\UnrealEngine_Animation_Tech\Engine\Build\BatchFiles\Build.bat MaterialShaderDemoEditor Win64 Development -Project="D:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT\MaterialShaderDemo.uproject" -WaitMutex -NoHotReloadFromIDE
```

Result:

```text
Build succeeded after closing the editor.
```

Automated manual validation:

```text
AVBOIT.Foundation.ValidateTransparentSortingSceneAndExit order=AB mode=PluginAVBOIT root=<TempRoot> screenshot=<TempRoot>\Raw\Manual_PluginAVBOIT_AB_Final.png
```

Latest TempResults:

```text
LocalVisualResults\TempResults\UE57\HIVE_4090x2\UE4-2G3-ManualTransparentSorting-Repro\20260702T082318Z
```

Render execution evidence:

- ManualComponents: 2
- RegistryProxyCount: 2
- AcceptedProxyCount: 2
- SkipReason: `Executed`
- ActualDrawOrder: `A_Green,B_Cyan`
- Passes: Clear/Splat/Integrate/ForwardUnlit/Composite all true

Visual evidence:

- `Raw\Manual_PluginAVBOIT_AB_Final.png` is still green-only.
- Pixel sample: left `(0,193,0)`, center `(0,202,0)`, right `(0,189,0)`.
- `blueish_ratio=0.0`.
- A temporary no-transmittance shader experiment showed both Green and Cyan, proving the second draw executes and the remaining issue is in transmittance/extinction, not scene spawning.

## Latest Docs

```text
docs/knowledge/ue5_avboit/UE42G2_MANUAL_TRANSPARENT_SORTING_REPRO_20260702T043439Z.md
docs/knowledge/ue5_avboit/UE42G3_MANUAL_TRANSPARENT_SORTING_ENTRY_REPAIR_20260702T065242Z.md
UE5_AVBOIT_Agent_Guidance_Package_v1/docs/checkpoints/archive/CHECKPOINT-0050-20260702T065242Z-UE-4-2G3-MANUAL-TRANSPARENT-SORTING-ENTRY.md
```

## Remaining Hard Gates

- Rebuild after closing the open Editor and verify the new commands are loaded.
- Fix PluginAVBOIT transmittance/extinction so manual AB visual shows the expected Cyan/B-front contribution.
- Confirm manual viewport visual after the fix; current command/status execution is proven, but visual repro is not passed.
- Splat/extinction order dependency still needs repair.
- PluginAVBOIT vs GPU Exact parity still fails current thresholds.
- PluginAVBOIT AB/BA order independence still fails current thresholds.
- Native UE Sorted Pixels OIT runtime comparison is not yet captured/proven.
- RenderDoc or PIX capture is not yet produced.
- Editor/PIE/Standalone lifecycle matrix is not yet complete.
- KeyResults promotion is not allowed.

Do not mark `SUCCESS`, `COMPLETED`, or `passed-local`, and do not proceed to UE-4.2H.
