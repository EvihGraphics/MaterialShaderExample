# UE4.2A: UE5.7 Render Hook Decision

## Candidate Entry Points
1. **`PostOpaqueRenderDelegate`**: Hooked via `IRendererModule::Get().RegisterPostOpaqueRenderDelegate`. Executes immediately after Opaque rendering. Depth is bound and accessible. SceneColor is unlit or partially lit depending on Deferred/Forward.
2. **`SceneViewExtension::PostRenderBasePassDeferred_RenderThread`**: Executes right after Base Pass. Depth is bound, but SceneColor does not yet contain deferred lighting. Translucency compositing here would happen over an unlit GBuffer.
3. **`SceneViewExtension::PrePostProcessPass_RenderThread`**: Executes after lighting and standard translucency (if rendered in BeforeDOF), right before the post-processing graph begins. Provides `Inputs.SceneTextures` containing the lit `SceneColor` and `SceneDepth`.
4. **`SceneViewExtension::SubscribeToPostProcessingPass`**: Current approach using `EPostProcessingPass::AfterDOF`. Executes late in the PP graph.

## Selected Entry Point
We will use **`SceneViewExtension::PrePostProcessPass_RenderThread`**.

### Call Timing
This runs right before the main post-processing graph begins.
- Opaque geometry has been rendered.
- Deferred lighting has resolved.
- Sky and standard (Separate) Translucency have likely been rendered.
- `SceneColor` contains the fully lit opaque scene.

### SceneColor and SceneDepth State
- `SceneColor` is provided via `Inputs.SceneTextures.SceneColorTexture` (typically `PF_FloatRGBA` HDR format).
- `SceneDepth` is provided via `Inputs.SceneTextures.SceneDepthTexture` (typically `PF_DepthStencil`).
- We can bind `SceneDepthTexture` as `DepthStencil` in our `FRenderTargetBindingSlots` using `FExclusiveDepthStencil::DepthRead_StencilNop` to allow hardware Depth Testing without writing to the depth buffer.

### Normal Translucency Order
By executing in `PrePostProcessPass_RenderThread`, we run *after* `BeforeTranslucency` but *before* `BeforeDOF` Post Processing. This allows our transparent objects to correctly composite over the lit scene.

### Multi-View Behavior
`SceneViewExtension` inherently supports multi-view (e.g. Stereo/VR, Editor splits) by iterating over all active views. The `PrePostProcessPass_RenderThread` callback is invoked per-view.

### No Engine Modification
This utilizes standard `ISceneViewExtension` APIs. No engine modifications are necessary.

### Known Limitations
- We must manually construct the `FRenderTargetBindingSlots` and bind `SceneDepthTexture` as a read-only depth target to achieve Raster Splat.
- SceneColor might be modified by preceding passes. We must ensure we correctly load and store or create intermediate textures if `SceneColor` doesn't allow direct Read/Write UAV composition.
