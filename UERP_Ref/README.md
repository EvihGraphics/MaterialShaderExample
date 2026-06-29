# UERP_Ref

This directory contains external Unreal Engine 5 rendering repositories that serve as references and integration test cases for AVBOIT.

## Rules
1. These submodules are strictly for testing UE5.7 compatibility and examining reference implementations.
2. They should **not** be included in the main `MaterialShaderDemo.uproject` dependencies to avoid pollution.
3. Each module is pinned to a specific commit, documented in `REFERENCE_MANIFEST.json`.
4. Any patches required to compile these against UE5.7 should be placed in `Patches/<ReferenceName>/` or maintained inside the build harness script.

## Building
Use the script `Scripts/Build-UERPRefs.ps1` to test the compilation of these references in isolation using the UE5.7 source build.
