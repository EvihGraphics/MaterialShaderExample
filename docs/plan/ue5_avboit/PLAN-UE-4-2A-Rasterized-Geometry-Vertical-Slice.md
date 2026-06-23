# PLAN-UE-4-2A: Rasterized Geometry Vertical Slice

## Overview
This plan implements the real Mesh Rasterization path for AVBOIT in Unreal Engine 5.7. It moves beyond the synthetic backend tests into the actual UE Render Dependency Graph (RDG) using real draw calls.

## Scope & Non-Goals
**In Scope:**
- Establish a plugin-owned SceneViewExtension or PostOpaque Render Delegate to insert RDG passes.
- Implement `AVBOIT.Raster.Clear`, `Splat`, `Integrate`, `ForwardShade`, and `Composite` passes.
- Rasterize real geometry (Quads) generating real hardware depth.
- Accumulate Extinction into a 64-slice RenderTarget/Buffer.
- Accumulate Color via additive hardware blending using FrontTransmittance.
- Composite over the actual `SceneColor`.
- Validate with the mathematical oracle regressions, and create a new Raster Test Suite.
- Capture RenderDoc evidence of the entire RDG pass execution.

**Non-Goals:**
- Do NOT implement the full Forge 15_Transparency scene (Lion, Particles, Refraction). This is reserved for UE-4.2B.
- Do NOT hook into the engine's standard translucent material path.
- Do NOT rewrite or delete the synthetic `AVBOITBackendTestAutomation` suite.

## Technical Execution
1. **Engine Render Hook Audit:** We will analyze UE 5.7 source to select the best plugin insertion point (e.g., `SceneViewExtension` vs `PostOpaqueRenderDelegate`).
2. **Component & Proxy:** Implement a minimal `UAVBOITTestMeshComponent` and `FAVBOITTestMeshSceneProxy` to feed geometry from Game Thread to Render Thread.
3. **Raster Passes:**
   - **Splat:** Render geometry with Depth Test Enabled, write disabled, bind Extinction UAV, output empty color.
   - **Integrate:** Reuse existing CS to convert Extinction to Transmittance.
   - **ForwardShade:** Re-render geometry, read Transmittance SRV, output Additive Blend to ColorAccumulation RT.
   - **Composite:** Fullscreen pass to composite `ColorAccumulation + ExistingSceneColor * TotalTransmittance`.
4. **Validation:** Implement a C++ automation suite `AVBOIT.Test.RasterSuite` covering Single Quad, Overlaps, Permutations, Depth Bounds, and Opaque Occlusion.
