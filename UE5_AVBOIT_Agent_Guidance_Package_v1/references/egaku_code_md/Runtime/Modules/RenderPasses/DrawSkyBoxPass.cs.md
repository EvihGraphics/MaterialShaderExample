# Egaku source: `Runtime/Modules/RenderPasses/DrawSkyBoxPass.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Egaku.Rendering.Pipeline.Common;
using Unity.Collections;
using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.Rendering.RendererUtils;


namespace Egaku.Rendering.Pipeline.Runtime {
    public class DrawSkyBoxPass : RenderPass {
        public RenderTargetDesc renderTargetDesc;
        public RenderPassDesc renderPassDesc;
        
        public CommandBuffer cmdBuffer;

        public override void Execute(RenderContext context) {
            var attachments = new NativeArray<AttachmentDescriptor>(2, Allocator.Temp);
            //renderTargetDesc.depthDesc.ConfigureClear(Color.black);
            renderTargetDesc.colorDesc.loadAction = RenderBufferLoadAction.Load;
            renderTargetDesc.depthDesc.loadAction = RenderBufferLoadAction.Load;
     //       cmdBuffer = CommandBufferPool.Get();
       //     renderTargetDesc.colorDesc.ConfigureClear(Color.blue);
         //   renderTargetDesc.depthDesc.ConfigureClear(Color.clear);
            const int depthIndex = 0, albedoIndex = 1;
            attachments[depthIndex] = renderTargetDesc.depthDesc;
            attachments[albedoIndex] = renderTargetDesc.colorDesc;
            
        //    RenderCommand.InjectCommandsAndClear(context,cmdBuffer);
            using (attachments)
            using (RenderCommand.CreateRenderPassScope(context, renderPassDesc.width, renderPassDesc.height,
                       renderPassDesc.msaaSamples, attachments, depthIndex)) {
                var colors = new NativeArray<int>(1, Allocator.Temp);
                colors[0] = albedoIndex;
                using (colors)
                using (RenderCommand.CreateSubPassScope(context, colors)) {
                    RendererList list =
                        context.InternalRenderContext.CreateSkyboxRendererList(context.Camera.InternalCamera);
                    cmdBuffer.DrawRendererList(list); 
                    RenderCommand.InjectCommandsAndClear(context,cmdBuffer);
                }
            }
        }
    }
}
```
