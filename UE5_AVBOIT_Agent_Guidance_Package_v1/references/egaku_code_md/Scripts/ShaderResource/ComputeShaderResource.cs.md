# Egaku source: `Scripts/ShaderResource/ComputeShaderResource.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using UnityEngine;

namespace ShaderResource {
    
    
    public class ComputeShaderResource {
        private static ComputeShaderRegistry _Registry;
        
        private static ComputeShaderRegistry _GetRegistry() {
         //   if (_Registry == null) {
                _Registry = Resources.Load<ComputeShaderRegistry>("ComputeShaderRegistry");
          //  }
            return _Registry;
        }
        
        public static ComputeShader GetComputeShader(string path) {
            int tryCount = 10;
            ComputeShader shader = null;
            while (tryCount>0) {
                shader = _GetRegistry().GetEntry(path).shader;
                if (shader != null) {
                    break;
                }
                tryCount--;
            }
            return shader;
        }
        
        
    }
}
```
