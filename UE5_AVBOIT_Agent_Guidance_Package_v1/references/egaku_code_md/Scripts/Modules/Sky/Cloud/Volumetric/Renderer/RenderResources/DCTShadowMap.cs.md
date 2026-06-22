# Egaku source: `Scripts/Modules/Sky/Cloud/Volumetric/Renderer/RenderResources/DCTShadowMap.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.Rendering.Universal;

namespace Modules.Sky.Cloud.Volumetric.Renderer.RenderResources {
    public struct DCTShadowMap {
        private RTHandle _handle;
        private RTHandle[] _sliceHandles;
        private RenderTargetIdentifier[] _sliceTargets;
        public void Setup(int width,int height,int sliceCount = 2) { 
            RenderTextureDescriptor desc = new RenderTextureDescriptor(width, height, RenderTextureFormat.ARGBFloat,0);
            desc.dimension = TextureDimension.Tex2D;
            desc.volumeDepth = 1;
            desc.enableRandomWrite = false;
            desc.bindMS = false;
            desc.useMipMap = true;
            desc.autoGenerateMips = true;
            _sliceHandles = new RTHandle[sliceCount];
            _sliceTargets = new RenderTargetIdentifier[sliceCount];
            for (int i = 0; i < sliceCount; i++) {
                RenderingUtils.ReAllocateIfNeeded(
                    ref _sliceHandles[i], desc, FilterMode.Bilinear, TextureWrapMode.Clamp,
                    name: $"VolumetricCloud - DCT Shadow Map {i}"
                );
                _sliceTargets[i] = _sliceHandles[i].rt.colorBuffer;
            }

            
        }
        
        public RTHandle GetHandle() {
            return _handle;
        }
        
        public RTHandle[] GetSliceHandles() {
            return _sliceHandles;
        }
        
        public RenderTargetIdentifier[] GetSliceTargets() {
            return _sliceTargets;
        }

        public int GetSliceCount() {
            return _handle.rt.volumeDepth;
        }
        
        public void Release() {
            if (_handle != null) {
                _handle.Release();
            }
        }
    }
}
```
