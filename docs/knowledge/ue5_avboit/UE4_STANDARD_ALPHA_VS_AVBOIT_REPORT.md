# Standard Alpha vs. AVBOIT Comparison Report

## 1. Overview
The fundamental value of AVBOIT is physical physical order independence. The UE-3.1 tests proved that the engine's built-in `TranslucencySortPriority` forces Standard Alpha to behave differently based purely on CPU submission sort order. This report formally compares Standard Alpha (order-dependent) with AVBOIT (order-independent) under synthetic testing.

## 2. Theoretical Validation Values
Given Front (Red, Alpha=0.5), Middle (Green, Alpha=0.5), Back (Blue, Alpha=0.5).

**Correct Physical Result**: `(0.5, 0.25, 0.125)`

| Submission Order | Standard Alpha Output | Standard Alpha Status | AVBOIT Output | AVBOIT Status |
|---|---|---|---|---|
| **RGB** | `(0.125, 0.25, 0.5)` | Wrong Physical Depth | `(0.5, 0.25, 0.125)` | Correct Physical Depth |
| **RBG** | `(0.125, 0.5, 0.25)` | Wrong Physical Depth | `(0.5, 0.25, 0.125)` | Correct Physical Depth |
| **GRB** | `(0.25, 0.125, 0.5)` | Wrong Physical Depth | `(0.5, 0.25, 0.125)` | Correct Physical Depth |
| **GBR** | `(0.5, 0.125, 0.25)` | Wrong Physical Depth | `(0.5, 0.25, 0.125)` | Correct Physical Depth |
| **BRG** | `(0.25, 0.5, 0.125)` | Wrong Physical Depth | `(0.5, 0.25, 0.125)` | Correct Physical Depth |
| **BGR** | `(0.5, 0.25, 0.125)` | **Correct** Physical Depth | `(0.5, 0.25, 0.125)` | Correct Physical Depth |

## 3. Conclusion
AVBOIT resolves the order-dependent nature of forward translucency. Regardless of how the rasterizer or CPU submits the primitives, AVBOIT builds the Extinction Volume atomically and integrates it according to physically correct log-depth slices, ensuring the composite correctly shades Front-to-Back.
