# Egaku source: `Scripts/Modules/CBT/Renderer/Shaders/CBTUpdateShader.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using System.Runtime.InteropServices;
using ShaderResource;
using UnityEngine;
using UnityEngine.Rendering;

namespace Modules.CBT.Renderer.Shaders {


    [StructLayout(LayoutKind.Sequential)]
    struct CBTUpdateContextConstants {
        public Matrix4x4 modelView;
        public Vector4 frustumPlanes0;
        public Vector4 frustumPlanes1;
        public Vector4 frustumPlanes2;
        public Vector4 frustumPlanes3;
        public Vector4 frustumPlanes4;
        public Vector4 frustumPlanes5;
        public Vector4 targetLod;
    }
   


    public struct CBTUpdateShader {
        public struct PropertiesID {
            public int cbtBuffer;
            public int cbtEvalResultBuffer;
            public int modelView;
            public int frustumPlanes;
            public int targetLod;
            public int cbtUpdateContextConstants;
        }

        public struct KernelID {
            public int splitPass;
            public int mergePass;
        }
        public static readonly int GroupSize = 64;
        public ComputeShader shader;
        public KernelID kernelID;
        public PropertiesID propertiesID;
 
        public void Init() {
            shader = ComputeShaderResource.GetComputeShader("Ocean/Mesh/CBT/CBTUpdate");
            kernelID.splitPass = shader.FindKernel("CBTUpdateKernel_Split");
            kernelID.mergePass = shader.FindKernel("CBTUpdateKernel_Merge");
            propertiesID.cbtBuffer = Shader.PropertyToID("_CBTBuffer");
            propertiesID.modelView = Shader.PropertyToID("_ModelView");
            propertiesID.frustumPlanes = Shader.PropertyToID("_FrustumPlanes");
            propertiesID.targetLod = Shader.PropertyToID("_TargetLod");
            propertiesID.cbtEvalResultBuffer = Shader.PropertyToID("_CBTEvalResultBuffer");
            propertiesID.cbtUpdateContextConstants = Shader.PropertyToID("CBTUpdateContextConstants");
        }

        public void SetCBTBuffer(GraphicsBuffer buffer) {
            shader.SetBuffer(kernelID.splitPass, propertiesID.cbtBuffer, buffer);
            shader.SetBuffer(kernelID.mergePass, propertiesID.cbtBuffer, buffer);
        }
        public void SetCBTEvalResultBuffer(GraphicsBuffer buffer) {
            shader.SetBuffer(kernelID.splitPass, propertiesID.cbtEvalResultBuffer, buffer);
            shader.SetBuffer(kernelID.mergePass, propertiesID.cbtEvalResultBuffer, buffer);
        }
        

        public void CmdSetCBTBuffer(CommandBuffer cmd, GraphicsBuffer buffer) {
            cmd.SetComputeBufferParam(shader, kernelID.splitPass, propertiesID.cbtBuffer, buffer);
            cmd.SetComputeBufferParam(shader, kernelID.mergePass, propertiesID.cbtBuffer, buffer);
        }
        
        public void CmdDispatchSplitPassIndirect(CommandBuffer cmd,GraphicsBuffer indirectBuffer,uint argsOffset = 0) {
            cmd.DispatchCompute(shader, kernelID.splitPass,indirectBuffer,argsOffset);
        }
        
        public void CmdDispatchMergePassIndirect(CommandBuffer cmd,GraphicsBuffer indirectBuffer,uint argsOffset = 0) {
            cmd.DispatchCompute(shader, kernelID.mergePass,indirectBuffer,argsOffset);
        }
        
        public void CmdDispatchSplitPass(CommandBuffer cmd,int cbtNodeCount) {
            int updateThreadGroupX = Math.Max(cbtNodeCount/GroupSize,1);
            cmd.DispatchCompute(shader, kernelID.splitPass, updateThreadGroupX, 1,1);
        }
                
        public void DispatchSplitPass(int cbtNodeCount) {
            int updateThreadGroupX = Math.Max(cbtNodeCount/GroupSize,1);
            shader.Dispatch( kernelID.splitPass, updateThreadGroupX, 1,1);
        }
        public void DispatchMergePass(int cbtNodeCount) {
            int updateThreadGroupX = Math.Max(cbtNodeCount/GroupSize,1);
            shader.Dispatch( kernelID.mergePass, updateThreadGroupX, 1,1);
        }
        
        public void CmdDispatchMergePass(CommandBuffer cmd,int cbtNodeCount) {
            int updateThreadGroupX = Math.Max(cbtNodeCount/GroupSize,1);
            cmd.DispatchCompute(shader, kernelID.mergePass, updateThreadGroupX, 1,1);
        }

        public void CmdSetContextConstants(CommandBuffer cmd, GraphicsBuffer buffer) {
            cmd.SetComputeConstantBufferParam(shader, propertiesID.cbtUpdateContextConstants, buffer,0, Marshal.SizeOf<CBTUpdateContextConstants>());
        }


        public void SetContextConstants(GraphicsBuffer buffer) {
            shader.SetConstantBuffer(propertiesID.cbtUpdateContextConstants, buffer, 0,
                Marshal.SizeOf<CBTUpdateContextConstants>());
        }
        
        
    }
}
```
