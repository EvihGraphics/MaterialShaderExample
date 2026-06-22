# Egaku source: `Common/Material/MaterialPropertyTableAttribute.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;

namespace Egaku.Rendering.Pipeline.Common {
    
    [AttributeUsage(AttributeTargets.Class)]
    public class MaterialPropertyTableAttribute : Attribute {
        public CodeGenerationRequirements requiredCodeGeneration;
        
    }
}
```
