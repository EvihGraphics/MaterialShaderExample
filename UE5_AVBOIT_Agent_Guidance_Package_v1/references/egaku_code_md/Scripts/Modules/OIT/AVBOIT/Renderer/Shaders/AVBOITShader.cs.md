# Egaku source: `Scripts/Modules/OIT/AVBOIT/Renderer/Shaders/AVBOITShader.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿
using System;
using Modules.OIT.AVBOIT.Material;
using ShaderResource;
using UnityEngine;
using UnityEngine.Rendering;

namespace Modules.OIT.AVBOIT.Renderer.Shaders {
    
    #if false
    public struct AVBOITShader {
        private static ComputeShader _VEBClearCS;
        private static ComputeShader _volumeTransmittanceIntegrationCS;
        private static ComputeShader _depthWarpLutBuildingCS;
        private static AVBOITComputeShaderConfig _computeShaderConfig = new();
        private static AVBOITShaderConfig _graphicsShaderConfig = new();

        struct ComputeKernelDesc {
            public ComputeShader shader;
            public int kernelId;
            public (uint,uint,uint) threadGroupSizes;
            public (int,int,int) threadDispatchSizes;
        }

        struct ComputeKernelConfigs {
            public ComputeKernelDesc volumeTransmittanceIntegration;
            public ComputeKernelDesc volumeExtinctionBufferClear;
            public ComputeKernelDesc volumeOccupancyBufferClear;
            public ComputeKernelDesc depthWarpLutBuilding;
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
        private static RTHandle _depthWarpLut;
        private static RTHandle _volumeSliceOccupancyBuffer;
        private static UnityEngine.Material _defaultMaterial; 
        private UnityEngine.Material _material;
        private Mesh _mesh;
        private Matrix4x4 _modelMatrix;


        public static void Initialize() {
            var csConfigBufferClear = _computeShaderConfig.GetBufferClearCSConfig();
            var csConfigVTI = _computeShaderConfig.GetVolumeTransmittanceIntegrationCSConfig();
            var csConfigDWLB = _computeShaderConfig.GetDepthWarpLutBuildingCSConfig();
            _VEBClearCS = ComputeShaderResource.GetComputeShader(
                csConfigBufferClear.GetComputeShaderAssetURL());
            _volumeTransmittanceIntegrationCS = ComputeShaderResource.GetComputeShader(
                csConfigVTI.GetComputeShaderAssetURL());
            _depthWarpLutBuildingCS = ComputeShaderResource.GetComputeShader(
                csConfigDWLB.GetComputeShaderAssetURL());
            
            // register compute kernels

            _computeKernelConfigs.volumeTransmittanceIntegration.shader = _volumeTransmittanceIntegrationCS;
            _computeKernelConfigs.volumeTransmittanceIntegration.kernelId = _volumeTransmittanceIntegrationCS.FindKernel(csConfigVTI.GetVolumeTransmittanceIntegrationPassKernelName());
            uint x, y, z;
            _volumeTransmittanceIntegrationCS.GetKernelThreadGroupSizes(_computeKernelConfigs.volumeTransmittanceIntegration.kernelId,
                out x, out y, out z);
            _computeKernelConfigs.volumeTransmittanceIntegration.threadGroupSizes = (x, y, z);
            _computeKernelConfigs.volumeExtinctionBufferClear.shader = _VEBClearCS;
            _computeKernelConfigs.volumeExtinctionBufferClear.kernelId = _VEBClearCS.FindKernel(csConfigBufferClear.GetVolumeExtinctionBufferClearPassKernelName());
            _VEBClearCS.GetKernelThreadGroupSizes(_computeKernelConfigs.volumeExtinctionBufferClear.kernelId, out x, out y, out z);
            _computeKernelConfigs.volumeExtinctionBufferClear.threadGroupSizes = (x, y, z);
            
            _computeKernelConfigs.volumeOccupancyBufferClear.shader = _VEBClearCS;
            _computeKernelConfigs.volumeOccupancyBufferClear.kernelId = _VEBClearCS.FindKernel(csConfigBufferClear.GetVolumeOccupancyBufferClearPassKernelName());
            _VEBClearCS.GetKernelThreadGroupSizes(_computeKernelConfigs.volumeOccupancyBufferClear.kernelId, out x, out y, out z);
            _computeKernelConfigs.volumeOccupancyBufferClear.threadGroupSizes = (x, y, z);
            
            _computeKernelConfigs.depthWarpLutBuilding.shader = _depthWarpLutBuildingCS;
            _computeKernelConfigs.depthWarpLutBuilding.kernelId = _depthWarpLutBuildingCS.FindKernel(csConfigDWLB.GetLutBuildingPassKernelName());
            _depthWarpLutBuildingCS.GetKernelThreadGroupSizes(_computeKernelConfigs.depthWarpLutBuilding.kernelId, out x, out y, out z);
            _computeKernelConfigs.depthWarpLutBuilding.threadGroupSizes = (x, y, z);
            
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
        
        public void SetMesh(Mesh mesh) {
            _mesh = mesh;
        }

        public bool IsValid() {
            return _mesh != null && _material!= null;
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
            int height = _volumeExtinctionBuffer.rt.height;
            int width = _volumeExtinctionBuffer.rt.width;
            int depth = _volumeExtinctionBuffer.rt.volumeDepth;
            uint threadGroupSizeX = _computeKernelConfigs.volumeExtinctionBufferClear.threadGroupSizes.Item1;
            uint threadGroupSizeY = _computeKernelConfigs.volumeExtinctionBufferClear.threadGroupSizes.Item2;
            uint threadGroupSizeZ = _computeKernelConfigs.volumeExtinctionBufferClear.threadGroupSizes.Item3;
            int threadGroupsDispatchX = (int)Mathf.Ceil((float)width / (float)threadGroupSizeX);
            int threadGroupsDispatchY = (int)Mathf.Ceil((float)height / (float)threadGroupSizeY);
            int threadGroupsDispatchZ =  (int)Mathf.Ceil((float)depth / (float)threadGroupSizeZ);
            _computeKernelConfigs.volumeExtinctionBufferClear.threadDispatchSizes = (threadGroupsDispatchX,
                threadGroupsDispatchY, threadGroupsDispatchZ);
        }

        public static void SetDepthWarpLut(RTHandle r) {
            _depthWarpLut = r;
            int length = _depthWarpLut.rt.width;
            uint threadGroupSizeX = _computeKernelConfigs.depthWarpLutBuilding.threadGroupSizes.Item1;
            int dispatchX = (int)Mathf.Ceil((float)length / (float)threadGroupSizeX);
            Shader.SetGlobalTexture("_AVBOIT_DepthWarpLut", _depthWarpLut);
            _computeKernelConfigs.depthWarpLutBuilding.threadDispatchSizes = (Math.Max(1,dispatchX/2), 1, 1);
        }

        public static void SetVolumeSliceOccupancyBuffer(RTHandle r) {
            _volumeSliceOccupancyBuffer = r;
            Shader.SetGlobalTexture("_AVBOIT_VolumeSliceOccupancyBuffer", _volumeSliceOccupancyBuffer);
            _computeKernelConfigs.volumeOccupancyBufferClear.threadDispatchSizes = (1, 1, 1);
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
           CmdDispatchComputeKernel(cmd,_computeKernelConfigs.volumeExtinctionBufferClear);
           CmdDispatchComputeKernel(cmd,_computeKernelConfigs.volumeOccupancyBufferClear);
        }
        
        public static void CmdDispatchDepthWarpLutBuildingPass(CommandBuffer cmd) {
            CmdDispatchComputeKernel(cmd,_computeKernelConfigs.depthWarpLutBuilding);
        }
        
        public void CmdDrawMeshVolumeOccupancyCollectionPass(CommandBuffer cmd) {
           cmd.DrawMesh(_mesh,_modelMatrix, _material, 0,_graphicsPassId.volumeOccupancyCollectionId);
        }
        
        
        public void CmdDrawMeshSplattingPass(CommandBuffer cmd) {
            cmd.DrawMesh(_mesh,_modelMatrix, _material, 0, _graphicsPassId.splattingPassId);
        }
        
        
        public void CmdDrawMeshTranslucencyForwardPrePass(CommandBuffer cmd) {
            cmd.DrawMesh(_mesh,_modelMatrix, _material, 0, _graphicsPassId.translucencyForwardPrePassId);
        }
        
        public void CmdDrawMeshTranslucencyForwardPass(CommandBuffer cmd) {
            cmd.DrawMesh(_mesh,_modelMatrix, _material, 0, _graphicsPassId.translucencyForwardPassId);
        }

        public void CmdDrawMeshTranslucencyForwardPass(CommandBuffer cmd, UnityEngine.Material material) {
            cmd.DrawMesh(_mesh,_modelMatrix, material, 0);
        }

        public static void CmdResolveBackground(CommandBuffer cmd, RTHandle dest) {
            cmd.Blit(null,dest,_defaultMaterial,_backgroundResolutionPassId);
        }
        
        
    }
#endif
}
```
