# Egaku source: `Shaders/Sky/Cloud/Volumetric/Shading/RayMarching/CloudVolumeRayMarching.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef CLOUD_VOLUME_RAY_MARCHING_HLSL
#define CLOUD_VOLUME_RAY_MARCHING_HLSL
#include "Assets/Shaders/Sky/Basic/VolumeBase/VolumeRayMarching.hlsl"
#include "../VolumeLighting/CloudVolumeScattering.hlsl"
#include "../../VolumetricCloud.BlueNoise.hlsl"


float3 CloudVolumeRayMarching_CalcVoxelLuminance(VoxelData voxelData,SamplePoint samplePoint,RayMarchingContext context,RayMarchingResult currentResult);

float3 VolumeRayMarching_CalcVoxelLuminance(VoxelData voxelData,SamplePoint samplePoint,RayMarchingContext context,RayMarchingResult currentResult)
{
    return CloudVolumeRayMarching_CalcVoxelLuminance(voxelData,samplePoint,context,currentResult);
}

RayMarchingResult VolumeRayMarching_UpdateResult(RayMarchingResult result,VoxelData voxelData,SamplePoint samplePoint,RayMarchingContext context,float3 luminance)
{
    float ds = samplePoint.stepSize;
    float3 voxelLuminanceStepped = VolumeScattering_CalcLuminanceHomogeneousApprox(voxelData,luminance,samplePoint.stepSize * voxelData.sigmaT);
    float3 voxelLuminanceTransmitted = result.transmittance * voxelLuminanceStepped;
    result.luminance += voxelLuminanceTransmitted;
    result.transmittance *= BeerTransmittance(voxelData.sigmaT * ds);
    result.density += voxelData.density * ds;
    result.distance += ds;
    return result;
}


SamplePoint VolumeRayMarching_CalcNextSamplePoint(SamplePoint samplePoint,VoxelData voxelData,RayMarchingContext context,
    RayMarchingResult currentResult)
{
    SamplePoint nextPoint;
   // DeclareParamFetch(VolumeRayMarchingDesc,desc);
    /*if (all(currentResult.transmittance < 0.001))
    {
        //stepSize = 0x3f3f3f3f;
    }
    */

    static bool inCloud = false;
    static bool sampled = false;
    float k = 1;
    /*if (inCloud == false)
    {
        if (voxelData.density > 0.0001f)
        {
            inCloud = true;
            // step back
            samplePoint.position = samplePoint.position - samplePoint.direction * samplePoint.stepSize;
        }
    }else
    {
        if (sampled == true)
        {
            if (voxelData.density < 0.0001f)
            {
                inCloud = false;
                sampled = false;
            }
        }else{
            if (voxelData.density > 0.0001f)
            {
                sampled = true;
            }
        }
    }
  
    if (inCloud)
    {
        k = 1;
    }else
    {
        k = 1;
    }*/
    float stepSize = k * context.stepSize;
    nextPoint.position = samplePoint.position + context.direction * stepSize;
    nextPoint.stepSize = stepSize;
    nextPoint.direction = context.direction;

    return nextPoint;
}



float3 CloudVolumeRayMarching_CalcVoxelLuminance(VoxelData voxelData,SamplePoint samplePoint,RayMarchingContext context,RayMarchingResult currentResult)
{
    int iterationCount = FetchParam(VolumeRayMarchingDesc).mainLightIteration;
    float3 viewDirection = -context.direction;
    float3 luminance = RayMarchingCalcVoxelLuminance(voxelData,samplePoint,currentResult,viewDirection,iterationCount);
    return luminance;
}

RayMarchingResult CloudVolumeRayMarching(Ray ray,BoundingBox box)
{
    int iterationCount = FetchParam(VolumeRayMarchingDesc).viewDirectionIteration;
    RayMarchingContext rayMarchingContext = GetRayMarchingContext(ray,box,iterationCount);
    return VolumeRayMarching_CalcAll(rayMarchingContext);
}

RayMarchingContext JitterRayMarchingEntryPoint(RayMarchingContext context)
{
    DeclareParamFetch(VolumeRayMarchingDesc,desc);
    TimeContext t = GetTimeContext();
    float4 bn = SampleBlueNoise( float2(frac(t.doubledTime),frac(t.tripledTime)));
    float k = bn.r;
    k = Remap(k,0,1,1 - desc.jitterRange,1 + desc.jitterRange);
    context.entryPosition += context.direction * k * context.stepSize;
    return context;
}


RayMarchingResult CloudVolumeRayMarching(Ray ray,BoundingSphere sphere)
{
    uint normalSampleCount = FetchParam(VolumeRayMarchingDesc).viewDirectionIteration;
    uint maxSampleCount = FetchParam(VolumeRayMarchingDesc).viewDirectionMaxSampleCount;
    BoundsHitResult boundsHit = CalcBoundsHit(sphere,ray);
    uint viewSampleCount = GetSphereVolumeSampleConfig(boundsHit,GetCloudVolumeBoundingSphere(),normalSampleCount,maxSampleCount).directionSampleCount;
    RayMarchingContext rayMarchingContext = GetRayMarchingContext(ray,boundsHit,viewSampleCount);
    rayMarchingContext = JitterRayMarchingEntryPoint(rayMarchingContext);
    RayMarchingResult result =  VolumeRayMarching_CalcAll(rayMarchingContext);
    return result;
}


RayMarchingResult CloudVolumeRayMarching(Ray ray,BoundingSphere sphere,int iterationCount)
{
    BoundsHitResult boundsHit = CalcBoundsHit(sphere,ray);
    RayMarchingContext rayMarchingContext = GetRayMarchingContext(ray,boundsHit,iterationCount);
    return VolumeRayMarching_CalcAll(rayMarchingContext);
}


RayMarchingResult CloudVolumeRayMarching(Ray ray,BoundsHitResult boundsHit,int iterationCount)
{
    RayMarchingContext rayMarchingContext = GetRayMarchingContext(ray,boundsHit,iterationCount);
    return VolumeRayMarching_CalcAll(rayMarchingContext);
}

RayMarchingResult CloudVolumeRayMarching(RayMarchingContext rayMarchingContext)
{
    return VolumeRayMarching_CalcAll(rayMarchingContext);
}

float3 GetViewDirectionAmbientLuminanceTransfer(float3 viewDirection,float viewDirectionDensityIntegration,float viewDirectionTransmittance)
{
    DeclareParamFetch(CloudVolumeDesc,desc);
    
    /*AmbientLight skyAmbient = GetSkyAmbientLight(SH_DiffuseIndirect(viewDirection));
    skyAmbient.ambientIntensity *= desc.ambientLightSkyFactor;*/
    float3 viewAmbientLuminanceTransfer = desc.ambientLightSkyFactor * desc.scatteringCoeff * viewDirectionDensityIntegration * viewDirectionTransmittance;
    return viewAmbientLuminanceTransfer;
}

float4 PackVolumetricCloudRayMarchingResult(RayMarchingResult result,float3 viewDirection)
{
    float sunLightTransfer = result.luminance.r;
    float groundAmbientTransfer = result.luminance.g;
    float ambientTransfer = GetViewDirectionAmbientLuminanceTransfer(viewDirection,result.density,result.transmittance);
    float opacity = 1 - max(result.transmittance.r, max(result.transmittance.g, result.transmittance.b));
    return float4(sunLightTransfer,groundAmbientTransfer,ambientTransfer,opacity);
}
#endif
```
