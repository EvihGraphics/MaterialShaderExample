# Egaku source: `Scripts/Modules/OIT/AVBOIT/Renderer/VirtualBlockBased/RenderResources/VolumeExtinctionBuffer.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.Rendering.Universal;

namespace Modules.OIT.AVBOIT.Renderer.VirtualBlockBased.RenderResources {
    public struct VolumeExtinctionBuffer {
        private RTHandle _handle;
        
        public int GetUAVSlotIndex() {
            return 2;
        }
    
        public void Setup(int width,int height,int sliceCount) { 
            RenderTextureDescriptor desc = new RenderTextureDescriptor(width, height);
            desc.dimension = TextureDimension.Tex3D;
            desc.volumeDepth = sliceCount;
            desc.depthBufferBits = 0;
            desc.colorFormat = RenderTextureFormat.RInt;
            desc.enableRandomWrite = true;
            RenderingUtils.ReAllocateIfNeeded(
                ref _handle,
                desc,
                FilterMode.Trilinear,
                TextureWrapMode.Clamp,
                name: $"AVBOIT - Volume Extinction Buffer"
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
