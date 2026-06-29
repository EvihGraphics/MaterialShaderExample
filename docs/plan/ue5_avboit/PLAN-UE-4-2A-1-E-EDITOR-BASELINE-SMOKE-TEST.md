# PLAN-UE-4-2A-1-E: Editor Baseline Smoke Test

## Overview
Following the successful validation of the AVBOIT mathematical accumulators and RDG geometry passes in a purely headless environment (UE-4.2A.1-H.1-C), this stage moves the validated RDG node implementation (`FAVBOITRasterRenderer`) into the active UE 5.7 Editor application context.

## Objectives
- Integrate the tested `FAVBOITRasterRenderer` into the active `AVBOITSceneViewExtension`.
- Wire `r.AVBOIT.Raster.Enable` to replace the legacy `r.AVBOIT.Smoke.Enable` for UI integration tests.
- Re-purpose the `AVBOITEditorPIEAutomation` tests to automatically launch the Editor, transition to PIE, and verify that the active `AVBOITRasterRenderer` correctly processes scene view depth and color resources without asserting or leaking.
- Create or update the `Run-AVBOITEditorPIEValidation.ps1` PowerShell harness to orchestrate the integration test.

## Exit Gates
- [ ] `AVBOITSceneViewExtension` gracefully delegates to `FAVBOITRasterRenderer::AddPasses` without disrupting standard PostProcessing passes when enabled.
- [ ] Editor PIE runs successfully without crashes or render hardware interface (RHI) validation errors.
- [ ] `AVBOIT.RunEditorPIECloseout` captures visual evidence of the Editor and PIE with `AVBOITRasterRenderer` active.
- [ ] Exit Code 0 achieved on automation script.
