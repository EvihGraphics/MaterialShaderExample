# MaterialShaderExample Baseline Map

Generated UTC: 2026-06-22T08:19:40Z

## Repository

| Item | Value |
|---|---|
| Baseline worktree | `D:\HTC\avboit\MaterialShaderExample` |
| Development worktree | `D:\HTC\avboit\AVBOIT开发` |
| Remote | `https://github.com/EvihGraphics/MaterialShaderExample.git` |
| Baseline branch | `master` |
| Development branch | `AVBOIT开发` tracking `origin/master` |
| HEAD | `a80b000f589e9953fcb5efbfd3eccbf247d62c4a` |
| Merge-base with `origin/master` | `a80b000f589e9953fcb5efbfd3eccbf247d62c4a` |
| EngineAssociation | `5.7` |

## Existing Plugin Flow

```text
AMaterialShaderExampleActor
  -> UMaterialShaderExampleSubsystem
  -> ENQUEUE_RENDER_COMMAND copy
  -> FExampleSceneViewExtension::PreRenderBasePass_RenderThread
  -> GraphBuilder.AddPass("Material Shader Example")
  -> FExampleMaterialShader compute dispatch
  -> /Plugin/MaterialShaderExample/MaterialShaderExample.usf
```

## Reusable Baseline Pieces

- `MaterialShaderExampleModule.cpp` maps `/Plugin/MaterialShaderExample` to the plugin shader directory.
- `MaterialShaderExampleActor` provides editor-visible material inputs and keeps the sample actor tickable in editor.
- `MaterialShaderExampleSubsystem` owns the `SceneViewExtension` and copies game-thread state to render-thread state.
- `ExampleSceneViewExtension` demonstrates RDG pass insertion and render-thread material shader dispatch.
- `ExampleMaterialShader` registers a `FMaterialShader` compute entry point.

## Do Not Treat As AVBOIT Contract

- `Nanite.VisBuffer64` and `Nanite.ShadingMask` lookup.
- Nanite private uniform-buffer dummy bindings.
- Shading-bin replacement logic.
- `MATUSAGE_VirtualHeightfieldMesh` reuse as a compile flag.

## Baseline Evidence

| Evidence | Path | Status |
|---|---|---|
| Local prior screenshot | `D:\HTC\avboit\AVBOIT开发\LocalVisualResults\HIVE_4090x2\initial_render.png` | present, 887x550, SHA-256 `F704DA676A2BCE7CBEAF5FC102B8651E45D26152A09DC7AA7CA26FF0E755D7B8` |
| Fresh UE build | not available | blocked: `ue57-path-missing` |
| Fresh UE run | not available | blocked: `ue57-path-missing` |

