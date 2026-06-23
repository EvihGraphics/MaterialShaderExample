# UE4.2A.1: Render Thread Registry Decision

## Context
Unreal Engine 5 handles Scene Rendering via `FSceneRenderer`, iterating over `FPrimitiveSceneInfo` to build mesh batches and draw commands. However, the AVBOIT raster passes (`Splat`, `ForwardShade`) are completely custom and must be injected outside the standard UE Translucency or Base passes. 
Because `FSceneViewExtensionBase::PrePostProcessPass_RenderThread` executes *after* the Base Pass, we need a way to track which `FAVBOITTestMeshSceneProxy` objects exist and gather their geometry to draw manually.

## Options Considered
1. **Iterate `View.Family->Scene` primitives:** This requires poking into private `FScene` internals or relying on `FMeshElementCollector`, which is highly coupled to standard pass structures (e.g. `RenderInMainPass`).
2. **Global Render-Thread Registry:** A singleton or Subsystem-managed list of active `FAVBOITTestMeshSceneProxy` pointers or immutable `FAVBOITRasterPrimitiveData` blocks.
3. **Dedicated Custom Render Pass Subsystem:** A module-level registry that the proxy registers with during `CreateRenderThreadResources` and unregisters during `DestroyRenderThreadResources`.

## Decision
We will use a **Module-level Render-Thread Registry** named `FAVBOITRasterSceneData`.

- The Game Thread `UAVBOITTestMeshComponent` pushes updates to its `FAVBOITTestMeshSceneProxy`.
- The `FAVBOITTestMeshSceneProxy`, during `CreateRenderThreadResources`, enqueues a command to register itself with `FAVBOITRasterSceneData`.
- During `DestroyRenderThreadResources`, it enqueues a command to unregister itself.
- `FAVBOITRasterSceneData` guarantees that it only holds valid (immutable or safely read-only) pointers to the active proxies.
- `FAVBOITSceneViewExtension` pulls the list of proxies from `FAVBOITRasterSceneData` to issue RDG `AddDrawPass` commands.

## Concurrency Safety
- Both registration and the actual drawing (`PrePostProcessPass_RenderThread`) execute on the **Render Thread**.
- The registry vector is only modified on the Render Thread.
- No `UObject` pointers are stored in the registry.
