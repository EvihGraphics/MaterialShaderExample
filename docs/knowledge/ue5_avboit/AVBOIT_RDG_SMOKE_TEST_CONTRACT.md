# AVBOIT RDG Smoke Test Contract

UTC: 2026-06-22T16:34:17Z

## Scope

The UE-2 RDG smoke test is an isolated validation pass for AVBOIT integration
points. It is not the production AVBOIT backend.

## Modes

| Mode | Contract |
|---:|---|
| 1 | Deterministic static UV gradient, checkerboard, and ViewRect edge marker. No frame, time, jitter, random, or temporal state dependency. |
| 2 | Real scene depth visualization using `SceneTexturesStruct.SceneDepthTexture` and `ConvertFromDeviceZ`. |
| 3 | SceneColor overlay with UV/checker tint for composite-path validation. |

## Required RDG Pass Names

```text
AVBOIT.Smoke.Clear
AVBOIT.Smoke.Fill
AVBOIT.Smoke.Composite
```

## Non-Goals

The smoke test must not introduce the full AVBOIT volume, atomics, integration,
transparent shade, final resolve, Niagara/Substrate integration, engine-source
changes, or optimization work.

