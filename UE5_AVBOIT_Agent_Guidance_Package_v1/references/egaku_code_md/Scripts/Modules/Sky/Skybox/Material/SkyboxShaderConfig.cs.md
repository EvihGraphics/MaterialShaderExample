# Egaku source: `Scripts/Modules/Sky/Skybox/Material/SkyboxShaderConfig.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using Egaku.Rendering.Pipeline.Common;

namespace Modules.Sky.Skybox.Material {
    
    public class SkyboxShaderConfig  : ShaderConfig {
        public override Type ParamTableType => typeof(SkyboxShaderParamTable);
        public override string ShaderURL => "MyCustom/Sky/Skybox";
        public override string ShaderName => "Skybox";
        public override string ShaderFileDirectory => "Shaders/Sky/Skybox";

        public override (string, string)[] PublicGlobalMacros => new [] { ("UNIVERSAL_RENDER_PIPELINE", "") };

        public class SkyboxSubShaderConfig : SubShaderConfig {
            
            public class SkyboxPassConfig : ShaderPassConfig {
                public override string[] PipelineStates => new[] { "Cull Off", "ZWrite Off" };

                public override string[] IncludedFiles => new[]{"Skybox.hlsl"};

                public override string[] PragmaDirectives => new[] { "target 4.0"};
            }
            

            
            public override (string, string)[] Tags => new[] { ("RenderPipeline", "UniversalPipeline")};
            /*public override (string, string)[] Tags => new[] { ("RenderPipeline", "UniversalPipeline"), 
                ("RenderType", "Transparent"), ("Queue", "Transparent") ,("LightMode","UniversalForward")};*/
            
            public override ShaderPassConfig[] PassConfigs => new ShaderPassConfig[] {
                new SkyboxPassConfig(),
            };
        }
        
        public override SubShaderConfig[] SubShaderConfigs => new SubShaderConfig[] {new SkyboxSubShaderConfig()};
        
    }
}
```
