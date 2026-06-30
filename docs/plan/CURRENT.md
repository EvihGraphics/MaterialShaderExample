# CURRENT - UE-4.2D Native OIT-Guided AVBOIT Foundation

Generated UTC: 20260630T171030Z

## Active Stage

- Stage: UE-4.2D Native OIT-Guided AVBOIT Foundation
- StageStatus: partial
- OverallProjectStatus: partial
- Branch: AVBOIT开发
- Local HEAD: e04b1f3abdfaadd6a00ec3e38ad61f8762e6b080
- Remote HEAD: e04b1f3abdfaadd6a00ec3e38ad61f8762e6b080
- Dirty-tree condition: untracked `LocalVisualResults/UE57/` is preserved and must not be staged as acceptance evidence.

## Current Implementation Boundary

- Plugin-first only. No UE5.7 engine source changes are allowed for this stage.
- Epic `TestSpriteMap1.umap` and dependent assets must remain unmodified.
- Existing UE-4.2C KeyResults remain partial and superseded for UE-4.2D acceptance.
- UE-4.2D KeyResults promotion is blocked until real Niagara AVBOIT draw packets, SceneColor composite, and GPU readback gates pass.

## Landed This Stage

- Runtime modes expanded to `EngineDefault`, `UESortedPixelsOIT`, `AVBOITNiagaraUnlit`, and `BufferOverview`.
- UE Sorted Pixels OIT mode is guarded by runtime detection of `r.OIT.SortedPixels`.
- Niagara AVBOIT status now records StageStatus, PromotionEligible, KnownBlockingAPIs, CVar state, particle hash, frame-graph resource state, default fallback use, and SceneColor composite status.
- Default Niagara sprite draw fallback is no longer accepted as AVBOIT proof in the plugin path.
- Niagara foundation RDG resources are named and scheduled: Extinction, Transmittance, ColorAccumulation, AlphaAccumulation, CompositeScratch, and BufferOverview.
- Runner supports UE-4.2D phase switches and writes `TempResultsManifest.json`, `GPUCaptureManifest.json`, and `PromotionDecision.json`; UE-4.2D promotion is withheld by policy.

## Local Validation

- Build: `ContentExamplesEditor Win64 Development` completed with exit code 0 on 2026-06-30T17:27Z.
- Status smoke: `UnrealEditor.exe -game` entered `BufferOverview`, wrote `Status.json`, verified Engine `Unlit`, and exited with code 0.
- Smoke evidence root: `LocalVisualResults/TempResults/UE57/HIVE_4090x2/UE4-2D-NativeOIT-Guided-AVBOIT/20260630T172741Z-status-smoke`
- Smoke log hard-fail scan: no fatal/assert/RDG/RHI/shader binding/GPU crash/Invalid socket handle/duplicate draw/no AVBOIT sprite draws matches.

## Known Blocking APIs / Gaps

- The plugin-first path has renderer/material metadata and particle counts, but not a public Niagara sprite material/VF draw packet hook sufficient to build true AVBOIT splats.
- `AVBOIT.Niagara.Composite` currently writes a scratch foundation resource, not final `SceneColor`.
- GPU readback counters for Extinction/Transmittance/Accumulation are contract fields but are not populated by real readback yet.
- Tint is kept as runtime CVar and overlay state, but acceptance requires consumption in the AVBOIT ForwardUnlit shader, not MID/default Niagara fallback.

## Validation Command

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File Scripts\Run-AVBOITNiagaraSpriteUnlitParity.ps1 `
  -ContentExamplesProject "D:\Users\l3d\Documents\Unreal Projects\ContentExamples\ContentExamples.uproject" `
  -RunNativeOITStudy -RunCoreBufferBringup -RunTestSpriteMap1 -CaptureBufferOverview
```

Expected current result: `blocked-local` or `partial`; not `SUCCESS`, `COMPLETED`, or `passed-local`.
