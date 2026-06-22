# Egaku source: `Scripts/Modules/Sky/Cloud/Volumetric/Renderer/RenderResources/DirectionOccupancyLut.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.Rendering.Universal;

namespace Modules.Sky.Cloud.Volumetric.Renderer.RenderResources {
    
    
    public struct DirectionOccupancyLut {
        private RTHandle _handle;
    
        public void Setup(int width,int height,int sliceCount) { 
            RenderTextureDescriptor desc = new RenderTextureDescriptor(width, height);
            desc.dimension = TextureDimension.Tex2D;
            desc.volumeDepth = 1;
            desc.depthBufferBits = 0;
            desc.colorFormat = RenderTextureFormat.ARGBFloat;
            desc.enableRandomWrite = true;
            
            RenderingUtils.ReAllocateIfNeeded(
                ref _handle,
                desc,
                FilterMode.Trilinear,
                TextureWrapMode.Clamp,
                name: $"VolumetricCloud - Direction Density Lut"
            );
        }
        
        public int GetUAVSlotIndex() {
            return 1;
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
