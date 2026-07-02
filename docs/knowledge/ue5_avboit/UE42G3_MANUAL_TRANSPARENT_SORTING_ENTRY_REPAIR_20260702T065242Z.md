# UE-4.2G.3 Manual Transparent Sorting Entry Repair

Generated UTC: 2026-07-02T06:52:42Z

## Finding

The reported viewport screenshot is a valid failure report. The previous manual document asked the user to set Foundation CVars, but those CVars do not spawn `TwoIntersectingQuads` in the Editor viewport.

Runtime Foundation rendering collects existing `UAVBOITTestMeshComponent` scene proxies from `FAVBOITRasterSceneData`. In `NewMap`, no such proxies exist, so the path can skip with `NoProxies`.

## Implemented Repair

- Added `AVBOIT.Foundation.SpawnTransparentSortingScene order=AB mode=PluginAVBOIT`.
- Added transient `TwoIntersectingQuads` actor/components using two `UAVBOITTestMeshComponent` quads:
  - `A_Green`, alpha `0.55`, custom local vertices with left depth `700cm` and right depth `1300cm`.
  - `B_Cyan`, alpha `0.55`, custom local vertices with left depth `1300cm` and right depth `700cm`.
- Added a transient camera and moves Editor perspective viewports to the repro view.
- Added `AVBOIT.Foundation.SetOrder`, `SetMode`, `Status`, and `CleanupTransparentSortingScene`.
- Added renderer status probe fields for actual render-thread draw order and submission orders.
- Routed `r.AVBOIT.Foundation.Mode 2` to a real PluginIdentity source-over pass; `Mode 3` continues to use the AVBOIT Foundation path.
- Fixed the multi-primitive RDG parameter contract by issuing Splat and ForwardUnlit as one RDG raster pass per primitive. The previous single pass captured per-draw RDG UAV/SRV parameters that were not patched for the second draw, suppressing B/Cyan in PluginAVBOIT.
- Added debug hit-count fields for the Splat debug pixel and explicit clears for actual debug/coverage readback buffers.
- Updated the validation script to use the UE-4.2G.3 stage and invoke the standalone manual repro unless explicitly skipped.

## Evidence Semantics

Each manual spawn writes:

```text
LocalVisualResults\TempResults\UE57\HIVE_4090x2\UE4-2G3-ManualTransparentSorting\<UTC>\ManualRepro\ManualReproManifest.json
```

When the runner supplies `root=<TempRoot>`, the manifest is written to:

```text
<TempRoot>\ManualRepro\ManualReproManifest.json
```

The manifest records scene, command, mode, order, CVars, map/world, camera, quad descriptors, component counts, commit, and `DesktopScreenshotUsed=false`.

## Verification

Command attempted:

```powershell
D:\UE\UnrealEngine_Animation_Tech\Engine\Build\BatchFiles\Build.bat MaterialShaderDemoEditor Win64 Development -Project="D:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT\MaterialShaderDemo.uproject" -WaitMutex -NoHotReloadFromIDE
```

Result:

```text
C++ compile completed for AVBOITFoundationManualReproCommands.cpp and AVBOITRasterRenderer.cpp.
Final link failed because the currently open UnrealEditor.exe holds UnrealEditor-MaterialShaderExample.dll and UnrealEditor-MaterialShaderExampleTests.dll.
```

This blocks loading the new commands in the already open editor. Close the editor, rebuild, reopen, then run the manual command.

## Follow-Up Verification After Editor Close

Build after closing the editor:

```powershell
D:\UE\UnrealEngine_Animation_Tech\Engine\Build\BatchFiles\Build.bat MaterialShaderDemoEditor Win64 Development -Project="D:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT\MaterialShaderDemo.uproject" -WaitMutex -NoHotReloadFromIDE
```

Result: succeeded.

Automated manual validation:

```text
AVBOIT.Foundation.ValidateTransparentSortingSceneAndExit order=AB mode=PluginAVBOIT root=<TempRoot> screenshot=<TempRoot>\Raw\Manual_PluginAVBOIT_AB_Final.png
```

Latest evidence before the RDG multi-draw repair:

```text
LocalVisualResults\TempResults\UE57\HIVE_4090x2\UE4-2G3-ManualTransparentSorting-Repro\20260702T082318Z
```

Render execution passed:

```text
ManualComponents=2
RegistryProxyCount=2
AcceptedProxyCount=2
SkipReason=Executed
ActualDrawOrder=A_Green,B_Cyan
Passes Clear/Splat/Integrate/ForwardUnlit/Composite=true
```

That run failed visual inspection for PluginAVBOIT:

```text
Manual_PluginAVBOIT_AB_Final.png center pixel = (0, 202, 0)
blueish_ratio = 0.0
```

`PluginIdentity` separately shows both Green and Cyan geometry, so the manual scene entry and camera are valid. The remaining failure is in the PluginAVBOIT transmittance/extinction path: the second primitive contribution is suppressed in the final PluginAVBOIT image.

Updated evidence after the RDG multi-draw repair:

```text
Build: MaterialShaderDemoEditor Win64 Development succeeded.
AB: LocalVisualResults\TempResults\UE57\HIVE_4090x2\UE4-2G3-ManualTransparentSorting-Repro\20260702T113757Z
BA: LocalVisualResults\TempResults\UE57\HIVE_4090x2\UE4-2G3-ManualTransparentSorting-Repro\20260702T112758Z
RandomSeed1: LocalVisualResults\TempResults\UE57\HIVE_4090x2\UE4-2G3-ManualTransparentSorting-Repro\20260702T113932Z
```

AB visual now passes the manual repro expectation:

```text
Raw\Manual_PluginAVBOIT_AB.png shows Green on the left and Cyan/Blue on the right.
Pixel sample: left=(0,203,133), center=(57,217,189), right=(0,192,180).
blueish_ratio=0.8625.
DebugPixelHitCount=2.
PackedExtinction has nonzero slices 16 and 19.
Transmittance reaches 0.204497397.
OverflowCount=0, OutOfBoundsCount=0.
```

Order spot checks:

```text
AB vs BA RGB MAE normalized=0.000468168459513435.
AB vs RandomSeed1 RGB MAE normalized=0.0005443318241648512.
```

## Status

Stage status remains `partial` for the overall UE-4.2G stage.

Promotion is not allowed. The manual entry, render execution, and PluginAVBOIT visual repro are repaired, but full UE-4.2G gates are still incomplete:

- Full exact-reference parity still needs a fresh pass/fail report.
- Full AB/BA/Random order-invariance metrics still need the prompted masks and thresholds.
- Native UE Sorted Pixels OIT proof is incomplete.
- GPU capture is incomplete.
- Lifecycle matrix is incomplete.

不允许进入 UE-4.2H.
