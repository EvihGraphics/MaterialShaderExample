# Egaku source: `Shaders/Sky/Atmosphere/Atmosphere.PrecomputedRayMarchingShading.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef ATMOSPHERE_PRECOMPUTED_RAY_MARCHING_SHADING_HLSL
#define ATMOSPHERE_PRECOMPUTED_RAY_MARCHING_SHADING_HLSL
#include "Atmosphere.TransmittanceLut.hlsl"
#include "Volume/AtmosphereVolumeRayMarching.hlsl"

float3 LookupAtmosphereDirectionTransmittance(BoundingSphere bounds,float3 position, float3 direction)
{
    float3 positionVec = position - bounds.center;
    float directionZenithCosine = dot(normalize(positionVec), direction);
    float radius = length(positionVec);
    float4 t = SampleAtmosphereTransmittanceLut(radius, directionZenithCosine,bounds.radiusMin, bounds.radiusMax);
    return t;
}

float3 LookupAtmosphereIntervalTransmittance(BoundingSphere bounds,float3 positionBegin, float3 positionEnd)
{
    float3 dirVec = positionEnd - positionBegin;
    float dirVecLen = length(dirVec);
    if (dirVecLen < 0.0001){
        return 1;
    }
    float3 direction = dirVec * rcp(dirVecLen);
    float3 t1 = LookupAtmosphereDirectionTransmittance(bounds, positionBegin, direction);
    float3 t2 = LookupAtmosphereDirectionTransmittance(bounds, positionEnd, direction);
    return t1 * rcp(t2);
}

float3 AtmosphereVolumeRayMarching_CalcDirectionTransmittance(float3 origin,float3 direction)
{
    BoundingSphere bounds = GetAtmosphereVolumeBoundingSphere();
    return LookupAtmosphereDirectionTransmittance(bounds, origin, direction);
}

RayMarchingResult AtmosphereVolumeRayMarching_UpdateResult(RayMarchingResult result,VoxelData voxelData,SamplePoint samplePoint,RayMarchingContext context,float3 luminance)
{
    BoundingSphere bounds = GetAtmosphereVolumeBoundingSphere();
    float ds = samplePoint.stepSize;
    result.luminance += luminance * result.transmittance * ds; //+ 0.5 * ds * samplePoint.direction
    result.transmittance = LookupAtmosphereIntervalTransmittance(bounds, context.entryPosition,samplePoint.position);
    result.density += voxelData.density * ds;
    result.distance += ds;
    return result;
}

#endif
```
