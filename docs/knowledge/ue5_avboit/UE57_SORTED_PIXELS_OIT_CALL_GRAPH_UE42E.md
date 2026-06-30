# UE5.7 Sorted Pixels OIT Call Graph for UE-4.2E

Generated UTC: 2026-06-30T19:02:06Z

## Runtime Contract

- Reference CVar: `r.OIT.SortedPixels`
- The 4.2E runtime mode `UESortedPixelsOIT` may only be treated as available when the CVar is registered at runtime.
- If the CVar is not registered, the overlay/status must display the mode as unavailable and no KeyResults promotion may depend on UE OIT output.

## Plugin Usage

`AVBOIT.Niagara.Mode UESortedPixelsOIT` performs:

- `r.AVBOIT.Enable = 0`
- `r.AVBOIT.Niagara.Enable = 0`
- `r.AVBOIT.Core.Identity.Enable = 0`
- `r.AVBOIT.Core.FixedSlice.Enable = 0`
- `r.AVBOIT.BufferOverview = 0`
- `r.OIT.SortedPixels = 1`, only if the CVar is registered
- Tint disabled

## Evidence Requirement

UE OIT comparison evidence must include:

- CVar registered state
- CVar requested value
- Engine Unlit view-mode contract
- Same fixed `PlayerCameraManager0` camera contract
- Same Niagara fixed-age state
- Side-by-side, difference, heatmap, coverage, and ROI images against Engine Default

## Current State

The plugin can request UE OIT mode by CVar when present. It does not use UE OIT as proof of the plugin AVBOIT path.
