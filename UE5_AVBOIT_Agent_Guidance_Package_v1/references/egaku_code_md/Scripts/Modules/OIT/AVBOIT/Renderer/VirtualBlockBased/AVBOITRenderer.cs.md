# Egaku source: `Scripts/Modules/OIT/AVBOIT/Renderer/VirtualBlockBased/AVBOITRenderer.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using System.Collections.Generic;
using Common.Rendering;
using Modules.OIT.AVBOIT.Renderer.VirtualBlockBased.RenderResources;
using Modules.OIT.AVBOIT.Renderer.VirtualBlockBased.Shaders;
using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.Rendering.Universal;

namespace Modules.OIT.AVBOIT.Renderer.VirtualBlockBased {
    
    public class AVBOITRenderer : CustomRenderer<AVBOITRenderInstance>{
        private readonly HashSet<AVBOITRenderInstance> _renderInstances = new ();
        
        private AVBOITRenderSettings _settings;
        private VolumeExtinctionBuffer _volumeExtinctionBuffer;
        private VolumeTransmittanceLut _volumeTransmittanceLut;
        private VolumeBlockOccupancyBuffer _volumeBlockOccupancyBuffer;
        private VolumeBlockLut _volumeBlockLut;
        private TranslucencyBuffer _translucencyBuffer;
        private ParallelScanAuxiliaryArray _parallelScanAuxiliaryArray;
        private TemporalRT _temporalRT;

        private bool _loaded = false;
        private bool _enabled = false;

        public override bool Enabled => _enabled;
        public override bool Valid => _loaded;
        
        public bool UseGPUBlending => _settings.useGPUBlending;
        
        
        public void SetEnabled() {
            _enabled = true;
        }

        public void SetDisabled() {
            _enabled = false;
        }
        
        public void ResetLoadState() {
            _loaded = false;
        }

        private void LoadFromSettings(in AVBOITRenderSettings settings) {
            AVBOITShader.GlobalInit();

            int voxelCntX = settings.virtualVolumeConfig.width;
            int voxelCntY = settings.virtualVolumeConfig.height;
            int voxelCntZ = settings.virtualVolumeConfig.sliceCount;

            int blockCntX = settings.volumeBlockConfig.widthDivisor; 
            int blockCntY = settings.volumeBlockConfig.heightDivisor;
            int blockCntZ = settings.volumeBlockConfig.sliceDivisor;

            int blockWidth = voxelCntX / blockCntX;
            int blockHeight = voxelCntY / blockCntY;
            int blockSlice = voxelCntZ / blockCntZ;

            int physicalBlockCnt = settings.physicalVolumeConfig.blockCount;
            int virtualBlockCnt = blockCntX * blockCntY * blockCntZ;
            _volumeExtinctionBuffer.Setup(blockWidth,blockHeight,blockSlice*physicalBlockCnt);
            _volumeTransmittanceLut.Setup(blockWidth,blockHeight,blockSlice*physicalBlockCnt);
            _volumeBlockOccupancyBuffer.Setup(blockCntX,blockCntY,blockCntZ);
            _volumeBlockLut.Setup(blockCntX,blockCntY,blockCntZ);


            uint parallelScanThreadGroupCount = AVBOITShader.GetVolumeBlockLutParallelBuilding_ScanThreadGroupSize().Item1;
            uint scanBlockSize = parallelScanThreadGroupCount * 2;
            uint scanBlockCount = (uint)(virtualBlockCnt + scanBlockSize - 1)/ scanBlockSize;
            _parallelScanAuxiliaryArray.Setup((int)scanBlockCount);
            AVBOITShader.SetVolumeBlockLutParallelBuilding_ScanThreadDispatch((uint)virtualBlockCnt/2,1,1);
            AVBOITShader.SetVolumeBlockLutParallelBuilding_FinalSumThreadDispatch((uint)virtualBlockCnt/2,1,1);
            AVBOITShader.SetVolumeBlockLutAuxiliaryArray(_parallelScanAuxiliaryArray.GetBuffer());
            AVBOITShader.SetPhysicalVolumeDimensions(blockWidth,blockHeight,blockSlice*physicalBlockCnt);
            AVBOITShader.SetVolumeBlockDimensions(blockWidth,blockHeight,blockSlice);
            AVBOITShader.SetVolumeBlockDivisor(blockCntX,blockCntY,blockCntZ);
            AVBOITShader.SetVirtualVolumeDimensions(voxelCntX,voxelCntY,voxelCntZ);
            AVBOITShader.SetFrustumSliceCount(voxelCntZ);
            AVBOITShader.SetVolumeExtinctionBuffer(_volumeExtinctionBuffer.GetHandle());
            AVBOITShader.SetVolumeTransmittanceLut(_volumeTransmittanceLut.GetHandle());
            AVBOITShader.SetVolumeBlockOccupancyBuffer(_volumeBlockOccupancyBuffer.GetHandle());
            AVBOITShader.SetVolumeBlockLut(_volumeBlockLut.GetHandle());
            AVBOITShader.SetVolumeTransmittanceIntegrationThreadDispatch((uint)voxelCntX, (uint)voxelCntY, 1);
            AVBOITShader.SetVolumeBlockLutThreadDispatch(1,1,1);
            AVBOITShader.SetProtectionPlaneBuildingThreadDispatch((uint)blockWidth,(uint)blockHeight,1);
        }

        private void LoadResources() {
            if (_loaded == false) {
                try {
                    LoadFromSettings(_settings);
                    _loaded = true;
                }catch (Exception e) {
                    Debug.LogError(e);
                    _enabled = false;
                    _loaded = false;
                }
            }
        }

        public void SetRenderSettings(in AVBOITRenderSettings settings) {
            _settings = settings;
        }
        

        public override bool TryAddRenderInstance(AVBOITRenderInstance instance) {
            return _renderInstances.Add(instance);
        }

        public override bool TryRemoveRenderInstance(AVBOITRenderInstance instance) {
            return _renderInstances.Remove(instance);
        }

        public override bool Initialize() {
            LoadResources();
            return _loaded;
        }
        
        
        public void SetFrustumFarPlane(float farPlane) {
            AVBOITShader.SetFrustumFarPlane(farPlane);
        }

        public void SetupTranslucencyBuffer(int width, int height) {
            _translucencyBuffer.Setup(width, height);
        }

        private void RecordRenderCommandsBufferClearPass(CommandBuffer cmd) {
            CoreUtils.SetRenderTarget(cmd, _volumeExtinctionBuffer.GetHandle(), ClearFlag.Color, Color.clear, 0, CubemapFace.Unknown, -1);
            CoreUtils.SetRenderTarget(cmd, _volumeBlockOccupancyBuffer.GetHandle(), ClearFlag.Color, Color.clear, 0, CubemapFace.Unknown, -1);
      //      CoreUtils.SetRenderTarget(cmd, _volumeTransmittanceLut.GetHandle(), ClearFlag.Color, Color.clear, 0, CubemapFace.Unknown, -1);
        }


        private void ReleaseResources() {
            _volumeExtinctionBuffer.Release();
            _volumeBlockOccupancyBuffer.Release();
            _volumeTransmittanceLut.Release();
            _volumeBlockLut.Release();
            _parallelScanAuxiliaryArray.Release();
            _temporalRT.Release();
            _translucencyBuffer.Release();
        }
        
        public override bool Destroy() {
            ReleaseResources();
            return true;
        }


        public void RecordRenderCommandsVolumeTransmittanceIntegrationPass(CommandBuffer cmd) {
            AVBOITShader.CmdDispatchVolumeTransmittanceIntegrationPass(cmd);
        }

        /*private void RecordRenderCommandsTranslucencyForwardPass(CommandBuffer cmd, AVBOITRenderInstance instance) {
            if (UseGPUBlending) {
                instance.RecordRenderCommandsTranslucencyForwardPassWithGPUBlending(cmd);
            } else {
                instance.RecordRenderCommandsTranslucencyForwardPass(cmd);
            }
        }
        private void RecordRenderCommandsTranslucencyForwardPass(CommandBuffer cmd) {
            /*if (UseGPUBlending) {
                _shader.CmdDrawMeshTranslucencyForwardPrePass(cmd, _mesh, _transform.localToWorldMatrix);
                _shader.CmdDrawMeshTranslucencyForwardPass(cmd,_mesh, _transform.localToWorldMatrix,_material);
            } else {
                _shader.CmdDrawMeshTranslucencyForwardPass(cmd, _mesh, _transform.localToWorldMatrix);
            }#1#
        }*/
        
     
        
        public void RecordRenderCommandsBufferClearPass(ref ScriptableRenderContext context, ref RenderingData data) {
            CommandBuffer cmd = CommandBufferPool.Get("AVBOIT - Buffer Clear Pass");
            RecordRenderCommandsBufferClearPass(cmd);
            context.ExecuteCommandBuffer(cmd);
            CommandBufferPool.Release(cmd);
        }
        
        public void RecordRenderCommandsDebugVolumeBufferPass(ref ScriptableRenderContext context, ref RenderingData data) {
            CommandBuffer cmd = CommandBufferPool.Get("AVBOIT - Debug Volume Buffer Pass");
            context.ExecuteCommandBuffer(cmd);
            CommandBufferPool.Release(cmd);
        }
        
        public void RecordRenderCommandsVolumeOccupancyCollectionPass(ref ScriptableRenderContext context, ref RenderingData data) {
            CommandBuffer cmd = CommandBufferPool.Get("AVBOIT - Volume Occupancy Collection Pass");

            int blockCntX = _settings.virtualVolumeConfig.width / 8;
            int blockCntY = _settings.virtualVolumeConfig.height / 8;
            _temporalRT.Setup(blockCntX,blockCntY);
            cmd.SetGlobalVector("_AVBOIT_OccupancyCollectionResolutions", new Vector4(blockCntX,blockCntY,1,1));
            cmd.SetRenderTarget(_temporalRT.GetHandle());
            cmd.SetRandomWriteTarget(_volumeBlockOccupancyBuffer.GetUAVSlotIndex(),_volumeBlockOccupancyBuffer.GetHandle());
            
            foreach (var rendererInstance in _renderInstances) {
                if (rendererInstance.Enabled && rendererInstance.Valid) {
                    rendererInstance.RecordRenderCommandsVolumeOccupancyCollectionPass(cmd);
                }
            }
            cmd.ClearRandomWriteTargets();
            context.ExecuteCommandBuffer(cmd);
            CommandBufferPool.Release(cmd);
        }
        
        public void RecordRenderCommandsVolumeBlockLutBuildingPass(ref ScriptableRenderContext context, ref RenderingData data) {
            CommandBuffer cmd = CommandBufferPool.Get("AVBOIT - Volume Block Lut Building Pass");
           // AVBOITShader.CmdDispatchVolumeBlockLutBuildingPass(cmd);
            AVBOITShader.CmdDispatchVolumeBlockLutParallelBuildingPass(cmd);
            context.ExecuteCommandBuffer(cmd);
            CommandBufferPool.Release(cmd);
        }
        
        
        public void RecordRenderCommandsSplattingPass(ref ScriptableRenderContext context, ref RenderingData data) {
            CommandBuffer cmd = CommandBufferPool.Get("AVBOIT - Splatting Pass");
            RecordRenderCommandsBufferClearPass(cmd);
            _temporalRT.Setup(_settings.virtualVolumeConfig.width, _settings.virtualVolumeConfig.height);
            
            cmd.SetRenderTarget(_temporalRT.GetHandle());
            cmd.SetRandomWriteTarget(_volumeExtinctionBuffer.GetUAVSlotIndex(),_volumeExtinctionBuffer.GetHandle());
            foreach (var rendererInstance in _renderInstances) {
                if (rendererInstance.Enabled && rendererInstance.Valid) {
                    rendererInstance.RecordRenderCommandsSplattingPass(cmd);
                }
            }
            cmd.ClearRandomWriteTargets();
            context.ExecuteCommandBuffer(cmd);
            CommandBufferPool.Release(cmd);
        }
        
        public void RecordRenderCommandsVolumeTransmittanceIntegrationPass(ref ScriptableRenderContext context, ref RenderingData data) {
            CommandBuffer cmd = CommandBufferPool.Get("AVBOIT - Volume Transmittance Integration Pass");
            RecordRenderCommandsVolumeTransmittanceIntegrationPass(cmd);
            context.ExecuteCommandBuffer(cmd);
            CommandBufferPool.Release(cmd);
        }
        
        public void RecordRenderCommandsTranslucencyForwardPass(ref ScriptableRenderContext context, ref RenderingData data) {
            CommandBuffer cmd = CommandBufferPool.Get("AVBOIT - Translucency Forward Pass");
            cmd.SetRenderTarget(_translucencyBuffer.GetHandle());
            cmd.ClearRenderTarget(false,true,Color.clear);
            if (UseGPUBlending) {
                foreach (var rendererInstance in _renderInstances) {
                    if (rendererInstance.Enabled && rendererInstance.Valid) {
                        rendererInstance.RecordRenderCommandsTranslucencyForwardPassWithGPUBlending(cmd);
                    }
                }
            } else {
                foreach (var rendererInstance in _renderInstances) {
                    if (rendererInstance.Enabled && rendererInstance.Valid) {
                        rendererInstance.RecordRenderCommandsTranslucencyForwardPass(cmd);
                    }
                }
            }
    
            RTHandle rt = data.cameraData.renderer.cameraColorTargetHandle;
            AVBOITShader.CmdResolveBackground(cmd,_translucencyBuffer.GetHandle());
            cmd.Blit(_translucencyBuffer.GetHandle(),rt);
            context.ExecuteCommandBuffer(cmd);
            CommandBufferPool.Release(cmd);
        }
        
    }
}
```
