# UE4.2A.1: CURRENT.md HEAD Semantics Decision

## Context
Previously, `CURRENT.md` used `Expected Remote HEAD` to specify the exact git commit SHA that should match the remote HEAD. However, because `CURRENT.md` itself is versioned in git, any update to `CURRENT.md` changes the HEAD, making it impossible to correctly record the final commit SHA inside the file without creating a self-referential paradox or causing superficial "status drift" errors when pointers are updated.

## Decision
We are changing the semantics of `CURRENT.md` fields to clearly separate the implementation code from the checkpoint pointer updates.

The new fields will be:
- **Current Milestone**: e.g., UE-4.2A
- **Current Status**: e.g., PARTIAL, PASSED
- **Implementation Baseline**: The commit SHA that contains the actual code implementation and the Result Checkpoint file creation.
- **Checkpoint Reference**: The path to the active or latest checkpoint document.

## Rationale
- **Implementation Baseline** specifies the exact state of the source code before purely administrative updates (like updating `CURRENT.md` itself) occur.
- Any subsequent commits that *only* update `CURRENT.md` or `CHECKPOINT_INDEX.md` will advance the actual branch HEAD, but the `Implementation Baseline` remains stable and verifiable against the code.
- This prevents the false "Status Drift" error that triggers unnecessary resets.
- The true active branch HEAD should always be obtained dynamically via `git rev-parse origin/AVBOIT开发`.
