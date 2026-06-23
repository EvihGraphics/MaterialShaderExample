# PLAN-UE-3: Plugin-Owned Deterministic RGB Test Content & Standard Alpha Reference

## Objective
Establish a deterministic RGB test content pipeline owned entirely by the plugin, without any Engine source modification or forking. This pipeline validates transparency sorting, single/double/triple layer combinations, and physical vs submission orders, serving as the ground truth before migrating to the true AVBOIT Backend.

## 1. State Integrity Repair
- Fix `CURRENT.md` checkpoint references.
- Append missing checkpoints (0012, 0013) to `CHECKPOINT_INDEX.md`.

## 2. Test Content Data Contract
- Define `FAVBOITTestPrimitiveDesc` with StableId, Transform, LinearColor, Alpha, PhysicalDepthIndex, SubmissionOrderIndex, and TranslucencySortPriority.
- Define `EAVBOITTestPreset` (Disabled, SingleWhite, DoubleRedBlue, TripleRGB).
- Define `EAVBOITOrderPermutation` for RGB, RBG, GRB, GBR, BRG, BGR.
- Define `EAVBOITTestReferenceMode` (StandardAlpha enabled; FutureAVBOIT disabled).

## 3. Fixed Camera and Background
- Map: `/Game/AVBOIT/TestMaps/AVBOIT_RGB_UnitTest`
- Camera: Position (0, 0, 0), Orthographic projection (width 512 cm), Forward +X, 1:1 aspect ratio.
- Background: Black Unlit Plane at X=700 cm.
- Environment: Dynamic lights, post-processing, fog, TAA, motion blur etc. disabled.

## 4. Unlit Linear Material
- Translucent, Unlit, Two-Sided (optional if fixed direction).
- Parameters: `TestColor` (Linear Vector) and `TestAlpha` (Scalar).
- No noise, refraction, depth fade, time inputs.

## 5. Single White Layer (Preset A)
- 1 Primitive at X=300 cm. Color (1,1,1), Alpha 0.5.
- Expected Linear RGB: (0.5, 0.5, 0.5), Transmittance: 0.5.

## 6. Double Red / Blue Layers (Preset B)
- Front: Red (1,0,0, 0.5) at X=300 cm.
- Back: Blue (0,0,1, 0.5) at X=400 cm.
- Expected Linear RGB: (0.5, 0.0, 0.25), Transmittance: 0.25.

## 7. Triple RGB Layers (Preset C)
- Front: Red (1,0,0, 0.5) at X=300 cm.
- Middle: Green (0,1,0, 0.5) at X=400 cm.
- Back: Blue (0,0,1, 0.5) at X=500 cm.
- Expected Linear RGB: (0.5, 0.25, 0.125), Transmittance: 0.125.

## 8. Six Submission Orders
- Keep physical depth unchanged.
- Output 6 permutations by modifying `TranslucencySortPriority`.
- Save `SubmissionOrders.csv` with metadata.

## 9. Standard Alpha Reference
- Differentiate physical depth order and submission order.
- Render 'CorrectSort' image.

## 10. Deterministic Evidence Capture
- Capture logic for Editor, PIE, Standalone modes.
- Diff pixel count/SHA-256 for deterministic evaluation.
- LocalVisualResults namespace setup.

## 11. UE-2 Regression
- Re-verify `AVBOIT.Smoke.Clear/Fill/Composite` still works without crashes.
- Original MaterialShaderExample works.

## 12. UE-3 Exit Gate
- All implementations above complete.
- No engine forks.
- Fatal/Assertion/RDG errors = 0.
- Switch state to UE-4 only when fully PASSED.
