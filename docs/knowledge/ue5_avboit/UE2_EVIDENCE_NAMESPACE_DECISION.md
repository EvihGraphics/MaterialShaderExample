# UE2 Evidence Namespace Decision

UTC: 2026-06-22T15:24:26Z

## Decision

All new UE-2.1 closeout evidence must use a machine-scoped namespace:

```text
LocalVisualResults/UE57/<MachineId>/UE2-Closeout/
```

For this machine:

```text
LocalVisualResults/UE57/HIVE-4090X2/UE2-Closeout/
```

## Required Layout

```text
Determinism/
Editor/
PIE/
Standalone/
Resize/
GPUCapture/
Logs/
```

## Rationale

Historical evidence already exists under:

```text
LocalVisualResults/UE57/Baseline
LocalVisualResults/UE57/UE2
```

Those paths belong to prior machine and prior closeout evidence. They remain
valid history but are not current-machine proof. New validation must bind every
result to machine ID, commit, UE Build.version, UTC time, and evidence path.

## Rules

- Do not move, rename, overwrite, or delete historical evidence.
- Do not use old screenshots to claim current commit validation.
- Store current-machine local UE root only in ignored local JSON and
  checkpoints, not in this shared namespace decision.
