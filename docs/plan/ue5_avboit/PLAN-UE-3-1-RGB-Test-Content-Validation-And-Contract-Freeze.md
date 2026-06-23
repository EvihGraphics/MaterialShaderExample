# PLAN-UE-3.1: RGB Test Content Validation and Contract Freeze

## Objective
Repair the flawed UE-3 implementation to establish a truly deterministic, dedicated SceneCapture-based validation pipeline. This ensures the 6 submission orders of TripleRGB correctly map to measurable standard-alpha visual outputs, freezing the Test Content contract for the upcoming UE-4 phase.

## 1. Audit and Reject Old Evidence
*   Compare existing UE-3 screenshots via Git Blob and SHA-256 to prove they were incorrectly generated (identical across presets/orders due to Viewport readback limitations).
*   Formally reject the old evidence in `UE3_EXISTING_EVIDENCE_REJECTION_REPORT.md` and isolate them in their existing path without deletion.
*   Establish a new dedicated path: `LocalVisualResults/UE57/<MachineId>/UE3-1-RGB-Validation/`

## 2. Implement Fixed SceneCapture Component
*   Discard `GEditor->GetActiveViewport()` or `GEngine->GameViewport` for numeric evidence.
*   Setup a dedicated `SceneCaptureComponent2D` in the Test Subsystem.
*   **Camera transform:** Position (0,0,0), Rotation Forward +X.
*   **Projection:** Orthographic, Width 512, Aspect 1:1, Output 512x512.
*   **Background:** Scene Capture Clear Color = Black (no extra opaque or translucent backdrop planes).

## 3. Test Material Validation
*   Validate `M_AVBOITTest_Unlit` is set to Translucent, Unlit, with Depth Test fixed, and `TestColor` / `TestAlpha` parameters present.

## 4. Resolve Submission Orders and Draw Ordering
*   Stop relying on the Actor Tick. Fix state synchronization by awaiting a render frame after setting parameters.
*   Define `FAVBOITSubmissionEntry` to strongly type StableId, SequencePosition, and SortPriority.
*   Ensure physical depth remains absolutely fixed while only varying `TranslucencySortPriority`.
*   Validate the UE 5.7 Sort Priority direction (Higher or Lower drawn first).

## 5. Formal Expected Values & Manifest Generation
*   Compute physical CPU-based expected linear Source-Over values for the correct sequence and the 5 other combinations.
*   Export an independent `Manifest.json` for each capture (e.g. `00_RGB.Manifest.json`) containing Camera, Material, Primitives, and Output info.
*   Generate `SubmissionOrders.csv` containing the data points.

## 6. Image Readback and Analytics
*   Readback the `SceneCaptureComponent2D` RenderTarget as Linear Data (RGBA16F or RGBA32F).
*   Extract a 64x64 center ROI.
*   Compute Mean Linear RGB, Max Deviation, and Mean Absolute Error against ExpectedValues. Tolerances must be met (MAE <= 0.002).

## 7. Determinism and Regressions
*   Execute the pipeline twice and compare SHA-256 for determinism.
*   Ensure Editor, PIE, and Standalone `-game` modes pass without crashing or bleeding resources.
*   Verify UE-2 Original Smoke pass is intact.
*   Capture GPU Draw Order (via RenderDoc) to prove correct sorting.

## 8. Exit Gate
*   All tests must pass and exact measured metrics must be reported.
*   Upon success, UE-3.1 transitions to `PASSED` and allows progression to UE-4: Plugin-Owned AVBOIT Backend Vertical Slice.
