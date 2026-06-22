# STATE POINTER DECISION

UTC: 2026-06-22T12:59:02Z

## Decision

`docs/plan/CURRENT.md` at the repository root is the only active recovery
pointer for the `AVBOIT开发` worktree.

The guidance package file
`UE5_AVBOIT_Agent_Guidance_Package_v1/docs/plan/CURRENT.md` is a mirror notice
only. It must not override the root UE worktree state, even when it contains
older The Forge status.

## Reason

The previous root `CURRENT.md` was stale: it recorded an older HEAD and claimed
UE 5.7 was missing. The current worktree is `AVBOIT开发` at
`d0ec701dd2abc79170f5dcf99df5451f78f2fd55`, and this machine has a verified
UE 5.7.4 install.

The package `CURRENT.md` also pointed at a historical The Forge branch and
checkpoint. That information is useful as reference history, but it is not an
active UE development state.

## Multi-Machine Rule

Repository state and machine-local UE state are separate:

- Repository state is portable: branch, HEAD, checkpoint, plan, source files.
- UE state is machine-local evidence: `UE57_ROOT`, `Build.version`, available
  binaries, GPU capture tools, and build/run results.

Every machine must rediscover and record its own UE 5.7 path before producing
runtime evidence.

