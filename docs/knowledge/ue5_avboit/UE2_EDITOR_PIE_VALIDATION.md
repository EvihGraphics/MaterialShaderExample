# UE2 Editor And PIE Validation

UTC: 2026-06-22T16:34:17Z

## Result

Editor and PIE closeout validation passed on `HIVE-4090X2` at commit
`59d8c84da73b6d585629978a5e7e85c7d0845a8c` with UE 5.7.4.

The validation helper uses editor APIs to start and end PIE twice, applies
enable/disable and mode changes across frames, writes viewport readback PNGs,
and logs lifecycle markers.

## Evidence

Evidence root:

```text
LocalVisualResults/UE57/HIVE-4090X2/UE2-Closeout
```

Accepted lifecycle log:

```text
Logs/EditorPIE_Closeout_Sync.log
```

Editor screenshots:

```text
Editor/00_Disabled.png
Editor/01_Mode1_Deterministic.png
Editor/02_Mode2_SceneDepth.png
Editor/03_Mode3_SceneColorOverlay.png
Editor/10_OriginalMaterialShaderRegression.png
```

PIE screenshots:

```text
PIE/00_Disabled.png
PIE/01_Mode1.png
PIE/02_Mode2_SceneDepth.png
PIE/03_Mode3_SceneColorOverlay.png
PIE/04_Lifecycle_FinalEnable.png
```

Lifecycle markers observed:

```text
Closeout observed BeginPIE. Simulating=false
Closeout observed EndPIE. Simulating=false
Closeout observed BeginPIE. Simulating=false
Closeout observed EndPIE. Simulating=false
Finished editor/PIE closeout sequence.
```

## Notes

Earlier asynchronous `FScreenshotRequest` and OS window screenshot attempts are
superseded and are not accepted PASS evidence. The accepted run uses synchronous
viewport pixel readback from the active editor or PIE viewport.

