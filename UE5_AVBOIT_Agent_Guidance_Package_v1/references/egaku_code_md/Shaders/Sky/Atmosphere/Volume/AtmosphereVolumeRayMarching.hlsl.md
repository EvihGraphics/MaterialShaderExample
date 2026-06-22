# Egaku source: `Shaders/Sky/Atmosphere/Volume/AtmosphereVolumeRayMarching.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef ATMOSPHERE_VOLUME_RAY_MARCHING_SHADING_HLSL
#define ATMOSPHERE_VOLUME_RAY_MARCHING_SHADING_HLSL
#include "Assets/Shaders/Sky/Basic/VolumeBase/VolumeRayMarching.hlsl"
#include "MieVolume/DustVolume.hlsl"
#include "RayleighVolume/AirVolume.hlsl"
#include "RayleighVolume/OzoneVolume.hlsl"
#include "AtmosphereVolume.hlsl"
#include "AtmosphereVolumeLighting.hlsl"
#include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Lighting.hlsl"

float3 RayMarchingCalcDirectionTransmittance(float3 origin,float3 direction,int iterationCount)
{
    Ray ray;
    
    ray.origin = origin;
    ray.direction = direction;
    ray.hitPoint = ray.origin + ray.direction * 1000000;
    BoundingSphere sphere = GetAtmosphereVolumeBoundingSphere();
    RayMarchingContext mainLightMarchingContext = GetRayMarchingContext(ray,sphere,iterationCount);
    float3 transmittance = VolumeRayMarching_CalcTransmittanceOnly(mainLightMarchingContext).transmittance;
    return transmittance;
}

// exposed interfaces
float3 AtmosphereVolumeRayMarching_CalcDirectionTransmittance(float3 origin,float3 direction);
RayMarchingResult AtmosphereVolumeRayMarching_UpdateResult(RayMarchingResult result,VoxelData voxelData,SamplePoint samplePoint,RayMarchingContext context,float3 luminance);



VoxelData SampleAtmosphereVoxel(SamplePoint samplePoint)
{
    float altitude = GetAltitudeFromPositionWS(samplePoint.position);
    VolumeDesc dust = GetDustVolumeDesc(altitude);
    VolumeDesc air = GetAirVolumeDesc(altitude);
    VolumeDesc ozone = GetOzoneVolumeDesc(altitude);
    VoxelData voxelData;
    voxelData.position = samplePoint.position;
    voxelData.density = FetchParam(AtmosphereVolumeDesc).atmosphereDensityScale;
    voxelData.sigmaAPerUnit = air.absorptionCoeff + dust.absorptionCoeff + ozone.absorptionCoeff;
    voxelData.sigmaSPerUnit = air.scatteringCoeff + dust.scatteringCoeff + ozone.scatteringCoeff;
    voxelData.sigmaTPerUnit = voxelData.sigmaAPerUnit + voxelData.sigmaSPerUnit;
    voxelData.sigmaA = air.absorptionCoeff + dust.absorptionCoeff + ozone.absorptionCoeff;
    voxelData.sigmaS = air.scatteringCoeff + dust.scatteringCoeff + ozone.scatteringCoeff;
    voxelData.sigmaA *= voxelData.density;
    voxelData.sigmaS *= voxelData.density;
    voxelData.sigmaT = voxelData.sigmaA + voxelData.sigmaS;
    return voxelData;
}

VoxelData SampleDustVoxel(SamplePoint samplePoint)
{
    VoxelData voxelData;
    float altitude = GetAltitudeFromPositionWS(samplePoint.position);
    VolumeDesc dust = GetDustVolumeDesc(altitude);
    voxelData.position = samplePoint.position;
    voxelData.density = FetchParam(AtmosphereVolumeDesc).atmosphereDensityScale;
    voxelData.sigmaAPerUnit = dust.absorptionCoeff;
    voxelData.sigmaSPerUnit = dust.scatteringCoeff;
    voxelData.sigmaTPerUnit = voxelData.sigmaAPerUnit + voxelData.sigmaSPerUnit;
    voxelData.sigmaA = dust.absorptionCoeff * voxelData.density;
    voxelData.sigmaS = dust.scatteringCoeff * voxelData.density;
    voxelData.sigmaT = voxelData.sigmaA + voxelData.sigmaS;
    return voxelData;
}

VoxelData SampleAirVoxel(SamplePoint samplePoint)
{
    VoxelData voxelData;
    float altitude = GetAltitudeFromPositionWS(samplePoint.position);
    VolumeDesc air = GetAirVolumeDesc(altitude);
    voxelData.position = samplePoint.position;
    voxelData.density = FetchParam(AtmosphereVolumeDesc).atmosphereDensityScale;
    voxelData.sigmaAPerUnit = air.absorptionCoeff;
    voxelData.sigmaSPerUnit = air.scatteringCoeff;
    voxelData.sigmaTPerUnit = voxelData.sigmaAPerUnit + voxelData.sigmaSPerUnit;
    voxelData.sigmaA = air.absorptionCoeff * voxelData.density;
    voxelData.sigmaS = air.scatteringCoeff * voxelData.density;
    voxelData.sigmaT = voxelData.sigmaA + voxelData.sigmaS;
    return voxelData;
}

VoxelData SampleOzoneVoxel(SamplePoint samplePoint)
{
    VoxelData voxelData;
    float altitude = GetAltitudeFromPositionWS(samplePoint.position);
    VolumeDesc ozone = GetOzoneVolumeDesc(altitude);
    voxelData.position = samplePoint.position;
    voxelData.density = FetchParam(AtmosphereVolumeDesc).atmosphereDensityScale;
    voxelData.sigmaAPerUnit = ozone.absorptionCoeff;
    voxelData.sigmaSPerUnit = ozone.scatteringCoeff;
    voxelData.sigmaTPerUnit = voxelData.sigmaAPerUnit + voxelData.sigmaSPerUnit;
    voxelData.sigmaA = ozone.absorptionCoeff * voxelData.density;
    voxelData.sigmaS = ozone.scatteringCoeff * voxelData.density;
    voxelData.sigmaT = voxelData.sigmaA + voxelData.sigmaS;
    return voxelData;
}



float3 VolumeRayMarching_CalcVoxelLuminance(VoxelData voxelData,SamplePoint samplePoint,RayMarchingContext context,RayMarchingResult currentResult)
{
    float3 viewDirection = -context.direction;
    Light light = GetMainLight();
    float3 mainLightDir = light.direction;
    float mainPhaseCosine = GetPhaseAngleCosine(light.direction,viewDirection);
    VoxelData air = SampleAirVoxel(samplePoint);
    VoxelData dust = SampleDustVoxel(samplePoint);
    VoxelData ozone = SampleOzoneVoxel(samplePoint);
    float3 transmittance = AtmosphereVolumeRayMarching_CalcDirectionTransmittance(samplePoint.position,mainLightDir);
    float phaseRayleigh = GetPhase_Rayleigh(mainPhaseCosine);
    float phaseMie = GetPhase_Mie(mainPhaseCosine,FetchParam(AtmosphereVolumeDesc).mieScatteringAnisotropy);
    
    float3 mainLightIntensity = 1;  // use unit intensity to defer the luminance calculation (get the precomputed radiance transfer as luminance)
    float3 voxelIlluminance = mainLightIntensity * transmittance;
    float3 airLuminanceTransfer = phaseRayleigh * air.sigmaS;
    float3 dustLuminanceTransfer = phaseMie * dust.sigmaS;
    float3 ozoneLuminanceTransfer = phaseRayleigh * ozone.sigmaS;
    float3 voxelLuminance = voxelIlluminance * (airLuminanceTransfer + dustLuminanceTransfer + ozoneLuminanceTransfer);
    return voxelLuminance;
}

RayMarchingResult VolumeRayMarching_UpdateResult(RayMarchingResult result,VoxelData voxelData,SamplePoint samplePoint,RayMarchingContext context,float3 luminance)
{
    return AtmosphereVolumeRayMarching_UpdateResult(result,voxelData,samplePoint,context,luminance);
}

VoxelData Volume_SampleVoxelData(SamplePoint samplePoint)
{
    return SampleAtmosphereVoxel(samplePoint);
}


SamplePoint VolumeRayMarching_CalcNextSamplePoint(SamplePoint samplePoint,VoxelData voxelData,RayMarchingContext context,
    RayMarchingResult currentResult)
{
    SamplePoint nextPoint = samplePoint;
    nextPoint.position = samplePoint.position + samplePoint.direction * context.stepSize;
    return nextPoint;
}

RayMarchingResult AtmosphereVolumeRayMarching_CalcPRT(Ray ray,BoundingSphere sphere,int sampleCount)
{
    BoundsHitResult boundsHit = CalcBoundsHit(sphere,ray);
    RayMarchingContext rayMarchingContext = GetRayMarchingContext(ray,boundsHit,sampleCount);
    return VolumeRayMarching_CalcAll(rayMarchingContext);
}

RayMarchingResult AtmosphereVolumeRayMarching_CalcTransmittance(Ray ray,BoundingSphere sphere,int iterationCount)
{
    BoundsHitResult boundsHit = CalcBoundsHit(sphere,ray);
    RayMarchingContext rayMarchingContext = GetRayMarchingContext(ray,boundsHit,iterationCount);
    return VolumeRayMarching_CalcTransmittanceOnly(rayMarchingContext);
}

float3 CalcAtmosphereTransmittanceIntegral(float3 rayOrigin,float3 rayDirection,float dist,int sampleCount)
{
    Ray ray;
    ray.origin = rayOrigin;
    ray.direction = rayDirection;
    ray.hitPoint = rayOrigin + rayDirection*dist;
    BoundingSphere sphere = GetAtmosphereVolumeBoundingSphere();
    RayMarchingResult result = AtmosphereVolumeRayMarching_CalcTransmittance(ray,sphere,sampleCount);
    return result.transmittance;
}

float4 ShadeAtmosphere_PRT(float3 cameraPosWS,float3 viewDirection,int sampleCount)
{
    Ray ray;
    ray.origin = cameraPosWS;
    ray.direction = viewDirection;
    ray.hitPoint = viewDirection*10000000;
    BoundingSphere sphere = GetAtmosphereVolumeBoundingSphere();
    RayMarchingResult result = AtmosphereVolumeRayMarching_CalcPRT(ray,sphere,sampleCount);
    float3 color = result.luminance;
    float opacity = 1;
    return float4(color,opacity);
}

#endif
```
