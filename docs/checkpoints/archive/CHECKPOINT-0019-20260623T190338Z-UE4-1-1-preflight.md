# CHECKPOINT 0019: UE-4.1.1 Preflight
**Timestamp**: 2026-06-23T19:03:38Z
**Stage**: UE-4.1.1 ACTIVE

## User Instruction
Execute UE-4.1.1 Core Math Acceptance Hardening.
- Re-audit UE-4.1 acceptance (found to be lacking).
- Implement explicit automated depth convention mapping tests.
- Convert readback into strict row-pitch, lifetime, and timeout validation.
- Provide JSON metrics, automated checks, and generate GPU capture.

## Repository State
* **Repository Root**: `D:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT`
* **Branch**: `AVBOIT开发`
* **Local HEAD**: `70d4b7b`
* **Remote HEAD**: `70d4b7b`
* **Working Tree**: clean
* **CURRENT Pointer**: `docs/checkpoints/archive/CHECKPOINT-0018-20260623T153732Z-UE4-1-result.md`
* **Checkpoint 0018 Claim**: PASSED (Now formally REOPENED)

## Runtime Context
* **Current Machine ID**: HIVE-4090X2 (assumed from previous logs)
* **UE Version**: 5.7.4
* **Evidence Root**: `LocalVisualResults/UE57/<MachineId>/UE4-1-1-Core-Acceptance/`

## Source Files Under Modification
* **Backend Source Files**: `AVBOITBackendReadback.cpp`, `AVBOITBackendRenderer.cpp`
* **Shader Source Files**: `AVBOITCommon.ush`, `AVBOITBuild.usf`, `AVBOITIntegrate.usf`, `AVBOITShade.usf`
* **Test Harness Source Files**: `AVBOITBackendTestAutomation.h/.cpp`

## Current Conventions (To be hardened)
* **Current Depth Mapping Formula**: `NormalizedDepth = log2(ZFar / LinearDepth) / log2(ZFar / ZNear)`
* **Current Slice Direction Comment**: Unclear/Inconsistent.
* **Current Integrate Direction**: Needs fixing.
* **Current Shade Sampling Rule**: Needs fixing.
* **Current Readback Row Pitch Handling**: Uses hardcoded `Width` instead of `OutRowPitchInPixels`.
* **Current Expected-Value Assertion Status**: None (Logs only).
* **Current Process Exit Status Handling**: Always 0.
* **Current GPU Capture Status**: Missing.

## First Planned Code Change
Create `UE4_DEPTH_SLICE_CONVENTION.md` to define slice mapping rules explicitly, and implement `TestDepthMapping()` in the test harness to assert slice selection correctness.
