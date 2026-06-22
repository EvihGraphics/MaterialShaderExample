# Egaku source: `Common/RenderPass/RenderPassNodeDesc.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using System.Collections.Generic;
using System.Reflection;

namespace Egaku.Rendering.Pipeline.Common {
    
    
    public class RenderPassNodeInputDesc {
        private string _name;
        private Type _type;
        private FieldInfo _fieldInfo;
        public string Name { get => _name; set => _name = value; }
        public Type ContentType { get => _type; set => _type = value; }
        public FieldInfo FieldInfo { get => _fieldInfo; set => _fieldInfo = value; }
    }


    public class RenderPassNodeOutputDesc {
        private string _name;
        private Type _type;
        private FieldInfo _fieldInfo;
        public string Name { get => _name; set => _name = value; }
        public Type ContentType { get => _type; set => _type = value; }
        public FieldInfo FieldInfo { get => _fieldInfo; set => _fieldInfo = value; }
    }
    
    public class RenderPassNodeDesc {
        private string _name;
        private Type _type;
        private RenderPassNodeInputDesc[] _inputDesc;
        private RenderPassNodeOutputDesc[] _outputDesc;
        public string Name { get => _name; set => _name = value;}
        public Type PassNodeType { get => _type; set => _type = value; }
        public RenderPassNodeInputDesc[] InputDesc { get => _inputDesc; set => _inputDesc = value; }
        public RenderPassNodeOutputDesc[] OutputDesc { get => _outputDesc; set => _outputDesc = value; }
    }
}
```
