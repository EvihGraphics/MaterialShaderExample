# Egaku source: `Scripts/Modules/Ocean/Material/OceanShaderConfig.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using Egaku.Rendering.Pipeline.Common;
using Egaku.Rendering.Pipeline.Common.Shader;
using Modules.Ocean.MaterialParamTable;

namespace Modules.Ocean.Material {
    
    
  //  [GenerateShaderParamTable(directory = "Shaders/Ocean",name = "Ocean")]
    public class OceanShaderConfig : ShaderConfig {
        public override Type ParamTableType => typeof(OceanShaderParamTable);
        public override string ShaderURL => "MyCustom/Ocean";
        public override string ShaderName => "Ocean";
        public override string ShaderFileDirectory => "Shaders/Ocean";

        public override (string, string)[] PublicGlobalMacros => new [] { ("UNIVERSAL_RENDER_PIPELINE", "") };

        public class OceanSubShaderConfig : SubShaderConfig {
            
            public class OceanForwardPassConfig : ShaderPassConfig {
                public override string[] IncludedFiles => new[]{"Ocean.hlsl"};
                
                public override string[] PipelineStates  => new[] { "Cull Off", "ZTest On","ZWrite On" };

                public override string[] PragmaDirectives => new[] { "multi_compile_instancing" , "target 4.0"};
            }
            
            public class OceanDepthPrepassConfig : ShaderPassConfig {
                public override string[] IncludedFiles => new[]{"Ocean.hlsl"};
                
                public override string[] PipelineStates  => new[] { "Cull Off", "ZTest ","ZWrite On" };

                public override string[] PragmaDirectives => new[] { "multi_compile_instancing" , "target 4.0"};
            }
            
            public override (string, string)[] Tags => new[] { ("RenderPipeline", "UniversalPipeline"), 
                ("RenderType", "Transparent"), ("Queue", "Transparent") ,("LightMode","UniversalForward")};
            
            public override ShaderPassConfig[] PassConfigs => new ShaderPassConfig[]{new OceanForwardPassConfig()};
        }
        
        public override SubShaderConfig[] SubShaderConfigs => new SubShaderConfig[] {new OceanSubShaderConfig()};
        
    }
    
    
}
```
