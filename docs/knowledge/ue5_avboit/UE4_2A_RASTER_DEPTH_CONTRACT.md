# UE4.2A: Raster Depth Contract

## UE Reverse-Z vs AVBOIT Linear Depth
Unreal Engine uses a **Reverse-Z** depth buffer. This means:
- The near plane is `Z = 1.0` (or `MaxZ`).
- The far plane is `Z = 0.0`.
- Depth values are stored non-linearly to maximize precision near the camera.

AVBOIT requires **Linear Depth** (a distance value) to map objects into the volumetric slices correctly.
Therefore, the Pixel Shader must convert the `SV_Position.z` (or interpolate World Position to View Space) into a positive `Linear Depth` distance.

We will use UE's standard conversion utilities (e.g., `ConvertFromDeviceZ`) to get linear view depth, and ensure the depth is positive (`LinearDepth > 0`).

## Linear Depth Mapping
The extracted `LinearDepth` is mapped to a Normalized Depth using:
`MapLinearDepthToNormDepth(LinearDepth, ZNear, ZFar)`

- **Physical Near Plane**: Normalized Depth 0.0 -> Slice 0
- **Physical Far Plane**: Normalized Depth 1.0 -> Slice 63

*Note: The `ZNear` and `ZFar` bounds must be explicit constants (or driven by test configuration) and should not dynamically shrink/expand per-frame to preserve temporal consistency.*

## Screen Pixel Coordinates
- We will use `SV_Position.xy` in the Pixel Shader.
- The Extinction volume is an `R32_UINT` Texture2DArray.
- The `x` and `y` indices correspond to `SV_Position.xy` bounded by `ViewRect`.
- Non-zero `ViewRect.Min` (e.g. split-screen) must be subtracted if the Extinction Volume texture size exactly matches the ViewRect size. If the texture matches the overall SceneColor size, `SV_Position.xy` maps 1:1.

## Stereo / Multi-View Limits
- Currently, stereo rendering is not fully supported by the AVBOIT structure. Multi-view scenarios must allocate separate volumetric buffers or use Texture2DArray with a `ViewIndex` dimension. For UE-4.2A, we assume a single primary view per `SceneViewExtension` invocation.

## Slice Boundary Processing
- `NormDepth` must be clamped `[0.0, 1.0]` before calculating the slice index.
- Slice index calculation: `clamp(floor(NormDepth * 64.0), 0, 63)`
- Any fragment falling outside the Near/Far range is clamped to the first or last slice, maintaining volume integrity without Out-Of-Bounds writes.
