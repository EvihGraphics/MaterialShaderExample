# Egaku source: `Shaders/OIT/AVBOIT/VirtualBlockBased/Buffers/VolumeTransmittanceLut.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef VOLUME_TRANSMITTANCE_LUT_HLSL
#define VOLUME_TRANSMITTANCE_LUT_HLSL

#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Texture.hlsl"
#include "Configs/VolumeConfig.hlsl"

#ifdef AVBOIT_USE_VOLUME_TRANSMITTANCE_LUT_SRV
#define AVBOIT_VOLUME_TRANSMITTANCE_LUT_DEFINED
Texture3D<float4> _AVBOIT_VolumeTransmittanceLut;
float4 VTLut_SampleVolumeTransmittance(float3 coords);

#else
#ifdef AVBOIT_USE_VOLUME_TRANSMITTANCE_LUT_UAV
#define AVBOIT_VOLUME_TRANSMITTANCE_LUT_DEFINED
RWTexture3D<float4> _AVBOIT_VolumeTransmittanceLut;
void VTLut_SetVolumeTransmittance(uint3 coord,float4 value);
#endif
#endif

uint3 VTLut_GetDimensions();
float4 VTLut_GetVolumeTransmittance(uint3 coords);

#ifdef AVBOIT_VOLUME_TRANSMITTANCE_LUT_DEFINED
uint3 VTLut_GetDimensions()
{
    #ifdef AVBOIT_GET_DIMENSIONS_FROM_SHADER_CONSTANTS
    return AVBOIT_GetVolumeConfig().physicalVolumeDimensions;
    #else
	
    uint x,y,z;
    _AVBOIT_VolumeTransmittanceLut.GetDimensions(x,y,z);
    return uint3(x,y,z);
    #endif
    
}

float4 VTLut_GetVolumeTransmittance(uint3 coords)
{
    return _AVBOIT_VolumeTransmittanceLut[coords];
}



#endif

float3 VTLut_GetSampleCoordsFromVoxelCoords(float3 voxelCoords)
{
    uint3 dims = VTLut_GetDimensions();
    float3 duvw = rcp(dims);
    voxelCoords.xy = voxelCoords.xy + float2(0.5,0.5); 
    voxelCoords.z = voxelCoords.z + 0.5;
    return voxelCoords * duvw;
}

#ifdef AVBOIT_USE_VOLUME_TRANSMITTANCE_LUT_SRV
float4 VTLut_SampleVolumeTransmittance(float3 coords)
{
    float3 uvw = VTLut_GetSampleCoordsFromVoxelCoords(coords);
    #ifndef AVBOIT_VOLUME_TRANSMITTANCE_LUT_NON_PIXEL_SHADER
    return SampleTexture3D(_AVBOIT_VolumeTransmittanceLut,sampler_LinearClamp, uvw);
    #else
    return SampleTexture3DLevel(_AVBOIT_VolumeTransmittanceLut,sampler_LinearClamp, uvw, 0);
    #endif
}
#endif



#ifdef AVBOIT_USE_VOLUME_TRANSMITTANCE_LUT_UAV
void VTLut_SetVolumeTransmittance(uint3 coords,float4 value)
{
    _AVBOIT_VolumeTransmittanceLut[coords] = value; 
}
#endif


#endif
```
