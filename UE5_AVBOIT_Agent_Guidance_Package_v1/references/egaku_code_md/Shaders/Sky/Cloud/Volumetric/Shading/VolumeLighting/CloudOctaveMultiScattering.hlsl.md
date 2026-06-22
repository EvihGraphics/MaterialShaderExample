# Egaku source: `Shaders/Sky/Cloud/Volumetric/Shading/VolumeLighting/CloudOctaveMultiScattering.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef CLOUD_OCTAVE_MULTI_SCATTERING_HLSL
#define CLOUD_OCTAVE_MULTI_SCATTERING_HLSL

#include "CloudVolumeLighting.hlsl"
#include "../../VolumetricCloud.ParamStruct.hlsl"

struct MultiScatterOctaveApproxDesc
{
    float sigmaTApproxFactor;
    float sigmaSApproxFactor;
    float phaseAngleCosineApproxFactor;
};

MultiScatterOctaveApproxDesc GetMultiScatterOctaveApproxDesc()
{
    DeclareParamFetch(CloudOctaveMultiScatterApproxDesc, octaveApproxDesc);
    MultiScatterOctaveApproxDesc desc;
    desc.phaseAngleCosineApproxFactor = octaveApproxDesc.phaseFactor;
    desc.sigmaSApproxFactor = octaveApproxDesc.scatteringFactor;
    desc.sigmaTApproxFactor = octaveApproxDesc.extinctionFactor;
    return desc;
}


VoxelData CalcMultiScatteringOctaveApprox_VoxelCoeffs(VoxelData voxelData,int order)
{
    VoxelData result;
    MultiScatterOctaveApproxDesc octaveDesc = GetMultiScatterOctaveApproxDesc();
    result.position = voxelData.position;
    result.sigmaS = voxelData.sigmaS * pow(octaveDesc.sigmaSApproxFactor, order);
    result.sigmaT = voxelData.sigmaT * pow(octaveDesc.sigmaTApproxFactor, order);
    result.sigmaA = result.sigmaT - result.sigmaS;
    result.sigmaSPerUnit = voxelData.sigmaSPerUnit * pow(octaveDesc.sigmaSApproxFactor, order);
    result.sigmaTPerUnit = voxelData.sigmaTPerUnit * pow(octaveDesc.sigmaTApproxFactor, order);
    result.sigmaAPerUnit = result.sigmaTPerUnit - result.sigmaSPerUnit;
    result.density = voxelData.density;
    return result;
}

float CalcMultiScatteringOctaveApprox_PhaseAngleCosine(float cosTheta,int order)
{
    MultiScatterOctaveApproxDesc octaveDesc = GetMultiScatterOctaveApproxDesc();
    return cosTheta * pow(octaveDesc.phaseAngleCosineApproxFactor, order);
}

float3 CalcCloudMultiScatteringOctaveApprox(VoxelData voxelData,SamplePoint samplePoint,RayMarchingResult currentViewMarchingResult,
    int octaveApproxMaxOrder,
    float mainLightPhaseCosine,
    float3 mainLightDirection,
    float mainLightTransmittance)
{
    DeclareParamFetch(CloudVolumeDesc,desc);
    int maxOrder = octaveApproxMaxOrder;
    float3 multiScatterLuminance = 0;
    float3 atmosphereTransmittance = 1; // calculate this in final skybox color shading
    [loop]
    for (int i = 0; i < maxOrder; i++)
    {
        float phaseCosine = CalcMultiScatteringOctaveApprox_PhaseAngleCosine(mainLightPhaseCosine, i);
        VoxelData octaveVoxelData = CalcMultiScatteringOctaveApprox_VoxelCoeffs(voxelData, i);
        float3 mainLightVisibility = atmosphereTransmittance * mainLightTransmittance;//BeerPowderAttenuate(desc.powderEffectFactor,octaveVoxelData.sigmaTPerUnit * density);
        //BeerTransmittance(octaveVoxelData.sigmaTPerUnit * density);
        float3 scatteredLuminance = CloudVolumeLighting_CalcCloudMainLightScatteringLuminance(octaveVoxelData,currentViewMarchingResult,mainLightVisibility,phaseCosine);
        multiScatterLuminance += scatteredLuminance;
    }
    return multiScatterLuminance;
}

#endif
```
