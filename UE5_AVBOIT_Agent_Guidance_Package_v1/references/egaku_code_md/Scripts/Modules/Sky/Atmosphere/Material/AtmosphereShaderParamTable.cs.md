# Egaku source: `Scripts/Modules/Sky/Atmosphere/Material/AtmosphereShaderParamTable.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Egaku.Rendering.Pipeline.Common;
using Egaku.Rendering.Pipeline.Common.Shader;
using UnityEngine;


namespace Modules.Sky.Atmosphere.Material {
    public class AtmosphereShaderParamTable : ShaderParamTable{

        [ShaderParam.Header(title = "Atmosphere Volume Settings")]
        [ShaderParam.ConstantBuffer(name = "PerMaterial")]
        public struct AtmosphereVolumeDesc {
            
            [ShaderParam.ParamProperty(label = "Atmosphere Density Scale")]
            public float atmosphereDensityScale;

            
            [ShaderParam.Range(min = 0,max = 1)]
            [ShaderParam.ParamProperty(label = "Dust Density Fraction")]
            public float dustDensityFraction;
            
            [ShaderParam.Range(min = 0,max = 1)]
            [ShaderParam.ParamProperty(label = "Air Density Fraction")]
            public float airDensityFraction;
            
            [ShaderParam.Range(min = 0,max = 1)]
            [ShaderParam.ParamProperty(label = "Ozone Density Fraction")]
            public float ozoneDensityFraction;
            
            [ShaderParam.Range(min = -0.999f,max = 0.999f)]
            [ShaderParam.ParamProperty(label = "Mie Scattering Anisotropy")]
            public float mieScatteringAnisotropy;

        }
        
    }
}
```
