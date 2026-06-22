# Egaku source: `Shaders/OIT/AVBOIT/VirtualBlockBased/Passes/BackgroundResolutionPass.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef AVBOIT_BACKGROUND_RESOLUTION_PASS_HLSL
#define AVBOIT_BACKGROUND_RESOLUTION_PASS_HLSL

#include "../../Utils/CommonUtils.hlsl"
#define AVBOIT_USE_VOLUME_TRANSMITTANCE_LUT_SRV
#include "../Buffers/VolumeTransmittanceLut.hlsl"
#define AVBOIT_USE_VOLUME_BLOCK_LUT_SRV
#include "../Buffers/VolumeBlockLut.hlsl"
#include "../Volume/Volume.hlsl"
#include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/DeclareOpaqueTexture.hlsl"


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
    float2 screenUV = GetNormalizedScreenSpaceUV(input.position);
    
    float3 backgroundLuminance = SampleSceneColor(screenUV);
    float4 baseColor = float4(backgroundLuminance,1);
    float2 virtualVoxelCoordsXY = VirtualVolume_GetVoxelCoordsXYFromUV(screenUV);
    float virtualSliceIndex = FrustumVolume_GetVolumeSliceCount() - 1;
    float3 virtualVoxelCoords = float3(virtualVoxelCoordsXY, virtualSliceIndex);

    float3 virtualBlockCoords = VirtualVolume_GetVolumeBlockCoords(virtualVoxelCoords);

    float physicalBlockIndex = VBLut_GetPhysicalBlockDesc(virtualBlockCoords).blockIndex;
  //  float minPhysicalBlockIndex = VBLut_GetMinPhysicalBlockIndexOfDepthDirection(virtualVoxelCoordsXY);
   // float virtualVolumeBlockIndexMin = VirtualVolume_GetVolumeBlockIndex(float3(virtualVoxelCoordsXY,0));
    float2 voxelCoordsXY = PhysicalVolume_GetVoxelCoordsXYFromVirtual(virtualVoxelCoordsXY);
    float sliceIndex = PhysicalVolume_GetVolumeSliceIndex(physicalBlockIndex);
    //sliceIndex = floor(sliceIndex); // we intend to access the last slice
    /*if (maxPhysicalBlockIndex - minPhysicalBlockIndex < 0.5)
    {
         //In this case, no translucent object has drawn in this direction.
        transmittanceFront = 1.0;
    }else{
        float sliceIndexFront = sliceIndex;
        transmittanceFront = VTLut_SampleVolumeTransmittance(float3(voxelCoordsXY, sliceIndexFront));
    }*/
    float sliceIndexFront = sliceIndex;
    float transmittanceFront = VTLut_SampleVolumeTransmittance(float3(voxelCoordsXY, sliceIndexFront));
    float3 finalLuminance = baseColor * transmittanceFront;
    return float4(finalLuminance,1);
}




#endif
```
