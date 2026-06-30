# UERP_Ref to UE-4.2E Source Mapping

Generated UTC: 2026-06-30T19:02:06Z

## Useful Reference Concepts

- Keep transparent rendering state explicit and mode-driven.
- Separate primitive-provider data from shared transparency core resources.
- Promote results only after objective buffer/readback/image gates.
- Preserve original material/particle alpha when applying debug tint.

## UE Integration Differences

- UE Niagara Sprite rendering is not a simple external mesh provider.
- The real sprite vertex factory and `FMeshBatch` are built inside UE Niagara private implementation.
- UE RDG resources are frame-local; cross-frame readback and promotion evidence must be copied into explicit JSON/image artifacts.
- Engine Unlit view mode and fixed runtime camera are part of the scene contract, not optional debug state.

## UE-4.2E Mapping

| UERP_Ref Concept | UE-4.2E Owner | Current State |
|---|---|---|
| Shared OIT resources | `MaterialShaderPlugin` core contracts | Added as public contract types |
| Primitive provider | Niagara adapter | Metadata-only, real draw blocked |
| Identity path | Shared core + adapter packet | Blocked by missing sprite draw packet hook |
| Fixed-slice AVBOIT | Shared core frame graph | Contracted; scratch foundation only |
| Buffer visualizer | Shared core readback/overview | Contracted; real readback pending |
| Tint | Identity/AVBOIT shader | Contracted; MID/fallback tint not acceptance |

## Blocking Rule

If the adapter cannot prove real VF, real material, particle attribute hash, and SceneColor composite, UE-4.2E status must remain `blocked-local` or `partial`.
