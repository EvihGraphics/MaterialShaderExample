# CHECKPOINT-0008: UE1/2 Preflight

UTC: 2026-06-22T12:59:02Z

Status: partial

Previous Checkpoint:
`docs/checkpoints/archive/CHECKPOINT-0007-20260622T081940Z-UE-bootstrap-result.md`

Supersedes stale state in:
`docs/plan/CURRENT.md`

## User Instruction

Implement the UE5.7 AVBOIT UE-1/UE-2 plan with repository state separated from
machine-local UE state.

## Repository State

| Item | Value |
|---|---|
| Repository root | `D:\HTC\avboit\AVBOIT开发` |
| Branch | `AVBOIT开发` |
| Local HEAD | `d0ec701dd2abc79170f5dcf99df5451f78f2fd55` |
| Remote HEAD | `origin/AVBOIT开发@d0ec701dd2abc79170f5dcf99df5451f78f2fd55` |
| Ahead / behind | `0 / 0` |
| Working tree before this checkpoint | clean |
| Active plan | `docs/plan/ue5_avboit/PLAN-UE-1-2-Baseline-And-RDG-Smoke-Test.md` |

Worktrees:

```text
D:/HTC/avboit/MaterialShaderExample  master       a80b000f589e9953fcb5efbfd3eccbf247d62c4a
D:/HTC/avboit/AVBOIT开发             AVBOIT开发   d0ec701dd2abc79170f5dcf99df5451f78f2fd55
```

## Reference Worktrees

| Worktree | Branch | HEAD | Working Tree |
|---|---|---|---|
| `D:\HTC\avboit\MaterialShaderExample` | `master` | `a80b000f589e9953fcb5efbfd3eccbf247d62c4a` | clean |
| `D:\HTC\avboit\The-Forge` | `baseline/theforge-1.58-windows-vs-dx12` | `39046051cef37bff0e52e497ca7df07ba9aebe65` | clean |

The Forge reference images:

| Path | Status | Resolution | SHA-256 |
|---|---|---:|---|
| `D:\HTC\avboit\The-Forge\LocalVisualResults\HIVE_4090x2\VisualResults\15_Transparency\Screenshots\UT_15_Transparency_DX12_Mode_0.png` | present | 1920x1080 | `EF55BD5173AD97B6354F0426C44E304D42231D243AAA7346275A5201F831075E` |
| `D:\HTC\avboit\The-Forge\LocalVisualResults\HIVE_4090x2\VisualResults\15_Transparency\Screenshots\UT_15_Transparency_DX12_Mode_5.png` | present | 1920x1080 | `F2B4F7E4A85D01B73DBE3358C7F97948DC8E6FD2F81647E1EBECE076AA3E897D` |

## Portable UE Requirement

- `MaterialShaderDemo.uproject` declares `EngineAssociation = 5.7`.
- Build target: `MaterialShaderDemoEditor Win64 Development`.
- Required local binaries are recorded in
  `docs/knowledge/ue5_avboit/UE57_TOOLCHAIN_DISCOVERY.md`.

## This Machine UE Discovery

| Item | Value |
|---|---|
| Machine | `DESKTOP-14SQPSC` |
| Local UE root | `E:\tools\Epic Games\UE_5.7` |
| UE version | 5.7.4 |
| Build branch | `++UE5+Release-5.7` |
| UnrealEditor.exe | present |
| Build.bat | present |
| UnrealBuildTool.dll | present |
| UnrealBuildTool.exe | present |
| GenerateProjectFiles.bat | not present |
| RenderDoc | present at `C:\Program Files\RenderDoc`, not on PATH |
| PIX | not found |

## Build/Run Evidence

Not started in this checkpoint. This checkpoint authorizes UE-1 build/run as
the next action. No C++/USF/assets were modified before this checkpoint.

Planned first runtime action:

```powershell
& "E:\tools\Epic Games\UE_5.7\Engine\Build\BatchFiles\Build.bat" `
  MaterialShaderDemoEditor Win64 Development `
  -Project="D:\HTC\avboit\AVBOIT开发\MaterialShaderDemo.uproject" `
  -WaitMutex
```

## Files Created Or Updated For Preflight

- `docs/plan/ue5_avboit/PLAN-UE-1-2-Baseline-And-RDG-Smoke-Test.md`
- `docs/knowledge/ue5_avboit/STATE_POINTER_DECISION.md`
- `docs/knowledge/ue5_avboit/UE57_TOOLCHAIN_DISCOVERY.md`
- `docs/checkpoints/archive/CHECKPOINT-0008-20260622T125902Z-UE1-2-preflight.md`
- `docs/checkpoints/CHECKPOINT_INDEX.md`
- `docs/plan/CURRENT.md`
- `UE5_AVBOIT_Agent_Guidance_Package_v1/docs/plan/CURRENT.md`

## Current Status

- Repository state: PASS
- Portable UE requirement: PASS
- This machine UE discovery: PASS
- UE-1 build/run: NOT STARTED
- UE-2 RDG smoke: NOT STARTED

## Resume

1. Rediscover current-machine `UE57_ROOT`; do not assume the absolute path is
   valid on another machine.
2. Read `docs/plan/CURRENT.md`.
3. Build `MaterialShaderDemoEditor Win64 Development`.
4. Archive UE-1 build/run evidence before starting UE-2 runtime code.

