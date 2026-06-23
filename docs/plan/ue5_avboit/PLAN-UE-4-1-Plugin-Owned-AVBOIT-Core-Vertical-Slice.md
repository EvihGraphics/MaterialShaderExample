# PLAN: UE-4.1 Plugin-Owned AVBOIT Core Math & RDG Vertical Slice

## Objective
Establish the core AVBOIT volumetric integration math and memory resources inside a plugin-owned RDG pipeline, completely decoupling from the previous flawed Engine Fork Linked-List attempts. Prove deterministic mathematical convergence independent of submission order.

## Implementation Steps
1. **State Metadata Repair**: Normalize checkpoint sequences and fix `CURRENT.md` metadata models.
2. **UE-3.1 Background Contract Repair**: Remove the faux-opaque material plane and use `SceneCapture->PrimitiveRenderMode = PRM_UseShowOnlyList` to absolutely isolate the testing domain from map pollution. Verify regression passes.
3. **The Forge Algorithm Freeze**: Extract exact constants (Slice count, Extinction Formula, Fixed-Point Scale) from `15_Transparency` and formalize in `UE4_AVBOIT_MATH_SPEC.md`.
4. **UE Resource Contract**: Establish the 64-slice `Texture2DArray` formats and memory usage for Extinction and Transmittance.
5. **Fragment Injection Contract**: Construct `FAVBOITInjectedFragment` and the Synthetic Deterministic Injection pattern from CPU.
6. **Clear Pass**: Implement `AVBOIT.Clear` to zero out the volumes.
7. **Build / Splat Pass**: Implement `AVBOIT.Build` to atomically accumulate Extinction into the Fixed-Point volume.
8. **Integrate Pass**: Implement `AVBOIT.Integrate` to convert Optical Depth into Transmittance.
9. **Composite Pass**: Implement `AVBOIT.Composite` to apply the background transmittance rule.
10. **Shade Pass**: Implement `AVBOIT.Shade` to add front-transmittance-weighted fragment colors to the Result.
11. **Direct Linear Readback**: Implement GPU Readback mechanism for test suite validation.
12. **SingleWhite Validation**: Validate single opaque/translucent layer Math convergence.
13. **DoubleRedBlue Order Independence**: Validate 2-layer permutation order independence.
14. **TripleRGB Six-Order Convergence**: Validate full 3-layer 6-permutation convergence to standard physical depth rendering.
15. **GPU Capture**: Verify passes using external graphics debugger (RenderDoc/PIX).
16. **UE-2 / UE-3 Regression**: Ensure standard functionality is unaffected.
17. **UE-4.1 Exit Gate**: Formally evaluate the implementation against the acceptance criteria.
