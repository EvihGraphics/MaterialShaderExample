# Egaku source: `Scripts/Modules/Sky/Cloud/Volumetric/Material/VolumetricCloudShaderParamTable.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Egaku.Rendering.Pipeline.Common;
using Egaku.Rendering.Pipeline.Common.Shader;
using UnityEngine;


namespace Modules.Sky.Cloud.Volumetric.Material {
    
    public class VolumetricCloudShaderParamTable  : ShaderParamTable{
        
        [ShaderParam.Header(title = "Cloud Volume Settings")]
        [ShaderParam.ConstantBuffer( name = "PerMaterial")]
        public struct CloudVolumeDesc {
            [ShaderParam.ParamProperty(label = "Earth Radius")]
            public float earthRadius;

            [ShaderParam.ParamProperty(label = "Cloud Height Lower Bound")]
            public float heightLowerBound;
            
            [ShaderParam.ParamProperty(label = "Cloud Height Upper Bound")]
            public float heightUpperBound;

            [ShaderParam.ParamProperty(label = "Scattering Coefficient")]
            public float scatteringCoeff;

            /*
            [ShaderParam.ParamProperty(label = "Scattering Defined Depth")]
            public float scatteringDefinedDepth;*/

            [ShaderParam.ParamProperty(label = "Absorption Coefficient")]
            public float absorptionCoeff;
            
            /*[ShaderParam.ParamProperty(label = "Absorption Defined Depth")]
            public float absorptionDefinedDepth;*/
            
            [ShaderParam.Range(min = -1f, max = 1f)]
            [ShaderParam.ParamProperty(label = "Phase G1")]
            public float phaseG1;
            
            [ShaderParam.Range(min = -1f, max = 1f)]
            [ShaderParam.ParamProperty(label = "Phase G2")]
            public float phaseG2;

            [ShaderParam.Range(min = 0, max = 10f)]
            [ShaderParam.ParamProperty(label = "Density Scale")]
            public float densityScale;
            
            [ShaderParam.ParamProperty(label = "Cloud Ambient Lighting Ground Factor")]
            public float ambientLightGroundFactor;
            
            [ShaderParam.ParamProperty(label = "Cloud Ambient Lighting Sky Factor")]
            public float ambientLightSkyFactor;
            
            [ShaderParam.ParamProperty(label = "Cloud Main Lighting Factor")]
            public float mainLightFactor;


            [ShaderParam.ParamProperty(label = "Cloud Powder Effect Factor")]
            public float powderEffectFactor;
            
            [ShaderParam.ParamProperty(label = "View Zenith Gradient Start")]
            public float viewZenithGradientStart;
            
            [ShaderParam.ParamProperty(label = "View Zenith Gradient End")]
            public float viewZenithGradientEnd;
            
            [ShaderParam.ParamProperty(label = "Distant View Zenith Gradient Start")]
            public float distantViewZenithGradientStart;
            
            [ShaderParam.ParamProperty(label = "Distant View Zenith Gradient End")]
            public float distantViewZenithGradientEnd;
            
        }
        
        [ShaderParam.Header(title = "Volume Ray Marching Settings")]
        [ShaderParam.ConstantBuffer( name = "PerMaterial")]
        public struct VolumeRayMarchingDesc {
            [ShaderParam.ParamProperty(label = "View Direction Iteration")]
            public int viewDirectionIteration;
            
            [ShaderParam.ParamProperty(label = "View Direction Max Sample Count")]
            public int viewDirectionMaxSampleCount;
            
            [ShaderParam.ParamProperty(label = "Main Light Direction Iteration")]
            public int mainLightIteration;

            [ShaderParam.ParamProperty(label = "Main Light Direction Max Sample Count")]
            public int mainLightMaxSampleCount;

            [ShaderParam.Range(min = 0.0f, max = 0.8f)]
            [ShaderParam.ParamProperty(label = "Sample Point Jitter Range")]
            public float jitterRange;
            
      
        }
        
        
   
        
        [ShaderParam.Header(title = " Cloud Basic Density Settings")]
        [ShaderParam.ConstantBuffer( name = "PerMaterial")]
        public struct CloudBasicDensityDesc {
            [ShaderParam.ParamProperty(label = "Noise Texture")]
            public Texture3D noiseTex;
            
            [ShaderParam.ParamProperty(label = "Noise Channel Weights")]
            public Vector4 channelWeights;
            
            [ShaderParam.ParamProperty(label = "Density Noise Scale")]
            public Vector4 noiseScale;
            
            [ShaderParam.ParamProperty(label = "Density Noise Offset")]
            public Vector4 noiseOffset;

            [ShaderParam.ParamProperty(label = "Density Volume Scale")]
            public float volumeScale;

        }
        
        [ShaderParam.Header(title = "Cloud Detail Density Settings")]
        [ShaderParam.ConstantBuffer( name = "PerMaterial")]
        public struct CloudDetailDensityDesc {
            [ShaderParam.ParamProperty(label = "Noise Texture")]
            public Texture3D noiseTex;
            
            [ShaderParam.ParamProperty(label = "Noise Channel Weights")]
            public Vector4 channelWeights;
            
            [ShaderParam.ParamProperty(label = "Density Noise Scale")]
            public Vector4 noiseScale;
            
            [ShaderParam.ParamProperty(label = "Density Noise Offset")]
            public Vector4 noiseOffset;
            
            [ShaderParam.ParamProperty(label = "Density Volume Scale")]
            public float volumeScale;
        }

        
        [ShaderParam.Header(title = "Distant Cloud Basic Density Settings")]
        [ShaderParam.ConstantBuffer( name = "PerMaterial")]
        public struct DistantCloudBasicDensityDesc {
            [ShaderParam.ParamProperty(label = "Noise Texture")]
            public Texture3D noiseTex;
            
            [ShaderParam.ParamProperty(label = "Noise Channel Weights")]
            public Vector4 channelWeights;
            
            [ShaderParam.ParamProperty(label = "Density Noise Scale")]
            public Vector4 noiseScale;
            
            [ShaderParam.ParamProperty(label = "Density Noise Offset")]
            public Vector4 noiseOffset;

            [ShaderParam.ParamProperty(label = "Density Volume Scale")]
            public float volumeScale;
            
        }
        
        [ShaderParam.Header(title = "Distant Cloud Detail Density Settings")]
        [ShaderParam.ConstantBuffer( name = "PerMaterial")]
        public struct DistantCloudDetailDensityDesc {
            [ShaderParam.ParamProperty(label = "Noise Texture")]
            public Texture3D noiseTex;
            
            [ShaderParam.ParamProperty(label = "Noise Channel Weights")]
            public Vector4 channelWeights;
            
            [ShaderParam.ParamProperty(label = "Density Noise Scale")]
            public Vector4 noiseScale;
            
            [ShaderParam.ParamProperty(label = "Density Noise Offset")]
            public Vector4 noiseOffset;
            
            [ShaderParam.ParamProperty(label = "Density Volume Scale")]
            public float volumeScale;
        }
        
        [ShaderParam.Header(title = "Weather Map Settings")]
        [ShaderParam.ConstantBuffer( name = "PerMaterial")]
        public struct WeatherMapDesc {
            [ShaderParam.ParamProperty(label = "Texture")]
            public Texture2D weatherMap;
            
            [ShaderParam.ParamProperty(label = "Scale")]
            public Vector4 scale;
            
            [ShaderParam.ParamProperty(label = "Offset")]
            public Vector4 offset;
            
            [ShaderParam.ParamProperty(label = "Map Scale")]
            public float mapScale;
        }

        [ShaderParam.Header(title = "Wind Settings")]
        [ShaderParam.ConstantBuffer( name = "PerMaterial")]
        public struct WindDesc {
            [ShaderParam.ParamProperty(label = "Direction")]
            public Vector4 direction;
            
            [ShaderParam.ParamProperty(label = "Speed")]
            public float speed;
        }
        
        [ShaderParam.Header(title = "Cloud Octave Multi Scattering Approximation Settings")]
        [ShaderParam.ConstantBuffer( name = "PerMaterial")]
        public struct CloudOctaveMultiScatterApproxDesc {
            
            [ShaderParam.Range(min = 0, max = 1.0f)]
            [ShaderParam.ParamProperty(label = "Extinction Factor")]
            public float extinctionFactor;
            
            [ShaderParam.Range(min = 0, max = 1.0f)]
            [ShaderParam.ParamProperty(label = "Scattering Factor")]
            public float scatteringFactor;
            
            [ShaderParam.Range(min = 0, max = 1.0f)]
            [ShaderParam.ParamProperty(label = "Phase Factor")]
            public float phaseFactor;

            [ShaderParam.ParamProperty(label = "Approx Count")]
            public int approxCount;
        }
        
        
    }
}
```
