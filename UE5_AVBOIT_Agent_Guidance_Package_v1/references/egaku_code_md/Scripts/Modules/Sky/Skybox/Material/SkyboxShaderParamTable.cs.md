# Egaku source: `Scripts/Modules/Sky/Skybox/Material/SkyboxShaderParamTable.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿



using Egaku.Rendering.Pipeline.Common;
using Egaku.Rendering.Pipeline.Common.Shader;

namespace Modules.Sky.Skybox.Material {
    public class SkyboxShaderParamTable : ShaderParamTable{
        [ShaderParam.Header(title = "Skybox Settings")]
        [ShaderParam.ConstantBuffer(name = "PerMaterial")]
        public struct SkyboxDesc {
            [ShaderParam.ParamProperty(label = "Sun Light Intensity")]
            public float sunLightIntensity;
        }
    }
}
```
