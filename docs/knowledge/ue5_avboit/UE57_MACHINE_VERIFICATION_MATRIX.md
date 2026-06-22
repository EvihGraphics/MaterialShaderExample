# UE57 Machine Verification Matrix

UTC: 2026-06-22T16:34:17Z

This matrix records portable verification status by machine. It must not contain
absolute UE install paths.

| Machine ID | Commit | UE Version | Build | Editor | PIE | Standalone | GPU Capture | Verified UTC |
|---|---|---|---|---|---|---|---|---|
| DESKTOP-14SQPSC | `59d8c84da73b6d585629978a5e7e85c7d0845a8c` | 5.7.4 | PASS | NOT COVERED | NOT COVERED | PASS | NOT COVERED | 2026-06-22T13:44:19Z |
| HIVE-4090X2 | `59d8c84da73b6d585629978a5e7e85c7d0845a8c` | 5.7.4 | PASS | PASS | PASS | PASS | BLOCKED | 2026-06-22T16:34:17Z |

## Notes

- `DESKTOP-14SQPSC` is historical evidence only and must not be used as current
  machine proof.
- `HIVE-4090X2` is the current closeout machine. Its row must be updated only
  from fresh evidence generated under
  `LocalVisualResults/UE57/HIVE-4090X2/UE2-Closeout`.
- `HIVE-4090X2` also has determinism, resize, lifecycle, original regression,
  and accepted log-scan evidence under the same root. GPU capture is blocked by
  external capture tooling, not by the RDG smoke implementation.
