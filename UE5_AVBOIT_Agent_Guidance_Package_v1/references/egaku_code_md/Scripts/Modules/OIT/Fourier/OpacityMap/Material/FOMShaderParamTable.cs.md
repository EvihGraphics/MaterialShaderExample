# Egaku source: `Scripts/Modules/OIT/Fourier/OpacityMap/Material/FOMShaderParamTable.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Egaku.Rendering.Pipeline.Common;
using UnityEngine;

namespace Modules.OIT.Fourier.OpacityMap.Material {
    

    public class FOMShaderParamTable {
   
        [ShaderParam.Header(title = "Translucency Settings")]
        [ShaderParam.ConstantBuffer(name = "PerMaterial")]
        public struct TranslucencyDesc {
            [ShaderParam.ParamProperty(label = "Base Color")]
            public Color baseColor;
        }
    }
}
```
