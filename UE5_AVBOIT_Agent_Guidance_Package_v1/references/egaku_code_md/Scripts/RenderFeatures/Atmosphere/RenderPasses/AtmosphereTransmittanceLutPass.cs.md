# Egaku source: `Scripts/RenderFeatures/Atmosphere/RenderPasses/AtmosphereTransmittanceLutPass.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿
using Modules.Sky.Atmosphere.Renderer;
using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.Rendering.Universal;


namespace RenderFeatures.Atmosphere.RenderPasses {
    public class AtmosphereTransmittanceLutPass : ScriptableRenderPass {
        private AtmosphereRenderer _renderer;

        public void SetRenderer(AtmosphereRenderer renderer) {
            _renderer = renderer;
        }

        public override void OnCameraSetup(CommandBuffer cmd, ref RenderingData renderingData) {

        }

        public override void OnCameraCleanup(CommandBuffer cmd) {
        
        }

        public override void Execute(ScriptableRenderContext ctx, ref RenderingData data) {
         //   if (_renderer != null && _renderer.Enabled) {
            _renderer.RecordRenderCommandsTransmittanceLutPass(ref ctx, ref data);
          //     _renderer.RecordRenderCommandsTransmittanceLutPass_RasterImpl(ref ctx, ref data);
         //   }
 
        }
    }
}
```
