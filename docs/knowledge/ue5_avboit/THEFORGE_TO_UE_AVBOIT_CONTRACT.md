# The Forge To UE AVBOIT Contract

Generated UTC: 2026-06-22T08:19:40Z

## Frozen For Bootstrap

| Contract Area | Current Reference |
|---|---|
| Pass order | Clear -> Splat/Build -> Integrate -> Composite/Resolve -> Forward/Shade |
| Volume depth | 64 slices in current The Forge implementation |
| Extinction | `-log(1 - min(alpha, 0.99))`, packed as fixed-point for atomic add |
| Depth | Reverse-Z input, linearized from clip info, then log-normalized into slice index |
| Transmittance integration | front-to-back accumulation using `exp(-extinction)` per slice |
| Transparent shade | samples previous slice transmittance and adds shaded RGB contribution |
| Debug evidence | The Forge GPU markers use Clear/Splat/Integrate/Composite/Forward AVBOIT labels |

## UE Candidate Mapping

| The Forge Stage | UE Bootstrap Candidate |
|---|---|
| Clear | RDG compute pass clearing plugin-owned textures/buffers |
| Splat/Build | plugin-owned deterministic geometry path before any general translucency hook |
| Integrate | RDG/global compute shader over view extent |
| Composite/Resolve | full-screen RDG pass into or beside SceneColor debug output |
| Forward/Shade | controlled test shader, not Niagara/Substrate/common translucency |

## Deterministic Ground Truth

- Single white alpha 0.5 over black: linear RGB `(0.5, 0.5, 0.5)`, `T = 0.5`.
- Front red, middle green, back blue, each alpha 0.5 over black: linear RGB `(0.5, 0.25, 0.125)`.
- Six draw orders must converge for the same physical depth order.

## Open Items

- UE build/run is blocked until a UE 5.7 path is supplied.
- The The Forge `gAVBOITMultiplier` is current reference behavior, not automatically accepted as the UE unit-test math contract.
- Pixel-level cross-engine parity is not a bootstrap gate.

