# PLAN: UE-4.1.1 Core Math Acceptance Hardening

## Objective
Formalize the UE-4.1 AVBOIT Core Math Implementation by migrating the "log-observation" tests to a strictly automated, evidence-producing executable harness. Freeze depth orientation conventions and ensure full GPU readback of intermediate volumes.

## Implementation Steps
1. **Repository State Audit**: Verify the starting branch and worktree.
2. **Existing UE-4.1 Claim Audit**: Review Checkpoint 0018 for completeness (Reopened).
3. **Depth Convention Freeze**: Define explicit semantics for Slice 0 vs Slice 63, Near vs Far, and integration direction.
4. **Slice Mapping Tests**: Add CPU/GPU tests for the depth mapping function endpoints.
5. **Readback Correctness Repair**: Fix `RowPitch` utilization and strict lifetime management (`TUniquePtr`) for `PendingReadback`. Add timeout handling.
6. **Executable Assertion Harness**: Introduce `FAVBOITBackendTestCase` with explicit `ExpectedColor` and `ExpectedTransmittance` validations. Ensure non-zero exit code on failure.
7. **Intermediate Volume Readback**: Enable GPU extraction of 64-slice Extinction and Transmittance buffers for the center pixel ROI.
8. **Single-Layer Validation**: Assert SingleWhite mathematical correctness.
9. **Double-Layer Physical Orientation Validation**: Assert NearRed/FarBlue and NearBlue/FarRed scenarios for depth sorting.
10. **Triple-Layer Six-Order Validation**: Assert convergence for Triple RGB permutations.
11. **Same-Slice Approximation Characterization**: Characterize collision resolution of two fragments placed in the same slice.
12. **External GPU Capture**: Capture the execution pipeline (Clear, Build, Integrate, Composite, Shade) using RenderDoc/PIX.
13. **Regression**: Validate against UE-2 Smoke, UE-3.1 Standard Alpha, and original plugin code.
14. **UE-4.1 Acceptance Gate**: Accept UE-4.1 if and only if all testing and evidence conditions are completely satisfied.
