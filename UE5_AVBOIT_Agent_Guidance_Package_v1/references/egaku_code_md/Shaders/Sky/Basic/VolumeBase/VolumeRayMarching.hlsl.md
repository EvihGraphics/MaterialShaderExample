# Egaku source: `Shaders/Sky/Basic/VolumeBase/VolumeRayMarching.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef VOLUME_RAY_MARCHING_HLSL
#define VOLUME_RAY_MARCHING_HLSL

#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Lighting/Volumetric.hlsl"
#include "Volume.hlsl"
#include "Ray.hlsl"

struct RayMarchingContext
{
    float3 entryPosition;
    float3 direction;
    float maxDistance;
    float stepSize;
    int iterationCount;
};

struct RayMarchingResult
{
    float3 luminance;
    float3 transmittance;
    float density;
    float distance;
};


struct SamplePoint
{
    float3 position;
    float3 direction;
    float stepSize;
};

struct SphereVolumeSampleConfig
{
    uint normalSampleCount;
    float normalSampleStepSize;
    uint directionSampleCount;
    float directionSampleStepSize;
};

// exposed interfaces
SamplePoint VolumeRayMarching_CalcNextSamplePoint(SamplePoint samplePoint,VoxelData voxelData,RayMarchingContext context,RayMarchingResult currentResult);
float3 VolumeRayMarching_CalcVoxelLuminance(VoxelData voxelData,SamplePoint samplePoint,RayMarchingContext context,RayMarchingResult currentResult);
VoxelData Volume_SampleVoxelData(SamplePoint samplePoint);
RayMarchingResult VolumeRayMarching_UpdateResult(RayMarchingResult result,VoxelData voxelData,SamplePoint samplePoint,RayMarchingContext context,float3 luminance);

RayMarchingResult VolumeRayMarching_UpdateResult_Default(RayMarchingResult result,VoxelData voxelData,SamplePoint samplePoint,RayMarchingContext context,float3 luminance)
{
    float ds = samplePoint.stepSize;
    result.luminance += luminance * result.transmittance * ds;
    result.transmittance *= BeerTransmittance(voxelData.sigmaT * ds);
    result.density += voxelData.density * ds;
    result.distance += ds;
    return result;
}


RayMarchingResult CalcVolumeRayMarching(RayMarchingContext rayMarchingContext)
{
    int iterationCount = rayMarchingContext.iterationCount;
    SamplePoint samplePoint;
    samplePoint.position = rayMarchingContext.entryPosition;
    samplePoint.stepSize = rayMarchingContext.stepSize;
    samplePoint.direction = rayMarchingContext.direction;
    RayMarchingResult result;
    result.density = 0;
    result.distance = 0;
    result.luminance = 0;
    result.transmittance = 1;
    
    [loop]
    for (int i = 0; i < iterationCount ; i++)
    {
        if (result.distance > rayMarchingContext.maxDistance)
        {
            break;
        }
   
        VoxelData voxelData = Volume_SampleVoxelData(samplePoint);
        float3 luminance = VolumeRayMarching_CalcVoxelLuminance(voxelData,samplePoint,rayMarchingContext,result);
        result = VolumeRayMarching_UpdateResult(result,voxelData,samplePoint,rayMarchingContext,luminance);
        samplePoint = VolumeRayMarching_CalcNextSamplePoint(samplePoint, voxelData,rayMarchingContext, result);
    }
    return result;
}

RayMarchingResult VolumeRayMarching_CalcDensityOnly(RayMarchingContext rayMarchingContext)
{
    int iterationCount = rayMarchingContext.iterationCount;
    SamplePoint samplePoint;
    samplePoint.position = rayMarchingContext.entryPosition;
    samplePoint.stepSize = rayMarchingContext.stepSize;
    samplePoint.direction = rayMarchingContext.direction;
    RayMarchingResult result;
    result.density = 0;
    result.distance = 0;
    result.luminance = 0;
    result.transmittance = 1;
    [loop]
    for (int i = 0; i < iterationCount ; i++)
    {
        if (result.distance > rayMarchingContext.maxDistance)
        {
            break;
        }
        VoxelData voxelData = Volume_SampleVoxelData(samplePoint);
        float ds = samplePoint.stepSize;
        result.density += voxelData.density * ds;
        result.distance += ds;
        samplePoint = VolumeRayMarching_CalcNextSamplePoint(samplePoint, voxelData,rayMarchingContext, result);
    }
    return result;
}


RayMarchingResult VolumeRayMarching_CalcTransmittanceOnly(RayMarchingContext rayMarchingContext)
{
    int iterationCount = rayMarchingContext.iterationCount;
    SamplePoint samplePoint;
    samplePoint.position = rayMarchingContext.entryPosition;
    samplePoint.stepSize = rayMarchingContext.stepSize;
    samplePoint.direction = rayMarchingContext.direction;
    RayMarchingResult result;
    result.density = 0;
    result.distance = 0;
    result.luminance = 0;
    result.transmittance = 1;
    [loop]
    for (int i = 0; i < iterationCount ; i++)
    {
        if (result.distance > rayMarchingContext.maxDistance)
        {
            break;
        }
        VoxelData voxelData = Volume_SampleVoxelData(samplePoint);
        float ds = samplePoint.stepSize;
        result.transmittance *= BeerTransmittance(voxelData.sigmaT * ds);
        result.distance += ds;
        samplePoint = VolumeRayMarching_CalcNextSamplePoint(samplePoint, voxelData,rayMarchingContext, result);
    }
    return result;
}

RayMarchingResult VolumeRayMarching_CalcAll(RayMarchingContext rayMarchingContext)
{
    return CalcVolumeRayMarching(rayMarchingContext);
}

RayMarchingContext GetRayMarchingContext(Ray ray, BoundsHitResult boundsHit,int iterationCount)
{
    RayMarchingContext context;
    context.direction = ray.direction;
    context.entryPosition = ray.origin + ray.direction * boundsHit.distToBounds;
    context.maxDistance = boundsHit.distInsideBounds;
    context.stepSize = boundsHit.distInsideBounds * rcp(max(1,iterationCount));
    context.iterationCount = step(1.0f,boundsHit.distInsideBounds) * iterationCount;
    return context;
}

RayMarchingContext GetRayMarchingContext(Ray ray, BoundsHitResult boundsHit,int iterationCount,float initialStepBias)
{
    RayMarchingContext context = GetRayMarchingContext(ray, boundsHit, iterationCount);
    float biasDist = context.stepSize*initialStepBias;
    context.entryPosition += context.direction * biasDist;
    context.maxDistance -= biasDist;
    context.iterationCount = step(1.0f,boundsHit.distInsideBounds) * iterationCount;
    return context;
}
RayMarchingContext GetRayMarchingContext(Ray ray, BoundingBox box,int iterationCount)
{
    BoundsHitResult boundsHit = CalcBoundsHit(box,ray);
    return GetRayMarchingContext(ray,boundsHit,iterationCount);
}

RayMarchingContext GetRayMarchingContext(Ray ray, BoundingSphere sphere,int iterationCount)
{
    BoundsHitResult boundsHit = CalcBoundsHit(sphere,ray);
    return GetRayMarchingContext(ray,boundsHit,iterationCount);
}

SphereVolumeSampleConfig GetSphereVolumeSampleConfig(BoundsHitResult boundsHit,BoundingSphere sphere,uint normalSampleCount,uint maxSampleCount)
{
    SphereVolumeSampleConfig config;
    float normalDist = sphere.radiusMax - sphere.radiusMin;
    config.normalSampleCount = normalSampleCount;
    config.normalSampleStepSize = normalDist * rcp(normalSampleCount);
    config.directionSampleCount = min(maxSampleCount,ceil(boundsHit.distInsideBounds / config.normalSampleStepSize));
    config.directionSampleStepSize = boundsHit.distInsideBounds * rcp(config.directionSampleCount);
    return config;
}

#endif
```
