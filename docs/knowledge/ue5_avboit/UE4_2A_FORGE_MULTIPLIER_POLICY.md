# UE4.2A: The Forge Multiplier Policy

## Overview
To address the luminance discrepancy observed in the AVBOIT Mode 5 pipeline vs Alpha Blending (Mode 0) in The Forge, we introduced a luminance multiplier.

We establish two explicit profiles to ensure mathematical oracle validation isn't polluted by visual-target hacks:

### 1. PhysicalBaseline (Default)
- **`ReferenceBrightnessMultiplier = 1.0`**
- Represents the strict mathematical integration of the volumetric slices.
- Used in ALL automated regression and mathematical testing suites.

### 2. ForgeLegacyParity
- **`ReferenceBrightnessMultiplier = 2.5`**
- Adjusts the empirical brightness to visually match The Forge's standard Alpha Blend mode.
- Used for visual screenshots and parity verification against the Forge reference.

## Application Rules
The multiplier MUST ONLY be applied to the **Forward Color Contribution**:
`Contribution = LinearColor * Alpha * FrontTransmittance * ReferenceBrightnessMultiplier`

**Prohibited Modifications:**
The multiplier MUST NEVER be applied to:
- `Alpha` (Fragment opacity)
- `Extinction` (Calculated density)
- `Packed Extinction` (Storage representation)
- `Transmittance` (Accumulated visibility)
- `Slice Mapping` or Distance mapping
- `Scene Background` or `Exposure`
- The Integration Compute Shader
