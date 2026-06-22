# Egaku source: `Shaders/Sky/Cloud/Volumetric/Buffers/VoxelMetaLut.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef VOXEL_META_LUT_HLSL
#define VOXEL_META_LUT_HLSL
#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Texture.hlsl"

struct VoxelMeta
{
    float sunTransmittance;
    float density;
    float viewRadianceTransfer;
    float flag;
    float segmentBegin;
    float segmentEnd;
};


#ifdef VOLUMETRIC_CLOUD_USE_VOXEL_META_LUT_UAV
#define VOLUMETRIC_CLOUD_USE_VOXEL_META_LUT_DEFINED
RWTexture3D<uint> _VolumetricCloud_VoxelMetaLut : register(u2);
#else

#ifdef VOLUMETRIC_CLOUD_USE_VOXEL_META_LUT_SRV
#define VOLUMETRIC_CLOUD_USE_VOXEL_META_LUT_DEFINED
Texture3D<uint> _VolumetricCloud_VoxelMetaLut;
#endif
#endif


float4 VoxelMetaLut_DecodeValueToFloat(uint value)
{
    float4 packedValueMaxValue = float4(1,1,1,1);
    uint4 mask = uint4(0xff,0xff00,0,0);
    uint4 unormed = uint4(value&mask.x,(value&mask.y)>>8,0,0);
    float4 normed = float4(unormed) * rcp(255.0);
    return normed * packedValueMaxValue;
}

VoxelMeta VoxelMetaLut_UnpackValue(float4 value)
{
    VoxelMeta entry;
    entry.viewRadianceTransfer = 0;
    entry.sunTransmittance = 0;
    entry.density = 0;
    entry.flag = 0;
    entry.segmentBegin = value.x;
    entry.segmentEnd = value.y;
    return entry;
}


uint VoxelMetaLut_EncodeValueToInt(float4 packedValue)
{
    float4 packedValueMaxValue = float4(1,1,1,1);
    float4 normed = clamp(packedValue,0,packedValueMaxValue) * rcp(packedValueMaxValue);
    uint4 mask = uint4(0xff,0xff,0,0);
    uint4 unormed = uint4(normed * 255.0);
    uint encoded = (unormed.x & mask.x) | ((unormed.y & mask.y) << 8);
    return encoded;
}

float4 VoxelMetaLut_PackValue(VoxelMeta entry)
{
    return float4(entry.segmentBegin,entry.segmentEnd,0,0);
}

#ifdef VOLUMETRIC_CLOUD_USE_VOXEL_META_LUT_DEFINED

uint3 VoxelMetaLut_GetDimensions()
{
    uint x,y,z;
    _VolumetricCloud_VoxelMetaLut.GetDimensions(x,y,z);
    return uint3(x,y,z);
}

VoxelMeta VoxelMetaLut_Read(float3 coords)
{
    uint encodedValue = _VolumetricCloud_VoxelMetaLut[coords];
    float4 packedValue = VoxelMetaLut_DecodeValueToFloat(encodedValue);
    return VoxelMetaLut_UnpackValue(packedValue);
}

#endif


#ifdef VOLUMETRIC_CLOUD_USE_VOXEL_META_LUT_UAV

void VoxelMetaLut_InterlockedWrite(float3 coords, uint encodedValue)
{
    uint oldVal;
    InterlockedExchange(_VolumetricCloud_VoxelMetaLut[coords],encodedValue,oldVal);
}

void VoxelMetaLut_Write(float3 coords, VoxelMeta entry)
{
    _VolumetricCloud_VoxelMetaLut[coords] = VoxelMetaLut_EncodeValueToInt(VoxelMetaLut_PackValue(entry));
}

void VoxelMetaLut_InterlockedWrite(float3 coords, VoxelMeta entry)
{
    VoxelMetaLut_InterlockedWrite(coords,VoxelMetaLut_EncodeValueToInt(VoxelMetaLut_PackValue(entry)));
}

void VoxelMetaLut_Add(float3 coords, VoxelMeta entry)
{
    _VolumetricCloud_VoxelMetaLut[coords] += VoxelMetaLut_EncodeValueToInt(VoxelMetaLut_PackValue(entry));
}

void VoxelMetaLut_InterlockedAdd(float3 coords, uint encodedValue)
{
    InterlockedAdd(_VolumetricCloud_VoxelMetaLut[coords],encodedValue);
}

void VoxelMetaLut_InterlockedAdd(float3 coords, VoxelMeta entry)
{
    VoxelMetaLut_InterlockedAdd(coords,VoxelMetaLut_EncodeValueToInt(VoxelMetaLut_PackValue(entry)));
}



void VoxelMetaLut_LinearBlendAdd(float3 coords, VoxelMeta entry)
{
    float3 coords000 = max(0,coords + float3(-0.5,-0.5,-0.5));
    float3 coords100 = max(0,coords + float3(0.5,-0.5,-0.5));
    float3 coords010 = max(0,coords + float3(-0.5,0.5,-0.5));
    float3 coords110 = max(0,coords + float3(0.5,0.5,-0.5));
    float3 coords001 = max(0,coords + float3(-0.5,-0.5,0.5));
    float3 coords101 = max(0,coords + float3(0.5,-0.5,0.5));
    float3 coords011 = max(0,coords + float3(-0.5,0.5,0.5));
    float3 coords111 = max(0,coords + float3(0.5,0.5,0.5));
    float3 weight000 = max(0,uint3(coords111) - coords000);
    float3 weight111 = max(0,coords111 - uint3(coords111));
    float3 weight001 = max(0,float3(weight000.x,weight000.y,weight111.z));
    float3 weight010 = max(0,float3(weight000.x,weight111.y,weight000.z));
    float3 weight011 = max(0,float3(weight000.x,weight111.y,weight111.z));
    float3 weight100 = max(0,float3(weight111.x,weight000.yz));
    float3 weight101 = max(0,float3(weight111.x,weight000.y,weight111.z));
    float3 weight110 = max(0,float3(weight111.x,weight111.y,weight000.z));
    
    float4 packedValue = VoxelMetaLut_PackValue(entry);
    VoxelMetaLut_InterlockedAdd(coords000,VoxelMetaLut_EncodeValueToInt(packedValue * weight000.x * weight000.y * weight000.z));
    VoxelMetaLut_InterlockedAdd(coords001,VoxelMetaLut_EncodeValueToInt(packedValue * weight001.x * weight001.y * weight001.z));
    VoxelMetaLut_InterlockedAdd(coords010,VoxelMetaLut_EncodeValueToInt(packedValue * weight010.x * weight010.y * weight010.z));
    VoxelMetaLut_InterlockedAdd(coords011,VoxelMetaLut_EncodeValueToInt(packedValue * weight011.x * weight011.y * weight011.z));
    VoxelMetaLut_InterlockedAdd(coords100,VoxelMetaLut_EncodeValueToInt(packedValue * weight100.x * weight100.y * weight100.z));
    VoxelMetaLut_InterlockedAdd(coords101,VoxelMetaLut_EncodeValueToInt(packedValue * weight101.x * weight101.y * weight101.z));
    VoxelMetaLut_InterlockedAdd(coords110,VoxelMetaLut_EncodeValueToInt(packedValue * weight110.x * weight110.y * weight110.z));
    VoxelMetaLut_InterlockedAdd(coords111,VoxelMetaLut_EncodeValueToInt(packedValue * weight111.x * weight111.y * weight111.z));
}


#endif
#define  VOLUMETRIC_CLOUD_USE_VOXEL_META_LUT_SRV
#ifdef VOLUMETRIC_CLOUD_USE_VOXEL_META_LUT_SRV

float4 TrilinearInterpolation(
    float4 v000, float4 v100, float4 v010, float4 v110,
    float4 v001, float4 v101, float4 v011, float4 v111,
    float3 f)
{
    float4 v00 = lerp(v000, v100, f.x);
    float4 v01 = lerp(v001, v101, f.x);
    float4 v10 = lerp(v010, v110, f.x);
    float4 v11 = lerp(v011, v111, f.x);
    float4 v0 = lerp(v00, v10, f.y);
    float4 v1 = lerp(v01, v11, f.y);
    return lerp(v0, v1, f.z);
}

float4 SmoothLerp(float4 a, float4 b, float4 t)
{
     return lerp(a, b, smoothstep(0,1, t));
}

float4 TrilinearSmoothInterpolation(
    float4 v000, float4 v100, float4 v010, float4 v110,
    float4 v001, float4 v101, float4 v011, float4 v111,
    float3 f)
{
    float4 v00 = SmoothLerp(v000, v100, f.x);
    float4 v01 = SmoothLerp(v001, v101, f.x);
    float4 v10 = SmoothLerp(v010, v110, f.x);
    float4 v11 = SmoothLerp(v011, v111, f.x);
    float4 v0 = SmoothLerp(v00, v10, f.y);
    float4 v1 = SmoothLerp(v01, v11, f.y);
    return SmoothLerp(v0, v1, f.z);
}

float4 VoxelMetaLut_ClampValue(float4 val,float4 edgeValue,float4 clampValue)
{
    if (all(val < edgeValue))
    {
        return clampValue;
    }else
    {
        return val;
    }
}

VoxelMeta VoxelMetaLut_Sample(float3 uvw)
{
  //  float4 value = SampleTexture3DLevel(_VolumetricCloud_VoxelMetaLut,sampler_LinearClamp, uvw,0);
    float3 coords = uvw * VoxelMetaLut_GetDimensions();
    coords -= float3(0.5,0.5,0.5);
    uint3 central = round(coords);
    uint3 coords000 = uint3(coords);
    float3 frac = coords - coords000;
    float4 cvalue = VoxelMetaLut_DecodeValueToFloat( _VolumetricCloud_VoxelMetaLut[central]);
    float4 value000 = VoxelMetaLut_DecodeValueToFloat( _VolumetricCloud_VoxelMetaLut[coords000]);
    value000 = VoxelMetaLut_ClampValue(value000,0.0001,cvalue);
    float4 value100 = VoxelMetaLut_DecodeValueToFloat( _VolumetricCloud_VoxelMetaLut[coords000 + uint3(1,0,0)]);
    value100 = VoxelMetaLut_ClampValue(value100,0.0001,cvalue);
    float4 value010 = VoxelMetaLut_DecodeValueToFloat( _VolumetricCloud_VoxelMetaLut[coords000 + uint3(0,1,0)]);
    value010 = VoxelMetaLut_ClampValue(value010,0.0001,cvalue);
    float4 value110 = VoxelMetaLut_DecodeValueToFloat( _VolumetricCloud_VoxelMetaLut[coords000 + uint3(1,1,0)]);
    value110 = VoxelMetaLut_ClampValue(value110,0.0001,cvalue);
    /*float4 value001 = VoxelMetaLut_DecodeValueToFloat( _VolumetricCloud_VoxelMetaLut[coords000 + uint3(0,0,1)]);
    value001 = VoxelMetaLut_ClampValue(value001,0.0001,cvalue);
    float4 value101 = VoxelMetaLut_DecodeValueToFloat( _VolumetricCloud_VoxelMetaLut[coords000 + uint3(1,0,1)]);
    value101 = VoxelMetaLut_ClampValue(value101,0.0001,cvalue);
    float4 value011 = VoxelMetaLut_DecodeValueToFloat( _VolumetricCloud_VoxelMetaLut[coords000 + uint3(0,1,1)]);
    value011 = VoxelMetaLut_ClampValue(value011,0.0001,cvalue);
    float4 value111 = VoxelMetaLut_DecodeValueToFloat( _VolumetricCloud_VoxelMetaLut[coords000 + uint3(1,1,1)]);
    value111 = VoxelMetaLut_ClampValue(value111,0.0001,cvalue);*/
    
    float4 value = TrilinearSmoothInterpolation(value000, value100, value010, value110,
        0, 0, 0, 0, frac);
    VoxelMeta entry = VoxelMetaLut_UnpackValue(value);
    return entry;
}

#endif




#endif
```
