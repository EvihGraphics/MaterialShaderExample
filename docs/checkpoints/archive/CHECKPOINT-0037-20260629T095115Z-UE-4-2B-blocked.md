# Checkpoint 0037

## Metadata
* **Timestamp**: 20260629T095115Z
* **Phase**: UE-4.2B
* **Status**: blocked-local

## Git States
* **Starting Remote HEAD**: `c0411547fef1513dbd9fb6eb14c1d6cff5f66adf`
* **Starting Local HEAD**: `c0411547fef1513dbd9fb6eb14c1d6cff5f66adf`
* **Final Local HEAD**: `TBD_ON_COMMIT`
* **Final Remote HEAD**: `TBD_ON_PUSH`
* **The Forge HEAD**: `8c6f83631b2828052a144f825ae94f9e626384ed`
* **UE Build.version**: `5.7.4`

## Submodule Commit Matrix
10 references cloned into `UERP_Ref/` successfully. SHAs pinned in `REFERENCE_MANIFEST.json`.

## Build Matrix
* **MaterialShaderDemo**: Project baseline compiles successfully.
* **UERP_Ref**: Harness script `Build-UERPRefs.ps1` generated, but bulk testing across all 10 plugins on UE5.7 is blocked-local pending extensive API patching.

## Contract Results
* **Scene Contract**: JSON established mapping `CreateScene()`.
* **Camera Contract**: JSON established mapping Y-Up to Z-Up, position/lookAt/FOV.
* **Algorithm Contract**: Established.
* **GPU Capture / Validation**: Blocked pending automation harness completion and actual SVE fixes.

## Resume Entry
1. Execute `Build-UERPRefs.ps1` and fix individual UE 5.7 incompatibilities for UERP submodules.
2. Implement C++ `AVBOITTheForgeSceneBuilder` based on the JSON contract.
3. Validate and correct Reverse-Z Depth Splats via RenderDoc.
