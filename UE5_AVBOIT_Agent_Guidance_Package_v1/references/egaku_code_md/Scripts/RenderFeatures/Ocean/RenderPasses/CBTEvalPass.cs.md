# Egaku source: `Scripts/RenderFeatures/Ocean/RenderPasses/CBTEvalPass.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Modules.CBT.Renderer;
using UnityEngine.Rendering;
using UnityEngine.Rendering.Universal;

namespace RenderFeatures.Ocean.RenderPasses {
    public class CBTEvalPass : ScriptableRenderPass{
        
        private CBTMeshRenderer _renderer;
        
        public void SetRenderer(CBTMeshRenderer renderer) {
            _renderer = renderer;
        }
        
        public override void OnCameraSetup(CommandBuffer cmd, ref RenderingData renderingData) {
            
        }

        public override void OnCameraCleanup(CommandBuffer cmd) {
           
        }
        
        public override void Execute(ScriptableRenderContext ctx, ref RenderingData data) {
            if (_renderer != null && _renderer.Enabled) {
                _renderer.RecordCBTEvalComputeCommands(ref ctx, data.cameraData.camera);
            }
        }
        
    }
}
```
