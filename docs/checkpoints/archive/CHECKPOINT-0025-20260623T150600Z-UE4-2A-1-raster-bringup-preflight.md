# CHECKPOINT-0025-20260623T150600Z-UE4-2A-1-raster-bringup-preflight

## System Context
- **Local HEAD:** 27af9b4
- **Machine ID:** HIVE_4090x2

## Preflight Verification
- Confirmed remote HEAD semantics and separated `Implementation Baseline` from current pointer.
- Confirmed baseline builds correctly (ExitCode: 0) after fixing `UAVBOITTestMeshComponent` stubs (missing `GetTypeHash` and wrong module API macro).

## Intent
Proceed to implement the real Raster Geometry path in UE 5.7:
1. Implement Render-Thread Registry for the Scene Proxy.
2. Create GPU Vertex/Index buffers.
3. Write Raster HLSL Shaders (Splat, ForwardShade, Composite).
4. Issue real RDG Draw Calls in `PrePostProcessPass_RenderThread`.
5. Verify Opaque Occlusion using `SceneDepth`.
