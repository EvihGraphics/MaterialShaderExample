# UE AVBOIT Insertion Decision

Generated UTC: 2026-06-22T08:19:40Z

## Decision

Use `MaterialShaderExample` as the baseline shell and introduce AVBOIT as an isolated plugin path in a later implementation cycle. Do not remove or rewrite the existing Nanite material-shader example during bootstrap.

## Rationale

- The existing project already proves shader source mapping, `FMaterialShader` registration, subsystem ownership, render-thread state copy, `SceneViewExtension`, RDG pass insertion, and compute dispatch.
- The existing Nanite private-buffer path is useful as baseline evidence but too specialized and fragile to be the default AVBOIT integration point.
- The first UE AVBOIT implementation should target deterministic plugin-owned test content, not general UE translucency, Niagara, Substrate, or an engine patch.

## Current Gate

Build/run is blocked by `ue57-path-missing`. Until UE 5.7 is located, this decision is source-confirmed only, not build-confirmed.

