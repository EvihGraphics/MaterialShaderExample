# Egaku source: `Scripts/Modules/Sky/Atmosphere/Renderer/RenderTargets/AtmospherePrtRenderTarget.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.Rendering.Universal;

namespace Modules.Sky.Atmosphere.Renderer.RenderTargets {
    
    
    public struct AtmospherePrtRenderTarget {
        private RTHandle _handle;
        
        public void Setup(int width, int height) {
            RenderTextureDescriptor desc = new RenderTextureDescriptor(width, height);
            desc.depthBufferBits = 0;
            desc.colorFormat = RenderTextureFormat.ARGBFloat;
            RenderingUtils.ReAllocateIfNeeded(ref _handle, desc, FilterMode.Bilinear, TextureWrapMode.Clamp, name: "Atmosphere Prt");
        }
        
        public RTHandle GetRenderTarget() {
            return _handle;
        }
    }
}
```
