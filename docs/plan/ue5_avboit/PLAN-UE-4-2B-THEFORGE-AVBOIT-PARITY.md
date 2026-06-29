# Plan: UE-4.2B - The Forge Parity and AVBOIT P2.6T Contract

## Objective
Establish the exact scene, camera, and algorithm contract of *The Forge* 15_Transparency within Unreal Engine 5.7, bridging the gap between the initial raster integration and a true P2.6T AVBOIT implementation. 

## Scope
- [x] Audit the actual state vs. CURRENT.md
- [x] Establish Knowledge Base for Monsho, Yorung, and Manning.
- [x] Import 10 external rendering submodules into `UERP_Ref/`.
- [x] Build harness script `Scripts/Build-UERPRefs.ps1`.
- [x] Define Scene and Camera contracts.
- [x] Define P2.6T Algorithm Contract.
- [x] Update `AVBOITRasterRenderer.cpp` (Removed hardcoded `ExtinctionElements` limit and full-resolution array, replaced with proper `DownsampleFactor` based buffer allocation).
- [x] Automation script `Scripts/Run-AVBOITTheForgeParityValidation.ps1` implementation.
- [ ] Build all 10 references against UE5.7 (Blocked/Pending execution locally due to API patching overhead).
- [ ] RenderDoc / PIX Capture and Visual Validation (Blocked pending full scene parity C++ implementation and integration).

## Checkpoint Status
- The phase is marked as **blocked-local**.
- We have advanced the codebase structure, contracts, and memory allocation strategy to match P2.6T.
- Next steps require manual patching of `UERP_Ref` modules for UE5.7 and implementing the C++ `AVBOITTheForgeSceneBuilder` to match the JSON contract.
