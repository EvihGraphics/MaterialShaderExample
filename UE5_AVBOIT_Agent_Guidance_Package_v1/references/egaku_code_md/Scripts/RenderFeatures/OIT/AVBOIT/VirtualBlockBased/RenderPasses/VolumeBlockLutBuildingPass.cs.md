# Egaku source: `Scripts/RenderFeatures/OIT/AVBOIT/VirtualBlockBased/RenderPasses/VolumeBlockLutBuildingPass.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Modules.OIT.AVBOIT.Renderer.VirtualBlockBased;
using UnityEngine.Rendering;
using UnityEngine.Rendering.Universal;

namespace RenderFeatures.OIT.AVBOIT.VirtualBlockBased.RenderPasses {
    
    public class VolumeBlockLutBuildingPass : ScriptableRenderPass{

        private AVBOITRenderer _renderer;

        public VolumeBlockLutBuildingPass(AVBOITRenderer renderer) {
            _renderer = renderer;
        }
        
        public override void Execute(ScriptableRenderContext ctx, ref RenderingData data) {
            _renderer.RecordRenderCommandsVolumeBlockLutBuildingPass(ref ctx,ref data);
        }
    }
}
```
