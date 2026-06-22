# Egaku source: `Shaders/Sky/Cloud/Volumetric/Volume/OctahedronVolume.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef OCTAHEDRON_VOLUME_HLSL
#define OCTAHEDRON_VOLUME_HLSL


float OctahedronVolume_LowerSliceBound();
float2 OctahedronVolume_DirectionDimensions();
float OctahedronVolume_DirectionSliceCount();
float OctahedronVolume_SliceThickness();

float3 OctahedronVolume_CenterPoint()
{
    return float3(0,0,0);
}

float3 OctahedronVolume_GetPositionDirection(float3 position)
{
    return normalize(position - OctahedronVolume_CenterPoint());
}

float OctahedronVolume_GetPositionDirectionDepth(float3 position)
{
    float depth = distance(position,OctahedronVolume_CenterPoint()) - OctahedronVolume_LowerSliceBound();
    return depth;
}

float3 OctahedronVolume_GetDirectionFromSampleCoords(float2 uv)
{
    float2 uv2 = uv * 2.0 - 1.0;
    uv2 = float2(uv2.x + uv2.y, uv2.x - uv2.y)*0.5;
    float3 n = float3(uv2,1-dot(1.0,abs(uv2)));
    return normalize(n).xzy;
}

float2 OctahedronVolume_GetDirectionSampleCoords(float3 direction)
{
    direction.xz /= dot(1.0, abs(direction));
    return float2(direction.x +direction.z, direction.x - direction.z) * 0.5 + 0.5;
}

float3 OctahedronVolume_GetDirection(float2 coords)
{
    float2 duv = rcp(OctahedronVolume_DirectionDimensions());
    return OctahedronVolume_GetDirectionFromSampleCoords(duv * (coords + 0.5));
}


float2 OctahedronVolume_GetDirectionCoords(float3 direction)
{
    float2 uv = OctahedronVolume_GetDirectionSampleCoords(direction);
    float2 dims = OctahedronVolume_DirectionDimensions();
    return uv * dims;
}

float OctahedronVolume_GetSliceIndex(float directionDepth)
{
    float thickness = OctahedronVolume_SliceThickness();
    return directionDepth * rcp(thickness); 
}

float OctahedronVolume_GetSliceCoord(float directionDepth)
{
    return OctahedronVolume_GetSliceIndex(directionDepth) + 0.5;
}

float OctahedronVolume_GetSliceSampleCoord(float sliceCoord)
{
    float delta = rcp(OctahedronVolume_DirectionSliceCount());
    return sliceCoord * delta;
}

float OctahedronVolume_GetSliceSampleCoordFromDirectionDepth(float directionDepth)
{
    return OctahedronVolume_GetSliceSampleCoord(OctahedronVolume_GetSliceCoord(directionDepth));
}

float OctahedronVolume_GetSliceCoord(float3 position)
{
    return OctahedronVolume_GetSliceCoord(OctahedronVolume_GetPositionDirectionDepth(position));
}

float3 OctahedronVolume_GetVoxelCoords(float3 position)
{
    float d = OctahedronVolume_GetSliceCoord(position);
    float2 dirCoords = OctahedronVolume_GetDirectionCoords(OctahedronVolume_GetPositionDirection(position));
    return float3(dirCoords,d);
}

float3 OctahedronVolume_GetVoxelSampleCoords(float3 position)
{
    float3 direction = OctahedronVolume_GetPositionDirection(position);
    float2 uv = OctahedronVolume_GetDirectionSampleCoords(direction);
    float w = OctahedronVolume_GetSliceSampleCoord(OctahedronVolume_GetSliceCoord(position));
    return float3(uv,w);
}


float3 OctahedronVolume_GetVoxelSampleCoords(float3 direction,float sliceCoord)
{
    float2 uv = OctahedronVolume_GetDirectionSampleCoords(direction);
    float w = OctahedronVolume_GetSliceSampleCoord(sliceCoord);
    return float3(uv,w);
}

#endif
```
