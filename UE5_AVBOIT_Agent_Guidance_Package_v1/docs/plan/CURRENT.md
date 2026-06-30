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
docs/checkpoints/archive/CHECKPOINT-0043-20260630T065215Z-UE-4-2C-NIAGARA-SPRITE-UNLIT-CORRECTION.md
```

Current mirrored status:

```text
partial
```

UE-4.2C Niagara Sprite Unlit Parity now has corrected Engine Unlit evidence.
The old `20260630T050908Z` visual evidence is superseded because it did not
record or verify UE Engine `ViewMode=Unlit`.

The corrected runtime TestSpriteMap1 `-game` capture uses the fixed Player
camera, verifies effective Engine Unlit show flags, records per-capture Unlit
contracts, and produces full TempResults under:

```text
LocalVisualResults/TempResults/UE57/HIVE_4090x2/UE4-2C-NiagaraSprite-UnlitParity/20260630T064800Z
```

The final `4.0s` visual set and key JSON evidence are promoted under:

```text
LocalVisualResults/KeyResults/UE57/HIVE_4090x2/UE4-2C-NiagaraSprite-UnlitParity/20260630T064800Z
```

The current status remains `partial`. Do not mark `SUCCESS`, `COMPLETED`, or
`passed-local` until bypass pixel identity, material equivalence, and
same-particle-state proof are implemented and all UE-4.2C hard gates pass.

Interactive switch commands are available in game mode:

```text
AVBOIT.Niagara.Mode EngineDefault
AVBOIT.Niagara.Mode AVBOITUnlit
AVBOIT.Niagara.ToggleDefaultPlugin
AVBOIT.Niagara.Status
```

Machine-local UE paths are not package-level truth. Every machine must
rediscover its UE root and record `Build.version` before producing build/run
evidence.
