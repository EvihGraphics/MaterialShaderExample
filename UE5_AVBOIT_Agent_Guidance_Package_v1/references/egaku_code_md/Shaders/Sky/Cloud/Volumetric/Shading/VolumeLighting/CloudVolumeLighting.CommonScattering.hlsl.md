# Egaku source: `Shaders/Sky/Cloud/Volumetric/Shading/VolumeLighting/CloudVolumeLighting.CommonScattering.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef CLOUD_VOLUME_LIGHTING_COMMON_SCATTERING_HLSL
#define CLOUD_VOLUME_LIGHTING_COMMON_SCATTERING_HLSL
#include "CloudVolumeLighting.hlsl"
#include "Assets/Shaders/Infrastructure/Utils.hlsl"
#include "../../VolumetricCloud.ParamStruct.hlsl"
#include "../../VolumetricCloud.SH.hlsl"
#include "../../VolumetricCloud.AtmospherePrt.hlsl"

struct AmbientLight
{
    float3 ambientIntensity;
    float ambientPhase;
};

float3 CloudVolumeLighting_GetMainLightDirection()
{
    return GetMainLight().direction;
}

AmbientLight GetSkyAmbientLight(float3 ambientSampleNormal)
{
    AmbientLight context;
    context.ambientIntensity = SampleAtmosphereOctahedronPrt(ambientSampleNormal);//SH_DiffuseIndirect(ambientSampleNormal);
    context.ambientPhase = INV_FOUR_PI;
    return context;
}

AmbientLight GetGroundAmbientLight(float3 ambientSampleNormal)
{
    AmbientLight context;
    context.ambientIntensity = SH_DiffuseIndirect(ambientSampleNormal);
    context.ambientPhase = INV_FOUR_PI;
    return context;
}

float3 CloudVolumeLighting_CalcCloudAmbientScatteringLuminance(VoxelData voxelData,RayMarchingResult currentViewMarchingResult,float3 viewDirection,float3 ambientVisibility)
{
    DeclareParamFetch(CloudVolumeDesc,desc);
    
    AmbientLight skyAmbient = GetSkyAmbientLight(WorldVerticalDir());
    AmbientLight groundAmbient;// = GetSkyAmbientLight(WorldVerticalDir());
    groundAmbient.ambientIntensity = desc.ambientLightGroundFactor;
    groundAmbient.ambientPhase = INV_FOUR_PI;
    float3 luminanceTop = 0;//ambientVisibility  * skyAmbient.ambientIntensity * skyAmbient.ambientPhase;
    float3 luminanceBottom = 0;
    luminanceBottom.g = currentViewMarchingResult.transmittance * groundAmbient.ambientIntensity * groundAmbient.ambientPhase;
    return  (luminanceTop + luminanceBottom) * voxelData.sigmaS;
}

float3 CloudVolumeLighting_CalcCloudMainLightScatteringLuminance(VoxelData voxelData,RayMarchingResult currentViewMarchingResult,float3 mainLightVisibility,float mainLightPhaseCosine)
{
    DeclareParamFetch(CloudVolumeDesc,desc);
    Light light = GetMainLight();
    float mainLightPhase = CloudPhaseFunction(mainLightPhaseCosine,desc.phaseG1,desc.phaseG2);
    float3 mainLightIntensity = desc.mainLightFactor;
    mainLightIntensity  = desc.mainLightFactor;
    
    float3 luminance = 0;
    luminance.r = mainLightPhase * mainLightIntensity;
    return luminance * mainLightVisibility * voxelData.sigmaS;
}

#endif
```
