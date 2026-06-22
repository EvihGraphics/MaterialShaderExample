# Egaku source: `Shaders/OIT/AVBOIT/VirtualBlockBased/Passes/TranslucencyForwardPass.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef AVBOIT_TRANSLUCENCY_FORWARD_PASS_HLSL
#define AVBOIT_TRANSLUCENCY_FORWARD_PASS_HLSL

#include "../../Utils/CommonUtils.hlsl"
#define AVBOIT_USE_VOLUME_TRANSMITTANCE_LUT_SRV
#include "../Buffers/VolumeTransmittanceLut.hlsl"
#define AVBOIT_USE_VOLUME_BLOCK_LUT_SRV
#include "../Buffers/VolumeBlockLut.hlsl"
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

// definition for predeclared interface
#ifndef AVBOIT_GET_DIMENSIONS_FROM_SHADER_CONSTANTS
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
    float2 screenUV = GetNormalizedScreenSpaceUV(input.position);
    
    float4 baseColor = FetchParam(TranslucencyDesc).baseColor;
    float2 virtualVoxelCoordsXY = VirtualVolume_GetVoxelCoordsXYFromUV(screenUV);
    float virtualSliceIndex = VirtualVolume_GetVolumeSliceIndex(viewDepth);
    float3 virtualVoxelCoords = float3(virtualVoxelCoordsXY, virtualSliceIndex);
    float3 virtualBlockCoords = VirtualVolume_GetVolumeBlockCoords(virtualVoxelCoords);
    
    float virtualVolumeBlockIndex = VirtualVolume_GetVolumeBlockIndex(virtualBlockCoords);

    float2 voxelCoordsXY = PhysicalVolume_GetVoxelCoordsXYFromVirtual(virtualVoxelCoordsXY);
    float sliceIndex = PhysicalVolume_GetVolumeSliceIndexFromVirtual(virtualVolumeBlockIndex);
    //float sliceIndexMin = PhysicalVolume_GetVolumeSliceIndexFromVirtual(virtualVolumeBlockIndexMin);
    float sliceBias = -2.5;
    uint blockSliceIndexEntry = VBLut_GetMinPhysicalBlockIndexOfDepthDirection(virtualBlockCoords);
    uint sliceIndexEntry = PhysicalVolume_GetVolumeSliceIndex(blockSliceIndexEntry);

    float sliceIndexFront = max(sliceIndex + sliceBias,sliceIndexEntry);
    
    float transmittanceFront = VTLut_SampleVolumeTransmittance(float3(voxelCoordsXY, sliceIndexFront));
    float3 finalLuminance = baseColor * transmittanceFront * baseColor.a;
    return float4(finalLuminance,transmittanceFront * baseColor.a);
}


#endif
```
