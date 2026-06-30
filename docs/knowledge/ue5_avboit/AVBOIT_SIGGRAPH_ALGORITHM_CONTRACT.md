# AVBOIT Algorithm Contract

Generated UTC: 20260630T171030Z

## Required Pass Order

1. Clear
2. SpriteSplat
3. Integrate
4. ForwardUnlit
5. Composite

## Resource Contract

- Extinction: receives per-sprite opacity/extinction contributions in depth bins.
- Transmittance: integrated from Extinction; at least one participating pixel must have transmittance lower than 1 for non-empty scenes.
- ColorAccumulation: receives unlit color contribution after transmittance evaluation.
- AlphaAccumulation: tracks opacity/coverage without changing the source alpha contract.
- Composite: combines SceneColor and accumulation into final SceneColor.
- BufferOverview: debug-only view of the real intermediate resources.

## Niagara Sprite Contract

- Position, Size, Rotation, Facing, SubUV, Color, Alpha/Opacity, Material, and simulation target must be recorded.
- CPU Niagara sprite is the first target.
- GPU Compute Sim, Mesh, Ribbon, and Light renderers are outside UE-4.2D scope unless explicitly marked unsupported.
- A particle-state hash must be recorded for deterministic comparison.

## Tint Contract

- Tint replaces RGB only in the AVBOIT ForwardUnlit shader path.
- Alpha and coverage must remain driven by the particle/material alpha.
- Engine Default and PluginBypass must never consume tint.
- Automatic parity capture must force tint off.

## Acceptance Gate

- Passing evidence requires real Niagara data, nonzero GPU resources, and SceneColor composite.
- Foundation/resource scheduling alone is not sufficient and must keep StageStatus `partial` or `blocked-local`.
