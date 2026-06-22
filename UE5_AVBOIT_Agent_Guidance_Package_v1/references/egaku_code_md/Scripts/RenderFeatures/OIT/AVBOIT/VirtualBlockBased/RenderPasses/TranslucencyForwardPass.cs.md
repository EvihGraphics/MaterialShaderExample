# Egaku source: `Scripts/RenderFeatures/OIT/AVBOIT/VirtualBlockBased/RenderPasses/TranslucencyForwardPass.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Modules.OIT.AVBOIT.Renderer.VirtualBlockBased;
using UnityEngine.Rendering;
using UnityEngine.Rendering.Universal;

namespace RenderFeatures.OIT.AVBOIT.VirtualBlockBased.RenderPasses {
    public class TranslucencyForwardPass : ScriptableRenderPass{
        
        private AVBOITRenderer _renderer;

        public TranslucencyForwardPass(AVBOITRenderer renderer) {
            _renderer = renderer;
        }
        
        public override void OnCameraSetup(CommandBuffer cmd, ref RenderingData renderingData) {
            
        }

        public override void OnCameraCleanup(CommandBuffer cmd) {
           
        }
        
        public override void Execute(ScriptableRenderContext ctx, ref RenderingData data) {
            _renderer.RecordRenderCommandsTranslucencyForwardPass(ref ctx,ref data);
        }
    }
}
```
