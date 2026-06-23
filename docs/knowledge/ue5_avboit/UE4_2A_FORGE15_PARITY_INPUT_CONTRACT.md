# UE4.2A: The Forge 1.58 Transparency Parity Input Contract

## Overview
This document extracts the exact scene parameters and camera settings from The Forge 1.58 (`15_Transparency`) reference project. These parameters will be used in UE-4.2B to construct a geometrically and optically identical scene.

## Camera Configuration
- **Initial Camera Position (`camPos`)**: `(-40.0, 17.0, 34.0)`
- **Initial Look-At (`lookAt`)**: `(0.0, 5.0, 0.0)`
- **Horizontal FOV**: `PI / 2.0f` (90 degrees)
- **Near Plane (`zNear`)**: `1.0f`
- **Far Plane (`zFar`)**: `4000.0f`
- **Projection Matrix**: `perspectiveReverseZ(horizontal_fov, aspectInverse, zNear, zFar)`

## Lighting
- **Light Position (`lightPos`)**: Typically aligned with a fixed directional shadow map camera.
- **Light Near Plane (`lightZNear`)**: `-100.0f`
- **Light Far Plane (`lightZFar`)**: `100.0f`
- **Light Projection**: `orthographicLH(-50.0f, 50.0f, -50.0f, 50.0f, 0.0f, lightZFar - lightZNear)`

## Scene Composition (Preview)
- Includes opaque objects (floor, pedestals).
- Includes transparent objects (Lion model, spheres, planes).
- Includes particle systems rendered as transparent billboards.

## Coordinate System
- **Handedness**: Left-Handed (LH). The Forge uses `orthographicLH` and D3D standard conventions by default.
- **Up Axis**: The camera settings imply `Y` is up. `(0, 5, 0)` is the look-at point above the origin.
- Unreal Engine uses a Left-Handed Z-Up coordinate system. When porting The Forge scene to UE, a `(X, Y, Z)` to `(Z, X, Y)` or similar rotational swizzle will be required to maintain the exact visual arrangement.

## AVBOIT Multiplier
- As established in the multiplier policy, to achieve visual parity with The Forge's standard Alpha Blend mode (Mode 0), an empirical `ReferenceBrightnessMultiplier = 2.5` is required in the forward color accumulation pass due to the energy conservation differences inherent to the AVBOIT accumulation formula versus standard alpha blending over opaque backgrounds.
