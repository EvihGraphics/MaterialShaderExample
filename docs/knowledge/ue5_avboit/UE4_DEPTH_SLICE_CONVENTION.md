# UE4 AVBOIT Depth & Slice Convention

## Explicit Ground Truth

To resolve inconsistencies between The Forge's original depth mapping and strict order-independent transparency requirements, we freeze the following convention for mathematical correctness:

| Concept | Semantic Meaning | Endpoint Value |
|---|---|---|
| **Camera Front** | Closest visible plane to the camera. | `ZNear` |
| **Camera Back** | Furthest visible plane from the camera. | `ZFar` |
| **Slice 0** | The physical **Near** plane. | `NormDepth = 0.0` |
| **Slice 63** | The physical **Far** plane. | `NormDepth = 1.0` |

### Depth Mapping Formula
```hlsl
NormDepth = log2(LinearDepth / ZNear) / log2(ZFar / ZNear);
```
- `LinearDepth = ZNear` → `NormDepth = 0.0` → `Slice 0`
- `LinearDepth = ZFar` → `NormDepth = 1.0` → `Slice 63`

### Integration Direction
**Near-to-Far** (Slice 0 to Slice 63).
- At step `z`, the `TransmittanceVolume` stores the product of extinctions from Slice `0` up to Slice `z`.
- Meaning: `TransmittanceVolume[z]` represents the **Camera-to-Slice** transmittance (the light attenuation caused by all slices from the camera down to and including `z`).

### Shade Sampling Meaning
When a fragment is injected at `zIndex`:
- It must be attenuated by all fragments strictly in front of it.
- Fragments in front occupy slices `0` to `zIndex - 1`.
- Therefore, **Front Transmittance** is sampled at `zIndex - 1`.
- If `zIndex == 0`, Front Transmittance is exactly `1.0` (nothing in front).

## The Forge Compatibility Note
The Forge's original implementation implicitly mapped `ZNear` to `1.0` (Slice 63) while still integrating from `0` to `63`. This physically meant it integrated from Far to Near, but sampled `zIndex - 1` as if it were Front Transmittance, effectively attenuating front fragments by the background behind them. We have explicitly abandoned that behavior for `MathematicalMode` to ensure correct physical ordering where front objects occlude back objects correctly.
