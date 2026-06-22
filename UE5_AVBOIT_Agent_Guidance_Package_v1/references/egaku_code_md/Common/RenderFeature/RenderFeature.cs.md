# Egaku source: `Common/RenderFeature/RenderFeature.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System.Collections.Generic;

namespace Egaku.Rendering.Pipeline.Common {
    
    
    public abstract class RenderFeature {

        public virtual void SetupRenderPasses() {
            
        }

        public virtual IEnumerable<RenderPassNode> GetRenderPassNodes() {
            return null;
        }
        
        

    }
}
```
