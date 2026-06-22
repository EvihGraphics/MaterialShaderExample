# Egaku source: `Common/CodeGenerator/ComputeShaderCodeGenerator.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿namespace Egaku.Rendering.Pipeline.Common {
    
    
    public class ComputeShaderCodeGenerator  : HLSLCodeGenerator{
        public ComputeShaderCodeGenerator(string directory,string fileName):base(directory,fileName) {
        }
        
        public ComputeShaderCodeGenerator() {
        }
        

        public void EmitKernelDeclaration(string kernelSymbol) {
            EmitPragmaDirective($"kernel {kernelSymbol}");
        }
    }
}
```
