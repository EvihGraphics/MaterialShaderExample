# Egaku source: `Scripts/Modules/OIT/AVBOIT/Renderer/VirtualSliceBased/RenderResources/VolumeSliceOccupancyBuffer.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.Rendering.Universal;

namespace Modules.OIT.AVBOIT.Renderer.VirtualSliceBased.RenderResources {
    public struct VolumeSliceOccupancyBuffer {
        private RTHandle _handle;

        public int GetUAVSlotIndex() {
            return 1;
        }
    
        public void Setup(int sliceCount) { 
            RenderTextureDescriptor desc = new RenderTextureDescriptor(sliceCount, 1);
            desc.dimension = TextureDimension.Tex2D;
            desc.volumeDepth = 1;
            desc.depthBufferBits = 0;
            desc.colorFormat = RenderTextureFormat.ARGBFloat;
            desc.enableRandomWrite = true;
            RenderingUtils.ReAllocateIfNeeded(
                ref _handle,
                desc,
                FilterMode.Bilinear,
                TextureWrapMode.Clamp,
                name: "AVBOIT - Volume Slice Occupancy Buffer"
            );
        }
        
        public RTHandle GetHandle() {
            return _handle;
        }
    }
}
```
