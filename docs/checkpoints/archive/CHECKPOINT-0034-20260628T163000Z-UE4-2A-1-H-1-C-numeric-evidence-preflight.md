# CHECKPOINT-0034-20260628T163000Z-UE4-2A-1-H-1-C-numeric-evidence-preflight

## Execution Summary
- **Starting Remote HEAD:** 406af5f95a1c633d75ea7037e97d09f143e99875
- **Starting Implementation Baseline:** bdea79a618d79a418ed038b1525742eedc929582
- **Ending Implementation Commit:** To be recorded (first commit of this phase)
- **Ending Remote HEAD:** To be recorded (after push)

## Notes
This preflight checkpoint supersedes the `COMPLETED` claim from Checkpoint 0033. Checkpoint 0033 completion claim superseded by UE-4.2A.1-H.1-C verification. The previous state was premature due to the following deviations:
* Synthetic JSON was fixed 12/12.
* Six Raster JSON files were identical, generated from a single RDG execution instead of individually per case.
* Splat pass hardcoded Slice 32 instead of correctly calculating it.
* Splat pass dummy color output polluted `ColorAccumulation`.
* GPU Log contained anomalous accumulation and Extinction (`Extinction[32] = 13861`) due to incorrect alpha assumptions and test layout.
* Registry / Resize were not authentically run in this headless RDG harness.

## Objective
The objective of this phase is to correct the erroneous `COMPLETED` state by transitioning to a robust, unforgeable, per-case, per-field RDG validation suite. The goal is to fully implement the multi-case raster harness, correctly execute GPU readback independently for each case, validate individual assertions for synthetic tests, and fix shader bugs in the Splat pass relating to slice calculation and render target pollution.

## Pending Tasks
- Implement `FAVBOITDirectRasterCase` and `FAVBOITDirectRasterCaseResult` to provide per-case isolation.
- Update `RunDirectRasterCase` to execute a separate RDG instantiation for each of the six test cases.
- Fix `AVBOITRasterSplat.usf` to use correctly projected slice index.
- Separate dummy color target logic so the Splat pass no longer pollutes `ColorAccumulation`.
- Implement rigorous per-case validation in the Headless Commandlet (VisibleWhiteAlphaHalf, OpaqueDepthOcclusion, NonBlackBackgroundComposite, and the LinearDepth Near/Middle/Far cases).
- Refactor synthetic backend to return an array of `FAVBOITSuiteResult` instead of requesting exit directly.
- Revamp `Run-AVBOITHeadlessValidation.ps1` to prevent false positive log scans, verify multiple unique JSON and CSV artifacts, and ensure total authenticity.
