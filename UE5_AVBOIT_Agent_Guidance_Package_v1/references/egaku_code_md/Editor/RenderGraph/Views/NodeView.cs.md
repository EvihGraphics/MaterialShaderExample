# Egaku source: `Editor/RenderGraph/Views/NodeView.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System.Collections.Generic;
using UnityEditor.Experimental.GraphView;
using UnityEngine.UIElements;

namespace Egaku.Rendering.Pipeline.Editor {
    
    
    public class NodeView : Node{
        
        
        public IEnumerable<Port> GetPorts() {
            return mainContainer.Query<Port>().ToList();
        }

    }
}
```
