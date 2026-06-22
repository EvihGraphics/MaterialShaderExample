# Egaku source: `Scripts/RenderFeatures/OIT/AVBOIT/VirtualBlockBased/RenderPasses/SplattingPass.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Modules.OIT.AVBOIT.Renderer.VirtualBlockBased;
using UnityEngine.Rendering;
using UnityEngine.Rendering.Universal;

namespace RenderFeatures.OIT.AVBOIT.VirtualBlockBased.RenderPasses {
    
    public class SplattingPass : ScriptableRenderPass {
        private AVBOITRenderer _renderer;

        public SplattingPass(AVBOITRenderer renderer) {
            _renderer = renderer;
        }
        
        public override void OnCameraSetup(CommandBuffer cmd, ref RenderingData renderingData) {
            
        }

        public override void OnCameraCleanup(CommandBuffer cmd) {
           
        }
        
        public override void Execute(ScriptableRenderContext ctx, ref RenderingData data) {
            _renderer.RecordRenderCommandsSplattingPass(ref ctx,ref data);
        }
    }
}
```
