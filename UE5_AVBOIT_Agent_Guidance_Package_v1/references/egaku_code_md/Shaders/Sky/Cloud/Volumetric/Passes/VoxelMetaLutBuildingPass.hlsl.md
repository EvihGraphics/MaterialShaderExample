# Egaku source: `Shaders/Sky/Cloud/Volumetric/Passes/VoxelMetaLutBuildingPass.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef VOXEL_META_LUT_BUILDING_PASS_HLSL
#define VOXEL_META_LUT_BUILDING_PASS_HLSL

#include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Lighting.hlsl"
#include "Assets/Shaders/Sky/Basic/VolumeBase/VolumeRayMarching.hlsl"
#include "../CloudVolume/CloudVolume.hlsl"
#include "Assets/Shaders/Sky/Basic/Utils.hlsl"
#define VOLUMETRIC_CLOUD_USE_VOXEL_META_LUT_UAV
#include "../Buffers/VoxelMetaLut.hlsl"
#include "../Volume/OctahedronVolume.hlsl"
//#pragma enable_d3d11_debug_symbols


uint _VolumetricCloud_SegmentTestNormalIteration;


struct Attributes {
    float4 vertex : POSITION;
    float2 uv : TEXCOORD0;
};

struct Varyings {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD1;
};

Varyings Vert(Attributes input) {
    Varyings output;
    output.position = TransformObjectToHClip(input.vertex);
    output.uv = input.uv;
    return output;
}

static BoundsHitResult currentDirectionBoundsHit;

float2 OctahedronVolume_DirectionDimensions()
{
    return VoxelMetaLut_GetDimensions().xy;
}

float OctahedronVolume_DirectionSliceCount()
{
    return VoxelMetaLut_GetDimensions().z;
}

float OctahedronVolume_SliceThickness()
{
    BoundingSphere sphere = GetCloudVolumeBoundingSphere();
    uint iterationNormal = FetchParam(VolumeRayMarchingDesc).viewDirectionIteration;
    return (sphere.radiusMax - sphere.radiusMin) / iterationNormal;
}


float OctahedronVolume_LowerSliceBound()
{
    return currentDirectionBoundsHit.distToBounds;
}

float CalcSunDirectionTransmittance(SamplePoint samplePoint,BoundingSphere sphere)
{
    Ray sunRay;
    sunRay.direction = GetMainLight().direction;
    sunRay.origin = samplePoint.position;
    sunRay.hitPoint = sunRay.origin + sunRay.direction * 1000000;
    BoundsHitResult boundsHit = CalcBoundsHit(sphere,sunRay);
    RayMarchingContext sunDirMarchingContext = GetRayMarchingContext(sunRay,boundsHit,4);
    float sunTransmittance = VolumeRayMarching_CalcTransmittanceOnly(sunDirMarchingContext).transmittance;
    return sunTransmittance;
}

float CalcSunDirectionDensity(SamplePoint samplePoint,BoundingSphere sphere)
{
    Ray sunRay;
    sunRay.direction = GetMainLight().direction;
    sunRay.origin = samplePoint.position;
    sunRay.hitPoint = sunRay.origin + sunRay.direction * 1000000;
    BoundsHitResult boundsHit = CalcBoundsHit(sphere,sunRay);
    RayMarchingContext sunDirMarchingContext = GetRayMarchingContext(sunRay,boundsHit,20,0.5);

    float sunDensity = -log(VolumeRayMarching_CalcTransmittanceOnly(sunDirMarchingContext).transmittance);
    return  sunDensity;
}


SamplePoint VolumeRayMarching_CalcNextSamplePoint(SamplePoint samplePoint,VoxelData voxelData,RayMarchingContext context,RayMarchingResult currentResult)
{
    samplePoint.position += samplePoint.direction * samplePoint.stepSize;
    return samplePoint;
}


void RayMarching(uint3 voxelPosBegin,Ray ray,BoundingSphere sphere)
{
    BoundsHitResult boundsHit = CalcBoundsHit(sphere,ray);
    currentDirectionBoundsHit = boundsHit;
    uint segmentCount = OctahedronVolume_DirectionSliceCount();
    uint iterationNormal = _VolumetricCloud_SegmentTestNormalIteration;
    SphereVolumeSampleConfig sampleConfig = GetSphereVolumeSampleConfig(boundsHit,sphere,iterationNormal,iterationNormal*3);
    uint viewSampleCount = sampleConfig.directionSampleCount;
    
    RayMarchingContext rayMarchingContext = GetRayMarchingContext(ray,boundsHit,viewSampleCount);
    
    SamplePoint samplePoint;
    samplePoint.position = rayMarchingContext.entryPosition;
    samplePoint.stepSize = rayMarchingContext.stepSize;
    samplePoint.direction = rayMarchingContext.direction;
    VoxelMeta entry;
    entry.flag = 0;
    entry.density = 0;
    entry.viewRadianceTransfer = 0;
    entry.sunTransmittance = 0;
    
    float lastPosMultiplier = 0;
    [loop]
    for (uint i = 0; i < segmentCount ; i++)
    {
        float3 volumeSegCoords = voxelPosBegin + uint3(0,0,i);
        float segmentBegin = 0;
        float segmentEnd = 0;
        bool inSegment = false;
        [loop]
        for (uint j = lastPosMultiplier; j < viewSampleCount; j++)
        {
            float thisPosMultiplier = j;
            samplePoint.position = rayMarchingContext.entryPosition + thisPosMultiplier*samplePoint.direction * samplePoint.stepSize;
            VoxelData voxelData = Volume_SampleVoxelData(samplePoint);
            if (inSegment)
            {
                segmentEnd = thisPosMultiplier;
            }else
            {
                segmentBegin = lastPosMultiplier;
            }
            lastPosMultiplier = thisPosMultiplier;
            if (voxelData.density > 0.0001f)
            {
                if (inSegment == false)
                {
                    inSegment = true;
                }
            }else{
                if (inSegment == true)
                {
                    break;
                }
            }
        }
        if (inSegment == false)
        {
            entry.segmentBegin = viewSampleCount;
            entry.segmentEnd = 0;
        }else
        {
            entry.segmentBegin = segmentBegin*rcp(viewSampleCount); // normalized
            entry.segmentEnd = segmentEnd*rcp(viewSampleCount); // normalized
            VoxelMeta oldEntry = VoxelMetaLut_Read(volumeSegCoords);
            if (oldEntry.segmentEnd - oldEntry.segmentBegin > 0.01f)
            {
                entry.segmentBegin = min(entry.segmentBegin,oldEntry.segmentBegin);
                entry.segmentEnd = max(entry.segmentEnd,oldEntry.segmentEnd);
            }
        }
        VoxelMetaLut_InterlockedWrite(volumeSegCoords,entry);
    }
}



float4 Frag(Varyings input) : SV_Target {
    float3 viewHit = GetWorldPosition(input.uv);
    BoundingSphere sphere = GetCloudVolumeBoundingSphere();
    Ray ray;
    ray.direction = OctahedronVolume_GetDirectionFromSampleCoords(input.uv);
    ray.origin = OctahedronVolume_CenterPoint();
    ray.hitPoint = viewHit;
    if (ray.direction.y < 0.03){
        return 0;
    }
    uint3 voxelPosBegin;
    voxelPosBegin.xy = OctahedronVolume_GetDirectionCoords(ray.direction);
    voxelPosBegin.z = 0;
    RayMarching(voxelPosBegin,ray, sphere);

    /*
    float2 duv = rcp(OctahedronVolume_DirectionDimensions());
    float2 uv1 = input.uv + float2(0.25,0.25)*duv;
    float2 uv2 = input.uv + float2(-0.25,0.25)*duv;
    float2 uv3 = input.uv + float2(0.25,-0.25)*duv;
    float2 uv4 = input.uv + float2(-0.25,-0.25)*duv;
    */
    
    /*ray.direction = OctahedronVolume_GetDirectionFromSampleCoords(uv1);
    RayMarching(voxelPosBegin,ray, sphere);
    ray.direction = OctahedronVolume_GetDirectionFromSampleCoords(uv2);
    RayMarching(voxelPosBegin,ray, sphere);
    ray.direction = OctahedronVolume_GetDirectionFromSampleCoords(uv3);
    RayMarching(voxelPosBegin,ray, sphere);
    ray.direction = OctahedronVolume_GetDirectionFromSampleCoords(uv4);
    RayMarching(voxelPosBegin,ray, sphere);*/
    
    
    
    return 1;
}


#endif
```
