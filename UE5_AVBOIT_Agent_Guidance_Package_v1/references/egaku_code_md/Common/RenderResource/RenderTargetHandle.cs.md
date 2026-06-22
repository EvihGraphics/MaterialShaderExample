# Egaku source: `Common/RenderResource/RenderTargetHandle.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using UnityEngine;
using UnityEngine.Rendering;

namespace Egaku.Rendering.Pipeline.Common {
    
    
    public class RenderTargetHandle {
        private RTHandle _internalHandle;
        
        public RenderTargetHandle() {
          
        }
        public RenderTargetHandle(RTHandle internalHandle) {
            _internalHandle = internalHandle;
        }
        
        public static implicit operator RenderTexture(RenderTargetHandle  rtHandle) {
            return rtHandle._internalHandle.rt;
        }
        
        public static implicit operator RTHandle(RenderTargetHandle rtHandle) {
            return rtHandle._internalHandle;
        }
        
        public static implicit operator RenderTargetIdentifier(RenderTargetHandle rtHandle) {
            return rtHandle._internalHandle.nameID;
        }
    }
}
```
