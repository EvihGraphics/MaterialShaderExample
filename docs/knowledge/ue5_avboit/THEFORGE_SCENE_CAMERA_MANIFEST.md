# The Forge Scene Camera Manifest

Generated UTC: 2026-06-22T08:19:40Z

## Camera

Confirmed in `15_Transparency.cpp`:

| Item | Value |
|---|---|
| Position | `(-40, 17, 34)` |
| LookAt | `(0, 5, 0)` |
| Motion parameters | acceleration `16`, braking `60`, max speed `20` |
| Horizontal FOV | `PI / 2` radians, 90 degrees |
| Projection | `perspectiveReverseZ(horizontal_fov, aspectInverse, zNear, zFar)` |

## Key Scene Content

Confirmed through `CreateScene()` source scan:

- Large ground cube.
- Transparent cube matrix.
- Red, green, blue plane/cube groups at alpha 0.9 and alpha 0.5.
- 25 low-alpha slices.
- Opaque/transparent depth test objects.
- Sphere and lion mesh.
- Transmittance/refraction/collimation test group.
- Measuring grid.
- Two particle systems.
- AVBOIT RGB ground truth planes:
  - Red: `(-10, 15, 10)`, color `(1, 0, 0, 0.5)`.
  - Green: `(-10, 15, 5)`, color `(0, 1, 0, 0.5)`.
  - Blue: `(-10, 15, 0)`, color `(0, 0, 1, 0.5)`.
  - Scale `(2, 2, 2)`, rotation `(-PI/2, -PI/4, 0)`.

## Visual References

| Mode | Path | Size | Pixel Format | SHA-256 |
|---|---|---|---|---|
| Mode 0 | `D:\HTC\avboit\The-Forge\LocalVisualResults\HIVE_4090x2\VisualResults\15_Transparency\Screenshots\UT_15_Transparency_DX12_Mode_0.png` | 1920x1080 | `Format24bppRgb` | `EF55BD5173AD97B6354F0426C44E304D42231D243AAA7346275A5201F831075E` |
| Mode 5 | `D:\HTC\avboit\The-Forge\LocalVisualResults\HIVE_4090x2\VisualResults\15_Transparency\Screenshots\UT_15_Transparency_DX12_Mode_5.png` | 1920x1080 | `Format24bppRgb` | `F2B4F7E4A85D01B73DBE3358C7F97948DC8E6FD2F81647E1EBECE076AA3E897D` |

## UE Replica Status

`not-started` in this bootstrap cycle. Gate A is blocked by missing UE 5.7 build/run path.

