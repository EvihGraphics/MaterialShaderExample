# Egaku source: `Shaders/OIT/AVBOIT/VirtualBlockBased/Buffers/VolumeBlockOccupancyBuffer.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef VOLUME_BLOCK_OCCUPANCY_BUFFER_HLSL
#define VOLUME_BLOCK_OCCUPANCY_BUFFER_HLSL
#include "Configs/VolumeConfig.hlsl"

struct VolumeBlockOccupancyBufferElem
{
    bool occupied;
};

struct VolumeBlockOccupancy
{
    bool occupied;
};


#ifdef AVBOIT_USE_VOLUME_BLOCK_OCCUPANCY_BUFFER_UAV
#define AVBOIT_VOLUME_BLOCK_OCCUPANCY_BUFFER_DEFINED
RWTexture3D<float4> _AVBOIT_VolumeBlockOccupancyBuffer : register(u1);
#else

#ifdef AVBOIT_USE_VOLUME_BLOCK_OCCUPANCY_BUFFER_SRV
#define AVBOIT_VOLUME_BLOCK_OCCUPANCY_BUFFER_DEFINED
Texture3D<float4> _AVBOIT_VolumeBlockOccupancyBuffer;
#endif
#endif

uint3 VBOBuffer_GetDimensions();
VolumeBlockOccupancy VBOBuffer_GetVolumeBlockOccupancy(uint3 coords);

VolumeBlockOccupancyBufferElem VBOBuffer_UnpackValue(float4 value)
{
    VolumeBlockOccupancyBufferElem elem;
    elem.occupied = value.r;
    return elem;
}

float4 VBOBuffer_GetPackedValue(VolumeBlockOccupancyBufferElem elem)
{
    float4 packed = float4(elem.occupied, 0, 0, 0);
    return packed;
}

#ifdef AVBOIT_VOLUME_BLOCK_OCCUPANCY_BUFFER_DEFINED
VolumeBlockOccupancy VBOBuffer_GetVolumeBlockOccupancy(uint3 coords)
{
    VolumeBlockOccupancyBufferElem elem = VBOBuffer_UnpackValue(_AVBOIT_VolumeBlockOccupancyBuffer[coords]);
    VolumeBlockOccupancy r;
    r.occupied = elem.occupied;
    return r;
}
#endif

#ifdef AVBOIT_USE_VOLUME_BLOCK_OCCUPANCY_BUFFER_UAV 
void VBOBuffer_SetVolumeBlockOccupied(uint3 coords)
{
    VolumeBlockOccupancyBufferElem elem;
    elem.occupied = true;
    _AVBOIT_VolumeBlockOccupancyBuffer[coords] = VBOBuffer_GetPackedValue(elem);
}

void VBOBuffer_ClearVolumeBlockOccupancy(uint3 coords)
{
    VolumeBlockOccupancyBufferElem elem;
    elem.occupied = false;
    _AVBOIT_VolumeBlockOccupancyBuffer[coords] = VBOBuffer_GetPackedValue(elem);
}
#endif

#ifdef AVBOIT_VOLUME_BLOCK_OCCUPANCY_BUFFER_DEFINED
uint3 VBOBuffer_GetDimensions()
{

    #ifdef AVBOIT_GET_DIMENSIONS_FROM_SHADER_CONSTANTS
    return AVBOIT_GetVolumeConfig().volumeBlockDivisor;
    #else
	
    uint x,y,z;
    _AVBOIT_VolumeBlockOccupancyBuffer.GetDimensions(x,y,z);
    return uint3(x,y,z);
    #endif
}
#endif

#endif
```
