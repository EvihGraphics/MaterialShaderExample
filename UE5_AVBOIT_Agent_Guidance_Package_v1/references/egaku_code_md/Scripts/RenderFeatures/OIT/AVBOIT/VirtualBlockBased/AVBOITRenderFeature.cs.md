# Egaku source: `Scripts/RenderFeatures/OIT/AVBOIT/VirtualBlockBased/AVBOITRenderFeature.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Common.Rendering;
using Modules.OIT.AVBOIT.Renderer.VirtualBlockBased;
using RenderFeatures.OIT.AVBOIT.VirtualBlockBased.RenderPasses;
using UnityEngine;
using UnityEngine.Rendering.Universal;

namespace RenderFeatures.OIT.AVBOIT.VirtualBlockBased {
    
    [DisallowMultipleRendererFeature("AVBOIT - Virtual Block Based")]
    public class AVBOITRenderFeature : ScriptableRendererFeature {
        private SplattingPass _splattingPass;
        private VolumeTransmittanceIntegrationPass _volumeTransmittanceIntegrationPass;
        private TranslucencyForwardPass _translucencyForwardPass;
        private BufferClearPass _bufferClearPass;
        private VolumeOccupancyCollectionPass _volumeOccupancyCollectionPass;
        private VolumeBlockLutBuildingPass _volumeBlockLutBuildingPass;
        private AVBOITRenderer _renderer;
        
        public override void AddRenderPasses(ScriptableRenderer renderer, ref RenderingData renderingData) {
            if (_renderer!=null && _renderer.Enabled && _renderer.Valid) {
                _renderer.SetFrustumFarPlane(renderingData.cameraData.camera.farClipPlane);
                _renderer.SetupTranslucencyBuffer(renderingData.cameraData.camera.pixelWidth, renderingData.cameraData.camera.pixelHeight);
                renderer.EnqueuePass(_bufferClearPass);
                renderer.EnqueuePass(_volumeOccupancyCollectionPass);
                renderer.EnqueuePass(_volumeBlockLutBuildingPass);
                renderer.EnqueuePass(_splattingPass);
                renderer.EnqueuePass(_volumeTransmittanceIntegrationPass);
                renderer.EnqueuePass(_translucencyForwardPass);
            }
        }

        public override void Create() {
            _renderer = CustomRendererManager.TryGetCustomRenderer<AVBOITRenderer>();
            _splattingPass = new (_renderer);
            _volumeTransmittanceIntegrationPass = new (_renderer);
            _translucencyForwardPass = new (_renderer);
            _bufferClearPass = new (_renderer);
            _volumeOccupancyCollectionPass = new (_renderer);
            _volumeBlockLutBuildingPass = new (_renderer);
            _bufferClearPass.renderPassEvent = RenderPassEvent.BeforeRendering;
            _volumeOccupancyCollectionPass.renderPassEvent = RenderPassEvent.BeforeRenderingPrePasses;
            _volumeBlockLutBuildingPass.renderPassEvent = RenderPassEvent.BeforeRenderingPrePasses;
            _splattingPass.renderPassEvent = RenderPassEvent.AfterRenderingPrePasses;
            _volumeTransmittanceIntegrationPass.renderPassEvent = RenderPassEvent.AfterRenderingPrePasses;
            _translucencyForwardPass.renderPassEvent = RenderPassEvent.BeforeRenderingTransparents;
        }
    }
}
```
