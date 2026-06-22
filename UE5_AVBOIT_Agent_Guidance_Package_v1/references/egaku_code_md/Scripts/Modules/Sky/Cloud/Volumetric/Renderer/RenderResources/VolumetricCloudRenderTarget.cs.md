# Egaku source: `Scripts/Modules/Sky/Cloud/Volumetric/Renderer/RenderResources/VolumetricCloudRenderTarget.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.Rendering.Universal;

namespace Modules.Sky.Cloud.Volumetric.Renderer.RenderResources{
        
    struct VolumetricCloudRenderTarget {
            
        private RTHandle[] _handles;
        private static readonly int _TemporalFilterRTCount = 2;
        private int _temporalFilterRTIndex;
        private int _count;

        public void Setup(int width,int height,bool enableUAV = false) {
            RenderTextureDescriptor desc = new RenderTextureDescriptor(width, height);
            _count = 4;
            _handles = new RTHandle[_count];
            _temporalFilterRTIndex = 0;
            desc.depthBufferBits = 0;
            desc.colorFormat = RenderTextureFormat.ARGBFloat;
            for (int i = 0; i < _count; i++) {
                RenderingUtils.ReAllocateIfNeeded(
                    ref _handles[i],
                    desc,
                    FilterMode.Bilinear,
                    TextureWrapMode.Clamp,
                    name: $"Volumetric Cloud RT {i}"
                );
            }
            desc.enableRandomWrite = enableUAV;
    
        }
        
        public void SwapTemporalFilterBuffer() {
            _temporalFilterRTIndex = 1 - _temporalFilterRTIndex;
        }
        public RTHandle GetTemporalFilterHistoryBuffer() {
            return _handles[_temporalFilterRTIndex];
        }
            
        public RTHandle GetTemporalFilterCurrentRT() {
            return _handles[1 - _temporalFilterRTIndex];
        }
     
        public RTHandle GetCloudShadingRT() {
            return _handles[2];
        }
        
        public RTHandle GetFinalResultRT() {
            return _handles[3];
        }
    }
}
```
