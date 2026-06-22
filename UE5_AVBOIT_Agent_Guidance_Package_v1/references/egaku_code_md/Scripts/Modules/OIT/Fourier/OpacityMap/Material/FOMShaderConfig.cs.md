# Egaku source: `Scripts/Modules/OIT/Fourier/OpacityMap/Material/FOMShaderConfig.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using Egaku.Rendering.Pipeline.Common;

namespace Modules.OIT.Fourier.OpacityMap.Material {

    public class FOMShaderConfig: ShaderConfig  {
        public override Type ParamTableType => typeof(FOMShaderParamTable);
        public override string ShaderURL => "MyCustom/OIT/Fourier/OpacityMap";
        public override string ShaderName => "FourierOpacityMap";
        public override string ShaderFileDirectory => "Shaders/OIT/Fourier/OpacityMap";

        public override (string, string)[] PublicGlobalMacros => new [] { ("UNIVERSAL_RENDER_PIPELINE", "") };
        
        
        public class OpacityMapShaderConfig : SubShaderConfig {
            
            public class FourierCoeffsBuildingPassConfig : ShaderPassConfig {
                public override string PassName => "FourierCoeffsBuilding";
                public override string[] PipelineStates => new[] { "Cull Off", "ZWrite Off","ZTest Off" ,"Blend One One" };

                public override string[] IncludedFiles => new[]{"Passes/FourierCoeffsBuildingPass.hlsl"};

                public override string[] PragmaDirectives => new[] { "target 5.0"};
            }
            
            public class TranslucencyForwardPassConfig : ShaderPassConfig {
                public override string PassName => "TranslucencyForward";
                public override string[] PipelineStates => new[] { "Cull Off", "ZWrite Off", "Blend One One,One Zero" };
                

                public override string[] IncludedFiles => new[]{"Passes/TranslucencyForwardPass.hlsl"};

                public override string[] PragmaDirectives => new[] { "target 5.0"};
            }
            
            public class BackgroundResolutionPassConfig : ShaderPassConfig {
                public override string PassName => "BackgroundResolution";
                public override string[] PipelineStates => new[] { "Cull Off", "ZWrite Off", "Blend One One,One Zero" };

                public override string[] IncludedFiles => new[]{"Passes/BackgroundResolutionPass.hlsl"};

                public override string[] PragmaDirectives => new[] { "target 5.0"};
            }
            public override (string, string)[] Tags => new[] { ("RenderPipeline", "UniversalPipeline")};

            public override ShaderPassConfig[] PassConfigs => new ShaderPassConfig[] {
                new FourierCoeffsBuildingPassConfig(),
                new TranslucencyForwardPassConfig(),
                new BackgroundResolutionPassConfig()
            };
            
            public string GetFourierCoeffsBuildingPassName() {
                return PassConfigs[0].PassName;
            }
        
            public string GetTranslucencyForwardPrePassName() {
                return PassConfigs[1].PassName;
            }
            
            public string GetBackgroundResolutionPassName() {
                return PassConfigs[2].PassName;
            }
        }
        
        
        public override SubShaderConfig[] SubShaderConfigs => new SubShaderConfig[] {new OpacityMapShaderConfig()};
        
        public OpacityMapShaderConfig GetOpacityMapShaderConfig() {
            return (OpacityMapShaderConfig)SubShaderConfigs[0];
        }
 
    }
}
```
