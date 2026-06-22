# Egaku source: `Scripts/RenderFeatures/OIT/AVBOIT/VirtualSliceBased/AVBOITRenderFeature.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Modules.OIT.AVBOIT.Renderer.VirtualSliceBased;
using RenderFeatures.OIT.AVBOIT.VirtualSliceBased.RenderPasses;
using UnityEngine;
using UnityEngine.Rendering.Universal;

namespace RenderFeatures.OIT.AVBOIT {
    
    [DisallowMultipleRendererFeature("AVBOIT - Virtual Slice Based")]
    public class AVBOITRenderFeature : ScriptableRendererFeature {
        
        public AVBOITRenderSettings settings;
        
        private SplattingPass _splattingPass;
        private VolumeTransmittanceIntegrationPass _volumeTransmittanceIntegrationPass;
        private TranslucencyForwardPass _translucencyForwardPass;
        private BufferClearPass _bufferClearPass;
        private VolumeOccupancyCollectionPass _volumeOccupancyCollectionPass;
        private DepthWarpLutBuildingPass _depthWarpLutBuildingPass;

        
        [ContextMenu("Reload Renderer")]
        public void ReloadRenderer() {
      //      AVBOITRenderer.ResetLoadState();
      //      AVBOITRenderer.Initialize();
        }
        
        public override void AddRenderPasses(ScriptableRenderer renderer, ref RenderingData renderingData) {
            /*AVBOITRenderer.CollectRenderInstances();
            if (AVBOITRenderer.Enabled) {
                AVBOITRenderer.SetFrustumFarPlane(renderingData.cameraData.camera.farClipPlane);
                AVBOITRenderer.SetupTranslucencyBuffer(renderingData.cameraData.camera.pixelWidth, renderingData.cameraData.camera.pixelHeight);
                renderer.EnqueuePass(_bufferClearPass);
                renderer.EnqueuePass(_volumeOccupancyCollectionPass);
                renderer.EnqueuePass(_depthWarpLutBuildingPass);
                renderer.EnqueuePass(_splattingPass);
                renderer.EnqueuePass(_volumeTransmittanceIntegrationPass);
                renderer.EnqueuePass(_translucencyForwardPass);
            }*/
            
                /*if (renderingData.cameraData.camera.cameraType == CameraType.SceneView || renderingData.cameraData.camera.cameraType == CameraType.Game) {
                    renderer.EnqueuePass(_evalPass);
                    renderer.EnqueuePass(_updatePass);
                    renderer.EnqueuePass(_surfacePass);
                }*/
        }

        public override void Create() {
            /*
            AVBOITRenderer.SetRenderSettings(settings);
            AVBOITRenderer.Initialize();
            */

            _splattingPass = new ();
            _volumeTransmittanceIntegrationPass = new ();
            _translucencyForwardPass = new ();
            _bufferClearPass = new ();
            _volumeOccupancyCollectionPass = new ();
            _depthWarpLutBuildingPass = new ();
            
            _bufferClearPass.renderPassEvent = RenderPassEvent.BeforeRendering;
            _volumeOccupancyCollectionPass.renderPassEvent = RenderPassEvent.BeforeRenderingPrePasses;
            _depthWarpLutBuildingPass.renderPassEvent = RenderPassEvent.BeforeRenderingPrePasses;
            _splattingPass.renderPassEvent = RenderPassEvent.AfterRenderingPrePasses;
            _volumeTransmittanceIntegrationPass.renderPassEvent = RenderPassEvent.AfterRenderingPrePasses;
            _translucencyForwardPass.renderPassEvent = RenderPassEvent.BeforeRenderingTransparents;
        }
    }
}
```
