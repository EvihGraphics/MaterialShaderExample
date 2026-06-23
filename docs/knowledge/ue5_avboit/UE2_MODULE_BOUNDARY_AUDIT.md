# UE2 Module Boundary Audit

## Current Dependencies
The `MaterialShaderExample` module uses private headers from the `Renderer` module to interact with the Deferred Shading Renderer and RDG.

## Known Usage
The following paths are included via `PrivateIncludePaths`:
- `Renderer/Private`
- `Renderer/Internal`

## Justification
- `Renderer/Private` is used because the scene rendering pass logic (`FDeferredShadingSceneRenderer`) is not exposed as a public API in UE. To hook into the RDG scene extension and manipulate early/late Z or scene color passes, private headers are required.
- `Renderer/Internal` contains new internal utilities for RDG graph builders and view states in modern UE versions, which are required for correct implementation of `FSceneViewExtensionBase`.

This is standard practice for advanced rendering plugins modifying the default pipeline, but must be monitored for API breakage between engine point releases.
