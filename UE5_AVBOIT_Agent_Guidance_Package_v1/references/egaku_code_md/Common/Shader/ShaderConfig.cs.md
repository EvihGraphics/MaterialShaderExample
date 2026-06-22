# Egaku source: `Common/Shader/ShaderConfig.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using Egaku.Rendering.Pipeline.Common.Shader;
using Egaku.Rendering.Pipeline.Utility;
using UnityEngine;
using UnityEngine.Rendering;

namespace Egaku.Rendering.Pipeline.Common {
    
    
    public abstract class ShaderConfig : ScriptableObject {

        public abstract Type ParamTableType { get; }

        public abstract string ShaderURL { get; }
        
        public abstract string ShaderName { get; }
        
        public abstract string ShaderFileDirectory { get; }
        
        public virtual string ParamTableFileName => $"{ShaderName}.ParamTable.hlsl";
        
        public virtual string ParamStructFileName => $"{ShaderName}.ParamStruct.hlsl";
        
        public virtual string ShaderLabFileName => $"{ShaderName}.shader";

        public virtual (string, string)[] PublicGlobalMacros { get => new (string, string)[]{}; }

        public abstract SubShaderConfig[] SubShaderConfigs { get; }
        

        public UnityEngine.Shader GetShader() {
            return UnityEngine.Shader.Find(ShaderName);
        }

        public ShaderParamTable GetParamTable() {
            return LangReflectionUtils.NewObjectInstance(ParamTableType) as ShaderParamTable;
        }
    }
    
    public abstract class SubShaderConfig   {
        
        public abstract (string,string)[] Tags { get; }

        public abstract ShaderPassConfig[] PassConfigs { get; }
        
    }

    public abstract class ShaderPassConfig {
        
        public virtual string PassName => null;
        
        public virtual string VSSymbol => "Vert";

        public virtual string PSSymbol => "Frag";
        
        public virtual (string,string)[] RenderStates => new (string,string)[]{};
        
        public abstract string[] IncludedFiles { get; }

        public virtual string[] PragmaDirectives => new string[] { };
        
        public virtual string[] PipelineStates => new string[] { };
    }

}
```
