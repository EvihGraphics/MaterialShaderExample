# Egaku source: `Scripts/Modules/CBT/Renderer/Shaders/CBTEvalShader.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System.Runtime.InteropServices;
using ShaderResource;
using UnityEngine;
using UnityEngine.Rendering;


namespace Modules.CBT.Renderer.Shaders {
     
    [StructLayout(LayoutKind.Sequential)]
    struct CBTEvalContextConstants {
        public Matrix4x4 model;
        public Matrix4x4 modelView;
        public Vector4 frustumPlanes0;
        public Vector4 frustumPlanes1;
        public Vector4 frustumPlanes2;
        public Vector4 frustumPlanes3;
        public Vector4 frustumPlanes4;
        public Vector4 frustumPlanes5;
        public Vector4 targetLod;
    }
    
    [StructLayout(LayoutKind.Sequential)]
    struct CBTEvalResultMeta {
        public uint resultCount;
    }


    public struct CBTEvalShader {
        public struct PropertiesID {
            public int cbtBuffer;
            public int cbtEvalResultBuffer;
            public int cbtEvalContextConstants;
            public int cbtEvalResultMetaBuffer;
        }

        public struct KernelID {
            public int cbtEvalNodesKernel_EvalSplit;
            public int cbtEvalNodesKernel_EvalMerge;
        }

        private ComputeShader _shader;
        private KernelID _kernelID;
        private PropertiesID _propertiesID;
   
        public void Init() {
            _shader = ComputeShaderResource.GetComputeShader("Ocean/Mesh/CBT/CBTEval");
            _kernelID.cbtEvalNodesKernel_EvalMerge = _shader.FindKernel("CBTEvalNodesKernel_EvalMerge");
            _kernelID.cbtEvalNodesKernel_EvalSplit = _shader.FindKernel("CBTEvalNodesKernel_EvalSplit");
            _propertiesID.cbtBuffer = Shader.PropertyToID("_CBTBuffer");
            _propertiesID.cbtEvalResultBuffer = Shader.PropertyToID("_CBTEvalResultBuffer");
            _propertiesID.cbtEvalContextConstants = Shader.PropertyToID("CBTEvalContextConstants");
            _propertiesID.cbtEvalResultMetaBuffer = Shader.PropertyToID("_CBTEvalResultMetaBuffer");
        }

        public void SetCBTBuffer(GraphicsBuffer buffer) {
            _shader.SetBuffer(_kernelID.cbtEvalNodesKernel_EvalMerge, _propertiesID.cbtBuffer, buffer);
            _shader.SetBuffer(_kernelID.cbtEvalNodesKernel_EvalSplit, _propertiesID.cbtBuffer, buffer);
        }

        public void SetCBTEvalResultBuffer(GraphicsBuffer buffer) {
            _shader.SetBuffer(_kernelID.cbtEvalNodesKernel_EvalMerge, _propertiesID.cbtEvalResultBuffer, buffer);
            _shader.SetBuffer(_kernelID.cbtEvalNodesKernel_EvalSplit, _propertiesID.cbtEvalResultBuffer, buffer);
        }


        
        public void SetCBTEvalContextCBuffer(GraphicsBuffer buffer) {
            _shader.SetConstantBuffer(_propertiesID.cbtEvalContextConstants, buffer,0,Marshal.SizeOf<CBTEvalContextConstants>());
        }
        
        public void SetCBTEvalResultMetaBuffer(GraphicsBuffer buffer) {
            _shader.SetBuffer(_kernelID.cbtEvalNodesKernel_EvalMerge, _propertiesID.cbtEvalResultMetaBuffer, buffer);
            _shader.SetBuffer(_kernelID.cbtEvalNodesKernel_EvalSplit, _propertiesID.cbtEvalResultMetaBuffer, buffer);
        }

        public void CmdDispatchEvalMergePassIndirect(CommandBuffer cmd,GraphicsBuffer indirectBuffer,uint argsOffset = 0) {
            cmd.DispatchCompute(_shader, _kernelID.cbtEvalNodesKernel_EvalMerge,indirectBuffer,argsOffset);
        }
        public void CmdDispatchEvalSplitPassIndirect(CommandBuffer cmd,GraphicsBuffer indirectBuffer,uint argsOffset = 0) {
            cmd.DispatchCompute(_shader, _kernelID.cbtEvalNodesKernel_EvalSplit,indirectBuffer,argsOffset);
        }
    }

}
```
