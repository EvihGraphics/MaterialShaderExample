# Egaku source: `Shaders/OIT/AVBOIT/VirtualBlockBased/Volume/PhysicalVolume.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef PHYSICAL_VOLUME_HLSL
#define PHYSICAL_VOLUME_HLSL
#include "PhysicalVolumeConfigInterface.hlsl"

uint3 PhysicalVolume_GetVolumeDimensionsInBlock()
{
    return PhysicalVolumeConfig_GetVolumeDimensions()/PhysicalVolumeConfig_GetVolumeBlockDimensions();
}

uint PhysicalVolume_GetVolumeBlockCount()
{
    uint3 dims = PhysicalVolume_GetVolumeDimensionsInBlock();
    return dims.x*dims.y*dims.z;
}

float2 myFmod(float2 x,float2 y)
{
    float2 divXY = x/y;
    float2 i = floor(divXY);
    float2 fracDivXY = divXY - i; 
    return y * fracDivXY;
}

float2 myFmod2(float2 x,float2 y)
{
    float2 divXY = x/y;
    float2 i = floor(divXY);
    return x - i * y;
}

float2 PhysicalVolume_GetVoxelCoordsXYFromVirtual(float2 virtualVoxelCoords)
{

    uint3 dims = PhysicalVolumeConfig_GetVolumeDimensions();
    float2 blockXY = dims.xy;

    // use following codes to calculate fmod instead of calling the intrinsic function.
    float2 division = virtualVoxelCoords/blockXY;
    float2 coordsXY = virtualVoxelCoords - blockXY * floor(division);
    /*if (any(uint2(coordsXY) != uint2(virtualVoxelCoords)))
    {
        return float2(999,999);
    }*/

    return coordsXY;
}

float PhysicalVolume_GetVolumeBlockIndexFromVirtual(float volumeBlockIndex)
{
    return PhysicalVolume_TransformVirtualVolumeBlockIndexToPhysical(volumeBlockIndex);
}

float PhysicalVolume_GetVolumeSliceIndex(float volumeBlockIndex)
{
    uint3 dims = PhysicalVolumeConfig_GetVolumeBlockDimensions();
    return volumeBlockIndex*dims.z;
}

float PhysicalVolume_GetVolumeSliceIndexFromVirtual(float volumeBlockIndex)
{
    float blockIndex = PhysicalVolume_GetVolumeBlockIndexFromVirtual(volumeBlockIndex);
    return PhysicalVolume_GetVolumeSliceIndex(blockIndex);
}






#endif
```
