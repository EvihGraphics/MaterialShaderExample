# Checkpoint Index

> Append-only checkpoint archive. The latest recovery pointer is `docs/plan/CURRENT.md`.

| Seq | Checkpoint | UTC Time | Status | Plan / Stage | Summary |
|---:|---|---|---|---|---|
| 0006 | [`CHECKPOINT-0006-20260622T081940Z-UE-bootstrap-worktree.md`](archive/CHECKPOINT-0006-20260622T081940Z-UE-bootstrap-worktree.md) | 2026-06-22T08:19:40Z | partial | UE bootstrap / Checkpoint A | Created `AVBOIT开发` worktree and recorded baseline/source evidence. |
| 0007 | [`CHECKPOINT-0007-20260622T081940Z-UE-bootstrap-result.md`](archive/CHECKPOINT-0007-20260622T081940Z-UE-bootstrap-result.md) | 2026-06-22T08:19:40Z | partial | UE bootstrap / Checkpoint B | Documentation-only bootstrap completed; UE build/run blocked by missing UE 5.7 path. |
| 0008 | [`CHECKPOINT-0008-20260622T125902Z-UE1-2-preflight.md`](archive/CHECKPOINT-0008-20260622T125902Z-UE1-2-preflight.md) | 2026-06-22T12:59:02Z | partial | UE-1/2 preflight | Separated portable repo state from machine-local UE state and verified this machine has UE 5.7.4. |
| 0009 | [`CHECKPOINT-0009-20260622T134419Z-UE1-2-rdg-smoke-result.md`](archive/CHECKPOINT-0009-20260622T134419Z-UE1-2-rdg-smoke-result.md) | 2026-06-22T13:44:19Z | implemented-with-test-gaps | UE-1/2 result | Built UE-1 baseline, added isolated UE-2 RDG smoke test, captured standalone screenshots/logs, and left Editor/PIE plus external GPU capture as gaps. |
