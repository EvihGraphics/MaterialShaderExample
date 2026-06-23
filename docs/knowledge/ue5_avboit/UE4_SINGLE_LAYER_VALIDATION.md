# UE4 AVBOIT Single Layer Validation
Validates the execution of a single full screen transparent layer injected directly via CPU arrays.

## Inputs
* **Color**: `(1.0, 1.0, 1.0)`
* **Alpha**: `0.5`
* **Physical Depth**: `100.0` (Mapped to an explicit slice)

## Expectations
* **Occupied Slices**: Exactly 1 slice.
* **Extinction Sum**: Fixed-point representation of `-log(1.0 - 0.5)`.
* **Transmittance**: `exp(-extinction)` = `0.5`.
* **Output Color**: `Color * Alpha * InitialTransmittance` = `1.0 * 0.5 * 1.0` = `0.5`.
* **Output Color Final**: `(0.5, 0.5, 0.5)`.
