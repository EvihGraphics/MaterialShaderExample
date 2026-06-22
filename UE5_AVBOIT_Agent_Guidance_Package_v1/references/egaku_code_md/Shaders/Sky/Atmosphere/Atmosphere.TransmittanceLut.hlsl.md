# Egaku source: `Shaders/Sky/Atmosphere/Atmosphere.TransmittanceLut.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef ATMOSPHERE_TRANSMITTANCE_LUT_HLSL
#define ATMOSPHERE_TRANSMITTANCE_LUT_HLSL

#include "Atmosphere.TransmittanceLutUtil.hlsl"
#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Texture.hlsl"

float4 SampleAtmosphereTransmittanceLut(float2 uv);

#ifdef USE_ATMOSPHERE_TRANSMITTANCE_LUT_SRV
Texture2D _AtmosphereTransmittanceLut;

float4 SampleAtmosphereTransmittanceLut(float2 uv)
{
    return SampleTexture(_AtmosphereTransmittanceLut,sampler_LinearClamp,uv);
}

#else
#ifdef USE_ATMOSPHERE_TRANSMITTANCE_LUT_UAV
RWTexture2D<float4> _AtmosphereTransmittanceLut;
#endif
#endif

float4 SampleAtmosphereTransmittanceLut(float positionRadius,float directionZenithCosine,float bottomRadius,float topRadius)
{
    TransmittanceLUTRayMarchingDesc desc = (TransmittanceLUTRayMarchingDesc)0;
    desc.directionZenithCosine = directionZenithCosine;
    desc.radius = positionRadius;
    float2 uv = GetTransmittanceLUTSampleUV(desc,bottomRadius,topRadius);
    return SampleAtmosphereTransmittanceLut(uv);
}

#endif
```
