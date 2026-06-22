# Egaku source: `Scripts/Modules/OIT/AVBOIT/Material/AVBOITShaderConfig.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using Egaku.Rendering.Pipeline.Common;

namespace Modules.OIT.AVBOIT.Material {
    
    
    public class AVBOITShaderConfig: ShaderConfig  {
        public override Type ParamTableType => typeof(AVBOITShaderParamTable);
        public override string ShaderURL => "MyCustom/OIT/AVBOIT/VirtualBlockBased";
        public override string ShaderName => "AVBOIT";
        public override string ShaderFileDirectory => "Shaders/OIT/AVBOIT/VirtualBlockBased";

        public override (string, string)[] PublicGlobalMacros => new [] { ("UNIVERSAL_RENDER_PIPELINE", "") };

        public class VirtualBlockBasedShaderConfig : SubShaderConfig {
            
            public class DebugVolumeBufferPassConfig : ShaderPassConfig {
                public override string PassName => "DebugVolumeBuffer";
                public override string[] PipelineStates => new[] { "Cull Off", "ZWrite Off" };

                public override string[] IncludedFiles => new[]{"Passes/DebugVolumeBufferPass.hlsl"};

                public override string[] PragmaDirectives => new[] { "target 5.0"};
                
            }
            
            public class SplattingPassConfig : ShaderPassConfig {
                public override string PassName => "Splatting";
                public override string[] PipelineStates => new[] { "Cull Back", "ZWrite Off","ColorMask 0" };

                public override string[] IncludedFiles => new[]{"Passes/SplattingPass.hlsl"};

                public override string[] PragmaDirectives => new[] { "target 5.0"};
            }
            
            public class TranslucencyForwardPrePassConfig : ShaderPassConfig {
                public override string PassName => "TranslucencyForwardPrePass";
                public override string[] PipelineStates => new[] { "Cull Back","ZWrite Off","Blend DstAlpha One,One Zero","ColorMask A"};

                public override string[] IncludedFiles => new[]{"Passes/TranslucencyForwardPass.hlsl"};

                public override string[] PragmaDirectives => new[] { "target 5.0"};
            }

            
            public class TranslucencyForwardPassConfig : ShaderPassConfig {
                public override string PassName => "TranslucencyForward";
                public override string[] PipelineStates => new[] { "Cull Back","ZWrite Off","Blend One One"};

                public override string[] IncludedFiles => new[]{"Passes/TranslucencyForwardPass.hlsl"};

                public override string[] PragmaDirectives => new[] { "target 5.0"};
            }
            
            public class BackgroundResolutionPassConfig : ShaderPassConfig {
                public override string PassName => "BackgroundResolution";
                public override string[] PipelineStates => new[] { "Cull Back","ZWrite Off","Blend One One, One Zero"};
                public override string[] IncludedFiles => new[]{"Passes/BackgroundResolutionPass.hlsl"};
                public override string[] PragmaDirectives => new[] { "target 5.0"};
            }
            
            public class VolumeOccupancyCollectionPassConfig : ShaderPassConfig {
                public override string PassName => "VolumeOccupancyCollection";
                public override string[] PipelineStates => new[] { "Cull Back","ZWrite Off","Conservative True","ColorMask 0"};
                public override string[] IncludedFiles => new[]{"Passes/VolumeOccupancyCollectionPass.hlsl"};
                public override string[] PragmaDirectives => new[] { "target 5.0"};
            }
            
            public override (string, string)[] Tags => new[] { ("RenderPipeline", "UniversalPipeline")};

            public override ShaderPassConfig[] PassConfigs => new ShaderPassConfig[] {
                new SplattingPassConfig(),
                new TranslucencyForwardPrePassConfig(),
                new TranslucencyForwardPassConfig(),
                new BackgroundResolutionPassConfig(),
                new VolumeOccupancyCollectionPassConfig()
            };
            
        
            public string GetSplattingPassName() {
                return PassConfigs[0].PassName;
            }
        
            public string GetTranslucencyForwardPrePassName() {
                return PassConfigs[1].PassName;
            }
        
            public string GetTranslucencyForwardPassName() {
                return PassConfigs[2].PassName;
            }
        
            public string GetBackgroundResolutionPassName() {
                return PassConfigs[3].PassName;
            }
        
            public string GetVolumeOccupancyCollectionPassName() {
                return PassConfigs[4].PassName;
            }
        }
        
        public override SubShaderConfig[] SubShaderConfigs => new SubShaderConfig[] {new VirtualBlockBasedShaderConfig()};

        public VirtualBlockBasedShaderConfig GetVirtualBlockBasedShaderConfig() {
            return (VirtualBlockBasedShaderConfig)SubShaderConfigs[0];
        }
        
        
    }
}
```
