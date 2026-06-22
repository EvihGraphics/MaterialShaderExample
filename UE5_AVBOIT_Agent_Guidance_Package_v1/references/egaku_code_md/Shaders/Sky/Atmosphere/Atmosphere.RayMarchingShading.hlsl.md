# Egaku source: `Shaders/Sky/Atmosphere/Atmosphere.RayMarchingShading.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef ATMOSPHERE_RAY_MARCHING_SHADING_HLSL
#define ATMOSPHERE_RAY_MARCHING_SHADING_HLSL
#include "Volume/AtmosphereVolumeRayMarching.hlsl"
#include "Assets/Shaders/Sky/Basic/Utils.hlsl"


float3 AtmosphereVolumeRayMarching_CalcDirectionTransmittance(float3 origin,float3 direction)
{
    return RayMarchingCalcDirectionTransmittance(origin, direction,20);
}

RayMarchingResult AtmosphereVolumeRayMarching_UpdateResult(RayMarchingResult result,VoxelData voxelData,SamplePoint samplePoint,RayMarchingContext context,float3 luminance)
{
    return VolumeRayMarching_UpdateResult_Default(result,voxelData,samplePoint,context,luminance);
}

Ray GetWorldSpaceRay(float2 screenUV)
{
    float3 positionWS = GetWorldPosition(screenUV);
    float3 rayHitPoint =  positionWS;
    float3 rayOriginWS = GetCameraPositionWS();
    float3 rayDirWS = normalize(positionWS - rayOriginWS);

    Ray ray;
    ray.origin = rayOriginWS;
    ray.direction = rayDirWS;
    ray.hitPoint = rayHitPoint;
    return ray;
}

Ray GetWorldSpaceRayEarthBased(float3 cameraPosWS,float3 viewHitPointWS)
{
    float3 rayHitPoint = viewHitPointWS;
    float3 rayOriginWS = cameraPosWS;
    float3 relativeVec = 0;//GetCameraPositionWS();//Make3DFromHorizontal(GetHorizontal(rayOriginWS),0);
    rayOriginWS = rayOriginWS - relativeVec;
    rayHitPoint = rayHitPoint - relativeVec;
    float3 rayDirWS = normalize(rayHitPoint - rayOriginWS);
    
    Ray ray;
    ray.origin = rayOriginWS;
    ray.direction = rayDirWS;
    ray.hitPoint = rayHitPoint;
    return ray;
}

/*
float4 ShadeAtmosphere(float3 cameraPosWS,float3 viewHitPointWS)
{
    Ray ray = GetWorldSpaceRayEarthBased(cameraPosWS,viewHitPointWS);
    if (ray.direction.y < 0.01){
        return float4(0,0,0,0);
    }
    BoundingSphere sphere = GetAtmosphereVolumeBoundingSphere();
    RayMarchingResult result = AtmosphereVolumeRayMarching_CalcPRT(ray,sphere,10);
    float opacity = 1 - max(result.transmittance.r, max(result.transmittance.g, result.transmittance.b));
    return float4(result.luminance,opacity);
}
*/


#endif
```
