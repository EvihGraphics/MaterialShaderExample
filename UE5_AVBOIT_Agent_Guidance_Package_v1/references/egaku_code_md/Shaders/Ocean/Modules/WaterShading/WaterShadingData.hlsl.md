# Egaku source: `Shaders/Ocean/Modules/WaterShading/WaterShadingData.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef WATER_SHADING_DATA_HLSL
#define WATER_SHADING_DATA_HLSL
#include "Ocean.ParamStruct.hlsl"
#include "../Utils.hlsl"
#include "WaterScattering.hlsl"
#include "WaterShadingHelperStructs.hlsl"



float3 ColorToAbsorptionCoeff(float3 color,float k)
{
    return rcp(color*k);
}

float3 ColorToScatteringCoeff(float3 color,float k)
{
    return color * k;
}




WaterShadingData CalcWaterShadingData(WaterShadingContext context)
{
    DeclareParamFetch(WaterVolumetricShadingDesc,volumeDesc);
    DeclareParamFetch(WaterSurfaceShadingDesc, surfaceDesc);
    //shadingDesc.absorptionFar = float4(0.10,0.250,0.50,100);
    //shadingDesc.absorptionFar = 1;
  //  shadingDesc.scatteringFar = float4(0.05,0.25,0.3,100);
    //shadingDesc.absorptionNear = float4(0);
    //shadingDesc.scatteringNear = float4();
   
    float blendFactor = 1.f - context.nearFarBlendFactor;
   // blendFactor = 1;
    float3 basicOverlayColor = lerp(surfaceDesc.overlayColorNear, surfaceDesc.overlayColorFar, blendFactor);
    float3 behindColorNear = volumeDesc.behindColorNear.rgb * volumeDesc.behindColorNear.a;
    float3 behindColorFar = volumeDesc.behindColorFar.rgb * volumeDesc.behindColorFar.a;
    float3 basicBehindColor = lerp(behindColorNear, behindColorFar, blendFactor);
    

    volumeDesc.absorptionFar.a = volumeDesc.absorptionFarDist;
    volumeDesc.absorptionNear.a = volumeDesc.absorptionNearDist;
    volumeDesc.scatteringFar.a = volumeDesc.scatteringFarDist;
    volumeDesc.scatteringNear.a = volumeDesc.scatteringNearDist;
    
    volumeDesc.absorptionFar.a *= volumeDesc.clarityFar;
    volumeDesc.scatteringFar.a /= volumeDesc.clarityFar;
    volumeDesc.absorptionNear.a *= volumeDesc.clarityNear;
    volumeDesc.scatteringNear.a /= volumeDesc.clarityNear;
    float3 absorptionFactorNear = ColorToAbsorptionCoeff(volumeDesc.absorptionNear.rgb,volumeDesc.absorptionNear.a);
    float3 absorptionFactorFar = ColorToAbsorptionCoeff(volumeDesc.absorptionFar.rgb,volumeDesc.absorptionFar.a);
    float3 scatteringFactorNear = ColorToScatteringCoeff(volumeDesc.scatteringNear.rgb,volumeDesc.scatteringNear.a);
    float3 scatteringFactorFar = ColorToScatteringCoeff(volumeDesc.scatteringFar.rgb,volumeDesc.scatteringFar.a);
    float3 basicAbsorptionFactor = lerp(absorptionFactorNear, absorptionFactorFar, blendFactor);
    float3 basicScatteringFactor = lerp(scatteringFactorNear, scatteringFactorFar, blendFactor*blendFactor);

    float3 foamScattering = basicScatteringFactor * volumeDesc.scatteringFoamScale * context.foamIntensity;
    float3 scattering = CalcScatteringFactor(context);
    float3 finalScattering = foamScattering + basicScatteringFactor * (scattering + 1); 
    float3 finalAbsorption = basicAbsorptionFactor / (1 + scattering);
    // Magic PhaseG

    float phaseGNear = 0;
    float phaseGFar = 0;
    float phaseGDeepSunHigh = 0.5f;
    float phaseGDeepSunLow = -0.7f;
    float phaseGShallowSunHigh = -0.2f;
    float phaseGShallowSunLow = 0.7f;
    float lightDirCos = GetVertical(context.mainLightDir);
    float phaseGDeepSun = lerp(phaseGDeepSunLow, phaseGDeepSunHigh, saturate(lightDirCos));
    float phaseGShallowSun = lerp(phaseGShallowSunLow, phaseGShallowSunHigh, saturate(lightDirCos));
    float phaseGBlendFactor = lerp(phaseGNear,phaseGFar, blendFactor*blendFactor);
    float phaseG = lerp(phaseGDeepSun, phaseGShallowSun, phaseGBlendFactor);
    
    WaterShadingData shadingData;
    shadingData.underwaterFog = basicOverlayColor * 0.35f;
    shadingData.surfaceLineColor = saturate(basicOverlayColor * 3.f);
    shadingData.foamWetColor = basicOverlayColor;
    shadingData.behindColor = basicBehindColor;
    shadingData.overlayColor = basicOverlayColor * 0.1f;
    shadingData.absorption = finalAbsorption;
    shadingData.scattering = finalScattering;
    shadingData.phaseG  = phaseG;
    shadingData.pixelNormalWS = context.pixelNormalWS;
    shadingData.overlayOpacity = 0.05;
    shadingData.roughness = 0.05f;
    shadingData.specular = 0.95;
    return shadingData;
}


#endif
```
