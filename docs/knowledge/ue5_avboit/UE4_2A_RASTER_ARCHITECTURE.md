# UE4.2A: Raster Architecture

## Overview
The goal of this architecture is to move from a purely synthetic compute-driven test harness into a real graphics pipeline using actual geometry, vertex shaders, and hardware depth testing.

## Pipeline Flow

1. **Synthetic Regression Path (Preserved)**
   - Operates exactly as before (Clear -> Build -> Integrate -> Shade).
   - Serves as the mathematical baseline.

2. **Raster Path (New Production Loop)**
   - **AVBOIT.Raster.Clear**: Clears the Extinction, Transmittance, and ColorAccumulation resources.
   - **AVBOIT.Raster.Splat**:
     - *Pass Type*: Raster (Draw Call)
     - *State*: Depth Test ENABLED, Depth Write DISABLED, Color Write DISABLED.
     - *Bindings*: UAV bound to Extinction Volume.
     - *Action*: Calculates slice depth from `SV_Position`/WorldPos, converts Alpha to Extinction, and performs an `InterlockedAdd` into the `R32_UINT` volume. Opaque occlusion natively rejects fragments via early/late Z testing.
   - **AVBOIT.Raster.Integrate**:
     - *Pass Type*: Compute (Near-to-Far Sweep)
     - *Action*: Sweeps slices 0-63, accumulating Extinction and converting to Transmittance.
   - **AVBOIT.Raster.ForwardShade**:
     - *Pass Type*: Raster (Draw Call)
     - *State*: Depth Test ENABLED (Equal or LessEqual), Color Write ENABLED (Additive Blend).
     - *Bindings*: SRV bound to Transmittance Volume.
     - *Action*: Re-draws the same geometry. Reads `TransmittanceVolume[Slice - 1]` to get `FrontTransmittance`. Outputs `Color * Alpha * FrontTransmittance` directly to the `ColorAccumulation` Render Target. Hardware Additive Blending handles overlaps natively!
   - **AVBOIT.Raster.Composite**:
     - *Pass Type*: Raster (Fullscreen Quad)
     - *Action*: Combines `ColorAccumulation` with the existing `SceneColor` scaled by `TotalTransmittance`.
