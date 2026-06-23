# UE4 AVBOIT Multi-Layer Validation
Validates the execution of multiple layers of transparency and Order Independence.

## Double Red Blue
* **Front**: Red (1, 0, 0), Alpha 0.5
* **Back**: Blue (0, 0, 1), Alpha 0.5
* **Expected Result**: `(0.5, 0.0, 0.25)`

Two fragment arrays will be pushed: `[Red, Blue]` and `[Blue, Red]`. Both must yield the exact same linear color.

## Triple RGB
* **Front**: Red (1, 0, 0), Alpha 0.5
* **Middle**: Green (0, 1, 0), Alpha 0.5
* **Back**: Blue (0, 0, 1), Alpha 0.5
* **Expected Result**: `(0.5, 0.25, 0.125)`

Six fragment arrays (RGB, RBG, GRB, GBR, BRG, BGR) will be pushed. All six permutations must natively and identically converge to `(0.5, 0.25, 0.125)` purely through GPU Atomic build, Integrate, and front-transmittance shaded Splat.
