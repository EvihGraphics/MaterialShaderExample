# Egaku source: `Common/RenderResource/RenderTargetDesc.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using UnityEngine;
using UnityEngine.Rendering;

namespace Egaku.Rendering.Pipeline.Common {
    public struct RenderTargetDesc {
        public RenderTexture color;
        public RenderTexture depth;
        public AttachmentDescriptor colorDesc;
        public AttachmentDescriptor depthDesc;
    }
    



}
```
