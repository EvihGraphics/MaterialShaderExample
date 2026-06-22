# Egaku source: `Common/RenderCamera.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using UnityEngine;
using UnityEngine.Rendering;

namespace Egaku.Rendering.Pipeline.Common {
    public class RenderCamera {
        private Camera _internalCamera;
        
        public Camera InternalCamera => _internalCamera;
        
        private RenderTargetIdentifier _backBuffer = new(BuiltinRenderTextureType.CameraTarget);
        
        public RenderTargetIdentifier BackBuffer => _backBuffer;
        
        public int PixelWidth => _internalCamera.pixelWidth;
        public int PixelHeight => _internalCamera.pixelHeight;
        
        public RenderCamera(Camera internalCamera) {
            _internalCamera = internalCamera;
        }

        public void TryGetCullingParameters(out ScriptableCullingParameters p) {
            _internalCamera.TryGetCullingParameters(out p);
        }
        
        public RenderTargetHandle GetBackBuffer() {
            return RenderTargetManager.Alloc(_internalCamera.activeTexture);
        }
        
        
    }
}
```
