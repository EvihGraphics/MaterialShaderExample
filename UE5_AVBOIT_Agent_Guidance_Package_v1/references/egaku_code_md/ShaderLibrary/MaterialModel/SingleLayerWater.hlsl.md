# Egaku source: `ShaderLibrary/MaterialModel/SingleLayerWater.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef SINGLE_LAYER_WATER_HLSL
#define SINGLE_LAYER_WATER_HLSL
#include "../Lighting/Volumetric.hlsl"
#include "Packages/com.unity.render-pipelines.core/ShaderLibrary/BSDF.hlsl"

// Single layer water from Unreal Engine


struct SingleLayerWaterDesc
{
    float3 scatterCoeff;
    float3 absorptionCoeff;
    float3 mainLightDirection;
    float3 mainLightIntensity;
    float3 environLightIntensity;
    float3 viewDirection;
    float3 behindWaterSceneLuminance;
    float waterVisibility;
    float phaseG;
    float underwaterRayDistance;
};

struct SingleLayerWaterOutput
{
    float3 luminance;
    float3 waterVolumeTransmittance;
};




// these directions all points to their source
float GetDirectionalLightPhase(float phaseG,float3 lightDirection,float3 scatterDirection)
{
    // directions must be normalized.
    return SchlickPhase(phaseG,dot(-lightDirection,scatterDirection));
}

float GetEnvironLightPhase()
{
    return IsotropicPhase();
}

float3 GetMediumScatteredLuminance(SingleLayerWaterDesc desc)
{
    float dirPhase = GetDirectionalLightPhase(desc.phaseG,desc.mainLightDirection,desc.viewDirection);
    float envPhase = GetEnvironLightPhase();
    float3 scatterLumin = desc.scatterCoeff * (dirPhase*desc.mainLightIntensity + envPhase*desc.environLightIntensity);
    return scatterLumin;
}

float3 GetWaterVolumeScattering(float3 scatteredLuminance,float3 extinctionCoeff,float3 volumeTransmittance)
{
    float3 volumeTransmittanceIntegral = HomogeneousMediumTransmittanceIntegral(extinctionCoeff,volumeTransmittance);
    return scatteredLuminance * volumeTransmittanceIntegral;
}

SingleLayerWaterOutput SingleLayerWater(SingleLayerWaterDesc desc)
{
    SingleLayerWaterOutput output;
 
    float3 extinctionCoeff = ExtinctionCoefficient(desc.scatterCoeff,desc.absorptionCoeff);
    float3 volumeTransmittance = HomogeneousMediumTransmittance(extinctionCoeff,desc.underwaterRayDistance);
    float3 scatteredLuminance = GetMediumScatteredLuminance(desc);
    float3 waterScatter = GetWaterVolumeScattering(scatteredLuminance,extinctionCoeff,volumeTransmittance);
    float3 waterBehind = desc.behindWaterSceneLuminance * volumeTransmittance;
    output.luminance = desc.waterVisibility * (waterScatter + waterBehind); 
    output.waterVolumeTransmittance = volumeTransmittance; 
    
    return output;
}


#endif
```
