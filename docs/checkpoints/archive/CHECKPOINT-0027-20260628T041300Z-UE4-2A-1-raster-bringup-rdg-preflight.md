# CHECKPOINT-0027-20260628T041300Z-UE4-2A-1-raster-bringup-rdg-preflight

## System Context
- **Starting HEAD:** `20ee47d`
- **Expected Remote HEAD:** `20ee47d`
- **Machine ID:** HIVE_4090x2

## Status
- **Milestone:** UE-4.2A.1
- **Status:** PREFLIGHT

## Preflight Checks
1.  **Git State**: Checked out `AVBOIT开发`. Working tree clean. `HEAD` is `20ee47d`, matching remote.
2.  **Implementation Baseline**: Confirmed `CURRENT.md` semantics update (baseline is currently `feb7059`, will update after this turn).
3.  **Hazard Mitigation Plan**: 
    - Fix `ViewRect.Min` out-of-bounds in Shaders by allocating RDG Volumes to `SceneDepth->Desc.Extent` (safer, less shader math).
    - Fix `Composite` Pass by using Hardware Blend instead of manual SceneColor fetch, as we cannot replace `SceneColor` pointer in `PrePostProcessPass_RenderThread`.
4.  **C++ Execution Plan**: 
    - Write C++ `DECLARE_GLOBAL_SHADER` macros and Parameter Structs for Splat, Forward, Composite.
    - Write the missing RDG code in `FAVBOITRasterRenderer::AddPasses`.
    - Create C++ Test Automation `AVBOITRasterBringUpTest`.

## Next Action
Implement the C++ Global Shader classes and the RDG orchestration logic.
