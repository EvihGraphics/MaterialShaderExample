# UE-4.2G.3 Manual Transparent Sorting Reproduction

Generated UTC: 2026-07-02T06:52:42Z

## Important Correction

The old CVar-only instructions were incomplete. These commands do not create geometry by themselves:

```text
r.AVBOIT.Foundation.Enable 1
r.AVBOIT.Foundation.Scene 1
r.AVBOIT.Foundation.Mode 3
```

They only configure the Foundation renderer. In an empty `NewMap` there are no `UAVBOITTestMeshComponent` proxies, so the renderer can skip with `NoProxies` and no `TwoIntersectingQuads` image will appear.

## MaterialShaderDemo Manual Flow

1. Close any running Unreal Editor instance that has this plugin loaded.

2. Rebuild the plugin:

```powershell
D:\UE\UnrealEngine_Animation_Tech\Engine\Build\BatchFiles\Build.bat MaterialShaderDemoEditor Win64 Development -Project="D:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT\MaterialShaderDemo.uproject" -WaitMutex -NoHotReloadFromIDE
```

3. Open:

```text
D:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT\MaterialShaderDemo.uproject
```

4. Open any map, including `NewMap`.

5. In the console or Output Log, run:

```text
AVBOIT.Foundation.SpawnTransparentSortingScene order=AB mode=PluginAVBOIT
```

This creates transient actors only. It does not save or modify the map.

6. Expected viewport result:

```text
Two intersecting transparent quads are centered in view.
Left side: A/Green is in front.
Right side: B/Cyan is in front.
Center: visible crossing/depth transition line.
```

7. Verify the render path is not skipping:

```text
AVBOIT.Foundation.Status
```

Expected status signals:

```text
ManualComponents=2
RegistryProxyCount>=2
SkipReason=Executed
ActualDrawOrder=A_Green,B_Cyan  ; for AB
```

If `SkipReason=NoProxies`, the transient scene did not register with the render thread yet or the command was not available from the rebuilt plugin.

## Switching During Manual Repro

Plugin AVBOIT:

```text
AVBOIT.Foundation.SetMode PluginAVBOIT
```

Plugin identity/source-over comparison:

```text
AVBOIT.Foundation.SetMode PluginIdentity
```

Submission order checks:

```text
AVBOIT.Foundation.SetOrder AB
AVBOIT.Foundation.SetOrder BA
AVBOIT.Foundation.SetOrder RandomSeed1
AVBOIT.Foundation.SetOrder RandomSeed2
AVBOIT.Foundation.SetOrder RandomSeed3
```

Cleanup:

```text
AVBOIT.Foundation.CleanupTransparentSortingScene
```

`ExactReference` and `BufferOverview` remain evidence-runner modes for now; the viewport command records those mode requests but does not claim they are complete runtime replacements.

## Evidence

Each spawn writes:

```text
LocalVisualResults\TempResults\UE57\HIVE_4090x2\UE4-2G3-ManualTransparentSorting\<UTC>\ManualRepro\ManualReproManifest.json
```

The manifest records project, map/world, command, CVars, scene, mode, order, camera, transient component count, expected visual result, RHI/GPU placeholders, commit, and `DesktopScreenshotUsed=false`.

The validation runner now uses a G.3 output stage:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File Scripts\Run-AVBOITFoundationQuadValidation.ps1 -ProjectPath "D:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT\MaterialShaderDemo.uproject" -SkipBuild -CaptureTool None -RunRandomOrders -TimeoutSeconds 600
```

It adds a standalone manual viewport repro command unless `-SkipManualViewportRepro` is supplied.

For automated command-path validation without running the full suite:

```text
AVBOIT.Foundation.ValidateTransparentSortingSceneAndExit order=AB mode=PluginAVBOIT root=<TempRoot> screenshot=<TempRoot>\Raw\Manual_PluginAVBOIT_AB.png
```

Latest local validation after closing the editor:

```text
LocalVisualResults\TempResults\UE57\HIVE_4090x2\UE4-2G3-ManualTransparentSorting-Repro\20260702T082318Z
```

This proves the command path and Foundation pass execution:

```text
ManualComponents=2
RegistryProxyCount=2
SkipReason=Executed
ActualDrawOrder=A_Green,B_Cyan
Clear/Splat/Integrate/ForwardUnlit/Composite=true
```

It does not prove the final PluginAVBOIT visual. The latest PluginAVBOIT screenshot is still green-only, with `blueish_ratio=0.0`. `PluginIdentity` shows both Green and Cyan geometry, so the remaining issue is in PluginAVBOIT transmittance/extinction, not in scene spawning.

## ContentExamples

1. Deploy/copy the plugin to ContentExamples using the existing deployment script.
2. Close any running ContentExamples editor instance before rebuilding.
3. Open:

```text
D:\Users\l3d\Documents\Unreal Projects\ContentExamples\ContentExamples.uproject
```

4. Run the same command:

```text
AVBOIT.Foundation.SpawnTransparentSortingScene order=AB mode=PluginAVBOIT
```

Do not save or modify:

```text
D:\Users\l3d\Documents\Unreal Projects\ContentExamples\Content\NiagaraExamples\Utilities\SpriteGeneration\TestSpriteMap1.umap
```

## Current Status

Status remains `partial`. The manual scene entry and render execution are repaired, but the PluginAVBOIT visual result is not passed yet. UE-4.2G still cannot be promoted until visual repro, order invariance, exact parity, Native OIT proof, GPU capture, and lifecycle gates all pass with real evidence.

不允许进入 UE-4.2H.
