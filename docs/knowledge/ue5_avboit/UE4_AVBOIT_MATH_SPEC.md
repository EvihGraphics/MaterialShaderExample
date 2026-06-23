# UE4 AVBOIT Math Specification & The Forge Contract
Extracted directly from `The-Forge` baseline commit `39046051cef37bff0e52e497ca7df07ba9aebe65`.

## Mathematical Constants & Mappings
| Property | The Forge Implementation | Planned UE4 Port | Difference / Reason |
|---|---|---|---|
| **Slice Count** | `64` (`#define AVBOIT_VOLUME_DEPTH 64`) | 64 | Match |
| **Extinction Formula** | `ext = -log(1.0f - min(color.w, 0.99f))` | `Ext = -log2(1.0f - min(Alpha, 0.99f)) * 0.693147` | Use `log2 * ln(2)` for UE shader perf, mathematically identical. |
| **Alpha Clamp** | `min(color.w, 0.99f)` | `min(Alpha, 0.99f)` | Match |
| **Fixed-Point Scale** | `10000.0f` | `10000.0f` | Match |
| **Fixed-Point Packing** | `uint(ext * 10000.0f)` | `(uint)(Ext * 10000.0f)` | Match |
| **Atomic Add Behavior** | `AtomicAdd` on `RWBuffer<uint>` | `InterlockedAdd` on `RWTexture2DArray<uint>` or `RWBuffer<uint>` | RDG resources mapped to UAVs. |
| **Depth Normalization** | Reverse-Z logarithmic: `log2(zFar / linearDepth) / log2(zFar / zNear)` | Reverse-Z Log mapping | Will use UE equivalent logarithmic linear depth mapping. |
| **Slice Index Clamp** | `clamp(uint(normalizedDepth * 64), 0, 63)` | `clamp(uint(NormDepth * 64), 0, 63)` | Match |
| **Transmittance Integration Direction** | 0 to 63 (Near to Far) | 0 to 63 (Near to Far) | Match |
| **Transmittance Stored Before/After Slice** | Stored AFTER: `transmittance *= exp(-ext); Write3D(coords, transmittance);` | Stored AFTER | Match |
| **Forward Shade Slice Sampling Rule** | Current Slice `zIndex`. Samples `zIndex - 1` for Front Transmittance. If `zIndex == 0`, Front Transmittance = `1.0f`. | Same rules | Match |
| **Composite Rule** | Samples furthest slice `volumeDepth - 1` for Total Transmittance. `BgColor * TotalTransmittance` | Same rules | Match |

## Contract Constraints
* **Mathematical Mode**: Must disable `gAVBOITMultiplier = 2.5`. The `Shade` pass must strictly use: `LinearColor * Alpha * FrontTransmittance` without empirical gain.
* **No Engine Fork**: We will execute the above logic via `RDG` passes dispatched from a `SceneViewExtension` or our test component, acting on Synthetic Fragments first.
