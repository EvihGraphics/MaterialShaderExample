# UE57 Project Contract

UTC: 2026-06-22T16:34:17Z

## Contract

| Item | Value |
|---|---|
| Required UE Version | 5.7 |
| EngineAssociation | 5.7 |
| Build Target | MaterialShaderDemoEditor |
| Platform | Win64 |
| Configuration | Development |
| Portable Target | MaterialShaderDemoEditor Win64 Development |

## Current Source Compatibility

Status: `PARTIAL`

The repository source is compatible enough to build and run the UE-2 RDG smoke
test on `HIVE-4090X2` at commit
`59d8c84da73b6d585629978a5e7e85c7d0845a8c`. Build, determinism, Editor, PIE,
Standalone, Resize, lifecycle, original regression, and accepted log-scan gates
have current-machine evidence. External GPU capture is still blocked, so the
portable project status remains `PARTIAL`.

## Latest Portable Verification Status

| Item | Value |
|---|---|
| Latest Commit Verified | `59d8c84da73b6d585629978a5e7e85c7d0845a8c` |
| Latest Portable Verification Status | `partial-gpu-capture-blocked` |
| Evidence Namespace | `LocalVisualResults/UE57/<MachineId>/UE2-Closeout` |

## Known UE API Risks

- SceneViewExtension post-process delegate lifetime must remain valid across
  Editor shutdown and repeated PIE sessions.
- RDG resources created for the smoke test must remain graph-local and must not
  become persistent cross-frame state.
- ViewRect handling must not assume `ViewRect.Min == (0, 0)`.
- DebugMode 2 must continue to use the real scene depth input and UE reverse-Z
  conversion.

## Path Rule

This project contract intentionally does not prescribe or record a machine UE
install path. Machine-local UE roots belong in ignored local state and in
machine-scoped checkpoints only.
