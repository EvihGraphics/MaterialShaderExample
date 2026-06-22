# Egaku source: `Scripts/Modules/OIT/AVBOIT/Renderer/Shaders/VirtualSliceBasedShader.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using Modules.OIT.AVBOIT.Material;
using ShaderResource;
using UnityEngine;
using UnityEngine.Rendering;


namespace Modules.OIT.AVBOIT.Renderer.Shaders {
    #if false
    public struct VirtualSliceBasedShader {
        
        private static ComputeShader _volumeTransmittanceIntegrationCS;
        private static ComputeShader _depthWarpLutBuildingCS;
        private static AVBOITComputeShaderConfig.VirtualSliceBasedShaderConfig _computeShaderConfig = new();
        private static AVBOITShaderConfig _graphicsShaderConfig = new();

        struct ComputeKernelDesc {
            public ComputeShader shader;
            public int kernelId;
            public (uint,uint,uint) threadGroupSizes;
            public (int,int,int) threadDispatchSizes;
        }

        struct ComputeKernelConfigs {
            public ComputeKernelDesc volumeTransmittanceIntegration;
            public ComputeKernelDesc volumeBlockLutBuilding;
            public ComputeKernelDesc volumeBlockLutParallelBuilding;
        }
        

        struct GraphicsPassId {
            public int splattingPassId;
            public int translucencyForwardPrePassId;
            public int translucencyForwardPassId;
            public int volumeOccupancyCollectionId;
        }
        
        
        private static int _backgroundResolutionPassId;
        private static ComputeKernelConfigs _computeKernelConfigs;
        private GraphicsPassId _graphicsPassId;
        private static RTHandle _volumeTransmittanceLut;
        private static RTHandle _volumeExtinctionBuffer;
        private static RTHandle _volumeBlockLut;
        private static RTHandle _volumeBlockOccupancyBuffer;
        private static UnityEngine.Material _defaultMaterial; 
        private UnityEngine.Material _material;

        private Matrix4x4 _modelMatrix;


        public static void Initialize() {
            var csConfigVTI = _computeShaderConfig.GetVolumeTransmittanceIntegrationCSConfig();
            var csConfigVBLutBuild = _computeShaderConfig.GetVolumeBlockLutBuildingCSConfig();

            _volumeTransmittanceIntegrationCS = ComputeShaderResource.GetComputeShader(
                csConfigVTI.GetComputeShaderAssetURL());
            _depthWarpLutBuildingCS = ComputeShaderResource.GetComputeShader(
                csConfigVBLutBuild.GetComputeShaderAssetURL());
            
            // register compute kernels

            _computeKernelConfigs.volumeTransmittanceIntegration.shader = _volumeTransmittanceIntegrationCS;
            _computeKernelConfigs.volumeTransmittanceIntegration.kernelId = _volumeTransmittanceIntegrationCS.FindKernel(csConfigVTI.GetVolumeTransmittanceIntegrationPassKernelName());
            uint x, y, z;
            _volumeTransmittanceIntegrationCS.GetKernelThreadGroupSizes(_computeKernelConfigs.volumeTransmittanceIntegration.kernelId,
                out x, out y, out z);
            _computeKernelConfigs.volumeTransmittanceIntegration.threadGroupSizes = (x, y, z);

            
            _computeKernelConfigs.volumeBlockLutBuilding.shader = _depthWarpLutBuildingCS;
            _computeKernelConfigs.volumeBlockLutBuilding.kernelId = _depthWarpLutBuildingCS.FindKernel(csConfigVBLutBuild.GetLutBuildingPassKernelName());
            _depthWarpLutBuildingCS.GetKernelThreadGroupSizes(_computeKernelConfigs.volumeBlockLutBuilding.kernelId, out x, out y, out z);
            _computeKernelConfigs.volumeBlockLutBuilding.threadGroupSizes = (x, y, z);
            
            _computeKernelConfigs.volumeBlockLutParallelBuilding.shader = _depthWarpLutBuildingCS;
            _computeKernelConfigs.volumeBlockLutParallelBuilding.kernelId = _depthWarpLutBuildingCS.FindKernel(csConfigVBLutBuild.GetLutParallelBuilding_ScanPassKernelName());
            _depthWarpLutBuildingCS.GetKernelThreadGroupSizes(_computeKernelConfigs.volumeBlockLutParallelBuilding.kernelId, out x, out y, out z);
            _computeKernelConfigs.volumeBlockLutParallelBuilding.threadGroupSizes = (x, y, z);
            
        }

        public void InitOITMaterial() {
            _material = new UnityEngine.Material(Shader.Find(_graphicsShaderConfig.ShaderURL));
            SetOITMaterial(_material);
        }
        
        public void SetBaseColor(in Color color) {
            _material.SetColor("_Translucency_baseColor",color);
        }

        public void SetOITMaterial(UnityEngine.Material material) {
            _material = material;
            if (_defaultMaterial == null) {
                _defaultMaterial = material;
                _backgroundResolutionPassId = _defaultMaterial.FindPass(_graphicsShaderConfig.GetBackgroundResolutionPassName());
            }
            _graphicsPassId.splattingPassId = _material.FindPass(_graphicsShaderConfig.GetSplattingPassName());
            _graphicsPassId.translucencyForwardPrePassId = _material.FindPass(_graphicsShaderConfig.GetTranslucencyForwardPrePassName());
            _graphicsPassId.translucencyForwardPassId = _material.FindPass(_graphicsShaderConfig.GetTranslucencyForwardPassName());
            _graphicsPassId.volumeOccupancyCollectionId = _material.FindPass(_graphicsShaderConfig.GetVolumeOccupancyCollectionPassName());
        }

        private static void SetFrustumSliceCurveFactor(int factor) {
            Shader.SetGlobalInt("_AVBOIT_FrustumSliceCurveFactor",factor);
        }

        public static void SetFrustumFarPlane(float farPlane) {
            Shader.SetGlobalFloat("_AVBOIT_FrustumFarPlane",farPlane);
        }
        


        public bool IsValid() {
            return _material!= null;
        }
        
        public void SetModelMatrix(in Matrix4x4 modelMatrix) {
            _modelMatrix = modelMatrix;
        }

        public static void SetVolumeTransmittanceLut(RTHandle r) {
            _volumeTransmittanceLut = r;
            Shader.SetGlobalTexture("_AVBOIT_VolumeTransmittanceLut",_volumeTransmittanceLut);
            int pixelHeight = _volumeTransmittanceLut.rt.height;
            int pixelWidth = _volumeTransmittanceLut.rt.width;
            uint threadGroupSizeX = _computeKernelConfigs.volumeTransmittanceIntegration.threadGroupSizes.Item1;
            uint threadGroupSizeY = _computeKernelConfigs.volumeTransmittanceIntegration.threadGroupSizes.Item2;
            uint threadGroupSizeZ = _computeKernelConfigs.volumeTransmittanceIntegration.threadGroupSizes.Item3;
            int threadGroupsDispatchX = (int)Mathf.Ceil((float)pixelWidth / (float)threadGroupSizeX);
            int threadGroupsDispatchY = (int)Mathf.Ceil((float)pixelHeight / (float)threadGroupSizeY);
            int threadGroupsDispatchZ = 1;
            _computeKernelConfigs.volumeTransmittanceIntegration.threadDispatchSizes = (threadGroupsDispatchX,
                threadGroupsDispatchY, threadGroupsDispatchZ);

            int sliceCount = _volumeTransmittanceLut.rt.volumeDepth;
            SetFrustumSliceCurveFactor(2*sliceCount);
        }

        public static void SetVolumeExtinctionBuffer(RTHandle r) {
            _volumeExtinctionBuffer = r;
            Shader.SetGlobalTexture("_AVBOIT_VolumeExtinctionBuffer",_volumeExtinctionBuffer);
        }

        public static void SetVolumeBlockLut(RTHandle r) {
            _volumeBlockLut = r;
            int length = _volumeBlockLut.rt.width;
            uint threadGroupSizeX = _computeKernelConfigs.volumeBlockLutBuilding.threadGroupSizes.Item1;
            int dispatchX = (int)Mathf.Ceil((float)length / (float)threadGroupSizeX);
            Shader.SetGlobalTexture("_AVBOIT_VolumeBlockLut", _volumeBlockLut);
            _computeKernelConfigs.volumeBlockLutBuilding.threadDispatchSizes = (Math.Max(1,dispatchX/2), 1, 1);
        }

        public static void SetVolumeBlockOccupancyBuffer(RTHandle r) {
            _volumeBlockOccupancyBuffer = r;
            Shader.SetGlobalTexture("_AVBOIT_VolumeBlockOccupancyBuffer", _volumeBlockOccupancyBuffer);
  
        }
        

        private static void CmdDispatchComputeKernel(CommandBuffer cmd,in ComputeKernelDesc desc) {
            int dispatchX = desc.threadDispatchSizes.Item1;
            int dispatchY =  desc.threadDispatchSizes.Item2;
            int dispatchZ =  desc.threadDispatchSizes.Item3;
            ComputeShader cs = desc.shader;
            int kernelId = desc.kernelId;
            cmd.DispatchCompute(cs,kernelId, dispatchX, dispatchY, dispatchZ);
        }
        
        public static void CmdDispatchVolumeTransmittanceIntegrationPass(CommandBuffer cmd) {
            CmdDispatchComputeKernel(cmd,_computeKernelConfigs.volumeTransmittanceIntegration);
        }
        
        public static void CmdDispatchBufferClearPass(CommandBuffer cmd) {

        }
        
        public static void CmdDispatchVolumeBlockLutBuildingPass(CommandBuffer cmd) {
            CmdDispatchComputeKernel(cmd,_computeKernelConfigs.volumeBlockLutBuilding);
        }
        
                
        public static void CmdDispatchVolumeBlockLutParallelBuildingPass(CommandBuffer cmd) {
            CmdDispatchComputeKernel(cmd,_computeKernelConfigs.volumeBlockLutParallelBuilding);
        }
        
        public void CmdDrawMeshVolumeOccupancyCollectionPass(CommandBuffer cmd, Mesh mesh) {
           cmd.DrawMesh(mesh,_modelMatrix, _material, 0,_graphicsPassId.volumeOccupancyCollectionId);
        }
        
        
        public void CmdDrawMeshSplattingPass(CommandBuffer cmd, Mesh mesh) {
            cmd.DrawMesh(mesh,_modelMatrix, _material, 0, _graphicsPassId.splattingPassId);
        }
        
        
        public void CmdDrawMeshTranslucencyForwardPrePass(CommandBuffer cmd, Mesh mesh) {
            cmd.DrawMesh(mesh,_modelMatrix, _material, 0, _graphicsPassId.translucencyForwardPrePassId);
        }
        
        public void CmdDrawMeshTranslucencyForwardPass(CommandBuffer cmd, Mesh mesh) {
            cmd.DrawMesh(mesh,_modelMatrix, _material, 0, _graphicsPassId.translucencyForwardPassId);
        }

        public void CmdDrawMeshTranslucencyForwardPass(CommandBuffer cmd,Mesh mesh, UnityEngine.Material material) {
            cmd.DrawMesh(mesh,_modelMatrix, material, 0);
        }

        public static void CmdResolveBackground(CommandBuffer cmd, RTHandle dest) {
            cmd.Blit(null,dest,_defaultMaterial,_backgroundResolutionPassId);
        }
        
    }

#endif
}
```
