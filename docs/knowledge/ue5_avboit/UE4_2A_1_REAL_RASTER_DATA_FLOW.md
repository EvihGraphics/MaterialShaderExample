# UE4.2A.1: Real Raster Data Flow

## 1. Game Thread Definition
- The user places a `UAVBOITTestMeshComponent` in the scene.
- Modifies properties: `MaterialParams.Color`, `MaterialParams.Alpha`, and standard `Transform`.
- UE calls `SendRenderTransform_Concurrent` or `CreateRenderState_Concurrent` which triggers `CreateSceneProxy`.

## 2. Proxy Initialization & Render Thread Registry
- `FAVBOITTestMeshSceneProxy` is constructed.
- Inside `FAVBOITTestMeshSceneProxy::CreateRenderThreadResources()`:
  - Generates immutable `Vertex Buffer` (e.g., 4 vertices for a Quad) and `Index Buffer` (e.g., 6 indices).
  - Enqueues a command to register `this` pointer into `FAVBOITRasterSceneData::ActiveProxies`.
- Inside `FAVBOITTestMeshSceneProxy::DestroyRenderThreadResources()`:
  - Enqueues a command to unregister `this` pointer from `FAVBOITRasterSceneData::ActiveProxies`.
  - Releases GPU buffers.

## 3. Render Pass Injection
- The engine rendering pipeline proceeds through Opaque, Lighting, and standard Translucency.
- `FAVBOITSceneViewExtension::PrePostProcessPass_RenderThread` is invoked right before Post-Processing (BeforeDOF).
- It retrieves `Inputs.SceneTextures.SceneDepth` and `SceneColor`.
- Checks `FAVBOITRasterSceneData::ActiveProxies`. If empty, returns early.

## 4. RDG Raster Passes Execution
- **Splat Pass:**
  - Creates an RDG Raster Pass.
  - Binds `SceneDepthTexture` as Read-Only Depth target (`FExclusiveDepthStencil::DepthRead_StencilNop`).
  - No Color Render Target bound.
  - Binds `ExtinctionVolume` as UAV.
  - Iterates `FAVBOITRasterSceneData::ActiveProxies` and issues `RHICmdList.DrawIndexedPrimitive()`.
- **Integrate Pass:**
  - Standard Compute Dispatch.
- **ForwardShade Pass:**
  - Creates an RDG Raster Pass.
  - Binds `ColorAccumulation` as Color Render Target with Additive Blending.
  - Binds `SceneDepthTexture` as Read-Only Depth.
  - Binds `TransmittanceVolume` as SRV.
  - Iterates proxies and issues `RHICmdList.DrawIndexedPrimitive()`.
- **Composite Pass:**
  - Fullscreen Quad Draw reading `ColorAccumulation`, `Transmittance`, and `SceneColor`, outputting to `SceneColor`.
