# Egaku source: `Scripts/Modules/CBT/Renderer/Shaders/CBTIndirectShader.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿
using ShaderResource;
using UnityEngine;
using UnityEngine.Rendering;


namespace Modules.CBT.Renderer.Shaders {

    public struct CBTIndirectShader {
        public struct PropertiesID {
            public int cbtBuffer;
            public int cbtEvalResultBuffer;
            public int drawIndirectBuffer;
            public int dispatchUpdateIndirectBuffer;
            public int dispatchEvalIndirectBuffer;
            public int cbtEvalResultMetaBuffer;
        }

        public struct KernelID {
            public int cbtUpdateIndirect;
        }
        
        public ComputeShader shader;
        public KernelID kernelID;
        public PropertiesID propertiesID;
   
        public void Init() {
            shader = ComputeShaderResource.GetComputeShader("Ocean/Mesh/CBT/CBTIndirect");
            kernelID.cbtUpdateIndirect = shader.FindKernel("CBTUpdateIndirectKernel");
            propertiesID.cbtEvalResultBuffer = Shader.PropertyToID("_CBTEvalResultBuffer");
            propertiesID.cbtBuffer = Shader.PropertyToID("_CBTBuffer");
            propertiesID.drawIndirectBuffer = Shader.PropertyToID("_DrawIndirectBuffer");
            propertiesID.dispatchUpdateIndirectBuffer = Shader.PropertyToID("_DispatchUpdateIndirectBuffer");
            propertiesID.dispatchEvalIndirectBuffer = Shader.PropertyToID("_DispatchEvalIndirectBuffer");
            propertiesID.cbtEvalResultMetaBuffer = Shader.PropertyToID("_CBTEvalResultMetaBuffer");
        }

        public void SetCBTBuffer(GraphicsBuffer buffer) {
            shader.SetBuffer(kernelID.cbtUpdateIndirect, propertiesID.cbtBuffer, buffer);
        }

        public void SetDrawIndirectBuffer(GraphicsBuffer buffer) {
            shader.SetBuffer(kernelID.cbtUpdateIndirect, propertiesID.drawIndirectBuffer, buffer);
        }
        
        public void SetCBTEvalResultBuffer(GraphicsBuffer buffer) {
            shader.SetBuffer(kernelID.cbtUpdateIndirect, propertiesID.cbtEvalResultBuffer, buffer);
        }
        
        public void SetCBTEvalResultMetaBuffer(GraphicsBuffer buffer) {
            shader.SetBuffer(kernelID.cbtUpdateIndirect, propertiesID.cbtEvalResultMetaBuffer, buffer);
        }

        
        public void SetCBTUpdateDispatchIndirectBuffer(GraphicsBuffer buffer) {
            shader.SetBuffer(kernelID.cbtUpdateIndirect, propertiesID.dispatchUpdateIndirectBuffer, buffer);
        }
        
        public void SetCBTEvalDispatchIndirectBuffer(GraphicsBuffer buffer) {
            shader.SetBuffer(kernelID.cbtUpdateIndirect, propertiesID.dispatchEvalIndirectBuffer, buffer);
        }

        public void CmdDispatch(CommandBuffer cmd) {
            cmd.DispatchCompute(shader, kernelID.cbtUpdateIndirect,1,1,1);
        }
    }
    
}
```
