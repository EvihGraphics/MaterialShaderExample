# Egaku source: `Scripts/Common/Rendering/CustomRenderer.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Common.ECS;

namespace Common.Rendering {
    
    
    
    public abstract class CustomRenderer : ComponentSystem {
        public abstract bool Valid { get; }
        public abstract bool Enabled { get; }
        
        public abstract bool Initialize();

        public abstract bool Destroy();

    }
    
    public abstract class CustomRenderer<T> : CustomRenderer where T : CustomRenderInstance {
        
        public abstract bool TryAddRenderInstance(T instance);
        
        public abstract bool TryRemoveRenderInstance(T instance);
        
    }
}
```
