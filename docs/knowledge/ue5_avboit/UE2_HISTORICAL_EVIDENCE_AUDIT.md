# UE2 Historical Evidence Audit

## Scope
This document audits the historical evidence changes between `59d8c84da73b6d585629978a5e7e85c7d0845a8c` (Checkpoint 0011 Base) and `5623a4270394cb3789ed52da01261339c51706fd` (Current HEAD).

## Deletions and Renames

The following files under `LocalVisualResults/UE57/Baseline` were effectively deleted or renamed to `HIVE-4090X2/UE2-Closeout/Logs` during the history:
- `Logs/Build-MaterialShaderDemoEditor-AfterScreenshotHelper.log` (Deleted)
- `Logs/Build-MaterialShaderDemoEditor-NoUBA-UTF8.log` (Deleted)
- `Logs/Build-MaterialShaderDemoEditor-NoUBA.log` (Deleted)
- `Logs/Build-MaterialShaderDemoEditor-SubstW-NoUBA.log` (Deleted)
- `Logs/Build-MaterialShaderDemoEditor.log` (Deleted)
- `Logs/MaterialShaderExample_EditorWindowCapture.log` (Deleted)
- `MaterialShaderExample_Baseline.png` (Deleted)
- `MaterialShaderExample_Baseline_Viewport.png` (Deleted)
- `Logs/MaterialShaderExample_Game_HighResShot_Comma.log` -> `Determinism_Mode1_A.log`
- `Logs/MaterialShaderExample_Game_HighResShot_Retry.log` -> `Resize_1280x720_00_Disabled.log`
- `Logs/MaterialShaderExample_Game_HighResShot.log` -> `Resize_1920x1080_02_Mode2.log`
- `Logs/MaterialShaderExample_Game_ScreenshotHelper.log` -> `Standalone_00_Disabled.log`

## Recovery Action

To preserve the evidence referenced by `CHECKPOINT-0011-20260622T163417Z-UE2-closeout-result.md`, the `LocalVisualResults/UE57/Baseline` directory from `59d8c84da73b6d585629978a5e7e85c7d0845a8c` was restored into a machine-scoped historical archive:

`LocalVisualResults/UE57/Historical/DESKTOP-14SQPSC/UE1-Baseline/59d8c84da73b6d585629978a5e7e85c7d0845a8c/`

This allows us to maintain current evidence in `HIVE-4090X2` while preserving the old pointers.
