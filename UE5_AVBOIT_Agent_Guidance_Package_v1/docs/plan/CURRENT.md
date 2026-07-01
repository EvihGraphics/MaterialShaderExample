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
docs/checkpoints/archive/CHECKPOINT-0047-20260701T133700Z-UE-4-2G-AVBOIT-QUAD-FOUNDATION.md
```

Current mirrored status:

```text
partial
```

UE-4.2G starts the Native-OIT-guided Foundation Quad gate. The current working tree has landed the shared frame config, primitive packet, Foundation CVars, config-driven Raster pass, ViewRect-local low-resolution volume addressing, and the validation runner skeleton.

`MaterialShaderDemoEditor` builds locally, including `MaterialShaderExampleNiagara`, but the full visual matrix, exact-reference parity, Native OIT comparison, lifecycle matrix, and RenderDoc/PIX capture are not complete.

Do not mark `SUCCESS`, `COMPLETED`, or `passed-local`, and do not proceed to UE-4.2H.
