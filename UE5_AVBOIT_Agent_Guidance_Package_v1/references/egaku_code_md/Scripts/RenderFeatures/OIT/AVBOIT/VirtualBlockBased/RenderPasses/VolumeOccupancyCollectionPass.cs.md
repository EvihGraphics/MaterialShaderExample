# Egaku source: `Scripts/RenderFeatures/OIT/AVBOIT/VirtualBlockBased/RenderPasses/VolumeOccupancyCollectionPass.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Modules.OIT.AVBOIT.Renderer.VirtualBlockBased;
using UnityEngine.Rendering;
using UnityEngine.Rendering.Universal;

namespace RenderFeatures.OIT.AVBOIT.VirtualBlockBased.RenderPasses {
    public class VolumeOccupancyCollectionPass : ScriptableRenderPass{
        
        private AVBOITRenderer _renderer;

        public VolumeOccupancyCollectionPass(AVBOITRenderer renderer) {
            _renderer = renderer;
        }
        
        public override void Execute(ScriptableRenderContext ctx, ref RenderingData data) {
            _renderer.RecordRenderCommandsVolumeOccupancyCollectionPass(ref ctx,ref data);
        }
    }
}
```
