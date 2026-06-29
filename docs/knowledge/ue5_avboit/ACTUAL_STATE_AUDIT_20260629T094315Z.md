# AVBOIT Actual State Audit

## Metadata
* **Time:** 2026-06-29T09:43:15Z
* **UE Git HEAD:** `260bb2e1c5610b31c63a36206eedd289409c5f11` (D:\UE\UnrealEngine_Animation_Tech)
* **UE Build.version:** 5.7.4 (BranchName: UE5)
* **MaterialShaderExample_AVBOIT Local/Remote HEAD:** `c0411547fef1513dbd9fb6eb14c1d6cff5f66adf`
* **The Forge HEAD:** `8c6f83631b2828052a144f825ae94f9e626384ed`

## CURRENT.md vs. Reality
* **CURRENT.md status:** `UE-4.2A.1-E Editor Baseline Smoke Test` (Baseline `bdea79a6...`)
* **Reality:** The repository has advanced to `c0411547...` (multiple commits ahead of `CURRENT.md`'s baseline, including headless validation and separating results into KeyResults/TempResults). `CURRENT.md` is outdated and doesn't reflect the recent bugfixes and structural changes.

## Current Data Flow & Scene State
1. **Scene Builder (`AVBOITEditorSceneBuilder.cpp`)**:
   - Creates a basic Camera.
   - Creates an AVBOIT Transparent Mesh (Single Quad, White, 0.5 Alpha).
   - Creates an Occluder Cube.
   - Creates a Background Cube.
   - Sets some CVar states for deterministic rendering.
2. **Raster Passes (`AVBOITRasterRenderer.cpp`)**:
   - **Clear, Splat, Integrate, ForwardShade, Composite** passes are present and added to the RDG.
   - A DebugReadback pass is also present.
3. **Data/Resource Constraints (Technical Debt)**:
   - `ExtinctionElements` is severely hardcoded to `1024` (`uint32 ExtinctionElements = TextureExtent.X * TextureExtent.Y * 64; if (true) { ExtinctionElements = 1024; }`), which avoids OOM but breaks any real semantics.
   - `TransmittanceVolume` is created as a 2D Array at FULL resolution (`TextureExtent`) with `64` slices. Extremely inefficient and incorrect for P2.6T.
   - `ZNear` / `ZFar` are hardcoded in multiple places (`View.NearClippingDistance + 10000.0f` and `10.0f` / `1000.0f`).
   - The test merely ensures no crashes rather than semantic correctness.

## Code that can be kept vs. replaced
* **Keep:** General RDG structure, CVar management, DebugReadback framework.
* **Replace:** 
   - Full resolution 2D array allocation for Transmittance.
   - Hardcoded `ExtinctionElements` limit.
   - Hardcoded `ZNear` / `ZFar`.
   - `AVBOITTestMeshSceneProxy` and `AVBOITEditorSceneBuilder` need to be expanded/replaced to support `The Forge` 15_Transparency scene contract (cubes, spheres, custom transforms, etc.).

## Final Scope for this Phase
1. Overhaul resource allocation to match The Forge P2.6T specs (downsampled).
2. Clean up hardcoded depths and array counts.
3. Re-implement the scene generator to match The Forge precisely.
4. Establish true numerical and visual parity.
