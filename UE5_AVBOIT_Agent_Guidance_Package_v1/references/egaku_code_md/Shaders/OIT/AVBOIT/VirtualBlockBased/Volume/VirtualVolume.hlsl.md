# Egaku source: `Shaders/OIT/AVBOIT/VirtualBlockBased/Volume/VirtualVolume.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef VIRTUAL_VOLUME_HLSL
#define VIRTUAL_VOLUME_HLSL
#include "VirtualVolumeConfigInterface.hlsl"
#include "FrustumVolume.hlsl"

uint VirtualVolume_GetVolumeBlockCount()
{
    uint3 dims = VirtualVolumeConfig_GetVolumeDimensionsInBlock();
    return dims.x*dims.y*dims.z;
}

uint3 VirtualVolume_GetVolumeDimensions()
{
    return VirtualVolumeConfig_GetVolumeBlockDimensions() * VirtualVolumeConfig_GetVolumeDimensionsInBlock();
}

float VirtualVolume_GetVolumeSliceIndex(float viewDepth)
{
    uint3 dims = VirtualVolume_GetVolumeDimensions();
    float sliceIndex = FrustumVolume_GetVolumeSliceIndex(viewDepth,dims.z);
    return sliceIndex;
}

float2 VirtualVolume_GetVoxelCoordsXYFromUV(float2 uv)
{
    uint3 dims = VirtualVolume_GetVolumeDimensions();
    float2 coordsXY = uv * (dims.xy - 1);
    return coordsXY;
}

float3 VirtualVolume_GetVoxelCoordsFromUVAndViewDepth(float2 uv,float viewDepth)
{
    float sliceIndex = VirtualVolume_GetVolumeSliceIndex(viewDepth);
    float2 coordsXY = VirtualVolume_GetVoxelCoordsXYFromUV(uv);
    return float3(coordsXY,sliceIndex);
}

float3 VirtualVolume_GetVolumeBlockCoords(float3 voxelCoords)
{
    uint3 dims = VirtualVolumeConfig_GetVolumeBlockDimensions();
    return voxelCoords*rcp(dims);
}

float VirtualVolume_GetVolumeBlockSliceIndex(float voxelSliceIndex)
{
    uint slicePerBlock = VirtualVolumeConfig_GetVolumeBlockDimensions().z;
    return voxelSliceIndex*rcp(slicePerBlock);
}

float VirtualVolume_GetVolumeBlockIndex(float3 blockCoords)
{
    uint3 dims = VirtualVolumeConfig_GetVolumeDimensionsInBlock();
    float depthFraction = blockCoords.z - floor(blockCoords.z);  // fraction for depth percentage position in a block
    float index = dot(uint3(blockCoords),uint3(dims.z,dims.z*dims.x,1)) + depthFraction;
    return index;
}

float3 VirtualVolume_GetVolumeBlockCoordsFromBlockIndex(float blockIndex)
{
    uint3 dims = VirtualVolumeConfig_GetVolumeDimensionsInBlock();
    float blockSliceIndex = fmod(blockIndex,dims.z);
    uint blockTubeIndex = uint(blockIndex)/dims.z;
    uint blockLayerIndex = blockTubeIndex/dims.x;
    return float3(blockTubeIndex%dims.x,blockLayerIndex,blockSliceIndex);
}


#endif
```
