# Egaku source: `Runtime/Modules/RenderPasses/ClearDepthColorPass.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Egaku.Rendering.Pipeline.Common;
using Unity.Collections;
using UnityEngine;
using UnityEngine.Rendering;

namespace Egaku.Rendering.Pipeline.Runtime {
    
    public class ClearDepthColorPass : RenderPass{
        
        public RenderTargetDesc renderTargetDesc;
        public RenderPassDesc renderPassDesc;
  
        
        public override void Execute(RenderContext context) {
 
            var attachments = new NativeArray<AttachmentDescriptor>(2, Allocator.Temp);
            const int depthIndex = 0, albedoIndex = 1;
            renderTargetDesc.colorDesc.ConfigureClear(Color.green);
            renderTargetDesc.depthDesc.ConfigureClear(Color.clear);
            attachments[depthIndex] = renderTargetDesc.depthDesc;
            attachments[albedoIndex] = renderTargetDesc.colorDesc;
          
            using (attachments)
            using (RenderCommand.CreateRenderPassScope(context,renderPassDesc.width, renderPassDesc.height, renderPassDesc.msaaSamples, attachments, depthIndex)) {
                var colors = new NativeArray<int>(1, Allocator.Temp);
                colors[0] = albedoIndex;
                using (colors)
                using (RenderCommand.CreateSubPassScope(context,colors)) {
                
                }
                
            }
        }
    }
}
```
