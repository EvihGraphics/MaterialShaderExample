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
docs/checkpoints/archive/CHECKPOINT-0044-20260630T171030Z-UE-4-2D-NATIVE-OIT-GUIDED-AVBOIT-FOUNDATION.md
```

Current mirrored status:

```text
partial
```

UE-4.2D is a Plugin-first Native OIT-Guided AVBOIT foundation stage. It records
the actual current state, blocks old fallback tint/default Niagara draw evidence
from being treated as AVBOIT success, and withholds KeyResults promotion until
real Niagara AVBOIT draw packets, SceneColor composite, and GPU readback gates
are proven.

Do not mark `SUCCESS`, `COMPLETED`, or `passed-local` for UE-4.2D in the current
state.
