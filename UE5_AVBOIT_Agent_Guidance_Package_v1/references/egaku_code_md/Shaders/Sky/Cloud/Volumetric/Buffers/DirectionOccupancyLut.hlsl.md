# Egaku source: `Shaders/Sky/Cloud/Volumetric/Buffers/DirectionOccupancyLut.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef DIRECTION_OCCUPANCY_LUT_HLSL
#define DIRECTION_OCCUPANCY_LUT_HLSL
#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Texture.hlsl"

#ifdef VOLUMETRIC_CLOUD_USE_DIRECTION_OCCUPANCY_LUT_UAV
#define VOLUMETRIC_CLOUD_USE_DIRECTION_OCCUPANCY_LUT_DEFINED
RWTexture2D<float4> _VolumetricCloud_DirectionOccupancyLut : register(u1);
#else

#ifdef VOLUMETRIC_CLOUD_USE_DIRECTION_OCCUPANCY_LUT_SRV
#define VOLUMETRIC_CLOUD_USE_DIRECTION_OCCUPANCY_LUT_DEFINED
Texture2D<float4> _VolumetricCloud_DirectionOccupancyLut;
#endif
#endif

#ifdef VOLUMETRIC_CLOUD_USE_DIRECTION_OCCUPANCY_LUT_SRV
float DirectionOccupancyLut_Sample(float2 uv)
{
    return SampleTextureLevel(_VolumetricCloud_DirectionOccupancyLut,sampler_LinearClamp, uv, 0).r;
}
#endif



#ifdef VOLUMETRIC_CLOUD_USE_DIRECTION_OCCUPANCY_LUT_DEFINED

uint2 DirectionOccupancyLut_GetDimensions()
{
    uint x,y;
    _VolumetricCloud_DirectionOccupancyLut.GetDimensions(x,y);
    return uint2(x,y);
}

#endif


#ifdef VOLUMETRIC_CLOUD_USE_DIRECTION_OCCUPANCY_LUT_UAV

void DirectionOccupancyLut_Write(uint2 coords, float value)
{
    _VolumetricCloud_DirectionOccupancyLut[coords] = float4(value,0,0,0);
}

float DirectionOccupancyLut_Read(uint2 coords)
{
    return _VolumetricCloud_DirectionOccupancyLut[coords];
}

#endif


#endif
```
