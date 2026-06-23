# CHECKPOINT 0017: UE-4.1 Preflight
**Timestamp**: 2026-06-23T07:00:00Z
**Stage**: UE-4.1 ACTIVE

## User Instruction
Execute UE-4.1: Plugin-Owned AVBOIT Core Math & RDG Vertical Slice. 
- Implement synthetic fragment deterministic integration.
- Strictly mathematically validate Order Independence using 64-slice volumes.
- Forbid linked list & Engine fork designs.

## Repository State
* **Repository Root**: `d:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT`
* **Branch**: `AVBOIT开发`
* **Local HEAD**: `a1099e9`
* **Remote HEAD**: `2bb1d77` (Before local commit)
* **Working Tree**: clean (Post-commit)

## Commits & Contracts
* **Latest UE-2 Verified Commit**: `90f9514c7744f0da343e480acc91e3a235c52220`
* **Latest UE-3.1 Runtime Verified Commit**: `ddc68ec0565e54a7195a3f00ae94c251e462e1bb`
* **Project UE Contract**: 5.7.4 (Win64 Development)
* **UE-3.1 Background Contract**: Repaired using `SceneCapture->PrimitiveRenderMode = PRM_UseShowOnlyList`. True regression validation passed.

## AVBOIT Constraints
* **The Forge Reference Branch**: `baseline/theforge-1.58-windows-vs-dx12`
* **The Forge Reference HEAD**: `39046051cef37bff0e52e497ca7df07ba9aebe65`
* **Algorithm Contract**: 64-slice extinction/transmittance integration.
* **Forbidden Design**: Fragment Linked-List, Head Pointers, BasePassPixelShader Fork.
* **First Planned Backend Change**: Parse The Forge mathematical constants and formulate `UE4_AVBOIT_MATH_SPEC.md` for Shader mapping.
