# Egaku source: `Scripts/Modules/Sky/Atmosphere/Material/AtmosphereShaderConfig.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using Egaku.Rendering.Pipeline.Common;

namespace Modules.Sky.Atmosphere.Material {
    public class AtmosphereShaderConfig :ShaderConfig{
        public override Type ParamTableType => typeof(AtmosphereShaderParamTable);
        public override string ShaderURL => "MyCustom/Sky/Atmosphere";
        public override string ShaderName => "Atmosphere";
        public override string ShaderFileDirectory => "Shaders/Sky/Atmosphere";

        public override (string, string)[] PublicGlobalMacros => new [] { ("UNIVERSAL_RENDER_PIPELINE", "") };

        public class AtmosphereSubShaderConfig : SubShaderConfig {
            
            public class AtmospherePrtPassConfig : ShaderPassConfig {
                public override string PassName => "AtmospherePrtPass";

                public override string[] PipelineStates => new[] { "Cull Off", "ZWrite Off" };

                public override string[] IncludedFiles => new[]{"Atmosphere.OctahedronPrtPass.hlsl"};

                public override string[] PragmaDirectives => new[] { "target 4.0"};
            }
            
            public class TransmittanceLutPassConfig : ShaderPassConfig {
                
                public override string PassName => "TransmittanceLutPass";
                public override string[] PipelineStates => new[] { "Cull Off", "ZWrite Off" };

                public override string[] IncludedFiles => new[]{"Atmosphere.TransmittanceLutPass.hlsl"};

                public override string[] PragmaDirectives => new[] { "target 4.0"};
            }
            
            public override (string, string)[] Tags => new[] { ("RenderPipeline", "UniversalPipeline")};
            /*public override (string, string)[] Tags => new[] { ("RenderPipeline", "UniversalPipeline"),
                ("RenderType", "Transparent"), ("Queue", "Transparent") ,("LightMode","UniversalForward")};*/
            
            public override ShaderPassConfig[] PassConfigs => new ShaderPassConfig[] {
                new AtmospherePrtPassConfig(),new TransmittanceLutPassConfig()
            };
        }
        public override SubShaderConfig[] SubShaderConfigs => new SubShaderConfig[] {new AtmosphereSubShaderConfig()};
        
        
        
    }
}
```
