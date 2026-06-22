# Egaku source: `Scripts/RenderFeatures/Cloud/RenderPasses/VolumetricCloudOctahedronMarchingPass.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Modules.Sky.Cloud.Volumetric.Renderer;
using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.Rendering.Universal;


namespace RenderFeatures.Cloud.RenderPasses {
    public class VolumetricCloudOctahedronMarchingPass : ScriptableRenderPass {
        private VolumetricCloudRenderer _renderer;

        public void SetRenderer(VolumetricCloudRenderer renderer) {
            _renderer = renderer;
        }

        public override void OnCameraSetup(CommandBuffer cmd, ref RenderingData renderingData) {
            _renderer.SetRenderTargetHandles();
        }

        public override void OnCameraCleanup(CommandBuffer cmd) {
        
        }

        public override void Execute(ScriptableRenderContext ctx, ref RenderingData data) {
            if (_renderer != null && _renderer.Enabled) {
                _renderer.RecordRenderCommandsOctahedronMarchingPass(ref ctx, ref data);
                _renderer.RecordRenderCommandsFilterPass(ref ctx,ref data);
            }
        }
    }
}
```
