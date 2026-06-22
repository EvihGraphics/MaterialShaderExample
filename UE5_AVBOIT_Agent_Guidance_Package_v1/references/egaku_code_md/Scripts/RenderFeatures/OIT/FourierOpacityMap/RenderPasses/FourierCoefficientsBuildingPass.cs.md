# Egaku source: `Scripts/RenderFeatures/OIT/FourierOpacityMap/RenderPasses/FourierCoefficientsBuildingPass.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Modules.OIT.Fourier.OpacityMap.Renderer;
using UnityEngine.Rendering;
using UnityEngine.Rendering.Universal;

namespace RenderFeatures.OIT.FourierOpacityMap.RenderPasses {
    
    
    public class FourierCoefficientsBuildingPass  : ScriptableRenderPass{
        private FOMRenderer _renderer;
        
        public FourierCoefficientsBuildingPass(FOMRenderer renderer) {
            _renderer = renderer;
        }
        
        public override void OnCameraSetup(CommandBuffer cmd, ref RenderingData renderingData) {

        }

        public override void OnCameraCleanup(CommandBuffer cmd) {
           
        }
        
        public override void Execute(ScriptableRenderContext ctx, ref RenderingData data) {
            _renderer.RecordRenderCommandsFourierCoefficientsBuildingPass(ref ctx,ref data);
        }

    }
}
```
