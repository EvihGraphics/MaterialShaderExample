# UE57 Project Contract

UTC: 2026-06-23T03:44:13Z

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

Status: `PASSED`

The repository source is compatible enough to build and run the UE-2 RDG smoke
test on `HIVE-4090X2` at commit
`90f9514c7744f0da343e480acc91e3a235c52220`. Build, determinism, Editor, PIE,
Standalone, Resize, lifecycle, original regression, accepted log-scan, and
external GPU capture gates have current-machine evidence. The portable
project status is now `PASSED`.

### Pointer

| Item | Value |
|---|---|
| Latest Commit Verified | `90f9514c7744f0da343e480acc91e3a235c52220` |
| Latest Portable Verification Status | `passed` |
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
