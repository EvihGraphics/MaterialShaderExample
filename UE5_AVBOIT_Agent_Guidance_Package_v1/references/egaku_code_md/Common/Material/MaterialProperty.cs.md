# Egaku source: `Common/Material/MaterialProperty.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using Egaku.Rendering.Pipeline.Common.Shader;

namespace Egaku.Rendering.Pipeline.Common {
    public class MaterialProperty {
        // indicates a attrib is packed into a Vector4,
        // and need to be retrieved from proper channel in shader codes. 
        [AttributeUsage(AttributeTargets.Field)]
        public class PackedIntoVector4Attribute: Attribute {
            public string packedParamName;
            public Vector4Channels channel;
        }


    }
}
```
