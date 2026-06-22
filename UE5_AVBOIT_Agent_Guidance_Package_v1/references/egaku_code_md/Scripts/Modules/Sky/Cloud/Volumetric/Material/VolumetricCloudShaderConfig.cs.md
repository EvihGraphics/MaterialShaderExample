# Egaku source: `Scripts/Modules/Sky/Cloud/Volumetric/Material/VolumetricCloudShaderConfig.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using Egaku.Rendering.Pipeline.Common;

namespace Modules.Sky.Cloud.Volumetric.Material {
 
    
    //  [GenerateShaderParamTable(directory = "Shaders/Ocean",name = "Ocean")]
    public class VolumetricCloudShaderConfig : ShaderConfig {
        public override Type ParamTableType => typeof(VolumetricCloudShaderParamTable);
        public override string ShaderURL => "MyCustom/Sky/Cloud/Volumetric";
        public override string ShaderName => "VolumetricCloud";
        public override string ShaderFileDirectory => "Shaders/Sky/Cloud/Volumetric";

        public override (string, string)[] PublicGlobalMacros => new [] { ("UNIVERSAL_RENDER_PIPELINE", "") };

        public class CloudSubShaderConfig : SubShaderConfig {
            
            public class CloudPostProcessPassConfig : ShaderPassConfig {
                public override string[] PipelineStates => new[] { "Cull Off", "ZWrite Off" };

                public override string[] IncludedFiles => new[]{"VolumetricCloud.ScreenSpace.hlsl"};

                public override string[] PragmaDirectives => new[] { "target 5.0"};
            }
            
            public class OctahedronMarchingPassConfig : ShaderPassConfig {
                public override string PassName => "OctahedronMarching";
                public override string[] PipelineStates => new[] { "Cull Off", "ZWrite Off", "ZTest Off"  };

                public override string[] IncludedFiles => new[]{"Passes/OctahedronRayMarchingPass.hlsl"};

                public override string[] PragmaDirectives => new[] { "target 5.0"};
            }
            
            public class TemporalFilterPassConfig : ShaderPassConfig {
                public override string PassName => "TemporalFilter";
                public override string[] PipelineStates => new[] { "Cull Off", "ZWrite Off", "ZTest Off" };

                public override string[] IncludedFiles => new[]{"Passes/FilterPass.hlsl"};

                public override string[] PragmaDirectives => new[] { "target 5.0"};
            }
            
            
            public class SunTransmittanceLutBuildingPassConfig : ShaderPassConfig {
                public override string PassName => "SunTransmittanceLutBuilding";
                public override string[] PipelineStates => new[] { "Cull Off", "ZWrite Off", "ZTest Off", "ColorMask 0" };

                public override string[] IncludedFiles => new[]{"Passes/SunTransmittanceLutBuildingPass.hlsl"};

                public override string[] PragmaDirectives => new[] { "target 5.0"};
            }
            
            public class VoxelMetaLutBuildingPassConfig : ShaderPassConfig {
                public override string PassName => "VoxelMetaLutBuilding";
                public override string[] PipelineStates => new[] { "Cull Off", "ZWrite Off", "ZTest Off", "ColorMask 0" };

                public override string[] IncludedFiles => new[]{"Passes/VoxelMetaLutBuildingPass.hlsl"};

                public override string[] PragmaDirectives => new[] { "target 5.0"};
            }
            
            public class DirectionOccupancyLutBuildingPassConfig : ShaderPassConfig {
                public override string PassName => "DirectionOccupancyLutBuilding";
                public override string[] PipelineStates => new[] { "Cull Off", "ZWrite Off", "ZTest Off" };

                public override string[] IncludedFiles => new[]{"Passes/DirectionOccupancyLutBuildingPass.hlsl"};

                public override string[] PragmaDirectives => new[] { "target 5.0"};
            }

            public class FourierShadowMapBuildingPassConfig : ShaderPassConfig {
                public override string PassName => "FourierShadowMapBuilding";
                public override string[] PipelineStates => new[] { "Cull Off", "ZWrite Off", "ZTest Off" };

                public override string[] IncludedFiles => new[]{"Passes/FourierShadowMapBuildingPass.hlsl"};

                public override string[] PragmaDirectives => new[] { "target 5.0"};
            }
            
            public class DCTShadowMapBuildingPassConfig : ShaderPassConfig {
                public override string PassName => "DCTShadowMapBuilding";
                public override string[] PipelineStates => new[] { "Cull Off", "ZWrite Off", "ZTest Off" };

                public override string[] IncludedFiles => new[]{"Passes/DCTShadowMapBuildingPass.hlsl"};

                public override string[] PragmaDirectives => new[] { "target 5.0"};
            }

            public override (string, string)[] Tags => new[] { ("RenderPipeline", "UniversalPipeline")};
            
            public override ShaderPassConfig[] PassConfigs => new ShaderPassConfig[] {
                new OctahedronMarchingPassConfig(),
                new TemporalFilterPassConfig(),
                new SunTransmittanceLutBuildingPassConfig(),
                new VoxelMetaLutBuildingPassConfig(),
                new DirectionOccupancyLutBuildingPassConfig(),
                new FourierShadowMapBuildingPassConfig(),
                new DCTShadowMapBuildingPassConfig()
            };
        }
        
        public override SubShaderConfig[] SubShaderConfigs => new SubShaderConfig[] {new CloudSubShaderConfig()};
        
    }

}
```
