# Egaku source: `Shaders/Sky/Cloud/Volumetric/Buffers/SunTransmittanceLut.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef SUN_TRANSMITTANCE_LUT_HLSL
#define SUN_TRANSMITTANCE_LUT_HLSL
#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Texture.hlsl"



#ifdef VOLUMETRIC_CLOUD_USE_SUN_TRANSMITTANCE_LUT_UAV
#define VOLUMETRIC_CLOUD_USE_SUN_TRANSMITTANCE_LUT_DEFINED
RWTexture3D<float4> _VolumetricCloud_SunTransmittanceLut : register(u1);
#else

#ifdef VOLUMETRIC_CLOUD_USE_SUN_TRANSMITTANCE_LUT_SRV
#define VOLUMETRIC_CLOUD_USE_SUN_TRANSMITTANCE_LUT_DEFINED
Texture3D<float4> _VolumetricCloud_SunTransmittanceLut;
#endif
#endif

#ifdef VOLUMETRIC_CLOUD_USE_SUN_TRANSMITTANCE_LUT_SRV
float SunTransmittanceLut_Sample(float3 uvw)
{
    return SampleTexture3DLevel(_VolumetricCloud_SunTransmittanceLut,sampler_LinearClamp, uvw, 2).r;
}
#endif
float4x4 _VolumetricCloud_SunTransmittanceLut_View;
float4x4 _VolumetricCloud_SunTransmittanceLut_ViewProjection;



float2 SunTransmittanceLut_GetSampleCoordsUV(float3 positionWS)
{
    float4 positionCS = mul(_VolumetricCloud_SunTransmittanceLut_ViewProjection, float4(positionWS,1));
    float2 uv = positionCS.xy / positionCS.w;
    return uv;
}

#ifdef VOLUMETRIC_CLOUD_USE_SUN_TRANSMITTANCE_LUT_DEFINED

uint3 SunTransmittanceLut_GetDimensions()
{
    uint x,y,z;
    _VolumetricCloud_SunTransmittanceLut.GetDimensions(x,y,z);
    return uint3(x,y,z);
}

#endif


#ifdef VOLUMETRIC_CLOUD_USE_SUN_TRANSMITTANCE_LUT_UAV

void SunTransmittanceLut_Write(uint3 coords, float value)
{
    _VolumetricCloud_SunTransmittanceLut[coords] = float4(value,0,0,0);
}

float SunTransmittanceLut_Read(uint3 coords)
{
    return _VolumetricCloud_SunTransmittanceLut[coords];
}

#endif


#endif
```
