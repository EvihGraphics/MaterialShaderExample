# Egaku source: `Shaders/OIT/AVBOIT/VirtualBlockBased/Passes/VolumeOccupancyCollectionPass.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef VOLUME_OCCUPANCY_COLLECTION_PASS_HLSL
#define  VOLUME_OCCUPANCY_COLLECTION_PASS_HLSL

#include "../Utils/CommonUtils.hlsl"
#define AVBOIT_USE_VOLUME_BLOCK_OCCUPANCY_BUFFER_UAV
#include "Buffers/VolumeBlockOccupancyBuffer.hlsl"
#define  AVBOIT_USE_VOLUME_EXTINCTION_BUFFER_SRV
#include "Buffers/VolumeExtinctionBuffer.hlsl"
#include "Volume/Volume.hlsl"

float2 _AVBOIT_OccupancyCollectionResolutions;

#pragma enable_d3d11_debug_symbols


// definition for predeclared interface
#ifndef AVBOIT_GET_DIMENSIONS_FROM_SHADER_CONSTANTS
uint3 PhysicalVolumeConfig_GetVolumeDimensions()
{
    return VEBuffer_GetDimensions();
}

uint3 PhysicalVolumeConfig_GetVolumeBlockDimensions()
{
    uint2 dimsXY = PhysicalVolumeConfig_GetVolumeDimensions().xy;
    uint sliceCount = FrustumVolume_GetVolumeSliceCount();
    uint slicePerBlock = sliceCount / VirtualVolumeConfig_GetVolumeDimensionsInBlock().z;
    return uint3(dimsXY,slicePerBlock);
}

uint3 VirtualVolumeConfig_GetVolumeDimensionsInBlock()
{
    return VBOBuffer_GetDimensions();
}

uint3 VirtualVolumeConfig_GetVolumeBlockDimensions()
{
    return PhysicalVolumeConfig_GetVolumeBlockDimensions();
}
#endif



struct Attributes {
    float4 vertex : POSITION;
};

struct Varyings {
    float4 position : SV_POSITION;
};

Varyings Vert(Attributes input) {
    Varyings output;
    output.position = TransformObjectToHClip(input.vertex);
    return output;
}

float4 Frag(Varyings input) : SV_Target {
    float viewDepth = GetLinearEyeDepthFromRawZ(input.position.z);
    
    float2 screenUV = input.position.xy / _AVBOIT_OccupancyCollectionResolutions;
    float3 virtualVoxelCoords = VirtualVolume_GetVoxelCoordsFromUVAndViewDepth(screenUV, viewDepth);
    float3 virtualBlockCoords = VirtualVolume_GetVolumeBlockCoords(virtualVoxelCoords);
    float sliceIndex = virtualVoxelCoords.z;
    VolumeSliceAccessDesc desc = GetVolumeSliceAccessDesc(sliceIndex);
    float volumeBlockSliceLower = VirtualVolume_GetVolumeBlockSliceIndex(desc.sliceIndexLower);
    float volumeBlockSliceUpper = VirtualVolume_GetVolumeBlockSliceIndex(desc.sliceIndexUpper);
    uint3 virtualBlockCoordsLower = uint3(virtualBlockCoords.xy,volumeBlockSliceLower);
    uint3 virtualBlockCoordsUpper = uint3(virtualBlockCoords.xy,volumeBlockSliceUpper);
    VBOBuffer_SetVolumeBlockOccupied(virtualBlockCoordsLower);
    if (any( virtualBlockCoordsLower!=virtualBlockCoordsUpper)){
        VBOBuffer_SetVolumeBlockOccupied(virtualBlockCoordsUpper);
    }
    return 0;
}


#endif
```
