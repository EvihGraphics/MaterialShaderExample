# Egaku source: `Runtime/Modules/RenderPasses/DrawOpaquePass.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Egaku.Rendering.Pipeline.Common;
using UnityEngine;
using UnityEngine.Rendering;

namespace Egaku.Rendering.Pipeline.Runtime {
    public class DrawOpaquePass: RenderPass {
        
        
        static readonly ShaderTagId _ShaderTagId = new("Basic");

        
        public override void Execute(RenderContext context) {
            context.Camera.TryGetCullingParameters(out var p);
            SortingSettings sortingSettings = new SortingSettings(context.Camera.InternalCamera);
            CullingResults r = context.Cull(ref p);
            DrawingSettings drawingSettings = new DrawingSettings(_ShaderTagId, sortingSettings);
            FilteringSettings filteringSettings = new FilteringSettings(RenderQueueRange.all);
            RenderCommand.DrawRenderers(context, r, ref drawingSettings, ref filteringSettings);
        }

    }
}
```
