# PLAN UE-4: OIT Core Fork & Engine Modification

## 1. Objective
Establish a formal engine fork or local source patch strategy to inject the AVBOIT (Adaptive Volumetric Blend Order Independent Transparency) compute passes into the Unreal Engine 5.7 rendering pipeline. Unlike the previously completed UE-2 smoke tests (which used RDG scene hooks) and UE-3 test content (which used pure plugin logic), UE-4 requires modifying the actual `MobileBasePass` or `DeferredShadingRenderer` to replace standard alpha blending with the AVBOIT accumulators.

## 2. Requirements & Constraints
*   **Target Engine**: UE 5.7 (`D:\UE\UnrealEngine_Animation_Tech`)
*   **Modification Pattern**: Minimize invasive changes. Use engine rendering extension points where possible, but if native engine shader modification is required (e.g., `BasePassPixelShader.usf` or `MobileBasePassPixelShader.usf`), clearly document the diffs and hook points.
*   **Data Structures**: Integrate `The-Forge` based linked-list / visibility node structures into the RDG.
*   **Fallback**: Must seamlessly fallback to standard translucency if the AVBOIT CVAR is disabled (`r.AVBOIT.Enable=0`).

## 3. Execution Phases

### Phase 4.1: Architecture Review & Engine Diff Setup
1. Identify the exact render hook locations in `FDeferredShadingSceneRenderer::Render` or `FMobileSceneRenderer::Render`.
2. Analyze the requirements for modifying `BasePassPixelShader.usf` to output to a UAV (RWStructuredBuffer) instead of or in addition to the standard SceneColor render target.
3. Establish a patch tracking system (e.g., `git diff` patches stored in `docs/engine_patches/`) so the project remains portable.

### Phase 4.2: Resource Allocation (RDG)
1. Initialize the global Head Pointer Texture (Screen size `R32_UINT`).
2. Initialize the structured buffer for Visibility Nodes (Color, Depth, Transmittance, NextNode).
3. Bind these resources during the Transparency Pass.

### Phase 4.3: Shader Injection
1. Patch `BasePassPixelShader.usf` to execute the node insertion logic (Atomic operations).
2. Create the `avboit_integrate.comp.usf` (Compute Shader) to sort the linked list and composite the final colors.
3. Dispatch the integrate compute shader immediately after the standard transparency pass.

### Phase 4.4: Local Verification
1. Run the `AVBOIT.Test.CaptureRGBSuite` command developed in UE-3.
2. Verify that the order-independent RGB permutations all yield the exact same visual result as the `CorrectSort` standard alpha reference.

## 4. Exit Gates
- [ ] **Engine Patch Established**: Clear, apply-able patch file exists in `docs/engine_patches/`.
- [ ] **Build Success**: Engine and Project compile successfully with the modifications.
- [ ] **OIT Activation**: Setting `r.AVBOIT.Enable=1` successfully redirects translucent pixel writing to the OIT buffers.
- [ ] **Visual Parity**: The UE-3 RGB Suite confirms order independence.
