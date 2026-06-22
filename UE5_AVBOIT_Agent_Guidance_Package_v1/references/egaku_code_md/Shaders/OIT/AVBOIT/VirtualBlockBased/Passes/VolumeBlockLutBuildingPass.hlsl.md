# Egaku source: `Shaders/OIT/AVBOIT/VirtualBlockBased/Passes/VolumeBlockLutBuildingPass.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef VOLUME_BLOCK_LUT_BUILDING_PASS_HLSL
#define VOLUME_BLOCK_LUT_BUILDING_PASS_HLSL

#define AVBOIT_USE_VOLUME_BLOCK_LUT_UAV
#include "Buffers/VolumeBlockLut.hlsl"
#define AVBOIT_USE_VOLUME_BLOCK_OCCUPANCY_BUFFER_UAV
#include "Buffers/VolumeBlockOccupancyBuffer.hlsl"
#define  AVBOIT_USE_VOLUME_EXTINCTION_BUFFER_UAV
#include "Buffers/VolumeExtinctionBuffer.hlsl"

#include "Volume/Volume.hlsl"

#define PARALLEL_SCAN_ELEM_TYPE float4
#define PARALLEL_SCAN_BLOCK_THREAD_COUNT 512
#define PARALLEL_BUILD_GROUP_THREAD_CONFIG (PARALLEL_SCAN_BLOCK_THREAD_COUNT),1,1

#include "Buffers/ParallelScanAuxiliaryArray.hlsl"
#include "../Utils/ParallelScan/ParallelScan.hlsl"
#include "../Utils/ParallelScan/AuxiliaryArray.hlsl"

float4 _AVBOIT_VolumeBlockLutBuildRange;

groupshared float shared_downsampleFrequency;
groupshared uint shared_virtualBlockCount;

float4 PackValueForScan(VolumeBlockOccupancy volumeOccupancy)
{
    float4 value = float4(volumeOccupancy.occupied, 0,0,0);
    return value;
}

PhysicalBlockDesc UnpackValue(uint index,float4 packedValue)
{
    PhysicalBlockDesc  desc;
    desc.blockIndex = packedValue.x;
    desc.viewDepth = packedValue.y;
    return desc;
}

float4 ParallelScan_GetElem(uint index)
{
    float4 value = 0;
    if (index < shared_virtualBlockCount)
    {
        int virtualBlockIndex = index;
        uint3 coords = VirtualVolume_GetVolumeBlockCoordsFromBlockIndex(virtualBlockIndex);
        VolumeBlockOccupancy volumeOccupancy = VBOBuffer_GetVolumeBlockOccupancy(coords);
        value = PackValueForScan(volumeOccupancy);
    }
    return value;
}

void ParallelScan_SetElem(uint index, float4 value)
{
    if (index < shared_virtualBlockCount)
    {
        PhysicalBlockDesc desc = UnpackValue(index,value);
        float3 coords = VirtualVolume_GetVolumeBlockCoordsFromBlockIndex(index);
        VBLut_CreateVolumeBlockLutEntry(coords,desc);
    }
}

void ParallelScan_SetZero(inout float4 t)
{
    t.x = 0;
}

void ParallelScan_Assign(float4 src, inout float4 dst)
{
    dst.x = src.x;
}

void ParallelScan_Operate(float4 src,inout float4 dst)
{
    dst.x += src.x;
}

#ifndef AVBOIT_GET_DIMENSIONS_FROM_SHADER_CONSTANTS
// definition for predeclared interface
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


void DownsampleVolumeBlockLut(int blockIndex,float frequency)
{
    float3 coords = VirtualVolume_GetVolumeBlockCoordsFromBlockIndex(blockIndex);
    PhysicalBlockDesc desc = VBLut_GetPhysicalBlockDesc(coords);
    desc.blockIndex = round(desc.blockIndex * frequency);
    VBLut_CreateVolumeBlockLutEntry(coords,desc);
}

PhysicalBlockDesc DownsampleVolumeBlockLut(PhysicalBlockDesc desc,float frequency)
{
    desc.blockIndex = floor(desc.blockIndex * frequency);
    return desc;
}

void DownsampleVolumeBlockLut(float frequency)
{
    uint virtualBlockCount = VirtualVolume_GetVolumeBlockCount();
    for (uint blockIndex = 0; blockIndex < virtualBlockCount; ++blockIndex)
    {
        DownsampleVolumeBlockLut(blockIndex,frequency);
    }
}

void BuildVolumeBlockLut()
{
    uint virtualBlockCount = VirtualVolume_GetVolumeBlockCount();
    uint lastBlockIndex = 0;

    for (uint virtualBlockIndex = 0; virtualBlockIndex < virtualBlockCount; ++virtualBlockIndex)
    {
        float3 coords = VirtualVolume_GetVolumeBlockCoordsFromBlockIndex(virtualBlockIndex);
        VolumeBlockOccupancy volumeOccupancy = VBOBuffer_GetVolumeBlockOccupancy(coords);
        PhysicalBlockDesc desc;
        desc.blockIndex = lastBlockIndex + volumeOccupancy.occupied;
        desc.viewDepth = 0;
        VBLut_CreateVolumeBlockLutEntry(coords,desc);
        lastBlockIndex = desc.blockIndex;
    }
    float blockAvailable = PhysicalVolume_GetVolumeBlockCount();
    float blockRequested =  VBLut_GetMaxPhysicalBlockIndex() + 1;
    float blockDownsampleFreq = blockAvailable / blockRequested;
    if (blockDownsampleFreq < 1.0) {
        DownsampleVolumeBlockLut(blockDownsampleFreq);
    }
}


void BuildVolumeBlockLut(uint threadIndex)
{
    ParallelScan_Inclusive(threadIndex);
}

[numthreads(1, 1, 1)]
void Kernel_BuildVolumeBlockLut()
{
    BuildVolumeBlockLut();
}


[numthreads(PARALLEL_BUILD_GROUP_THREAD_CONFIG)]
void Kernel_BuildVolumeBlockLutParallel_Scan(uint dispatchThreadId : SV_DispatchThreadID,uint groupThreadId : SV_GroupThreadID,uint groupId : SV_GroupID)
{
    shared_virtualBlockCount = VirtualVolume_GetVolumeBlockCount();
    GroupMemoryBarrierWithGroupSync();
    BuildVolumeBlockLut(dispatchThreadId);
    DeviceMemoryBarrierWithGroupSync();
    uint maxGroupThreadId = ParallelScan_BlockThreadCount() - 1;
    if (groupThreadId == maxGroupThreadId){
        uint lastElemIndex = 2*dispatchThreadId + 1;
        float3 coords = VirtualVolume_GetVolumeBlockCoordsFromBlockIndex(lastElemIndex);
        float blockIndex = VBLut_GetPhysicalBlockDesc(coords).blockIndex;
        ParallelScan_WriteToAuxiliaryArray(groupId,blockIndex);
    }
}

[numthreads(1,1,1)]
void Kernel_BuildVolumeBlockLutParallel_InitAuxiliaryArray()
{
    ParallelScan_InitAuxiliaryArray();
}

PhysicalBlockDesc ApplyFinalSum(PhysicalBlockDesc desc)
{
    desc.blockIndex += ParallelScan_GetPreviousBlockSum();
    return desc;
}



[numthreads(PARALLEL_BUILD_GROUP_THREAD_CONFIG)]
void Kernel_BuildVolumeBlockLutParallel_FinalSum(uint dispatchThreadId : SV_DispatchThreadID,uint groupThreadId : SV_GroupThreadID,uint groupId : SV_GroupID)
{
    if (groupId < 1){
        return;
    }
    
    if (groupThreadId == 0){
        ParallelScan_LoadPreviousBlockSum(groupId);
        float blockAvailable = PhysicalVolume_GetVolumeBlockCount();
        float blockRequested = ParallelScan_ReadFromAuxiliaryArray(ParallelScan_GetAuxiliaryArrayLength() - 1) + 1;
        float blockDownsampleFreq = blockAvailable / blockRequested;
        shared_downsampleFrequency = blockDownsampleFreq;
    }
    GroupMemoryBarrierWithGroupSync();
    uint elemIndexA = 2*dispatchThreadId;
    uint elemIndexB = elemIndexA + 1;

    uint3 coordsA = VirtualVolume_GetVolumeBlockCoordsFromBlockIndex(elemIndexA);
    uint3 coordsB = VirtualVolume_GetVolumeBlockCoordsFromBlockIndex(elemIndexB);
    PhysicalBlockDesc descA = VBLut_GetPhysicalBlockDesc(coordsA);
    PhysicalBlockDesc descB = VBLut_GetPhysicalBlockDesc(coordsB);
    descA = ApplyFinalSum(descA);
    descB = ApplyFinalSum(descB);
    if (shared_downsampleFrequency < 1.0) {
        descA = DownsampleVolumeBlockLut(descA,shared_downsampleFrequency);
        descB = DownsampleVolumeBlockLut(descB,shared_downsampleFrequency);
    }
    VBLut_CreateVolumeBlockLutEntry(coordsA,descA);
    VBLut_CreateVolumeBlockLutEntry(coordsB,descB);
}


[numthreads(8, 4, 1)]
void Kernel_BuildProtectionPlane(uint2 dispatchThreadId : SV_DispatchThreadID)
{
    uint3 firstBlockCoords = uint3(dispatchThreadId,0);
    //uint blockSliceCnt = VirtualVolumeConfig_GetVolumeDimensionsInBlock().z;
  //  uint3 lastBlockCoords = uint3(dispatchThreadId,blockSliceCnt - 1);
    VBOBuffer_SetVolumeBlockOccupied(firstBlockCoords);
   // VBOBuffer_SetVolumeBlockOccupied(lastBlockCoords);
}

#endif
```
