# CHECKPOINT 0021: UE-4.1.2 Preflight

## Status: preflight

## Context
- User Instruction: Implement UE-4.1.2 Intermediate Volume Readback, Physical Depth Proof, Same-Slice Characterization & GPU Capture Closeout.
- Repository Root: D:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT
- Branch: AVBOIT开发
- Local HEAD: 202deedd6ec164350f56660b063e769809838aa2
- Remote HEAD: 202deedd6ec164350f56660b063e769809838aa2
- Ahead / Behind: 0 / 0
- Working Tree: clean
- Worktree List: master (MaterialShaderExample), AVBOIT开发 (MaterialShaderExample_AVBOIT)

## Pointers
- Current Pointer: UE-4.1.2
- Checkpoint 0020 Claim: UE-4.1.1 PASSED
- Current Machine ID: To be resolved from local env.
- UE Version: 5.7
- Evidence Root: LocalVisualResults/UE57/<MachineId>/UE4-1-2-Acceptance-Closeout/

## Test Status
- Current Test Case Count: 9
- Current Passed Count: 9
- Current Failed Count: 0

## Readback Status
- Current Result Readback: Implemented
- Current Extinction Readback: Missing
- Current Transmittance Readback: Missing

## Coverage Status
- Current Mapping Test Coverage: Missing
- Current Physical Reversal Coverage: Missing
- Current Same-Slice Coverage: Missing
- Current GPU Capture Status: Missing
- Current Regression Status: Missing

## Action Plan
First Planned Code Change: Implement `AVBOITMapLinearDepthToNormalized` and `AVBOITMapLinearDepthToSlice` in C++ to match `AVBOITCommon.ush`. Create the CPU mapping verification tests.
