# Egaku source: `Shaders/Sky/Atmosphere/Atmosphere.TransmittanceLutPass.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef ATMOSPHERE_TRANSMITTANCE_LUT_PASS_HLSL
#define ATMOSPHERE_TRANSMITTANCE_LUT_PASS_HLSL
#include "Atmosphere.TransmittanceLut.hlsl"
#include "Volume/AtmosphereVolumeRayMarching.hlsl"

#pragma enable_d3d11_debug_symbols

int _TransmittanceLut_SamplePerDirection;

struct Attributes {
    float4 vertex : POSITION;
    float2 uv : TEXCOORD0;
};

struct Varyings {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

float3 RenderAtmosphereTransmittanceLut(float2 uv)
{
    BoundingSphere sphere = GetAtmosphereVolumeBoundingSphere();
    TransmittanceLUTRayMarchingDesc desc = GetTransmittanceLUTRayMarchingDirection(uv, sphere.radiusMin, sphere.radiusMax);
    float3 rayOrigin = WorldVerticalDir() * desc.radius + sphere.center;
    float3 rayDirection = WorldVerticalDir() * desc.directionZenithCosine;
    rayDirection.x = sqrt(1.0f - desc.directionZenithCosine * desc.directionZenithCosine);
    float3 transmittance = CalcAtmosphereTransmittanceIntegral(rayOrigin, rayDirection,desc.dist, _TransmittanceLut_SamplePerDirection);
    return transmittance;
}

Varyings Vert(Attributes input) {
    Varyings output;
    output.position = TransformObjectToHClip(input.vertex);
    output.uv = input.uv;
    return output;
}

float4 Frag(Varyings input) : SV_Target  {
    float2 uv = input.uv;
    float3 transmittance = RenderAtmosphereTransmittanceLut(uv);
    return float4(transmittance, 1.0f);
}

#endif
```
