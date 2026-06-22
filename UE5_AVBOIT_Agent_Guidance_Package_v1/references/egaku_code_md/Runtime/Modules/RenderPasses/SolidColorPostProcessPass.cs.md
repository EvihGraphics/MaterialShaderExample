# Egaku source: `Runtime/Modules/RenderPasses/SolidColorPostProcessPass.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using System.Collections.Generic;
using Egaku.Rendering.Pipeline.Common;
using Unity.Collections;
using UnityEngine;
using UnityEngine.Rendering;
using Action = Unity.Plastic.Antlr3.Runtime.Misc.Action;

namespace Egaku.Rendering.Pipeline.Runtime {
    public class SolidColorPostProcessPass : RenderPass {

        public RenderTargetDesc renderTargetDesc;
        public RenderPassDesc renderPassDesc;

        public CommandBuffer cmdBuffer;
        
        
        public SolidColorPostProcessPass() {
            Order = 1;
            Name = nameof(SolidColorPostProcessPass);
       
        }
        
        public string tag = "pass1";

        public Color SolidColor { get; set;}
        private Shader _shader = Shader.Find("Egaku Render Pipeline/Blit");
        Material _mat;
        public override void Execute(RenderContext context) {
            
            var attachments = new NativeArray<AttachmentDescriptor>(2, Allocator.Temp);
            //renderTargetDesc.depthDesc.ConfigureClear(Color.black);
            //renderTargetDesc.depthDesc.loadAction = RenderBufferLoadAction.DontCare;
         
            if (_mat is null) {
                _mat = new Material(_shader);
            }
            _mat.SetVector("_Color", SolidColor);
          //  const int depthIndex = 0, albedoIndex = 1;
        //    attachments[depthIndex] = renderTargetDesc.depthDesc;
         //   attachments[albedoIndex] = renderTargetDesc.colorDesc;
          
            var sourceTexture = renderTargetDesc.color;
            
            RenderTextureDescriptor descriptor = new RenderTextureDescriptor(renderPassDesc.width,
                renderPassDesc.height, RenderTextureFormat.ARGB32);
            RenderTargetIdentifier tempTex = new RenderTargetIdentifier(0);
            cmdBuffer.GetTemporaryRT(0,descriptor);
            cmdBuffer.Blit(sourceTexture,tempTex);
            cmdBuffer.Blit(tempTex,sourceTexture,_mat);
            cmdBuffer.ReleaseTemporaryRT(0);

        //    using(attachments);
            
             /*
             using (attachments)
             using (RenderCommand.CreateRenderPassScope(context, renderPassDesc.width, renderPassDesc.height,
                        renderPassDesc.msaaSamples, attachments, depthIndex)) {
                 var colors = new NativeArray<int>(1, Allocator.Temp);
                 colors[0] = albedoIndex;
                 using (colors)
                 using (RenderCommand.CreateSubPassScope(context, colors)) {
                     var sourceTexture = renderTargetDesc.color;
                     var tempTex = new RenderTexture(sourceTexture);
                     cmdBuffer.Blit(sourceTexture,tempTex,_mat);
                     cmdBuffer.Blit(tempTex,sourceTexture);
                     RenderCommand.InjectCommandsAndClear(context, cmdBuffer);
                     cmdBuffer.Release();
                 }
             }*/

        }
        
    }
}
```
