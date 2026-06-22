# Egaku source: `Shaders/Sky/Cloud/Volumetric/Passes/VoxelMetaLutBuildingPass.CSImpl.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef VOXEL_META_LUT_BUILDING_PASS_CSIMPL_HLSL
#define VOXEL_META_LUT_BUILDING_PASS_CSIMPL_HLSL


#include "Assets/Shaders/Sky/Basic/VolumeBase/VolumeRayMarching.hlsl"
#include "../CloudVolume/CloudVolume.hlsl"
#define VOLUMETRIC_CLOUD_USE_VOXEL_META_LUT_UAV
#include "../Buffers/VoxelMetaLut.hlsl"
#include "../Volume/OctahedronVolume.hlsl"

//#pragma enable_d3d11_debug_symbols



#define VOXEL_META_LUT_SAMPLE_MULTIPLIER_PER_DIRECTION 4
#define VOXEL_META_LUT_KERNEL_THREAD_GROUP_SIZE (VOXEL_META_LUT_SAMPLE_MULTIPLIER_PER_DIRECTION),(VOXEL_META_LUT_SAMPLE_MULTIPLIER_PER_DIRECTION), 1


groupshared VoxelMeta gs_voxelMetas[VOXEL_META_LUT_SAMPLE_MULTIPLIER_PER_DIRECTION*VOXEL_META_LUT_SAMPLE_MULTIPLIER_PER_DIRECTION];

uint _VolumetricCloud_SegmentTestNormalIteration;
float4 _VoxelMetaLut_DirectionDimensions;

uint VoxelMetaLut_SamplePerDirection()
{
    return VOXEL_META_LUT_SAMPLE_MULTIPLIER_PER_DIRECTION * VOXEL_META_LUT_SAMPLE_MULTIPLIER_PER_DIRECTION;
}

void TempStoreVoxelMeta(VoxelMeta voxelMeta,uint2 coords)
{
    gs_voxelMetas[coords.y*VOXEL_META_LUT_SAMPLE_MULTIPLIER_PER_DIRECTION + coords.x] = voxelMeta;
}

VoxelMeta ReadTempStoredVoxelMeta(uint2 coords)
{
    return gs_voxelMetas[coords.y*VOXEL_META_LUT_SAMPLE_MULTIPLIER_PER_DIRECTION + coords.x];
}

VoxelMeta ReadTempStoredVoxelMeta(uint index)
{
    return gs_voxelMetas[index];
}



float2 OctahedronVolume_DirectionDimensions()
{
    return _VoxelMetaLut_DirectionDimensions.xy * VOXEL_META_LUT_SAMPLE_MULTIPLIER_PER_DIRECTION;
}

float OctahedronVolume_DirectionSliceCount()
{
    return _VoxelMetaLut_DirectionDimensions.z;
}

void RayMarching_CSImpl(uint2 sampledDirectionCoords,uint2 sampledDirectionCoordsOffset,Ray ray,BoundingSphere sphere)
{
    BoundsHitResult boundsHit = CalcBoundsHit(sphere,ray);
    uint segmentCount = OctahedronVolume_DirectionSliceCount();
    uint iterationNormal = _VolumetricCloud_SegmentTestNormalIteration;
    SphereVolumeSampleConfig sampleConfig = GetSphereVolumeSampleConfig(boundsHit,sphere,iterationNormal,iterationNormal*30);
    uint viewSampleCount = sampleConfig.directionSampleCount;
    
    RayMarchingContext rayMarchingContext = GetRayMarchingContext(ray,boundsHit,viewSampleCount);
    
    SamplePoint samplePoint;
    samplePoint.position = rayMarchingContext.entryPosition;
    samplePoint.stepSize = rayMarchingContext.stepSize;
    samplePoint.direction = rayMarchingContext.direction;
    VoxelMeta entry;
    entry.flag = 0;
    entry.density = 0;
    entry.viewRadianceTransfer = 0;
    entry.sunTransmittance = 0;
    
    float lastPosMultiplier = 0;

    [loop]
    for (uint i = 0; i < segmentCount ; i++)
    {
        float segmentBegin = 0;
        float segmentEnd = 0;
        bool inSegment = false;
        
        [loop]
        for (uint j = lastPosMultiplier; j < viewSampleCount; j++)
        {
            float thisPosMultiplier = j;
            samplePoint.position = rayMarchingContext.entryPosition + thisPosMultiplier*samplePoint.direction * samplePoint.stepSize;
            VoxelData voxelData = Volume_SampleVoxelData(samplePoint);
            if (inSegment)
            {
                segmentEnd = thisPosMultiplier;
            }else
            {
                segmentBegin = lastPosMultiplier;
            }
            lastPosMultiplier = thisPosMultiplier;
            if (voxelData.density > 0.0001f)
            {
                if (inSegment == false)
                {
                    inSegment = true;
                }
            }else{
                if (inSegment == true)
                {
                    break;
                }
            }
        }
        
        if (inSegment == false)
        {
            entry.segmentBegin = viewSampleCount;
            entry.segmentEnd = 0;
            entry.flag = 0;
        }else
        {
            entry.segmentBegin = segmentBegin*rcp(viewSampleCount); // normalized
            entry.segmentEnd = segmentEnd*rcp(viewSampleCount); // normalized
            entry.flag = 1;
        }

        TempStoreVoxelMeta(entry,sampledDirectionCoordsOffset);
        GroupMemoryBarrierWithGroupSync();
        [branch]
        if (all(sampledDirectionCoordsOffset == 0))
        {
            VoxelMeta finalEntry = ReadTempStoredVoxelMeta(0);
            [unroll]
            for (uint k = 1; k < VoxelMetaLut_SamplePerDirection(); k++)
            {
                VoxelMeta sampledEntry = ReadTempStoredVoxelMeta(k);
                finalEntry.segmentBegin = min(finalEntry.segmentBegin,sampledEntry.segmentBegin);
                finalEntry.segmentEnd = max(finalEntry.segmentEnd,sampledEntry.segmentEnd);
                finalEntry.flag = max(finalEntry.flag,sampledEntry.flag);
            }
            float3 volumeSegCoords = uint3(sampledDirectionCoords,i);
            if (finalEntry.flag < 0.5)
            {
                finalEntry.segmentBegin = 0;
                finalEntry.segmentEnd = 0;
            }
            VoxelMetaLut_Write(volumeSegCoords,finalEntry);
        }
        GroupMemoryBarrierWithGroupSync();
    }
}

[numthreads(VOXEL_META_LUT_KERNEL_THREAD_GROUP_SIZE)]
void Kernel_VoxelMetaLutBuilding(uint2 threadGroupId : SV_GroupID,
    uint2 groupThreadId : SV_GroupThreadID,
    uint2 dispatchThreadId : SV_DispatchThreadID)
{
    
    BoundingSphere sphere = GetCloudVolumeBoundingSphere();
    Ray ray;
    ray.direction = OctahedronVolume_GetDirection(dispatchThreadId);
    ray.origin = OctahedronVolume_CenterPoint();
    ray.hitPoint = ray.direction*1000000;
    
    RayMarching_CSImpl(threadGroupId, groupThreadId,ray, sphere);
}




#endif
```
