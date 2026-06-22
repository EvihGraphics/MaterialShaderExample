# Egaku source: `Shaders/OIT/AVBOIT/VirtualBlockBased/Buffers/VolumeExtinctionBuffer.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef VOLUME_EXTINCTION_BUFFER_HLSL
#define VOLUME_EXTINCTION_BUFFER_HLSL

#include "Volume/VolumeUtils.hlsl"
#include "Configs/VolumeConfig.hlsl"

#ifdef AVBOIT_USE_VOLUME_EXTINCTION_BUFFER_UAV
#define VOLUME_EXTINCTION_BUFFER_DEFINED
RWTexture3D<uint> _AVBOIT_VolumeExtinctionBuffer : register(u2);
void VEBuffer_AddRawValue(uint value, uint3 coords);
void VEBuffer_SetRawValue(uint value, uint3 coords);
#else

#ifdef AVBOIT_USE_VOLUME_EXTINCTION_BUFFER_SRV
#define VOLUME_EXTINCTION_BUFFER_DEFINED
Texture3D<uint> _AVBOIT_VolumeExtinctionBuffer;
#endif

#endif

uint VEBuffer_GetRawValue(uint3 coords);
uint3 VEBuffer_GetDimensions();

#ifdef AVBOIT_USE_VOLUME_EXTINCTION_BUFFER_UAV
void VEBuffer_AddRawValue(uint value, uint3 coords)
{
    InterlockedAdd(_AVBOIT_VolumeExtinctionBuffer[coords], value);
}

void VEBuffer_SetRawValue(uint value, uint3 coords)
{
    _AVBOIT_VolumeExtinctionBuffer[coords] = value;
}
#endif

#ifdef VOLUME_EXTINCTION_BUFFER_DEFINED
uint3 VEBuffer_GetDimensions()
{
    #ifdef AVBOIT_GET_DIMENSIONS_FROM_SHADER_CONSTANTS
    return AVBOIT_GetVolumeConfig().physicalVolumeDimensions;
    #else
	
    uint x = 0,y = 0,z = 0;
    _AVBOIT_VolumeExtinctionBuffer.GetDimensions(x,y,z);
    return uint3(x,y,z);
    #endif
    
}

uint VEBuffer_GetRawValue(uint3 coords)
{
    return _AVBOIT_VolumeExtinctionBuffer[coords];
}
#endif

float VEBuffer_UnpackExtinctionFromUint32(uint packedExtinction)
{
    const float maxExtinction = 3;
    uint maxBit = 0x000000ff;
    float ext = maxExtinction * ((float)packedExtinction/(float)maxBit);
    return ext;
}

float3 VEBuffer_UnpackExtinctionRGBFromUint32(uint packedExtinction)
{
    const float3 maxExtinction = float3(3,3,3);
    uint maxBit = 0x000000ff;
    uint3 packedRGB = 0;
    packedRGB.r = packedExtinction & maxBit;
    packedRGB.g = (packedExtinction >> 8) & maxBit;
    packedRGB.b = (packedExtinction >> 16) & maxBit;
    float3 ext = maxExtinction * ((float3)packedRGB/(float)maxBit);
    return ext;
}

uint VEBuffer_PackExtinctionToUint32(float extinction)
{
    const float maxExtinction = 3;
    uint maxBit = 0x000000ff;
    uint value = maxBit * (float)(extinction/maxExtinction);
    return value;
}

uint VEBuffer_PackExtinctionToUint32(float3 extinction)
{
    const float3 maxExtinction = float3(3,3,3);
    uint maxBit = 0x000000ff;
    uint3 valueRGB = maxBit * (float3)(extinction/maxExtinction);
    uint value = valueRGB.r | (valueRGB.g << 8) | (valueRGB.b << 16);
    return value;
}

float VEBuffer_GetVoxelExtinction(uint3 coords)
{
    return VEBuffer_UnpackExtinctionFromUint32(VEBuffer_GetRawValue(coords));
}

float3 VEBuffer_GetVoxelExtinctionRGB(uint3 coords)
{
    return VEBuffer_UnpackExtinctionRGBFromUint32(VEBuffer_GetRawValue(coords));
}


#ifdef AVBOIT_USE_VOLUME_EXTINCTION_BUFFER_UAV
void VEBuffer_AddVoxelExtinction(float extinctionValue, uint3 coords)
{
    uint extinctionValueUint32 = VEBuffer_PackExtinctionToUint32(extinctionValue);
    VEBuffer_AddRawValue(extinctionValueUint32, coords);
}

void VEBuffer_AddVoxelExtinction(float3 extinctionValue, uint3 coords)
{
    uint extinctionValueUint32 = VEBuffer_PackExtinctionToUint32(extinctionValue);
    VEBuffer_AddRawValue(extinctionValueUint32, coords);
}


void VEBuffer_AddVoxelExtinctionLinear(float extinctionValue, uint2 coordsXY, VolumeSliceAccessDesc sliceAccessDesc)
{
    float value1 = extinctionValue * sliceAccessDesc.lowerFraction;
    float value2 = extinctionValue * sliceAccessDesc.upperFraction;
    VEBuffer_AddVoxelExtinction(value1,uint3(coordsXY,sliceAccessDesc.sliceIndexLower));
    VEBuffer_AddVoxelExtinction(value2,uint3(coordsXY,sliceAccessDesc.sliceIndexUpper));
}

void VEBuffer_AddVoxelExtinctionLinear(float3 extinctionValue, uint2 coordsXY, VolumeSliceAccessDesc sliceAccessDesc)
{
    float3 value1 = extinctionValue * sliceAccessDesc.lowerFraction;
    float3 value2 = extinctionValue * sliceAccessDesc.upperFraction;
    VEBuffer_AddVoxelExtinction(value1,uint3(coordsXY,sliceAccessDesc.sliceIndexLower));
    VEBuffer_AddVoxelExtinction(value2,uint3(coordsXY,sliceAccessDesc.sliceIndexUpper));
}
#endif

#endif
```
