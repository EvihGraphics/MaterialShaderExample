# Egaku source: `Shaders/Sky2/Cloud/Cloud.shader`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
Shader "Custom/Cloud"
{
    Properties{
        [HideInInspector]
        _MainTex("MainTex" , 2D) = "white"{}  // must be declared in properties scope
        
        [Header(Cloud Bounds Settings)]
        _CloudBounds_Min("Cloud Bounds Min", Vector) = (-100, -100, -100, 1)
        _CloudBounds_Max("Cloud Bounds Max", Vector) = (100, 100, 100, 1)

        [Header(Cloud Basic Settings)]
        [NoScaleOffset]
        _BasicCloud_DensityVolumeTex("Basic Cloud Density Volume Texture", 3D) = "" {}
        _BasicCloud_ValueOffset("Value Offset", Vector) = (0, 0, 0, 0)
        _BasicCloud_ValueScale("Value Scale", Vector) = (1, 1, 1, 1)
        _BasicCloud_CoordsScale("Coordinates Scale", Vector) = (1, 1, 1, 1)
        _BasicCloud_ChannelWeights("Channel Weights", Vector) = (1, 0, 0, 0)

        [Header(Cloud Detail Settings)]
        [NoScaleOffset]
        _DetailCloud_DensityVolumeTex("Detail Cloud Density Volume Texture", 3D) = "" {}
        _DetailCloud_ValueOffset("Value Offset", Vector) = (0, 0, 0, 0)
        _DetailCloud_ValueScale("Value Scale", Vector) = (1, 1, 1, 1)
        _DetailCloud_CoordsScale("Coordinates Scale", Vector) = (1, 1, 1, 1)
        _DetailCloud_ChannelWeights("Channel Weights", Vector) = (1, 0, 0, 0)
        
        [Header(Cloud Weather Map Settings)]
        [NoScaleOffset]
        _WeatherMap_Tex("Weather Map Texture", 2D) = "" {}
        _WeatherMap_ValueOffset("Value Offset", Vector) = (0, 0, 0, 0)
        _WeatherMap_ValueScale("Value Scale", Vector) = (1, 1, 1, 1)
        _WeatherMap_CoordsScale("Coordinates Scale", Vector) = (1, 1, 1, 1)
        _WeatherMap_ChannelWeights("Channel Weights", Vector) = (1, 0, 0, 0)

        
        [Header(Cloud Lighting Settings)]
        _Lighting_DensityScale("Cloud Density Scale",Float) = 1.0
        _Lighting_SigmaExt("Lighting Extinction Coefficient", Vector) = (0.1, 0.1, 0.1, 1)
        _Lighting_SigmaScatter("Lighting Scattering Coefficient", Vector) = (0.5, 0.5, 0.5, 1)
        _Lighting_PhaseG1("Hg Phase G1 Factor", Range(-1.0,1.0)) = 0.8
        _Lighting_PhaseG2("Hg Phase G2 Factor", Range(-1.0,1.0)) = -0.2
        _Lighting_AmbientLightColor("Ambient Light Color", Color) = (1,1,1)
        _Lighting_AmbientLightPhase("Ambient Light Phase", Range(0.0,1.0)) = 0.1
        
        [Header(Cloud Octave Multi Scattering Settings)]
        _Lighting_OctaveMultiScatter_SigmaExtFactor("Octave Extinction Factor", Range(0.0,1.0)) = 1.0
        _Lighting_OctaveMultiScatter_SigmaScatterFactor("Octave Scattering Factor", Range(0.0,1.0)) = 1.0
        _Lighting_OctaveMultiScatter_PhaseFactor("Octave Phase Factor", Range(0.0,1.0)) = 1.0
        _Lighting_OctaveMultiScatter_OctaveCount("Octave Count", Int) = 4
        
        [Header(Ray Marching Settings)]
        _RayMarching_ViewMarchStepSize("View March Step Size", Float) = 0.1
        _RayMarching_LightMarchStepCount("Light March Step Count", Float) = 16
    }

    HLSLINCLUDE
        #include "HLSLInclude/cloud_DeclareVars.hlsl"
    ENDHLSL


    SubShader{
        Tags {
            "RenderPipeline" = "UniversalRenderPipeline"
        }

        Pass{
            Name "Ray Marching Pass"

            Cull Off
            ZTest Always
            ZWrite Off

            HLSLPROGRAM
      //      #pragma enable_d3d11_debug_symbols
            #pragma vertex vert_screenspace_blit
            #pragma fragment frag
            #pragma target 3.0
            #include "HLSLInclude/cloud_RayMarchingPassShaderImpl.hlsl"
            ENDHLSL
        }

    }

}
```
