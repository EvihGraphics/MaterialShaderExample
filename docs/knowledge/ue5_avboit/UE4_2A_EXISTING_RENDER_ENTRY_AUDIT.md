# UE4.2A: Existing Render Entry Audit

## Current SceneViewExtension State
- **Existence**: Yes, `FAVBOITSceneViewExtension` currently exists.
- **Why it was disabled (if applicable)**: It is conditionally enabled via the `AVBOIT::Smoke::IsEnabled()` cvar.
- **Current Rendering Hook**: It hooks into the post-processing graph via `SubscribeToPostProcessingPass`, specifically targeting `EPostProcessingPass::AfterDOF`.

## Current Synthetic Test Call Chain
1. Game Thread sets up parameters in `MaterialShaderExampleSubsystem` or the test automation harness.
2. Render Thread invokes `AVBOITBackendTestAutomation` which bypasses the SceneViewExtension entirely! It manually enqueues rendering commands (`ENQUEUE_RENDER_COMMAND`) and builds a standalone standalone RDG graph with `FRDGBuilder`.
3. The standalone synthetic test executes `AVBOITBuild.usf`, `AVBOITIntegrate.usf`, and `AVBOITShade.usf` using injected fragments and fixed dummy `FRDGTexture` objects.

## Ownership and Hardcoded Values
- **Game Thread Owner**: `AVBOITSmokeSubsystem` manages the `FAVBOITSceneViewExtension`. The `AVBOITBackendTestAutomation` is a separate C++ module.
- **Render Thread Owner**: The RDG Graph Builder inside the specific delegate or `ENQUEUE_RENDER_COMMAND` block.
- **RDG Resource Owner**: The standalone RDG graph during the test execution. Resources are transient.
- **Hardcoded 512x512**: The synthetic testing suite currently hardcodes `512x512` texture sizes for the intermediate Extinction and Transmittance volumes.
- **Hardcoded CenterPixel=(256,256)**: Debug readback targets a fixed pixel index based on the 512x512 resolution.
- **Resource Creation/Destruction**: Textures are created per-execution using `GraphBuilder.CreateTexture` and destroyed immediately after the execution finishes (transient).

## Multi-View and Resize Behavior
- **Multi-View**: The synthetic test is completely agnostic to active Views; it runs purely headless. The `AVBOITSceneViewExtension` runs per active `FSceneView` but is currently a mock smoke effect.
- **Resize**: Since the synthetic test uses a hardcoded 512x512 resolution, it does not respond to Viewport resizing.
- **SceneColor / SceneDepth Access**: The synthetic test does NOT access `SceneColor` or `SceneDepth`. It uses dummy textures and manual depth arrays. The `Smoke` post-process effect accesses `SceneColor` via `FPostProcessMaterialInputs`, but does not use Depth for hardware testing.
