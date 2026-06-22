# Egaku source: `Common/RenderPass/RenderPassManager.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System.Collections.Generic;

namespace Egaku.Rendering.Pipeline.Common {
    
    
    public class RenderPassManager {
        private struct PassNodeMeta {
            public RenderPassNode currentNode;
            public RenderPassNode[] referencingNodes;
            public RenderPassNode[] referencedNodes;
        }



        private Dictionary<RenderPassNode, PassNodeMeta> _passNodes = new();
        
        
        
        private class RenderGraph {
            
        }
        

    }
}
```
