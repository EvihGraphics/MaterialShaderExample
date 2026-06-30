# CHECKPOINT-0043 UE-4.2C Niagara Sprite Unlit Correction

UTC: 2026-06-30T06:52:15Z
Status: partial
Stage: UE-4.2C Niagara Sprite Unlit Parity

## Summary

Corrected the UE-4.2C Niagara sprite evidence pipeline after visual review found
that the prior evidence did not prove UE Engine Unlit mode. The corrected run
forces `UGameViewportClient::SetViewMode(VMI_Unlit)`, records the effective
`EngineShowFlagOverride(ESFIM_Game, VerifiedViewMode)` flags, and blocks local
acceptance if `VerifiedViewMode=Unlit` cannot be proven.

The previous evidence root `20260630T050908Z` is superseded for visual
acceptance. It remains `partial` implementation history only.

## Implemented

- Added runtime Unlit view-mode contract evidence:
  - `RequestedViewMode=Unlit`
  - `VerifiedViewMode=Unlit`
  - `ViewModeIndex=2`
  - `ShowFlagLighting=false`
  - `ShowFlagLightFunctions=false`
  - `ShowFlagDynamicShadows=false`
  - `ShowFlagAtmosphere=false`
  - `ShowFlagFog=false`
- Added `CaptureManifest.json` with per-screenshot mode, desired age, filename,
  and Unlit verification fields.
- Added KeyResults promotion for the final `4.0s` capture set and
  `LATEST.txt`.
- Added runtime switch commands:
  - `AVBOIT.Niagara.Mode EngineDefault`
  - `AVBOIT.Niagara.Mode AVBOITUnlit`
  - `AVBOIT.Niagara.ToggleDefaultPlugin`
  - `AVBOIT.Niagara.Status`
- Added an interactive startup command path through
  `AVBOIT.Niagara.Interactive`.

## Evidence

Temp evidence root:

```text
LocalVisualResults/TempResults/UE57/HIVE_4090x2/UE4-2C-NiagaraSprite-UnlitParity/20260630T064800Z
```

KeyResults root:

```text
LocalVisualResults/KeyResults/UE57/HIVE_4090x2/UE4-2C-NiagaraSprite-UnlitParity/20260630T064800Z
```

KeyResults selected images:

```text
13_EngineDefault_4000ms.png
14_PluginBypass_4000ms.png
15_AVBOITNiagaraUnlit_4000ms.png
16_DebugBuffers_4000ms.png
```

Observed evidence:

- `RunSummary.json`: `UnrealExitCode=0`, `HardFailPatterns=[]`,
  `ViewModeVerifiedUnlit=true`, `ScreenshotCount=16`,
  `KeyResultsPromoted=true`, `Status=partial`.
- `ViewModeContractRuntime.json`: `VerifiedViewMode=Unlit`,
  `VerifiedViewModeIsUnlit=true`, lighting/fog/atmosphere/dynamic shadows off.
- `RendererBindingManifest.json`: 18 AVBOIT sprite draws, 1530 particles, all
  required RDG pass names recorded.
- `ImageMetrics.json`: Bypass vs Engine Default final image differs
  (`MAE=0.0029282161552892762`, `MaxError=0.34509803921568627`), so the phase
  remains `partial`.

Interactive switch self-test:

```text
LocalVisualResults/TempResults/UE57/HIVE_4090x2/UE4-2C-NiagaraSprite-UnlitParity/InteractiveToggleSelfTest-20260630T064800Z
```

`StatusBefore.json` reported `Mode=EngineDefault`; `StatusAfter.json` reported
`Mode=AVBOITNiagaraUnlit`. Both kept `VerifiedViewMode=Unlit` and
`VerifiedViewModeIsUnlit=true`.

## Validation Commands

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File Scripts\Run-AVBOITNiagaraSpriteUnlitParity.ps1 -ContentExamplesProject "D:\Users\l3d\Documents\Unreal Projects\ContentExamples\ContentExamples.uproject" -TimeoutSeconds 1200
```

Result: ContentExamplesEditor build succeeded, Unreal ran in `-game`, generated
16 screenshots, promoted final-key results, and exited with code 0.

## Current Limits

This checkpoint remains `partial`. Do not mark `SUCCESS`, `COMPLETED`, or
`passed-local` because bypass pixel identity, material equivalence, and
same-particle-state proof still need stronger closure.
