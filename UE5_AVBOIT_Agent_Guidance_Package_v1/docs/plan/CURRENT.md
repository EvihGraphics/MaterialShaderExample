# Guidance Package Current Mirror

This file is not the active recovery pointer. The active UE worktree state is:

```text
docs/plan/CURRENT.md
```

in the repository root of:

```text
MaterialShaderExample_AVBOIT
```

Latest mirrored checkpoint:

```text
docs/checkpoints/archive/CHECKPOINT-0046-20260701T052657Z-UE-4-2F-ROI-RED-TINT-VISUAL-GATE.md
```

Current mirrored status:

```text
partial
```

UE-4.2F adds a UE-rendered ROI red-tint visual gate for `TestSpriteMap1`.
`AVBOIT.Niagara.ShowOverlay` must be visible and clickable in GameViewport or
Editor PIE LevelViewport, and `AVBOIT.Niagara.CaptureTintComparison` must
capture EngineDefault-before, PluginAVBOIT-red-tint, and EngineDefault-after
through `UE.FScreenshotRequest`.

KeyResults promotion is allowed only for a passing final-timepoint ROI red-tint
visual gate. This does not pass the UE-4.2E real Niagara draw bridge gates.

Do not mark `SUCCESS`, `COMPLETED`, or `passed-local` for the overall project in
the current state.
