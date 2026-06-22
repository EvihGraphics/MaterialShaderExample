# Egaku source: `Scripts/Common/Rendering/CustomRenderInstance.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Common.ECS;

namespace Common.Rendering {
    public abstract class CustomRenderInstance {
        public abstract bool Valid { get; }
        
        public abstract bool Enabled { get; }
    }
}
```
