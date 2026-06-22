# Egaku source: `Runtime/Modules/RenderPasses/BlitRenderPass.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Egaku.Rendering.Pipeline.Common;
using UnityEditor.VersionControl;
using UnityEngine;
using UnityEngine.Rendering;

namespace Egaku.Rendering.Pipeline.Runtime {
    public class BlitRenderPass : RenderPass {
        
        public BlitRenderPass() {
            Order = 2;
            Name = nameof(SolidColorPostProcessPass);
        }


        
        public static readonly string Tag = nameof(BlitRenderPass);
        public RenderTexture Source { get; set; }
        
        public RenderTargetIdentifier Destination { get; set; }

        public override void Execute(RenderContext context) {
            CommandBuffer cb = CommandBufferPool.Get();
            cb.Blit(Source,Destination);
            RenderCommand.InjectCommandsAndClear(context,cb);
            CommandBufferPool.Release(cb);
        }
        
    }
}
```
