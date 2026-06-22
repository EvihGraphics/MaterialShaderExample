# Egaku source: `Common/RenderProcedure.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using UnityEngine.UIElements;

namespace Egaku.Rendering.Pipeline.Common {

    public class D {
        private Action t;
    }
    
    public delegate void RenderProcedure<TInput>();
    
    public interface IRenderProcedure {
        public void Execute<TInput>(TInput input);
    }
    
}
```
