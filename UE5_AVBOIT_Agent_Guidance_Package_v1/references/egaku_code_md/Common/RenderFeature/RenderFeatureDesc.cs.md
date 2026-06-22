# Egaku source: `Common/RenderFeature/RenderFeatureDesc.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System.Collections.Generic;

namespace Egaku.Rendering.Pipeline.Common {
    
    public class RenderFeatureDesc {

        private string _name;
        
        // It's declared as an array since we always get these desc from PipelineMetaUtils,
        // and other modules just use it, the add or remove method is unnecessary. 
        private RenderPassNodeDesc[] _passNodesDesc;
        
        public string Name {get => _name; set => _name = value; }
        
        public RenderPassNodeDesc[] PassNodesDesc {get => _passNodesDesc; set => _passNodesDesc = value; }
        
        
        public RenderFeatureDesc() { }
        
        
        
    }
    
    
}
```
