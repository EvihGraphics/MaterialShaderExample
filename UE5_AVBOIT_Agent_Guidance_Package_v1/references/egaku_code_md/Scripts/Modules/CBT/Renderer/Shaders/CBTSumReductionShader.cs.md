# Egaku source: `Scripts/Modules/CBT/Renderer/Shaders/CBTSumReductionShader.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using ShaderResource;
using UnityEngine;
using UnityEngine.Rendering;

namespace Modules.CBT.Renderer.Shaders {
  public struct CBTSumReductionShader {
        public struct PropertiesID {
            public int cbtBuffer;
            public int cbtNodeDepth;
        }

        public struct KernelID {
            public int sumReductionPrepass;
            public int sumReduction;
        }

        public ComputeShader shader;
        public KernelID kernelID;
        public PropertiesID propertiesID;
        private int _groupSize;
        
        public void Init() {
            shader = ComputeShaderResource.GetComputeShader("Ocean/Mesh/CBT/CBTSumReduction");
            kernelID.sumReduction = shader.FindKernel("CBTSumReductionKernel");
            kernelID.sumReductionPrepass = shader.FindKernel("CBTSumReductionKernel_Prepass");
            propertiesID.cbtBuffer = Shader.PropertyToID("_CBTBuffer");
            propertiesID.cbtNodeDepth = Shader.PropertyToID("_CBTNodeDepth");
            
            shader.GetKernelThreadGroupSizes(kernelID.sumReduction, out uint x, out uint y, out uint z);
            _groupSize = (int)(x*y*z);
        }

        public void SetCBTBuffer(GraphicsBuffer buffer) {
            shader.SetBuffer(kernelID.sumReduction, propertiesID.cbtBuffer, buffer);
            shader.SetBuffer(kernelID.sumReductionPrepass, propertiesID.cbtBuffer, buffer);
        }

        public void CmdSetCBTBuffer(CommandBuffer cmd, GraphicsBuffer buffer) {
            cmd.SetComputeBufferParam(shader, kernelID.sumReduction, propertiesID.cbtBuffer, buffer);
            cmd.SetComputeBufferParam(shader, kernelID.sumReductionPrepass, propertiesID.cbtBuffer, buffer);
        }
        
        public void CmdSetCBTNodeDepth(CommandBuffer cmd,int depth) {
            cmd.SetComputeIntParam(shader, propertiesID.cbtNodeDepth, depth);
        }

        public void CmdDispatchPrepass(CommandBuffer cmd, int sumNodeCount) {
            cmd.DispatchCompute(shader, kernelID.sumReductionPrepass, 
                Math.Max(sumNodeCount/_groupSize,1), 1,1);
        }
        
        public void CmdDispatch(CommandBuffer cmd, int sumNodeCount) {
            cmd.DispatchCompute(shader, kernelID.sumReduction, 
                Math.Max(sumNodeCount/_groupSize,1), 1,1);
        }
        public void Dispatch(int sumNodeCount) {
            shader.Dispatch( kernelID.sumReduction, 
                Math.Max(sumNodeCount/_groupSize,1), 1,1);
        }

        public void SetCBTNodeDepth(int depth) {
            shader.SetInt(propertiesID.cbtNodeDepth, depth);
        }

    }
}
```
