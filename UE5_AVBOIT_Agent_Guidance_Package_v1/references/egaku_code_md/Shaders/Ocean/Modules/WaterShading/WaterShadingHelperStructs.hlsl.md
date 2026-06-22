# Egaku source: `Shaders/Ocean/Modules/WaterShading/WaterShadingHelperStructs.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef WATER_SHADING_HELPER_STRUCTS_HLSL
#define WATER_SHADING_HELPER_STRUCTS_HLSL


struct WaterShadingData
{
    float3 behindColor;
    float3 surfaceLineColor;
    float3 overlayColor;
    float3 underwaterFog;
    float3 absorption;
    float3 scattering;
    float3 foamWetColor;
    float3 pixelNormalWS;
    float overlayOpacity;
    float roughness;
    float phaseG;
    float specular;
};

struct FoamShadingData
{
    float3 baseColor;
    float3 normalWS;
    float specular;
    float roughness;
    float opacity;
    float mask;
};




struct WaterShadingContext
{
    float3 positionWS;
    float3 mainLightDir;
    float3 viewDir;
    float3 vertexNormalWS;
    float3 pixelNormalWS;
    float2 uv;
    float2 velocity;
    float nearFarBlendFactor;
    float foamIntensity;
};


#endif
```
