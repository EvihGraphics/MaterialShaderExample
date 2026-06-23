# UE4 AVBOIT GPU Readback Contract

We require exact CPU-side inspection of the GPU integration logic to ensure it strictly respects The Forge's physical correctness without relying entirely on UE's visualization layer.

## Mechanism
* Implementation will utilize `FRHIGPUTextureReadback` mapping the RDG `Result` texture to CPU memory at the end of the test suite phase.
* Wait for `IsReady()` to guarantee the render thread has flushed.
* Read the `FFloat16Color` or `FFloat32Color` raw linear data.

## ROI Metrics
Similar to UE-3.1, a 64x64 Region Of Interest (ROI) centered at (256, 256) inside the 512x512 texture will be evaluated to avoid any boundary/edge effects or projection anomalies.
* **Mean RGB**: Average linear color in the ROI.
* **MAE (Mean Absolute Error)**: Sum of absolute differences from the `Expected` values per channel, divided by count.
* **MaxAbs (Maximum Absolute Error)**: The highest divergence in any channel within the ROI.

## File Manifest Generation
The automation suite will output `Metrics.json` and a final `LinearReadback.bin` or PNG per iteration, exactly matching the structure of the TripleRGB submission permutations to prove order independence.
