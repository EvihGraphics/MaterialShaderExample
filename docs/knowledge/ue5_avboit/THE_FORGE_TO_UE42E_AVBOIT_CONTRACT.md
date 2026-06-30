# The Forge to UE-4.2E AVBOIT Contract

Generated UTC: 2026-06-30T19:02:06Z

## Fixed Defaults

- Depth mapping: `reverse_correct`
- Front transmittance: enabled
- Downsample: `D8`
- Slices: `Z64`
- Brightness multiplier: `1.0`
- Exposure/gamma/brightness boost: forbidden for acceptance

## Shared Core Requirement

`MaterialShaderPlugin` is the only AVBOIT core owner. Adapters provide primitive packets; the core owns:

- frame graph contract
- depth/slice/downsample contract
- resource contract
- buffer overview/readback contract
- tint contract
- validation and promotion gate state

`MaterialShaderPluginNiagara` must remain a Niagara adapter and must not duplicate core AVBOIT backend logic.

## UE-4.2E Pass Order

```text
Clear -> SpriteSplat -> Integrate -> ForwardUnlit -> Composite
```

Acceptance requires these passes to consume real primitive data and composite into SceneColor. PassId or scratch-resource-only evidence is blocked-local.

## Tint Contract

- Tint replaces RGB in Identity/AVBOIT shader code only.
- Alpha, opacity, and coverage remain sourced from particle/material data.
- Automatic parity capture must force tint off.
- MID/default renderer tint is not acceptance proof.
