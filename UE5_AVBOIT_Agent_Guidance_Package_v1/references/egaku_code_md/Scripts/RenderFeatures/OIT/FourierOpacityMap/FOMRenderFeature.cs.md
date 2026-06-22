# Egaku source: `Scripts/RenderFeatures/OIT/FourierOpacityMap/FOMRenderFeature.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Common.Rendering;
using Modules.OIT.Fourier.OpacityMap.Renderer;
using RenderFeatures.OIT.FourierOpacityMap.RenderPasses;
using UnityEngine.Rendering.Universal;

namespace RenderFeatures.OIT.FourierOpacityMap {
    
    [DisallowMultipleRendererFeature("Fourier Opacity Map")]
    public class FOMRenderFeature : ScriptableRendererFeature {
        private FourierCoefficientsBuildingPass _fourierCoefficientsBuildingPass;
        private TranslucencyForwardPass _translucencyForwardPass;
        private FOMRenderer _renderer;
        
        public override void AddRenderPasses(ScriptableRenderer renderer, ref RenderingData renderingData) {
            if (_renderer!=null && _renderer.Enabled && _renderer.Valid) {
                _renderer.SetupTranslucencyBuffer(renderingData.cameraData.camera.pixelWidth, renderingData.cameraData.camera.pixelHeight);
                renderer.EnqueuePass(_fourierCoefficientsBuildingPass);
                renderer.EnqueuePass(_translucencyForwardPass);
            }
        }

        public override void Create() {
            _renderer = CustomRendererManager.TryGetCustomRenderer<FOMRenderer>();
            _fourierCoefficientsBuildingPass = new (_renderer);
            _translucencyForwardPass = new (_renderer);
            _fourierCoefficientsBuildingPass.renderPassEvent = RenderPassEvent.BeforeRenderingPrePasses;
            _translucencyForwardPass.renderPassEvent = RenderPassEvent.BeforeRenderingTransparents;
        }
    }
}
```
