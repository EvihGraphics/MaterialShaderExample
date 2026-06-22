# Egaku source: `Shaders/Sky/Cloud/Volumetric/Shading/VolumeLighting/CloudVolumeScattering.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef CLOUD_VOLUME_SCATTERING_HLSL
#define CLOUD_VOLUME_SCATTERING_HLSL
#include "CloudOctaveMultiScattering.hlsl"
#include "../../CloudVolume/CloudVolume.hlsl"
#include "Assets/Shaders/Sky/Basic/RaySpace/OctahedronConcentric.hlsl"

#define VOLUMETRIC_CLOUD_USE_DCT_SHADOW_MAP_SRV
#include "../../Buffers/DCTShadowMap.hlsl"

float RayMarchingCalcDirectionDensity(float3 origin,float3 direction,int iterationCount)
{
    Ray ray;
    ray.origin = origin;
    ray.direction = direction;
    ray.hitPoint = ray.origin + ray.direction * 100000000;
    BoundingSphere sphere = GetCloudVolumeBoundingSphere();
    BoundsHitResult boundsHit = CalcBoundsHit(sphere,ray);
    float distNormal = sphere.radiusMax - sphere.radiusMin; 
    float rayStep = distNormal / iterationCount;
    iterationCount = min(iterationCount * 1.5 ,boundsHit.distInsideBounds / rayStep);
    RayMarchingContext mainLightMarchingContext = GetRayMarchingContext(ray,boundsHit,iterationCount,1);
    float density = VolumeRayMarching_CalcDensityOnly(mainLightMarchingContext).density;
    return density;
}

float SampleDirectionTransmittance(float3 origin,float3 direction)
{
    Ray ray;
    ray.origin = origin;
    ray.direction = direction;
    ray.hitPoint = ray.origin + ray.direction * 100000000;
    BoundingSphere sphere = GetCloudVolumeBoundingSphere();
    BoundsHitResult boundsHit = CalcBoundsHitInsideBoundingSphere(sphere,ray);
    float3 hitPoint = ray.origin + ray.direction*boundsHit.distInsideBounds;

    float remappingFactor = 0.92;
    float volumeFactor = GetCloudVolumeUpperBound() * rcp(GetEarthRadius());
    float3 remappedCenter = sphere.center + remappingFactor * GetEarthRadius() * WorldVerticalDir();
    
    float3 dir = normalize(hitPoint - remappedCenter);

    float polarRadiusFactor = sqrt(1 + (1 - remappingFactor*remappingFactor) * rcp(volumeFactor * (2 + volumeFactor)));
    
    float2 diskPolar = OctahedronConcentric_GetHemiDirectionDiskPolar(dir);
    diskPolar.x *= polarRadiusFactor;
    
    float2 uv = OctahedronConcentric_ConcentricDiskPolarToSquare(diskPolar);

    uint iterationNormal = FetchParam(VolumeRayMarchingDesc).mainLightIteration;
    uint maxSampleCount = FetchParam(VolumeRayMarchingDesc).mainLightMaxSampleCount;
    SphereVolumeSampleConfig sampleConfig = GetSphereVolumeSampleConfig(boundsHit,sphere,iterationNormal,maxSampleCount);
    uint viewSampleCount = sampleConfig.directionSampleCount;
    
    float n = boundsHit.distInsideBounds * rcp(sampleConfig.directionSampleStepSize);
    DCTCoeffs coeffs = DCTShadowMap_SampleDCTCoeffs(uv);
    float baseAngle = DCT_CalcBaseAngle(n,viewSampleCount+1);
    float transmittance = DCT_Decode(coeffs,baseAngle);
    return transmittance;
}

float RayMarchingCalcDirectionTransmittance(float3 origin,float3 direction,int iterationCount)
{
    Ray ray;
    ray.origin = origin;
    ray.direction = direction;
    ray.hitPoint = ray.origin + ray.direction * 100000000;
    BoundingSphere sphere = GetCloudVolumeBoundingSphere();
    BoundsHitResult boundsHit = CalcBoundsHit(sphere,ray);
    float distNormal = sphere.radiusMax - sphere.radiusMin; 
    float rayStep = distNormal / iterationCount;
    iterationCount = min(iterationCount * 1.5 ,boundsHit.distInsideBounds / rayStep);
    RayMarchingContext mainLightMarchingContext = GetRayMarchingContext(ray,boundsHit,iterationCount);
    return  VolumeRayMarching_CalcTransmittanceOnly(mainLightMarchingContext).transmittance;
}



float3 RayMarchingCalcVoxelLuminance(VoxelData voxelData,SamplePoint samplePoint,RayMarchingResult currentViewMarchingResult,float3 viewDirection,int iterationCount)
{
    if (voxelData.density < 0.0001){
        return 0;
    }
    if (all(currentViewMarchingResult.transmittance < 0.002))
    {
        return 0;
    }
    float3 mainLightDir = CloudVolumeLighting_GetMainLightDirection();
   // float mainLightTransmittance = RayMarchingCalcDirectionTransmittance(voxelData.position,mainLightDir,iterationCount);
    float mainLightTransmittance = SampleDirectionTransmittance(voxelData.position,mainLightDir);
    int octaveApproxMaxOrder = FetchParam(CloudOctaveMultiScatterApproxDesc).approxCount;
    float mainPhaseCosine = GetPhaseAngleCosine(mainLightDir,viewDirection);
    float3 voxelLuminance = CalcCloudMultiScatteringOctaveApprox(voxelData,samplePoint,currentViewMarchingResult,octaveApproxMaxOrder,mainPhaseCosine,
        mainLightDir,mainLightTransmittance);
    voxelLuminance += CloudVolumeLighting_CalcCloudAmbientScatteringLuminance(voxelData,currentViewMarchingResult,viewDirection,currentViewMarchingResult.transmittance);
    return voxelLuminance;
}


#endif
```
