# AVBOIT P2.6T Algorithm Migration Contract (The Forge -> UE5.7)

This document ensures strict alignment between the established P2.6T baseline in *The Forge* and the UE5.7 implementation.

| Item | The Forge (Baseline) | Current UE (Baseline) | Target UE | Status |
| --- | --- | --- | --- | --- |
| XY Resolution | `WindowWidth / DownsampleFactor` | Full SceneDepth extent | `ViewRect.Size() / DownsampleFactor` | Precise Alignment Required |
| Z Slice Count | `32 / 48 / 64` (Depends on case) | Fixed 64 slices | Dynamic based on Configuration | Precise Alignment Required |
| Extinction Format | Buffer `R32_UINT` | Buffer `R32_UINT` | Buffer `R32_UINT` | Precise Alignment Required |
| Transmittance Format | Texture2DArray `R32_FLOAT` | Texture2DArray `R32_FLOAT` | Texture2DArray `R32_FLOAT` | Precise Alignment Required |
| Accum Color Format | Texture2D `RGBA16F` | `PF_FloatRGBA` | `PF_FloatRGBA` | Precise Alignment Required |
| Clear Value | Extinction: `0`, Transmittance: `1.0f`, Color: `0,0,0,0` | Hardcoded 0 | Exact matches via Compute | Precise Alignment Required |
| Reverse-Z Linearization | Near=1, Far=0. Proper projection. | Partially implemented. | Align with The Forge P2.6T projection bounds. | Precise Alignment Required |
| Splat Depth Slice | P2.6T mapping function | Partially correct | P2.6T exact match | Precise Alignment Required |
| Downsample Normalization | Divided by `factor * factor` | Hardcoded 1.0 / un-normalized | Normalization applied in splat / integrate | Precise Alignment Required |
| Transmittance Direction | `Front` | `Front` | `Front` | Precise Alignment Required |
| Weight Propagation | `Selected / Front` | `Selected / Front` | `Selected / Front` | Precise Alignment Required |
| Integrate Order | Front to Back | Front to Back | Front to Back | Precise Alignment Required |
| Forward Accumulation | Volumetric Integration | Hardware Blend `Add` | Hardware Blend `Add` | Precise Alignment Required |
| Composite Blend | `SrcAlpha + Dest * (1 - SrcAlpha)` (Equivalent) | UE Composite Shader | Exact Color match | Precise Alignment Required |
| ViewRect Offset | Handled via viewport | Not properly handled (uses `TextureExtent`) | Fully handled via `ViewRectMin` | Precise Alignment Required |
| Draw Order Independence | Order independent | Not rigorously verified | Verified via permutation testing | Independent Verification |
