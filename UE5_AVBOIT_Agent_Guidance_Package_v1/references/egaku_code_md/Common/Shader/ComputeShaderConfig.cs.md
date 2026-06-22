# Egaku source: `Common/Shader/ComputeShaderConfig.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using UnityEngine;

namespace Egaku.Rendering.Pipeline.Common.Shader {
    
    public abstract class ComputeShaderConfig : ScriptableObject{

        public abstract string FileURL { get; }
        
        public abstract string ComputeShaderName { get; }
        
        public abstract string FileDirectory { get; }
        
        public abstract ComputeShaderKernelConfig[] KernelConfigs { get; }
        
        public virtual string FileName  => $"{ComputeShaderName}.compute";
    }

    public abstract class ComputeShaderKernelConfig {
        public abstract string KernelSymbolName { get; }
        public abstract string[] IncludedFiles { get; }
     //   public abstract (uint,uint,uint) ThreadGroupSize { get; }
    }
}
```
