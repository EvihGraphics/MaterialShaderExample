# Egaku source: `Common/RenderResource/RenderTargetManager.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using UnityEngine;
using UnityEngine.Rendering;

namespace Egaku.Rendering.Pipeline.Common {
    public class RenderTargetManager {

        public static RenderTargetHandle Alloc(RenderTexture  texture) {
            return new RenderTargetHandle(RTHandles.Alloc(texture));
        }

        public static RenderTargetHandle Alloc(int width, int height) {
            return new RenderTargetHandle(RTHandles.Alloc(width,height));
        }

    }
}
```
