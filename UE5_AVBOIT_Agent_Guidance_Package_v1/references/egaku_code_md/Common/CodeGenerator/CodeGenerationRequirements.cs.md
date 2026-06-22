# Egaku source: `Common/CodeGenerator/CodeGenerationRequirements.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿namespace Egaku.Rendering.Pipeline.Common {
    public enum CodeGenerationRequirements {
        CSharpParamTable = 0x1,
        ShaderParamDefinition = 0x4,
        ShaderParamHelperStruct = 0x8,
        ShaderParamGetter = 0x10
    }
}
```
