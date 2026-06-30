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
docs/checkpoints/archive/CHECKPOINT-0045-20260630T190206Z-UE-4-2E-REAL-NIAGARA-SPRITE-DRAW-BRIDGE.md
```

Current mirrored status:

```text
blocked-local
```

UE-4.2E is a Plugin-first real Niagara Sprite draw bridge and shared AVBOIT core
stage. It adds shared contracts, runtime modes, real-resource gate CVars, and a
minimal UE5.7 Niagara hook proposal, while explicitly blocking KeyResults
promotion until real VF/material/particle-attribute hash/SceneColor composite
and GPU evidence gates pass.

Do not mark `SUCCESS`, `COMPLETED`, or `passed-local` for UE-4.2E in the current
state.
