# Egaku source: `Scripts/Modules/OIT/AVBOIT/Renderer/VirtualBlockBased/RenderResources/TemporalRT.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.Rendering.Universal;

namespace Modules.OIT.AVBOIT.Renderer.VirtualBlockBased.RenderResources {
    public struct TemporalRT {
        private RTHandle _handle;
    
        public void Setup(int width,int height) { 
            RenderTextureDescriptor desc = new RenderTextureDescriptor(width, height, RenderTextureFormat.ARGBFloat,0);
            desc.volumeDepth = 1;
            desc.enableRandomWrite = false;
            RenderingUtils.ReAllocateIfNeeded(
                ref _handle,
                desc,
                FilterMode.Bilinear,
                TextureWrapMode.Clamp,
                name: $"AVBOIT - Discarded Temporal Render Target"
            );
        }
        
        public RTHandle GetHandle() {
            return _handle;
        }
        
        public void Release() {
            if (_handle != null) {
                _handle.Release();
            }
        }
    }
}
```
