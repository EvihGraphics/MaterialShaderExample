# Egaku source: `Shaders/Sky2/Cloud/HLSLInclude/cloud_DeclareVars.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef CLOUD_DECLARE_VARS_HLSL
#define CLOUD_DECLARE_VARS_HLSL

Texture2D _MainTex;
SamplerState sampler_MainTex;

float _Lighting_DensityScale;
float3 _Lighting_SigmaExt;
float3 _Lighting_SigmaScatter;
float _Lighting_PhaseG1;
float _Lighting_PhaseG2;
float3 _Lighting_AmbientLightColor;
float _Lighting_AmbientLightPhase;
float _Lighting_OctaveMultiScatter_SigmaExtFactor;
float _Lighting_OctaveMultiScatter_SigmaScatterFactor;
float _Lighting_OctaveMultiScatter_PhaseFactor;
int _Lighting_OctaveMultiScatter_OctaveCount;

float _RayMarching_ViewMarchStepSize;
float _RayMarching_LightMarchStepCount;

float3 _CloudBounds_Max;
float3 _CloudBounds_Min;


Texture3D _BasicCloud_DensityVolumeTex;
SamplerState sampler_BasicCloud_DensityVolumeTex;
float4 _BasicCloud_ValueOffset;
float4 _BasicCloud_ValueScale;
float4 _BasicCloud_CoordsScale;
float4 _BasicCloud_ChannelWeights;

Texture3D _DetailCloud_DensityVolumeTex;
SamplerState sampler_DetailCloud_DensityVolumeTex;
float4 _DetailCloud_ValueOffset;
float4 _DetailCloud_ValueScale;
float4 _DetailCloud_CoordsScale;
float4 _DetailCloud_ChannelWeights;

Texture2D _WeatherMap_Tex;
SamplerState sampler_WeatherMap_Tex;
float4 _WeatherMap_ValueOffset;
float4 _WeatherMap_ValueScale;
float4 _WeatherMap_CoordsScale;
float4 _WeatherMap_ChannelWeights;


#endif
```
