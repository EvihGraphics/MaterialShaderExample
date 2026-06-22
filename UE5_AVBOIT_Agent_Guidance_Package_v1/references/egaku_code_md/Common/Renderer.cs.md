# Egaku source: `Common/Renderer.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;

namespace Egaku.Rendering.Pipeline.Common {
    public abstract class Renderer {
        
        
        public abstract void SetupPasses();
        
        public abstract void CreatePasses();
        
        public abstract void Init();

        public abstract void Reset();

        public abstract void Render();
    }
}
```
