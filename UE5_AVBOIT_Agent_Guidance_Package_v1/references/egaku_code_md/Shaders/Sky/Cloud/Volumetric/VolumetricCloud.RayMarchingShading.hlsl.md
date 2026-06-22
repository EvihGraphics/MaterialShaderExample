# Egaku source: `Shaders/Sky/Cloud/Volumetric/VolumetricCloud.RayMarchingShading.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef VOLUMETRIC_CLOUD_RAY_MARCHING_SHADING_HLSL
#define VOLUMETRIC_CLOUD_RAY_MARCHING_SHADING_HLSL

#include "VolumetricCloud.SH.hlsl"
#include "Assets/Shaders/Sky/Basic/Utils.hlsl"
#include "VolumetricCloud.AtmospherePrt.hlsl"
#define VOLUMETRIC_CLOUD_USE_VOXEL_META_LUT_SRV
#include "Buffers/VoxelMetaLut.hlsl"



float2 OctahedronVolume_DirectionDimensions()
{
    return VoxelMetaLut_GetDimensions().xy;
}

float OctahedronVolume_DirectionSliceCount()
{
    return VoxelMetaLut_GetDimensions().z;
}




float3 CloudVolumeLighting_GetMainLightDirection()
{
    return GetMainLight().direction;
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


float4 ShadeVolumetricCloud(float3 cameraPosWS,float3 viewHitPointWS)
{
    Ray ray = GetWorldSpaceRayEarthBased(cameraPosWS,viewHitPointWS);
    
    if (ray.direction.y < 0.01){
        return float4(0,0,0,0);
    }
    BoundingSphere sphere = GetCloudVolumeBoundingSphere();
    RayMarchingResult result = CloudVolumeRayMarching(ray,sphere);
    float opacity = 1 - max(result.transmittance.r, max(result.transmittance.g, result.transmittance.b));
   // float4 bn = SampleBlueNoise(screenUV + _Time.y);
   // return float4(bn.rgb,1);
    return float4(result.luminance,opacity);
}



uint _VolumetricCloud_SegmentTestNormalIteration;

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

float4 ShadeVolumetricCloudHemiOctahedronStratified(float3 cameraPosWS,float3 viewDirection)
{
    Ray ray;
    ray.origin = cameraPosWS;
    ray.direction = viewDirection;
    ray.hitPoint = viewDirection*10000000;

    BoundingSphere sphere = GetCloudVolumeBoundingSphere();
    BoundsHitResult boundsHit = CalcBoundsHit(sphere,ray);
    RayMarchingResult result;
    result.luminance = 0;
    result.transmittance = 1;
    result.density = 0;
    result.distance = 0;
    uint segmentCount = min(VoxelMetaLut_GetDimensions().z,100);

 
    uint normalSampleCount = FetchParam(VolumeRayMarchingDesc).viewDirectionIteration;
    uint targetMaxSampleCount = normalSampleCount*3;
    SphereVolumeSampleConfig sampleCfg = GetSphereVolumeSampleConfig(boundsHit,sphere,normalSampleCount,targetMaxSampleCount);

    float3 direction = OctahedronVolume_GetDirectionFromSampleCoords(OctahedronVolume_GetDirectionSampleCoords(viewDirection));
    float2 directionCoords = OctahedronVolume_GetDirectionCoords(direction);
    uint segmentTestNormalSampleCount = _VolumetricCloud_SegmentTestNormalIteration;
    Ray ray2 = ray;
    ray2.direction = direction;
    BoundsHitResult boundsHit2 = CalcBoundsHit(sphere,ray2);
    SphereVolumeSampleConfig segmentPrescanSampleCfg = GetSphereVolumeSampleConfig(boundsHit2,sphere,segmentTestNormalSampleCount,segmentTestNormalSampleCount*1000);
    float segmentPrescanMaxMultiplier = segmentPrescanSampleCfg.directionSampleCount;
    float segmentUnitLength = segmentPrescanSampleCfg.directionSampleStepSize;
    
    [loop]
    for (uint i = 0; i < segmentCount; i++)
    {
        VoxelMeta thisSegMeta = VoxelMetaLut_Read(uint3(directionCoords,i));
        float segmentBegin = round(thisSegMeta.segmentBegin * segmentPrescanMaxMultiplier);
        float segmentEnd = round(thisSegMeta.segmentEnd * segmentPrescanMaxMultiplier);
        float segmentLengthMultiplier = segmentEnd - segmentBegin;
        if (segmentLengthMultiplier < 0.1)
        {
            break;
        }
 
        float segmentLength = segmentLengthMultiplier * segmentUnitLength;
        uint segmentSampleCount = max(0,ceil(segmentLength * rcp(sampleCfg.directionSampleStepSize)));
        BoundsHitResult segmentBoundsHit;
        segmentBoundsHit.distToBounds = boundsHit.distToBounds + segmentUnitLength * segmentBegin;
        segmentBoundsHit.distInsideBounds = segmentLength;
        RayMarchingContext rayMarchingContext = GetRayMarchingContext(ray,segmentBoundsHit,segmentSampleCount);
        RayMarchingResult segmentResult = CloudVolumeRayMarching(rayMarchingContext);
        result.luminance += result.transmittance*segmentResult.luminance;
        result.transmittance *= segmentResult.transmittance;
        result.density += segmentResult.density;
        result.distance += segmentResult.distance;
        if (result.transmittance.r < 0.01)
        {
            break;
        }
    }
    return PackVolumetricCloudRayMarchingResult(result,viewDirection);
}


float4 ShadeVolumetricCloudHemiOctahedron(float3 cameraPosWS,float3 viewDirection)
{
    Ray ray;
    ray.origin = cameraPosWS;
    ray.direction = viewDirection;
    ray.hitPoint = viewDirection*10000000;
    BoundingSphere sphere = GetCloudVolumeBoundingSphere();
    RayMarchingResult result = CloudVolumeRayMarching(ray,sphere);
    return PackVolumetricCloudRayMarchingResult(result,viewDirection);
}

#endif
```
