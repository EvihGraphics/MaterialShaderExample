# Egaku source: `Shaders/OIT/AVBOIT/VirtualBlockBased/Passes/VolumeTransmittanceIntegrationPass.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef TRANSMITTANCE_INTEGRATION_PASS_HLSL
#define TRANSMITTANCE_INTEGRATION_PASS_HLSL

#define AVBOIT_USE_VOLUME_EXTINCTION_BUFFER_SRV
#include "Buffers/VolumeExtinctionBuffer.hlsl"
#define AVBOIT_USE_VOLUME_TRANSMITTANCE_LUT_UAV
#include "Buffers/VolumeTransmittanceLut.hlsl"
#define AVBOIT_USE_VOLUME_BLOCK_LUT_SRV
#include "Buffers/VolumeBlockLut.hlsl"

#include "Volume/Volume.hlsl"

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
    return VBLut_GetDimensions();
}

uint3 VirtualVolumeConfig_GetVolumeBlockDimensions()
{
    return PhysicalVolumeConfig_GetVolumeBlockDimensions();
}
#endif




void RayMarchThroughVolume(uint3 volumeEntryCoords, uint sliceCount)
{
    float3 transmittance = 1;
    float opticalDepth = 0;
    for (uint i = volumeEntryCoords.z; i < volumeEntryCoords.z + sliceCount; i++)
    {
        uint3 voxelCoords = uint3(volumeEntryCoords.xy, i);
        float voxelExtinction = VEBuffer_GetVoxelExtinction(voxelCoords);
        float3 voxelExtinctionRGB = VEBuffer_GetVoxelExtinctionRGB(voxelCoords);
        float3 scatteringPrt = 1;
        transmittance *= scatteringPrt * exp(-voxelExtinction);
       // transmittance *= scatteringPrt * exp(-voxelExtinctionRGB);
        opticalDepth += voxelExtinction;
        VTLut_SetVolumeTransmittance( voxelCoords,float4(transmittance, exp(-opticalDepth)));
    }
}


[numthreads(8, 4, 1)]
void Kernel_RenderTransmittanceLut(uint2 dispatchThreadId : SV_DispatchThreadID)
{
    if (any(dispatchThreadId >= VirtualVolume_GetVolumeDimensions().xy))
    {
        return;
    }
    uint2 virtualVoxelCoordsXY = dispatchThreadId;
    uint2 virtualBlockCoordsXY = VirtualVolume_GetVolumeBlockCoords(float3(virtualVoxelCoordsXY,0)).xy;
    uint blockSliceIndexExit = VBLut_GetMaxPhysicalBlockIndexOfDepthDirection(virtualBlockCoordsXY);
    uint blockSliceIndexEntry = VBLut_GetMinPhysicalBlockIndexOfDepthDirection(virtualBlockCoordsXY);
    //if (blockSliceIndexEntry == blockSliceIndexExit){
    //    return;
  //  }
    
    uint sliceIndexEntry = PhysicalVolume_GetVolumeSliceIndex(blockSliceIndexEntry/* + 0.5*/);
    uint sliceIndexExit = PhysicalVolume_GetVolumeSliceIndex(blockSliceIndexExit + 1/*+ 0.5*/) ;
    //if (sliceIndexEntry == 1){
   //     sliceIndexEntry = 0;
    //}
    sliceIndexExit = max(sliceIndexExit,sliceIndexEntry);
    uint sliceCount = sliceIndexExit - sliceIndexEntry;
    uint2 voxelCoordsXY = PhysicalVolume_GetVoxelCoordsXYFromVirtual(virtualVoxelCoordsXY);
    RayMarchThroughVolume(uint3(voxelCoordsXY,sliceIndexEntry),sliceCount);
}


#endif
```
