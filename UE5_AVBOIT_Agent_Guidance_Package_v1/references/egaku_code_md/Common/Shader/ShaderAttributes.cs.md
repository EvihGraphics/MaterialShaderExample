# Egaku source: `Common/Shader/ShaderAttributes.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;

namespace Egaku.Rendering.Pipeline.Common {
    
    [AttributeUsage(AttributeTargets.Class)]
    public class GenerateShaderParamTableAttribute : Attribute {
        public string directory;
        public string name;
    }

    [AttributeUsage(AttributeTargets.Class)]
    public class MaterialShaderAttribute : Attribute {
        public string shaderURL;
    }

    public class ShaderParamAttribute : Attribute {
        
    }
    
    
    public abstract class ShaderParam {

        [AttributeUsage((AttributeTargets.Struct | AttributeTargets.Field | AttributeTargets.Class))]
        public class ConstantBufferAttribute : ShaderParamAttribute {
            public string name = "UnityPerMaterial";
        }
        
        [AttributeUsage((AttributeTargets.Struct | AttributeTargets.Field))]
        public class InstancedAttribute : ShaderParamAttribute {
        }
        
        
        [AttributeUsage(AttributeTargets.Field | AttributeTargets.Struct)]
        public class ParamPropertyAttribute :  ShaderParamAttribute {
            public string label = "";
            public bool exposed = true;
        }
        
        [AttributeUsage(AttributeTargets.Field)]
        public class ToggleAttribute :  ShaderParamAttribute {
        }
        
        [AttributeUsage(AttributeTargets.Field)]
        public class RangeAttribute :  ShaderParamAttribute {
            public float min;
            public float max;
        }
        
        [AttributeUsage(AttributeTargets.Struct)]
        public class HeaderAttribute : ShaderParamAttribute {
            public string title;
        }
        [AttributeUsage(AttributeTargets.Field)]
        public class HDRAttribute : ShaderParamAttribute {
        }


    }
    

    
    
}
```
