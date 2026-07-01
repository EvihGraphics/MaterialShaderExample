# CURRENT - UE-4.2F ROI Red Tint Visual Gate

Generated UTC: 2026-07-01T05:26:57Z

## Active Stage

- Stage: UE-4.2F ROI Red Tint Visual Gate
- StageStatus: partial
- OverallProjectStatus: partial
- Branch: AVBOIT开发
- Dirty-tree condition: untracked `LocalVisualResults/UE57/` is preserved and must not be staged as acceptance evidence.

## Implementation Boundary

- Visual evidence must come from UE rendering output, specifically `UE.FScreenshotRequest` or another UE built-in screenshot/readback path.
- Desktop screenshots are forbidden for gate evidence.
- Epic `TestSpriteMap1.umap` and dependent assets must remain unmodified.
- The UE-4.2F visual gate can promote KeyResults only for the ROI red-tint target. It does not claim the UE-4.2E real Niagara VF/material/SceneColor bridge gates are complete.

## Landed This Stage

- Added `Config/AVBOIT/TestSpriteMap1TintROI.json` with the user-specified normalized ROI: `Left=0.062`, `Top=0.417`, `Right=0.918`, `Bottom=0.814`.
- Fixed `AVBOIT.Niagara.ShowOverlay` to attach to either `GameViewport` or the active Editor LevelViewport and to report `OverlayAttachTarget`, attach errors, mouse state, and input mode state through `AVBOIT.Niagara.Status`.
- Showing the overlay now requests a visible mouse cursor and applies clickable Game+UI input; hiding it restores runtime input state.
- Fixed runtime tint material application so `PluginIdentity`, `PluginAVBOIT`, and legacy `AVBOITNiagaraUnlit` apply the tint MID when tint is enabled and clear it for Engine Default/bypass/debug paths.
- Added a tint-visible fallback draw for the blocked plugin path so UE-4.2F can produce visible red-tint evidence while still marking the real Niagara draw bridge as blocked.
- Added `AVBOIT.Niagara.CaptureTintComparison root=<dir> times=<list> color=<r,g,b,a> roi=<json>`, which captures `EngineDefaultBefore`, `PluginAVBOIT_RedTint`, and `EngineDefaultAfter` via `FScreenshotRequest`.
- Extended `Scripts/Run-AVBOITNiagaraSpriteUnlitParity.ps1` with `-RunTintVisualGate`, `-TintColor`, and `-TintROI`.
- The runner now writes TempResults crops, side-by-side, difference, heatmap, coverage mask, annotated goal image, `TintVisualMetrics.json`, `TintROIContractRuntime.json`, and `PromotionDecision.json`.

## Gate Rules

- EngineDefault before/after ROI MAE must be `<= 1/255`.
- Plugin ROI changed pixels must be red-dominant with `RedCoverageRatio >= 0.98`.
- Mean red advantage over changed ROI pixels must be `>= 0.12`.
- The final time point must have enough changed ROI pixels to prove the visual target is not empty.
- Raw screenshots must have `ScreenshotSource=UE.FScreenshotRequest`, `DesktopScreenshotUsed=false`, and `OverlayHidden=true`.
- Any fatal/assert/RDG/RHI/shader binding/GPU crash/Invalid socket handle/duplicate draw/no AVBOIT sprite draws failure blocks promotion.

## Current Blocking APIs / Gaps

- Real Niagara sprite VF/material/mesh batch construction remains inside UE private `NiagaraRendererSprites.cpp`.
- The UE-4.2F tint-visible fallback is allowed only for the red-tint visual target. It is not accepted as a real PluginIdentity or PluginAVBOIT draw bridge.
- Overall project status remains partial until UE-4.2E real draw bridge gates are satisfied.

## Validation Command

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File Scripts\Run-AVBOITNiagaraSpriteUnlitParity.ps1 `
  -ContentExamplesProject "D:\Users\l3d\Documents\Unreal Projects\ContentExamples\ContentExamples.uproject" `
  -RunTintVisualGate -TintColor 1,0,0,1 -CaptureTimes 4.0
```

Expected current result: KeyResults promotion is allowed only if the final UE screenshot shows the configured ROI particles red in `PluginAVBOIT_RedTint` and unchanged grey in both Engine Default captures. Do not mark `SUCCESS`, `COMPLETED`, or `passed-local`.
