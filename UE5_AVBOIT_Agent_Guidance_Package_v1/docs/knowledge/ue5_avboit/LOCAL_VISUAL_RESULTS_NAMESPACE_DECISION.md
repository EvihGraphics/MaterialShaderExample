# LocalVisualResults Namespace and Version Control Decision

## Context
As the automated Editor, PIE, and Headless rendering test pipelines execute, they generate significant amounts of visual telemetry, including high-resolution rendering screenshots, intermediate GPU JSON dumps, and trace logs. 

Previously, all files were dumped into a unified `LocalVisualResults` directory, which risked large binary files accidentally polluting the Git history if left untracked or mismanaged.

## Decision
To better manage large artifact output and preserve a clean repository history, `LocalVisualResults` is formally divided into two distinct namespaces:

1. **`TempResults`**: 
   - **Purpose**: Acts as the default sink for *all* automation pipeline outputs, rendering dumps, logs, and RHI traces.
   - **Version Control**: Explicitly ignored in `.gitignore` (`LocalVisualResults/TempResults/`). These files remain completely local to the executing machine and are not tracked.
   
2. **`KeyResults`**: 
   - **Purpose**: Represents "关键视觉结果" (Key Visual Results). These are strictly curated artifacts (e.g., successful visual passing frames, `Acceptance.json` records, diff-passing frames) that formally prove pipeline success for a specific development milestone.
   - **Version Control**: Actively tracked by Git and uploaded to the remote repository as evidence of completion and pipeline stability.

## Implementation Standard
- Automation scripts (such as `Run-AVBOITEditorPIEValidation.ps1` and `Run-AVBOITHeadlessValidation.ps1`) MUST set their default `BaseEvidenceRoot` to target `TempResults`.
- Upon successful execution (`ExitCode -eq 0`), the scripts are responsible for automatically copying key artifacts (e.g., `Editor\01_Visible.png` and `15_Acceptance\Acceptance.json`) into the corresponding run folder within the `KeyResults` hierarchy.
- Global evidence paths resolved in C++ via `AVBOITEvidencePath.cpp` MUST point to `TempResults` by default to prevent runtime bloat from accidental capture dumps.
