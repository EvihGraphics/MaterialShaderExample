# CHECKPOINT-0050 - UE-4.2G.3 Manual Transparent Sorting Entry

UTC: 2026-07-02T06:52:42Z

Status: partial

## Summary

Repaired the manual viewport entry for Foundation transparent sorting. The previous instructions were incomplete because Foundation CVars did not instantiate `TwoIntersectingQuads`; they only configured the renderer. A `NewMap` with no `UAVBOITTestMeshComponent` proxies could therefore render nothing or show unrelated debug/map content.

## Changes

- Added `AVBOIT.Foundation.SpawnTransparentSortingScene order=AB mode=PluginAVBOIT`.
- Added transient two-quad Foundation scene generation and camera alignment.
- Added `AVBOIT.Foundation.SetOrder`, `SetMode`, `Status`, and `CleanupTransparentSortingScene`.
- Added renderer probe draw-order fields.
- Added runtime PluginIdentity mode routing for `r.AVBOIT.Foundation.Mode 2`.
- Added custom per-vertex quad geometry for the manual scene: A/Green left-near/right-far and B/Cyan left-far/right-near.
- Fixed the PluginAVBOIT multi-primitive RDG parameter contract by issuing Splat and ForwardUnlit as one RDG raster pass per primitive.
- Added debug hit-count fields and explicit clears for the actual debug/coverage readback buffers.
- Updated manual repro documentation with the corrected command-driven flow.
- Updated the validation runner to use the UE-4.2G.3 stage and a standalone manual repro manifest gate.

## Verification

Attempted:

```powershell
D:\UE\UnrealEngine_Animation_Tech\Engine\Build\BatchFiles\Build.bat MaterialShaderDemoEditor Win64 Development -Project="D:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT\MaterialShaderDemo.uproject" -WaitMutex -NoHotReloadFromIDE
```

Observed:

```text
AVBOITFoundationManualReproCommands.cpp compiled.
AVBOITRasterRenderer.cpp compiled.
Link failed with LNK1104 because the currently open UnrealEditor.exe holds plugin DLLs.
```

After closing the editor, the rebuild succeeded.

Automated delayed validation command:

```text
AVBOIT.Foundation.ValidateTransparentSortingSceneAndExit order=AB mode=PluginAVBOIT root=<TempRoot> screenshot=<TempRoot>\Raw\Manual_PluginAVBOIT_AB_Final.png
```

Earlier TempResults before the RDG multi-draw repair:

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
Clear/Splat/Integrate/ForwardUnlit/Composite=true
```

That visual result did not pass:

```text
Manual_PluginAVBOIT_AB_Final.png remains green-only.
Pixel sample left=(0,193,0), center=(0,202,0), right=(0,189,0).
blueish_ratio=0.0.
```

Updated TempResults after the repair:

```text
AB: LocalVisualResults\TempResults\UE57\HIVE_4090x2\UE4-2G3-ManualTransparentSorting-Repro\20260702T113757Z
BA: LocalVisualResults\TempResults\UE57\HIVE_4090x2\UE4-2G3-ManualTransparentSorting-Repro\20260702T112758Z
RandomSeed1: LocalVisualResults\TempResults\UE57\HIVE_4090x2\UE4-2G3-ManualTransparentSorting-Repro\20260702T113932Z
```

Current manual visual result passes the G.3 reproduction goal:

```text
Raw\Manual_PluginAVBOIT_AB.png shows Green on the left and Cyan/Blue on the right.
Pixel sample left=(0,203,133), center=(57,217,189), right=(0,192,180).
blueish_ratio=0.8625.
DebugPixelHitCount=2.
PackedExtinction has nonzero slices 16 and 19.
Transmittance reaches 0.204497397.
OverflowCount=0.
OutOfBoundsCount=0.
```

Order spot checks:

```text
AB vs BA RGB MAE normalized=0.000468168459513435.
AB vs RandomSeed1 RGB MAE normalized=0.0005443318241648512.
```

## Remaining Blockers

- Run the full G.3 validation runner and collect updated matrix evidence.
- Resolve the remaining UE-4.2G hard gates: full exact parity, full masked order-invariance metrics, Native OIT proof, GPU capture, and lifecycle.

不允许进入 UE-4.2H.
