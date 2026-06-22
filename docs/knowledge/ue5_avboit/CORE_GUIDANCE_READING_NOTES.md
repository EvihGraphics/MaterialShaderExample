# Core Guidance Reading Notes

Generated UTC: 2026-06-22T08:19:40Z

## Sources Read Or Rechecked

- `UE5_AVBOIT_Agent_Guidance_Package_v1\prompt\AGENT_PROMPT_UE5_AVBOIT_BOOTSTRAP.md`
- `UE5_AVBOIT_Agent_Guidance_Package_v1\prompt\AGENT_PROMPT_FULL.md`
- `UE5_AVBOIT_Agent_Guidance_Package_v1\references\pdf_md\INDEX.md`
- `UE5_AVBOIT_Agent_Guidance_Package_v1\references\pdf_md\AVBOIT_SIG2025_MDROBOT-final.md`
- `UE5_AVBOIT_Agent_Guidance_Package_v1\references\pdf_md\在UE5外部模块中扩展复杂渲染管线实践.md`
- `UE5_AVBOIT_Agent_Guidance_Package_v1\docs\knowledge\validation\ue5.7_nanite_api_migration.md`
- `UE5_AVBOIT_Agent_Guidance_Package_v1\docs\skill\checkpoint-archive-skill\SKILL.md`
- `UE5_AVBOIT_Agent_Guidance_Package_v1\docs\skill\materialshaderexample-baseline-skill\SKILL.md`
- `UE5_AVBOIT_Agent_Guidance_Package_v1\docs\skill\ue5-avboit-learning-development-skill-v1\SKILL.md`

## Engineering Constraints Extracted

- `MaterialShaderExample` is the UE baseline. AVBOIT development must not delete or overwrite the existing Nanite material-shader sample.
- The usable UE skeleton is: module shader directory mapping, actor parameters, engine subsystem, game-thread to render-thread copy, `SceneViewExtension`, RDG pass insertion, `FMaterialShader`, and GPU markers.
- The existing Nanite visibility-buffer replacement is baseline behavior, not the mandatory AVBOIT implementation strategy.
- UE work must be checkpointed before and after each instruction cycle. `docs/plan/CURRENT.md` is the mutable resume pointer; archived checkpoints are immutable.
- `SingleVolume.zip` remains forbidden. The local extracted plugin tree can be inspected read-only only as secondary engineering reference.
- The AVBOIT algorithm contract must be frozen before shader migration: clear, build/splat extinction, integrate transmittance, shade/forward transparent color, and resolve/composite.
- Build/run claims require real UE 5.7 tools. No `UnrealEditor.exe` or `UnrealBuildTool.exe` was found by lightweight discovery, so build/run evidence is blocked until the engine path is supplied.

## Current Conclusion

Status: `partial`

The bootstrap reading supports worktree creation, source inventory, and contract documentation. It does not support claiming UE build/run readiness because the UE 5.7 installation path is unresolved.

