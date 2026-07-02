# PLAN UE-4.2G AVBOIT Quad Foundation

## Summary

Build a deterministic Foundation raster gate in `MaterialShaderPlugin` for intersecting transparent quads. The accepted path is real raster splat, extinction integration, forward unlit accumulation, and SceneColor composite, compared against an exact per-pixel sorted reference.

## Implementation

- Use `FAVBOITFrameConfig` as the single source for near/far depth, downsample factor, slice count, view rect, texture extent, and volume extent.
- Use `FAVBOITPrimitivePacket`/sorted raster draw data so AB, BA, ABC, CBA, and random orders affect the actual draw call sequence.
- Run Foundation pass events as `AVBOIT.Foundation.Clear`, `Splat`, `Integrate`, `ForwardUnlit`, and `Composite`.
- Keep old `r.AVBOIT.Raster.Enable` as a deprecated alias and avoid duplicate old/new drawing.
- Keep `MaterialShaderPluginNiagara` compiling, but do not implement the real Niagara data bridge in this stage.

## Validation

- Build `MaterialShaderDemoEditor` and ContentExamples with the plugins deployed.
- Run `Scripts/Run-AVBOITFoundationQuadValidation.ps1`.
- Require Single Layer identity, two intersecting quads, CPU/GPU depth-slice parity, resource readback, AB/BA/random order independence, exact reference parity, Native OIT status reporting, lifecycle checks, and GPU capture.
- Promote KeyResults only when all hard gates pass; otherwise write TempResults and `PromotionDecision.json` with status `partial`, `blocked-local`, or `failed`.

## Current Result

- UE-4.2G.1 closeout added direct Foundation visual evidence, CPU exact reference generation, readback-backed plugin outputs, real log scanning, and strict marker semantics.
- Latest TempResults: `LocalVisualResults\TempResults\UE57\HIVE_4090x2\UE4-2G-AVBOIT-Quad-Foundation\20260701T145304Z`.
- Latest status is `partial`, not `passed-local`.
- Current blockers: incomplete screenshot matrix, failed AVBOIT-vs-Exact parity, failed AB/BA order-independence, missing Native OIT proof, missing lifecycle matrix, and missing RenderDoc/PIX capture.
- KeyResults promotion and UE-4.2H entry remain forbidden until all hard gates pass.
- UE-4.2G.2 supersedes the old analytic reference with GPU layer readback ground truth and finds the earliest AB/BA divergence at `Splat.Extinction`.
- Latest G.2 TempResults: `LocalVisualResults\TempResults\UE57\HIVE_4090x2\UE4-2G2-AVBOIT-Transparent-Sorting-Forensics\20260702T043159Z`.
- G.2 status remains `partial`; fix Splat/extinction order dependency before EngineDefault, Native OIT, BufferOverview, GPU capture, lifecycle, or UE-4.2H.
- UE-4.2G.3 repairs the manual viewport entry: `AVBOIT.Foundation.SpawnTransparentSortingScene order=AB mode=PluginAVBOIT` now creates transient `TwoIntersectingQuads` proxies and a camera instead of relying on CVar-only geometry creation.
- G.3 build verification is currently blocked at final link because an open UnrealEditor instance holds `UnrealEditor-MaterialShaderExample.dll` and `UnrealEditor-MaterialShaderExampleTests.dll`; close Editor, rebuild, reopen, and run `AVBOIT.Foundation.Status`.
- G.3 status remains `partial`; manual viewport repro must show two intersecting transparent quads and report `SkipReason=Executed` before it can be treated as evidence.
