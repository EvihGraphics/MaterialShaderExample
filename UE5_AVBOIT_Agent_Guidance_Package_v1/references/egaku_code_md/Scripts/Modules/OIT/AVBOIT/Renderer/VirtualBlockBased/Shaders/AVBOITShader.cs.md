# Egaku source: `Scripts/Modules/OIT/AVBOIT/Renderer/VirtualBlockBased/Shaders/AVBOITShader.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using Modules.OIT.AVBOIT.Material;
using ShaderResource;
using UnityEngine;
using UnityEngine.Rendering;


namespace Modules.OIT.AVBOIT.Renderer.VirtualBlockBased.Shaders {
    public struct AVBOITShader {
        private static ComputeShader _volumeTransmittanceIntegrationCS;
        private static ComputeShader _volumeBlockLutBuildingCS;
        private static AVBOITComputeShaderConfig.VirtualBlockBasedShaderConfig _computeShaderConfig = new();
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
            public ComputeKernelDesc volumeBlockLutParallelBuilding_Scan;
            public ComputeKernelDesc volumeBlockLutParallelBuilding_FinalSum;
            public ComputeKernelDesc volumeBlockLutParallelBuilding_AuxiliaryArrayInit;
            public ComputeKernelDesc protectionPlaneBuilding;
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
        private static GraphicsBuffer _volumeBlockLutParallelBuilding_AuxiliaryArray;
        private static UnityEngine.Material _defaultMaterial; 
        private UnityEngine.Material _material;
        
        public static void GlobalInit() {
            var csConfigVTI = _computeShaderConfig.GetVolumeTransmittanceIntegrationCSConfig();
            var csConfigVBLutBuild = _computeShaderConfig.GetVolumeBlockLutBuildingCSConfig();
   
            _volumeTransmittanceIntegrationCS = ComputeShaderResource.GetComputeShader(
                csConfigVTI.GetComputeShaderAssetURL());
            _volumeBlockLutBuildingCS = ComputeShaderResource.GetComputeShader(
                csConfigVBLutBuild.GetComputeShaderAssetURL());
            
            // register compute kernels

            _computeKernelConfigs.volumeTransmittanceIntegration.shader = _volumeTransmittanceIntegrationCS;
            _computeKernelConfigs.volumeTransmittanceIntegration.kernelId = _volumeTransmittanceIntegrationCS.FindKernel(csConfigVTI.GetVolumeTransmittanceIntegrationPassKernelName());
            uint x, y, z;
            _volumeTransmittanceIntegrationCS.GetKernelThreadGroupSizes(_computeKernelConfigs.volumeTransmittanceIntegration.kernelId,
                out x, out y, out z);
            _computeKernelConfigs.volumeTransmittanceIntegration.threadGroupSizes = (x, y, z);
            
            _computeKernelConfigs.volumeBlockLutBuilding.shader = _volumeBlockLutBuildingCS;
            _computeKernelConfigs.volumeBlockLutBuilding.kernelId = _volumeBlockLutBuildingCS.FindKernel(csConfigVBLutBuild.GetLutBuildingPassKernelName());
            _volumeBlockLutBuildingCS.GetKernelThreadGroupSizes(_computeKernelConfigs.volumeBlockLutBuilding.kernelId, out x, out y, out z);
            _computeKernelConfigs.volumeBlockLutBuilding.threadGroupSizes = (x, y, z);
            
            _computeKernelConfigs.protectionPlaneBuilding.shader = _volumeBlockLutBuildingCS;
            _computeKernelConfigs.protectionPlaneBuilding.kernelId = _volumeBlockLutBuildingCS.FindKernel(csConfigVBLutBuild.GetProtectionPlaneBuildingKernelName());
            _volumeBlockLutBuildingCS.GetKernelThreadGroupSizes(_computeKernelConfigs.protectionPlaneBuilding.kernelId, out x, out y, out z);
            _computeKernelConfigs.protectionPlaneBuilding.threadGroupSizes = (x, y, z);
            
            _computeKernelConfigs.volumeBlockLutParallelBuilding_Scan.shader = _volumeBlockLutBuildingCS;
            _computeKernelConfigs.volumeBlockLutParallelBuilding_Scan.kernelId = _volumeBlockLutBuildingCS.FindKernel(csConfigVBLutBuild.GetLutParallelBuilding_ScanPassKernelName());
            _volumeBlockLutBuildingCS.GetKernelThreadGroupSizes(_computeKernelConfigs.volumeBlockLutParallelBuilding_Scan.kernelId, out x, out y, out z);
            _computeKernelConfigs.volumeBlockLutParallelBuilding_Scan.threadGroupSizes = (x, y, z);
            
            _computeKernelConfigs.volumeBlockLutParallelBuilding_AuxiliaryArrayInit.shader = _volumeBlockLutBuildingCS;
            _computeKernelConfigs.volumeBlockLutParallelBuilding_AuxiliaryArrayInit.kernelId = _volumeBlockLutBuildingCS.FindKernel(csConfigVBLutBuild.GetLutParallelBuilding_AuxiliaryArrayInitPassKernelName());
            _volumeBlockLutBuildingCS.GetKernelThreadGroupSizes(_computeKernelConfigs.volumeBlockLutParallelBuilding_AuxiliaryArrayInit.kernelId, out x, out y, out z);
            _computeKernelConfigs.volumeBlockLutParallelBuilding_AuxiliaryArrayInit.threadGroupSizes = (x, y, z);
            _computeKernelConfigs.volumeBlockLutParallelBuilding_AuxiliaryArrayInit.threadDispatchSizes = (1, 1, 1);
            
            _computeKernelConfigs.volumeBlockLutParallelBuilding_FinalSum.shader = _volumeBlockLutBuildingCS;
            _computeKernelConfigs.volumeBlockLutParallelBuilding_FinalSum.kernelId = _volumeBlockLutBuildingCS.FindKernel(csConfigVBLutBuild.GetLutParallelBuilding_FinalSumPassKernelName());
            _volumeBlockLutBuildingCS.GetKernelThreadGroupSizes(_computeKernelConfigs.volumeBlockLutParallelBuilding_FinalSum.kernelId, out x, out y, out z);
            _computeKernelConfigs.volumeBlockLutParallelBuilding_FinalSum.threadGroupSizes = (x, y, z);
        }

        public void InitInnerMaterial() {
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
                _backgroundResolutionPassId = _defaultMaterial.FindPass(_graphicsShaderConfig.GetVirtualBlockBasedShaderConfig().GetBackgroundResolutionPassName());
            }
            
            _graphicsPassId.splattingPassId = _material.FindPass(_graphicsShaderConfig.GetVirtualBlockBasedShaderConfig().GetSplattingPassName());
            _graphicsPassId.translucencyForwardPrePassId = _material.FindPass(_graphicsShaderConfig.GetVirtualBlockBasedShaderConfig().GetTranslucencyForwardPrePassName());
            _graphicsPassId.translucencyForwardPassId = _material.FindPass(_graphicsShaderConfig.GetVirtualBlockBasedShaderConfig().GetTranslucencyForwardPassName());
            _graphicsPassId.volumeOccupancyCollectionId = _material.FindPass(_graphicsShaderConfig.GetVirtualBlockBasedShaderConfig().GetVolumeOccupancyCollectionPassName());
        }

        private static void SetFrustumSliceCurveFactor(int factor) {
            Shader.SetGlobalInt("_AVBOIT_FrustumSliceCurveFactor",factor);
        }

        public static void SetFrustumFarPlane(float farPlane) {
            Shader.SetGlobalFloat("_AVBOIT_FrustumFarPlane",farPlane);
        }

        public static void SetFrustumSliceCount(int count) {
            Shader.SetGlobalInt("_AVBOIT_FrustumSliceCount", count);
        }
        
        public static void SetPhysicalVolumeDimensions(int width,int height,int slice) {
            Shader.SetGlobalVector("_AVBOIT_PhysicalVolumeDimensions", new Vector3(width,height,slice));
        }
        
        public static void SetVolumeBlockDimensions(int width,int height,int slice) {
            Shader.SetGlobalVector("_AVBOIT_VolumeBlockDimensions", new Vector3(width,height,slice));
        }
        public static void SetVolumeBlockDivisor(int x,int y,int z) {
            Shader.SetGlobalVector("_AVBOIT_VolumeBlockDivisor", new Vector3(x,y,z));
        }
        
        public static void SetVirtualVolumeDimensions(int width,int height,int slice) {
            Shader.SetGlobalVector("_AVBOIT_VirtualVolumeDimensions", new Vector3(width,height,slice));
        }
        
        
        public bool IsValid() {
            return _material!= null;
        }
        
        private static (int,int,int) GetComputeKernelGroupDispatchConfig((uint,uint,uint) threadGroupSizes,(uint,uint,uint) targetDispatchedThreads) {
            int threadGroupsDispatchX = (int)Mathf.Ceil((float)targetDispatchedThreads.Item1 / (float)threadGroupSizes.Item1);
            int threadGroupsDispatchY = (int)Mathf.Ceil((float)targetDispatchedThreads.Item2 / (float)threadGroupSizes.Item2);
            int threadGroupsDispatchZ = (int)Mathf.Ceil((float)targetDispatchedThreads.Item3 / (float)threadGroupSizes.Item3);
            return (threadGroupsDispatchX, threadGroupsDispatchY, threadGroupsDispatchZ);
        }
        
        public static void SetVolumeTransmittanceIntegrationThreadDispatch(uint x, uint y, uint z) {
            _computeKernelConfigs.volumeTransmittanceIntegration.threadDispatchSizes =
                GetComputeKernelGroupDispatchConfig(_computeKernelConfigs.volumeTransmittanceIntegration.threadGroupSizes, (x, y, z));
        }
        
        public static void SetProtectionPlaneBuildingThreadDispatch(uint x, uint y, uint z) {
            _computeKernelConfigs.protectionPlaneBuilding.threadDispatchSizes =
                GetComputeKernelGroupDispatchConfig(_computeKernelConfigs.protectionPlaneBuilding.threadGroupSizes, (x, y, z));
        }
        
        public static void SetVolumeBlockLutThreadDispatch(uint x, uint y, uint z) {
            _computeKernelConfigs.volumeBlockLutBuilding.threadDispatchSizes =
                GetComputeKernelGroupDispatchConfig(_computeKernelConfigs.volumeBlockLutBuilding.threadGroupSizes, (x, y, z));
        }
        
        public static (uint,uint, uint) GetVolumeBlockLutParallelBuilding_ScanThreadGroupSize() {
            return _computeKernelConfigs.volumeBlockLutParallelBuilding_Scan.threadGroupSizes;
        }
        
        public static void SetVolumeBlockLutParallelBuilding_ScanThreadDispatch(uint x, uint y, uint z) {
            _computeKernelConfigs.volumeBlockLutParallelBuilding_Scan.threadDispatchSizes =
                GetComputeKernelGroupDispatchConfig(_computeKernelConfigs.volumeBlockLutParallelBuilding_Scan.threadGroupSizes, (x, y, z));
        }
        
        public static void SetVolumeBlockLutParallelBuilding_FinalSumThreadDispatch(uint x, uint y, uint z) {
            _computeKernelConfigs.volumeBlockLutParallelBuilding_FinalSum.threadDispatchSizes =
                GetComputeKernelGroupDispatchConfig(_computeKernelConfigs.volumeBlockLutParallelBuilding_FinalSum.threadGroupSizes, (x, y, z));
        }
        
        

        public static void SetVolumeTransmittanceLut(RTHandle r) {
            _volumeTransmittanceLut = r;
            Shader.SetGlobalTexture("_AVBOIT_VolumeTransmittanceLut",_volumeTransmittanceLut);
            int sliceCount = _volumeTransmittanceLut.rt.volumeDepth;
            SetFrustumSliceCurveFactor(2*sliceCount);
        }

        public static void SetVolumeExtinctionBuffer(RTHandle r) {
            _volumeExtinctionBuffer = r;
            Shader.SetGlobalTexture("_AVBOIT_VolumeExtinctionBuffer",_volumeExtinctionBuffer);
        }
        
        public static void SetVolumeBlockLutAuxiliaryArray(GraphicsBuffer buf) {
            _volumeBlockLutParallelBuilding_AuxiliaryArray = buf;
            Shader.SetGlobalBuffer("_AVBOIT_ParallelScanAuxiliaryArray",_volumeBlockLutParallelBuilding_AuxiliaryArray);
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
        

        
        public static void CmdDispatchVolumeBlockLutBuildingPass(CommandBuffer cmd) {
            CmdDispatchComputeKernel(cmd,_computeKernelConfigs.protectionPlaneBuilding);
            CmdDispatchComputeKernel(cmd,_computeKernelConfigs.volumeBlockLutBuilding);
        }
        
                
        public static void CmdDispatchVolumeBlockLutParallelBuildingPass(CommandBuffer cmd) {
            CmdDispatchComputeKernel(cmd,_computeKernelConfigs.protectionPlaneBuilding);
            CmdDispatchComputeKernel(cmd,_computeKernelConfigs.volumeBlockLutParallelBuilding_Scan);
            CmdDispatchComputeKernel(cmd,_computeKernelConfigs.volumeBlockLutParallelBuilding_AuxiliaryArrayInit);
            CmdDispatchComputeKernel(cmd,_computeKernelConfigs.volumeBlockLutParallelBuilding_FinalSum);
        }
        
        public void CmdDrawMeshVolumeOccupancyCollectionPass(CommandBuffer cmd, Mesh mesh,in Matrix4x4 modelMatrix) {
           cmd.DrawMesh(mesh,modelMatrix, _material, 0,_graphicsPassId.volumeOccupancyCollectionId);
        }


        
        public void CmdDrawMeshSplattingPass(CommandBuffer cmd, Mesh mesh,in Matrix4x4 modelMatrix) {
            cmd.DrawMesh(mesh,modelMatrix, _material, 0, _graphicsPassId.splattingPassId);
        }
        
        
        public void CmdDrawMeshTranslucencyForwardPrePass(CommandBuffer cmd, Mesh mesh,in Matrix4x4 modelMatrix) {
            cmd.DrawMesh(mesh,modelMatrix, _material, 0, _graphicsPassId.translucencyForwardPrePassId);
        }
        
        public void CmdDrawMeshTranslucencyForwardPass(CommandBuffer cmd, Mesh mesh,in Matrix4x4 modelMatrix) {
            cmd.DrawMesh(mesh,modelMatrix, _material, 0, _graphicsPassId.translucencyForwardPassId);
        }

        public void CmdDrawMeshTranslucencyForwardPass(CommandBuffer cmd,Mesh mesh,in Matrix4x4 modelMatrix, UnityEngine.Material material) {
            cmd.DrawMesh(mesh,modelMatrix, material, 0);
        }

        public static void CmdResolveBackground(CommandBuffer cmd, RTHandle dest) {
            cmd.Blit(null,dest,_defaultMaterial,_backgroundResolutionPassId);
        }
        
    }
}
```
