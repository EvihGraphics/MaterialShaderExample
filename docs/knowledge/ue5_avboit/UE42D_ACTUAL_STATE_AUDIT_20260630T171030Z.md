# UE-4.2D Actual State Audit

Generated UTC: 20260630T171030Z

## Git And Workspace

- Repo: `D:\Users\l3d\Documents\AVBOIT\MaterialShaderExample_AVBOIT`
- Branch: `AVBOIT开发`
- Local HEAD: `e04b1f3abdfaadd6a00ec3e38ad61f8762e6b080`
- Remote HEAD: `e04b1f3abdfaadd6a00ec3e38ad61f8762e6b080`
- Existing dirty item before implementation: `?? LocalVisualResults/UE57/`
- Resolution: preserve and ignore the untracked local visual directory; do not delete or stage it.

## CURRENT State

- Root `docs/plan/CURRENT.md` did not exist before UE-4.2D work.
- Guidance mirror existed at `UE5_AVBOIT_Agent_Guidance_Package_v1/docs/plan/CURRENT.md` and pointed at UE-4.2C partial work.
- UE-4.2D creates the root current file and makes the Guidance mirror point at it.

## Rendering State Before UE-4.2D Patch

- `FNiagaraRendererAVBOITSprites::GetDynamicMeshElements` captured Niagara sprite metadata and particle count.
- When tint was enabled, it still called `FNiagaraRendererSprites::GetDynamicMeshElements`; the visible tint path was therefore the default Niagara sprite renderer plus a transient MID.
- `AVBOITNiagaraDebug.usf` wrote fixed PassId marker colors into `AVBOIT.Niagara.EvidenceTexture`.
- `EvidenceTexture` was not composited into `SceneColor`.
- This meant previous evidence proved command scheduling and overlay/CVar state, but not true AVBOIT rendering.

## Code Reused

- Runtime scanner and transient renderer conversion.
- Fixed runtime camera and Engine Unlit view-mode contract.
- UE-4.2C capture harness, screenshot sequencing, log hard-fail scan, and partial acceptance policy.
- Existing main-plugin raster AVBOIT resource names and pass ordering as reference material.

## Code Replaced / Reframed

- PassId marker shader is reframed into a UE-4.2D foundation probe with named intermediate resources.
- Default Niagara draw fallback is no longer accepted as AVBOIT proof; status records fallback usage as a failure condition.
- Tint material override is retained only as cleanup/audit code. It is not the acceptance path.

## Blocking Conditions

- No public Niagara sprite material/VF packet hook has been proven from plugin-only code.
- SceneViewExtension `PrePostProcessPass_RenderThread` foundation currently allocates resources but does not write final `SceneColor`.
- GPU readback statistics are contract fields and remain unproven until real readback is wired.
- Therefore StageStatus remains `partial` and KeyResults promotion is blocked.
