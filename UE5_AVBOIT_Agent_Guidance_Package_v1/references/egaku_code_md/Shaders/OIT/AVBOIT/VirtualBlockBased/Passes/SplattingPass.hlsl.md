# Egaku source: `Shaders/OIT/AVBOIT/VirtualBlockBased/Passes/SplattingPass.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef SPLATTING_PASS_HLSL
#define SPLATTING_PASS_HLSL
#include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Lighting.hlsl"
#include "../Utils/CommonUtils.hlsl"

#define AVBOIT_USE_VOLUME_EXTINCTION_BUFFER_UAV
#include "Buffers/VolumeExtinctionBuffer.hlsl"
#define AVBOIT_USE_VOLUME_BLOCK_LUT_SRV
#include "Buffers/VolumeBlockLut.hlsl"
#include "AVBOIT.ParamStruct.hlsl"

#include "Volume/Volume.hlsl"

#pragma enable_d3d11_debug_symbols

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
    float2 virtualVoxelCoordsXY = screenCoords - 0.5;
    float virtualSliceIndex = VirtualVolume_GetVolumeSliceIndex(viewDepth);
    float3 virtualBlockCoords = VirtualVolume_GetVolumeBlockCoords(float3(virtualVoxelCoordsXY,virtualSliceIndex));
    float virtualBlockIndex = VirtualVolume_GetVolumeBlockIndex(virtualBlockCoords); 
    float2 physicalVoxelCoordsXY = PhysicalVolume_GetVoxelCoordsXYFromVirtual(virtualVoxelCoordsXY);
    float physicalSliceIndex = PhysicalVolume_GetVolumeSliceIndexFromVirtual(virtualBlockIndex);
    VolumeSliceAccessDesc desc = GetVolumeSliceAccessDesc(physicalSliceIndex);
    
    float extinction = GetExtinctionFromBaseColor(FetchParam(TranslucencyDesc).baseColor);
    float3 extinctionRGB = GetExtinctionRGBFromBaseColor(FetchParam(TranslucencyDesc).baseColor);
    VEBuffer_AddVoxelExtinctionLinear(extinction,physicalVoxelCoordsXY,desc);
   // VEBuffer_AddVoxelExtinctionLinear(extinctionRGB,physicalVoxelCoordsXY,desc);

    return float4(0,0,0,0);
}


#endif
```
