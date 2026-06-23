# UE-3 Existing Evidence Rejection Report

## Overview
During the initial UE-3 execution, automated screenshots were captured using active viewport readbacks (`FViewport->ReadPixels`) coupled with a ticker interval. 

## Files Audited
- `SingleWhite/StandardAlpha.png`
- `DoubleRedBlue/StandardAlpha_CorrectSort.png`
- `TripleRGB/CorrectSort.png`
- `TripleRGB/SubmissionOrders/00_RGB.png`
- `TripleRGB/SubmissionOrders/01_RBG.png`
- `TripleRGB/SubmissionOrders/02_GRB.png`
- `TripleRGB/SubmissionOrders/03_GBR.png`
- `TripleRGB/SubmissionOrders/04_BRG.png`
- `TripleRGB/SubmissionOrders/05_BGR.png`

## Identical Blob Groups
All 9 audited images share exactly the same Git Blob SHA and SHA-256 hash:
- **Git Blob:** `8d8ddcb3a3d73cb7aaa834c529972882fb896644`
- **SHA-256:** `61F6B6245A6EC43CB1BB2305760A0420B3AE7DCB7EE650923EE70D9F06524B41`

## Why Evidence Is Invalid
Because the images are physically identical at the binary level, they fail to demonstrate that the visual state responded to the `Preset` and `Order` permutations. A single image has been replicated 9 times.

## Probable Capture Failure
The `FViewport->ReadPixels` capture mechanism within the `FTSTicker` callback failed to synchronize with the GPU Render Thread. Specifically:
1. Setting CVars and rebuilding the scene actor does not immediately result in a drawn frame on the GPU.
2. The fixed time delay (e.g., waiting 0.5s) merely waits on the game thread clock. If the rendering thread is stalled or the viewport is not actively redrawn (e.g., running `-game` headless or window inactive), the frame buffer is stale.
3. The viewport captures the exact same stale visual state over and over.

## Resolution
Existing UE-3 Visual Evidence: **REJECTED**

The old evidence will remain isolated in `LocalVisualResults/UE57/<MachineId>/UE3-RGB-TestContent/` to preserve historical record, but it will no longer be considered valid.

## Replacement Evidence Root
New visual evidence will be generated using a dedicated, deterministic `SceneCaptureComponent2D` and saved to:
`LocalVisualResults/UE57/<MachineId>/UE3-1-RGB-Validation/`
