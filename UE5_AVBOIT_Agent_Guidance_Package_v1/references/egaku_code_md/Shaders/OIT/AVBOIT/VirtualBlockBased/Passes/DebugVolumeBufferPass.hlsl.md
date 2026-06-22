# Egaku source: `Shaders/OIT/AVBOIT/VirtualBlockBased/Passes/DebugVolumeBufferPass.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once

#ifndef DEBUG_VOLUME_BUFFER_PASS_HLSL
#define DEBUG_VOLUME_BUFFER_PASS_HLSL
#include "../../Utils/CommonUtils.hlsl"
#define AVBOIT_USE_VOLUME_TRANSMITTANCE_LUT_SRV
#include "../Buffers/VolumeTransmittanceLut.hlsl"
#define AVBOIT_USE_VOLUME_BLOCK_LUT_SRV
#include "../Buffers/VolumeBlockLut.hlsl"
#define AVBOIT_USE_VOLUME_BLOCK_OCCUPANCY_BUFFER_SRV
#include "../Buffers/VolumeBlockOccupancyBuffer.hlsl"
#include "../AVBOIT.ParamStruct.hlsl"
#include "../Volume/Volume.hlsl"

#pragma enable_d3d11_debug_symbols



struct Attributes {
    float4 vertex : POSITION;
};

struct Varyings {
    float4 position : SV_POSITION;
};


float2 GetScreenUVFromHClipPosition(float4 positionHCS)
{
    float2 screenUV = positionHCS.xy / positionHCS.w;
    screenUV.x = screenUV.x * 0.5 + 0.5;
    screenUV.y = -screenUV.y * 0.5 + 0.5;
    return screenUV;
}

Varyings Vert(Attributes input) {
    Varyings output;
    output.position = TransformObjectToHClip(input.vertex);
    return output;
}

#ifndef AVBOIT_GET_DIMENSIONS_FROM_SHADER_CONSTANT
// definition for predeclared interface
uint3 PhysicalVolumeConfig_GetVolumeDimensions()
{
    return VTLut_GetDimensions();
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
    return VBLut_GetDimensions();
}

uint3 VirtualVolumeConfig_GetVolumeBlockDimensions()
{
    return PhysicalVolumeConfig_GetVolumeBlockDimensions();
}
#endif


float PhysicalVolume_TransformVirtualVolumeBlockIndexToPhysical(float virtualVolumeBlockIndex)
{
    float3 coords = VirtualVolume_GetVolumeBlockCoordsFromBlockIndex(virtualVolumeBlockIndex);
    return VBLut_SamplePhysicalBlockDesc(coords).blockIndex;
}

float4 Frag(Varyings input) : SV_Target {
    float viewDepth = GetLinearEyeDepthFromRawZ(input.position.z);
    float2 screenCoords = input.position.xy;
    float2 voxelCoordsXY = VirtualVolume_GetVoxelCoordsXYFromUV(screenCoords);
    float sliceIndex = VirtualVolume_GetVolumeSliceIndex(viewDepth);
    VolumeSliceAccessDesc desc = GetVolumeSliceAccessDesc(sliceIndex);
    float3 volumeBlockCoords = VirtualVolume_GetVolumeBlockCoords(float3(voxelCoordsXY,sliceIndex));
    bool occupied = VBOBuffer_GetVolumeBlockOccupancy(volumeBlockCoords).occupied;
    return float4(occupied,0,0,1);
}


#endif
```
