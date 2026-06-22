# UE2 Smoke Determinism Decision

UTC: 2026-06-22T16:34:17Z

## Decision

`AVBOITSmoke.usf` DebugMode 1 is deterministic. It no longer reads
`View.StateFrameIndexMod8` or any time, jitter, random, temporal counter, or
frame-index state.

DebugMode 1 now draws a static UV gradient, checkerboard, and ViewRect edge
marker. The deterministic closeout capture uses opacity `1.0` so the image does
not blend through frame-varying scene color.

## Evidence

| Item | Value |
|---|---|
| Machine | `HIVE-4090X2` |
| Commit | `59d8c84da73b6d585629978a5e7e85c7d0845a8c` |
| UE Version | 5.7.4 |
| Evidence Root | `LocalVisualResults/UE57/HIVE-4090X2/UE2-Closeout` |
| Image A | `Determinism/Mode1_A.png` |
| Image B | `Determinism/Mode1_B.png` |
| Metrics | `Determinism/Mode1_Determinism_Metrics.txt` |

Metrics:

```text
SHA256_A   : 74AB5ADE5159AABD0E73910698F2BD204BD2F659451DA399FED83220F17320E1
SHA256_B   : 74AB5ADE5159AABD0E73910698F2BD204BD2F659451DA399FED83220F17320E1
MAE        : 0
MaxAbs     : 0
DiffPixels : 0
```

## Result

Determinism gate: `PASS`

