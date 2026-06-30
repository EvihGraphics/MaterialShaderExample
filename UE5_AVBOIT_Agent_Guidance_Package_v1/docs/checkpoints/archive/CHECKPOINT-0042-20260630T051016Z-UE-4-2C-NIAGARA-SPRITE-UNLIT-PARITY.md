# CHECKPOINT-0042 UE-4.2C Niagara Sprite Unlit Parity

UTC: 2026-06-30T05:10:16Z
Status: partial
Stage: UE-4.2C Niagara Sprite Unlit Parity

Superseded: this checkpoint's visual evidence root `20260630T050908Z` did not
record or verify UE Engine `ViewMode=Unlit`. It remains useful as partial
implementation history, but it is not valid acceptance visual evidence. Use
CHECKPOINT-0043 and KeyResults `20260630T064800Z` for the corrected Unlit
capture contract.

## Summary

UE-4.2C now has an isolated Niagara integration plugin and an automated runtime capture path for Epic's original `TestSpriteMap1` in ContentExamples. The core `MaterialShaderExample` runtime module still has no hard Niagara dependency; Niagara-specific code lives in the new `MaterialShaderExampleNiagara` project plugin.

The automation enters `-game`, applies the user-provided PlayerCameraManager reference transform through a transient runtime camera, and records the runtime camera contract. It does not save or modify `TestSpriteMap1.umap` or dependent Epic assets.

## Implemented

- Added `Config/AVBOIT/TestSpriteMap1Camera.json` with the authoritative runtime transform:
  - Location `X=-2798.789173, Y=36.626050, Z=457.510006`
  - Rotation `Pitch=17.254463, Yaw=-8.130192, Roll=0`
  - Scale `1,1,1`
- Added `Config/AVBOIT/TestSpriteMap1SceneContract.json` for runtime mode, resolution, capture times, CVars, and hard gates.
- Added `Scripts/Deploy-AVBOITPluginToContentExamples.ps1` with `Copy`, `Junction`, `Remove`, and `Validate` modes.
- Added `Scripts/Run-AVBOITNiagaraSpriteUnlitParity.ps1` for deploy, build, `-game` launch, capture, log scan, and evidence summary.
- Added isolated plugin `Plugins/MaterialShaderPluginNiagara`.
- Implemented `UNiagaraAVBOITSpriteRendererProperties` and `FNiagaraRendererAVBOITSprites`.
- Implemented transient runtime renderer conversion from official Niagara sprite renderers to AVBOIT sprite renderers without saving source assets.
- Captured real Niagara sprite dynamic particle data and suppressed default draw in AVBOIT mode.
- Added RDG evidence pass scheduling for:
  - `AVBOIT.Niagara.Clear`
  - `AVBOIT.Niagara.SpriteSplat`
  - `AVBOIT.Niagara.Integrate`
  - `AVBOIT.Niagara.ForwardUnlit`
  - `AVBOIT.Niagara.Composite`
- Added feature scanner, renderer conversion manifest, runtime camera contract, renderer binding manifest, screenshots, and hard-fail log scan.

## Evidence

Latest evidence root:

```text
LocalVisualResults/TempResults/UE57/HIVE_4090x2/UE4-2C-NiagaraSprite-UnlitParity/20260630T050908Z
```

Key files:

```text
RunSummary.json
Acceptance.json
CameraContractRuntime.json
FeatureManifest.json
FeatureManifestAfterConversion.json
RendererConversionManifest.json
RendererBindingManifest.json
01_EngineDefault_0500ms.png ... 16_DebugBuffers_4000ms.png
UnrealEditor.log
```

Observed evidence:

- `RunSummary.json`: `UnrealExitCode=0`, no hard-fail patterns, `Status=partial`.
- `Acceptance.json`: `Status=partial`, `SuccessWithheld=true`.
- `CameraContractRuntime.json`: `RunMode=Game`, `EditorViewportUsed=false`, `FOV=90`, `AspectRatio=1.7777777777777777`, `NearPlane=10`, `ScreenPercentage=100`, and the requested camera transform.
- `FeatureManifest.json`: 9 Niagara components, 18 official sprite renderers before conversion.
- `FeatureManifestAfterConversion.json`: 18 AVBOIT sprite renderers after transient runtime conversion.
- `RendererBindingManifest.json`: 18 AVBOIT sprite draws, 1530 particles on the final captured frame, default draw suppressed, all five RDG evidence pass names recorded.
- 16 screenshots were generated for capture times `0.5`, `1.0`, `2.0`, and `4.0` seconds across Engine Default, Plugin Bypass, AVBOIT Niagara Unlit, and Debug Buffers.

## Validation Commands

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File Scripts\Test-UEPluginDescriptors.ps1
```

Result: descriptor check completed; engine had two pre-existing disabled damaged experimental plugin descriptors.

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File Scripts\Run-AVBOITNiagaraSpriteUnlitParity.ps1 -ContentExamplesProject "D:\Users\l3d\Documents\Unreal Projects\ContentExamples\ContentExamples.uproject" -TimeoutSeconds 900
```

Result: ContentExamplesEditor build succeeded, Unreal ran in `-game`, captured evidence, and exited with code 0.

## Current Limits

This checkpoint is intentionally `partial`. It must not be marked `SUCCESS`, `COMPLETED`, or `passed-local` because:

- Pixel metrics are not implemented yet.
- Material evaluation is not equivalent to the engine sprite material path yet.
- Same-particle-state proof is not yet sufficient.
- Bypass vs Engine Default is captured but not yet evaluated for pixel identity.
- Full-map thresholds, single-layer identity thresholds, and Missing/Unexpected Renderer Count gates are not yet enforced.

## Recovery Notes

Continue from this checkpoint by adding the image metric stage and a stronger same-state proof. The runtime conversion path is transient and should remain non-saving; do not commit changes to Epic `TestSpriteMap1.umap`.
