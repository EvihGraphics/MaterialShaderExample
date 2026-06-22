# Egaku source: `Shaders/Ocean/Modules/WaterShading/Foam/WaterSurfaceFoam.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef WATER_SURFACE_FOAM_HLSL
#define WATER_SURFACE_FOAM_HLSL
#include "../WaterShadingData.hlsl"
#include "FoamMap.hlsl"

FoamShadingData CalcFoamShadingData(WaterShadingData waterShadingData,WaterShadingContext context)
{
    DeclareParamFetch(WaterSurfaceShadingDesc, waterSurfaceShadingDesc);

    float3 foamBaseColor = waterSurfaceShadingDesc.foamBaseColor;

    float4 foamDetailColor = waterSurfaceShadingDesc.foamDetailColor;
    
    FoamData foamData = SampleFoamMap(context.uv,context.velocity);
    float3 foamNormalWS = TangentSpaceDefToWorldSpaceDef(foamData.normal);
    float detailFoam = foamData.detailFoam;
    float softFoam = foamData.softFoam;

    // modulate foam color
    float foamNormalBlendFactor = 0.8;

    float3 foamWetColor = waterShadingData.foamWetColor;
    float softFoamIntensity = saturate(context.foamIntensity * waterSurfaceShadingDesc.softFoamScale);
    float3 softFoamBaseColor = lerp(foamWetColor, foamBaseColor, softFoamIntensity);
    float3 detailFoamBaseColor = foamBaseColor * lerp((GetVertical(foamNormalWS) + detailFoam) * foamDetailColor, 1, foamDetailColor.a);
    
    float softDetailFoamBlendFactor = saturate(saturate(detailFoam - 0.2) * foamNormalBlendFactor);
    float3 foamColor = lerp(softFoamBaseColor,detailFoamBaseColor,softDetailFoamBlendFactor);
    
    // modulate foam opacity
    float foamSoftBaseOpacity = 0.5;
    float foamSoftMaxOpacity = 1;
    float foamSoftVelocityScale = 1;
    
    float foamShallowOffset = 0.5;
    float foamShallowScale = 2;
    float fluidFoamShallowResult = context.foamIntensity * (detailFoam + foamShallowOffset) * foamShallowScale;
    
    float bottomFoamOpacity = min(length(context.velocity) * foamSoftVelocityScale + foamSoftBaseOpacity,foamSoftMaxOpacity);
    bottomFoamOpacity *= fluidFoamShallowResult;
    float topFoamOpacity = saturate(detailFoam - 0.4);
    float foamColorOpacity = context.foamIntensity * max(saturate(detailFoam - topFoamOpacity), softFoam * bottomFoamOpacity);
    
    FoamShadingData foamShadingData;
    foamShadingData.baseColor = foamColor;//*bottomFoamOpacity;
    foamShadingData.opacity = saturate(foamColorOpacity);
    foamShadingData.mask = context.foamIntensity;
    foamShadingData.normalWS = foamNormalWS;
    foamShadingData.roughness = 0.8;
    foamShadingData.specular = 0.1;
    return foamShadingData;
}

#endif
```
