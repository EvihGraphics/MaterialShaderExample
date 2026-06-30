# UERP_Ref Pipeline Study For UE-4.2D

Generated UTC: 20260630T171030Z

## Purpose

UERP_Ref is used as a local pipeline study reference for how UE render extensions, RDG resources, and validation evidence should be organized. UE-4.2D does not import UERP_Ref code into the plugin.

## Useful Patterns

- Keep validation output separated into TempResults and promoted KeyResults.
- Record exact runtime contracts rather than relying on editor viewport state.
- Treat renderer source maps and API blockers as first-class evidence.
- Block promotion when evidence is partial.

## Applied To UE-4.2D

- Runtime status writes `StageStatus`, `OverallProjectStatus`, and `PromotionEligible`.
- Runner writes `TempResultsManifest.json`, `GPUCaptureManifest.json`, and `PromotionDecision.json`.
- KeyResults promotion is withheld for UE-4.2D until all hard gates pass.
- Dirty local visual output is preserved but not treated as acceptance evidence.

## Remaining Work

- Add real GPU readback payloads for Extinction, Transmittance, Accumulation, and Composite.
- Capture RenderDoc/PIX manifests for real pass input/output proof.
- Replace foundation probe resources with real Niagara sprite splat inputs.
