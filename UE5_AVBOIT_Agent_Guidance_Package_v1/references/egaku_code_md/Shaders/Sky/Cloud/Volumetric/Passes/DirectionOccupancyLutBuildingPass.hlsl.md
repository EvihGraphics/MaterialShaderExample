# Egaku source: `Shaders/Sky/Cloud/Volumetric/Passes/DirectionOccupancyLutBuildingPass.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef DIRECTION_OCCUPANCY_LUT_BUILDING_PASS_HLSL
#define DIRECTION_OCCUPANCY_LUT_BUILDING_PASS_HLSL

#include "Assets/Shaders/Sky/Basic/VolumeBase/VolumeRayMarching.hlsl"
#include "../CloudVolume/CloudVolume.hlsl"
#include "Assets/Shaders/Sky/Basic/Utils.hlsl"
#include "../Volume/OctahedronVolume.hlsl"

//#pragma enable_d3d11_debug_symbols

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

uint _VolumetricCloud_SegmentTestNormalIteration;


float4 RayMarching(Ray ray,BoundingSphere sphere)
{
    BoundsHitResult boundsHit = CalcBoundsHit(sphere,ray);
    uint iterationNormal = _VolumetricCloud_SegmentTestNormalIteration;
    SphereVolumeSampleConfig sampleConfig = GetSphereVolumeSampleConfig(boundsHit,sphere,iterationNormal,iterationNormal*3);
    uint viewSampleCount = sampleConfig.directionSampleCount;
    RayMarchingContext rayMarchingContext = GetRayMarchingContext(ray,boundsHit,viewSampleCount);
    SamplePoint samplePoint;
    samplePoint.position = rayMarchingContext.entryPosition;
    samplePoint.stepSize = rayMarchingContext.stepSize;
    samplePoint.direction = rayMarchingContext.direction;
    [loop]
    for (uint i = 0; i < viewSampleCount ; i++)
    {
        VoxelData voxelData = Volume_SampleVoxelData(samplePoint);
        if (voxelData.density > 0.01)
        {
            return 1;
        }
        samplePoint.position += samplePoint.direction * samplePoint.stepSize;
    }
    return 0;
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
    return RayMarching(ray, sphere);
}


#endif
```
