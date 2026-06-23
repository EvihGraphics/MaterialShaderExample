# UE-4 Engine Mod Plan Correction

## Current Decision
No Engine Fork.

## Current Algorithm
64-slice AVBOIT volumetric extinction / transmittance pipeline.

## Rejected Interpretation
Per-pixel linked-list OIT.
(The previous plan mistakenly introduced Head Pointers and Linked-List OIT which violates the AVBOIT contract).

## Revisit Engine Modification
Only after plugin-owned UE-4 vertical slice proves a specific public API blocker.
