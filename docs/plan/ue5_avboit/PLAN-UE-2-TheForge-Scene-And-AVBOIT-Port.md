# PLAN-UE-2: The Forge Scene And AVBOIT Port Bootstrap

## Summary

This plan transitions the repository from The Forge AVBOIT evidence into the UE `MaterialShaderExample` worktree without touching runtime code in the bootstrap cycle.

## Scope

- Maintain `MaterialShaderExample` baseline behavior.
- Use `AVBOIT开发` branch and worktree for all generated docs/checkpoints.
- Freeze source maps, guidance inventory, The Forge visual references, and UE insertion decision.
- Leave UE replica, AVBOIT resource smoke test, and shader migration `not-started` until UE 5.7 build/run gates pass.

## Implementation Notes

- Active worktree: `D:\HTC\avboit\AVBOIT开发`.
- The Forge reference: `D:\HTC\avboit\The-Forge`, branch `baseline/theforge-1.58-windows-vs-dx12`, HEAD `39046051cef37bff0e52e497ca7df07ba9aebe65`.
- UE build/run gate: blocked until `UE57_ROOT` or an explicit UE 5.7 path provides `UnrealEditor.exe` and `UnrealBuildTool.exe`.
- Ignored PDFs are external local evidence from the original checkout and are not committed.

## Acceptance

- Git worktree exists, clean, and based on `origin/master`.
- Root `docs/` contains current inventory, source maps, contract, checkpoint index, and CURRENT pointer.
- No C++/USF/assets are modified.
- Final status is `partial` unless UE 5.7 build/run evidence is added later.

