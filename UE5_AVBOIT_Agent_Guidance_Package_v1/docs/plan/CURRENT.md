# Guidance Package Current Mirror

The active recovery pointer is:

```text
docs/plan/CURRENT.md
```

in the repository root of:

```text
MaterialShaderExample_AVBOIT
```

Latest mirrored checkpoint:

```text
docs/checkpoints/archive/CHECKPOINT-0048-20260701T145642Z-UE-4-2G-FOUNDATION-VISUAL-CLOSEOUT.md
```

Current mirrored status:

```text
partial
```

UE-4.2G.1 continues the Native-OIT-guided Foundation Quad gate. The current working tree has landed the shared frame config, primitive packet, Foundation CVars, config-driven Raster pass, ViewRect-local low-resolution volume addressing, direct Foundation visual suite, CPU exact reference output, plugin identity shaders, resolved-alpha pass, full-image readback helper, real log scan, and runner-owned marker semantics.

`MaterialShaderDemoEditor` and `ContentExamplesEditor` build locally. The latest runner output is:

```text
LocalVisualResults\TempResults\UE57\HIVE_4090x2\UE4-2G-AVBOIT-Quad-Foundation\20260701T145304Z
```

The run is `partial`: direct raster and synthetic checks pass, log scan has zero critical matches, but the full visual matrix is missing 7 files, PluginAVBOIT fails exact-reference parity, PluginAVBOIT fails AB/BA order-independence, Native OIT is not proven, lifecycle is not complete, and no RenderDoc/PIX capture was produced.

Do not mark `SUCCESS`, `COMPLETED`, or `passed-local`, and do not proceed to UE-4.2H.
