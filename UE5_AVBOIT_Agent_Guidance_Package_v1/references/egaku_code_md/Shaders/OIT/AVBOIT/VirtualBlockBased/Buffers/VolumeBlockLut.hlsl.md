# Egaku source: `Shaders/OIT/AVBOIT/VirtualBlockBased/Buffers/VolumeBlockLut.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef VOLUME_BLOCK_LUT_HLSL
#define VOLUME_BLOCK_LUT_HLSL
#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Texture.hlsl"
#include "Configs/VolumeConfig.hlsl"
struct VolumeBlockLutEntry
{
	float blockIndex;
	float viewDepth;
};

struct PhysicalBlockDesc
{
	float blockIndex;
	float viewDepth;
};

#ifdef AVBOIT_USE_VOLUME_BLOCK_LUT_UAV
#define AVBOIT_VOLUME_BLOCK_LUT_DEFINED
RWTexture3D<float4> _AVBOIT_VolumeBlockLut;
#else

#ifdef AVBOIT_USE_VOLUME_BLOCK_LUT_SRV
#define AVBOIT_VOLUME_BLOCK_LUT_DEFINED
Texture3D<float4> _AVBOIT_VolumeBlockLut;
#endif
#endif

uint3 VBLut_GetDimensions();

#ifdef AVBOIT_VOLUME_BLOCK_LUT_DEFINED
uint3 VBLut_GetDimensions()
{
	#ifdef AVBOIT_GET_DIMENSIONS_FROM_SHADER_CONSTANTS
	return AVBOIT_GetVolumeConfig().volumeBlockDivisor;
	#else
	
	uint x,y,z;
	_AVBOIT_VolumeBlockLut.GetDimensions(x,y,z);
	return uint3(x,y,z);
	#endif
}
#endif



VolumeBlockLutEntry VBLut_UnpackValue(float4 value)
{
	VolumeBlockLutEntry entry;
	entry.blockIndex = value.r;
	entry.viewDepth = value.g;
	return entry;
}

float4 VBLut_GetPackedValue(VolumeBlockLutEntry entry)
{
	float4 packed = float4(entry.blockIndex, entry.viewDepth, 0, 0);
	return packed;
}

float3 VBLut_GetSampleCoordsFromVolumeBlockCoords(float3 volumeBlockCoords)
{
	uint3 dims = VBLut_GetDimensions();
	float3 duvw = rcp(dims);
	volumeBlockCoords.xy = uint2(volumeBlockCoords.xy) + float2(0.5,0.5); // prevent sampler from filtering of X, Y dimensions.
	// dimension z of the volume block coords is the float index of the physical block.
	// it's fraction part is the percentage position inside the block, while the texture sampler
	// treat 0.5 as the center of the texel.
	// so we need to add 0.5 to the z coordinate.
	volumeBlockCoords.z = volumeBlockCoords.z + 0.5; 
	return volumeBlockCoords * duvw;
}

#ifdef AVBOIT_VOLUME_BLOCK_LUT_DEFINED

PhysicalBlockDesc VBLut_GetPhysicalBlockDesc(uint3 blockCoords)
{
	float4 val = _AVBOIT_VolumeBlockLut[blockCoords];
	VolumeBlockLutEntry entry = VBLut_UnpackValue(val);
	PhysicalBlockDesc desc;
	desc.blockIndex = entry.blockIndex;
	desc.viewDepth = entry.viewDepth;
	return desc;
}
#endif


#ifdef AVBOIT_USE_VOLUME_BLOCK_LUT_SRV

PhysicalBlockDesc VBLut_SamplePhysicalBlockDesc(float3 virtualBlockCoords)
{
	float3 uvw =  VBLut_GetSampleCoordsFromVolumeBlockCoords(virtualBlockCoords);
	float4 val = SampleTexture3DLevel(_AVBOIT_VolumeBlockLut,sampler_LinearClamp, uvw,0);
	VolumeBlockLutEntry entry = VBLut_UnpackValue(val);
	PhysicalBlockDesc desc;
	desc.blockIndex = entry.blockIndex;
	desc.viewDepth = entry.viewDepth;
	return desc;
}

#endif


#ifdef AVBOIT_USE_VOLUME_BLOCK_LUT_UAV
void VBLut_CreateVolumeBlockLutEntry(uint3 virtualBlockCoords,PhysicalBlockDesc desc)
{
	VolumeBlockLutEntry entry;
	entry.blockIndex = desc.blockIndex;
	entry.viewDepth = desc.viewDepth;
	float4 packed = VBLut_GetPackedValue(entry);
	_AVBOIT_VolumeBlockLut[virtualBlockCoords] = packed;
}
#endif


float VBLut_GetMaxPhysicalBlockIndex()
{
	uint3 dims = VBLut_GetDimensions();
	return VBLut_GetPhysicalBlockDesc(dims - uint3(1,1,1)).blockIndex;
}

float VBLut_GetMaxPhysicalBlockIndexOfDepthDirection(uint2 direction)
{
	uint3 dims = VBLut_GetDimensions();
	return VBLut_GetPhysicalBlockDesc(uint3(direction.xy,dims.z - 1)).blockIndex;
}



float VBLut_GetMinPhysicalBlockIndexOfDepthDirection(uint2 direction)
{
	return VBLut_GetPhysicalBlockDesc(uint3(direction.xy,0)).blockIndex;
}



#endif
```
