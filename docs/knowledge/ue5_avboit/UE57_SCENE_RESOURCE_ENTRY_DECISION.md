# UE57 Scene Resource Entry Decision

UTC: 2026-06-22T16:34:17Z

## Decision

The UE-2 smoke test enters through the existing scene view extension and RDG
post-process path. Scene resources remain graph-local for the smoke test.

## Rules

- Do not add persistent AVBOIT backend resources during UE-2.
- Do not modify engine source.
- DebugMode 2 must continue to use the real scene depth texture supplied by UE.
- ViewRect handling must remain explicit and must not assume a zero origin.

