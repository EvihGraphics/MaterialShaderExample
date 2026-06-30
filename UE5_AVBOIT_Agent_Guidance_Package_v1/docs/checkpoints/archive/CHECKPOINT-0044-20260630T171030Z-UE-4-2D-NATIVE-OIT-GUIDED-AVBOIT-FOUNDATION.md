# CHECKPOINT-0044 - UE-4.2D Native OIT-Guided AVBOIT Foundation

UTC Time: 2026-06-30T17:10:30Z

Status: partial

## Scope

- Created root recovery pointer `docs/plan/CURRENT.md`.
- Mirrored the guidance package `CURRENT.md` back to the root pointer.
- Added UE-4.2D actual-state audit and source/algorithm reference maps.
- Expanded Niagara runtime modes to Engine Default, UE Sorted Pixels OIT, Plugin AVBOIT Unlit, and AVBOIT Buffer Overview.
- Added UE-4.2D CVar/status contracts and result-manifest placeholders.

## Implementation Notes

- Plugin-first policy is preserved; no UE5.7 engine source was modified.
- `LocalVisualResults/UE57/` was already untracked and remains untouched.
- Default `FNiagaraRendererSprites` fallback is no longer accepted as AVBOIT proof in plugin mode.
- The current Niagara scene extension allocates and schedules foundation RDG resources but does not write final SceneColor.
- Runtime status writes `PromotionEligible=false` and explicit known blocking APIs.

## Hard Gates Not Yet Passed

- Real Niagara sprite material/VF draw packet is not proven.
- AVBOIT Composite does not yet write final SceneColor.
- GPU readback proof for Extinction, Transmittance, Accumulation, and Composite is not implemented.
- Deterministic same-particle-state proof remains incomplete.

## Local Validation

- `ContentExamplesEditor Win64 Development` build completed with exit code 0.
- A short `UnrealEditor.exe -game` status smoke entered `BufferOverview`, wrote `Status.json`, verified Engine `Unlit`, and exited with code 0.
- Smoke hard-fail log scan found no fatal/assert/RDG/RHI/shader binding/GPU crash/Invalid socket handle/duplicate draw/no AVBOIT sprite draws matches.

## Recovery Pointer

Use:

```text
docs/plan/CURRENT.md
```

Do not use old UE-4.2C KeyResults as UE-4.2D acceptance evidence.
