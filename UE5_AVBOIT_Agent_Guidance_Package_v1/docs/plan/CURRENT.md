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
docs/checkpoints/archive/CHECKPOINT-0011-20260622T163417Z-UE2-closeout-result.md
```

Current mirrored status:

```text
partial
```

UE-2.1 build, determinism, Editor, PIE, Standalone, Resize, lifecycle,
original-regression, and accepted log-scan evidence is present under:

```text
LocalVisualResults/UE57/HIVE-4090X2/UE2-Closeout
```

External GPU capture remains blocked. UE-2 must not be marked `PASSED` until
RenderDoc or PIX evidence proves `AVBOIT.Smoke.Clear`,
`AVBOIT.Smoke.Fill`, and `AVBOIT.Smoke.Composite`.

Machine-local UE paths are not package-level truth. Every machine must
rediscover its UE root and record `Build.version` before producing build/run
evidence.
