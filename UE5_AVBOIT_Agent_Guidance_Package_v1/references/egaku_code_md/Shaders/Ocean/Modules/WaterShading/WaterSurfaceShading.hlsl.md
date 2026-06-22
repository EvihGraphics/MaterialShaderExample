# Egaku source: `Shaders/Ocean/Modules/WaterShading/WaterSurfaceShading.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef WATER_SURFACE_SHADING
#define WATER_SURFACE_SHADING

#include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Lighting.hlsl"

#include "WaterShadingData.hlsl"

#include "Foam/WaterSurfaceFoam.hlsl"

float4 AlphaBlend(float3 baseColorDst,float alphaDst,float3 baseColorSrc, float alphaSrc)
{
    float a = alphaSrc + alphaDst  - alphaDst * alphaSrc;
    float3 preAlphaDst = baseColorDst * alphaDst;
    return float4(lerp(preAlphaDst,baseColorSrc,alphaSrc)/max(a,0.0001), a);
}

WaterShadingData BlendFoamWaterShadingData(FoamShadingData foamShadingData, WaterShadingData waterShadingData)
{
    WaterShadingData result = waterShadingData;
    float4 color = AlphaBlend(waterShadingData.overlayColor,waterShadingData.overlayOpacity,
        foamShadingData.baseColor, foamShadingData.opacity);
    result.overlayOpacity = color.a;
    result.overlayColor = color.rgb;
    
    float3 waterNormalTS = WorldSpaceDefToTangentSpaceDef(waterShadingData.pixelNormalWS);
    float2 foamNormalTS = WorldSpaceDefToTangentSpaceDef(foamShadingData.normalWS);
    float foamNormalScale = 0.5 * foamShadingData.mask;
    float3 normalTS;
    normalTS.xy = foamNormalScale *  waterNormalTS.z * foamNormalTS.xy + waterNormalTS.xy;
    normalTS.z = waterNormalTS.z;
    result.pixelNormalWS = TangentSpaceDefToWorldSpaceDef(normalTS);
    result.specular = lerp(waterShadingData.specular,foamShadingData.specular,foamShadingData.mask);
    result.roughness = lerp(waterShadingData.roughness,foamShadingData.roughness,foamShadingData.mask);
    return result;
}

WaterShadingData CalcWaterSurfaceShadingData(WaterShadingContext shadingContext)
{
    WaterShadingData shadingData = CalcWaterShadingData(shadingContext);
    FoamShadingData foamShadingData = CalcFoamShadingData(shadingData,shadingContext);
    shadingData = BlendFoamWaterShadingData(foamShadingData, shadingData);
    return shadingData;
}



#endif
```
