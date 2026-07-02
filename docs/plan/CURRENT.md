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
- Added custom per-vertex manual quad geometry so `TwoIntersectingQuads` has A/Green front on the left and B/Cyan front on the right without saving a map.
- Fixed the PluginAVBOIT multi-primitive RDG parameter contract by issuing Splat and ForwardUnlit as one RDG raster pass per primitive.
- Added debug hit-count fields and explicit debug/coverage buffer clears for the manual evidence path.
- Updated manual reproduction docs and checkpoint 0050.
- Updated `Scripts/Run-AVBOITFoundationQuadValidation.ps1` to use a G.3 stage and standalone manual repro manifest gate.

## Manual Repro

Close the currently running Unreal Editor, rebuild, reopen `MaterialShaderDemo.uproject`, then run:

```text
AVBOIT.Foundation.SpawnTransparentSortingScene order=AB mode=PluginAVBOIT
AVBOIT.Foundation.Status
```

Expected visual: two intersecting transparent quads in the viewport, with left side A/Green in front and right side B/Cyan in front.

Switch order:

```text
AVBOIT.Foundation.SetOrder AB
AVBOIT.Foundation.SetOrder BA
AVBOIT.Foundation.SetOrder RandomSeed1
AVBOIT.Foundation.SetOrder RandomSeed2
AVBOIT.Foundation.SetOrder RandomSeed3
```

Switch mode:

```text
AVBOIT.Foundation.SetMode PluginIdentity
AVBOIT.Foundation.SetMode PluginAVBOIT
```

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
LocalVisualResults\TempResults\UE57\HIVE_4090x2\UE4-2G3-ManualTransparentSorting-Repro\20260702T113757Z
```

Render execution evidence:

- ManualComponents: 2
- RegistryProxyCount: 2
- AcceptedProxyCount: 2
- SkipReason: `Executed`
- ActualDrawOrder: `A_Green,B_Cyan`
- Passes: Clear/Splat/Integrate/ForwardUnlit/Composite all true

Visual evidence:

- `Raw\Manual_PluginAVBOIT_AB.png` shows Green on the left and Cyan/Blue on the right.
- Pixel sample: left `(0,203,133)`, center `(57,217,189)`, right `(0,192,180)`.
- `blueish_ratio=0.8625`.
- Manual debug payload: `DebugPixelHitCount=2`, nonzero extinction slices `16` and `19`, transmittance reaches `0.204497397`, overflow/out-of-bounds are `0`.

Order spot checks:

- BA evidence: `LocalVisualResults\TempResults\UE57\HIVE_4090x2\UE4-2G3-ManualTransparentSorting-Repro\20260702T112758Z`, `ActualDrawOrder=B_Cyan,A_Green`.
- RandomSeed1 evidence: `LocalVisualResults\TempResults\UE57\HIVE_4090x2\UE4-2G3-ManualTransparentSorting-Repro\20260702T113932Z`, `ActualDrawOrder=B_Cyan,A_Green`.
- AB vs BA RGB MAE normalized: `0.000468168459513435`.
- AB vs RandomSeed1 RGB MAE normalized: `0.0005443318241648512`.

## Latest Docs

```text
docs/knowledge/ue5_avboit/UE42G2_MANUAL_TRANSPARENT_SORTING_REPRO_20260702T043439Z.md
docs/knowledge/ue5_avboit/UE42G3_MANUAL_TRANSPARENT_SORTING_ENTRY_REPAIR_20260702T065242Z.md
UE5_AVBOIT_Agent_Guidance_Package_v1/docs/checkpoints/archive/CHECKPOINT-0050-20260702T065242Z-UE-4-2G3-MANUAL-TRANSPARENT-SORTING-ENTRY.md
```

## Remaining Hard Gates

- Full prompted exact-reference parity still needs a fresh pass/fail report.
- Full prompted AB/BA/Random order-independence still needs masked metrics and threshold judgment.
- Native UE Sorted Pixels OIT runtime comparison is not yet captured/proven.
- RenderDoc or PIX capture is not yet produced.
- Editor/PIE/Standalone lifecycle matrix is not yet complete.
- KeyResults promotion is not allowed.

Do not mark `SUCCESS`, `COMPLETED`, or `passed-local`, and do not proceed to UE-4.2H.
