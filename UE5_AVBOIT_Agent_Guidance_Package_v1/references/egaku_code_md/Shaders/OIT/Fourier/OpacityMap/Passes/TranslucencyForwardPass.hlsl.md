# Egaku source: `Shaders/OIT/Fourier/OpacityMap/Passes/TranslucencyForwardPass.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef TRANSLUCENCY_FORWARD_PASS_HLSL
#define TRANSLUCENCY_FORWARD_PASS_HLSL

#define FOM_FOURIER_COEFFS_HARMONICS_COUNT 7
#include "../Common/FourierCoeffs.hlsl"
#include "../Common/FourierTransmittance.hlsl"
#define FOM_USE_FOURIER_COEFFS_LUT_SRV
#include "../Buffers/FourierCoeffsLut.hlsl"
#include "Assets/Shaders/Infrastructure/ScreenSpaceUtils.hlsl"
#include "../FourierOpacityMap.ParamStruct.hlsl"

#pragma enable_d3d11_debug_symbols


struct Attributes {
    float4 vertex : POSITION;
};

struct Varyings {
    float4 position : SV_POSITION;
};

Varyings Vert(Attributes input) {
    Varyings output;
    output.position = TransformObjectToHClip(input.vertex);
    return output;
}


float4 Frag(Varyings input) : SV_Target {
    float2 screenUV = GetNormalizedScreenSpaceUV(input.position);
    float viewDepth = GetLinear01DepthFromRawZ(input.position.z);
    FourierCoeffs coeffs = FOM_SampleFourierCoeffs(screenUV);
    float transmittance = FOM_EvalTransmittance(coeffs,viewDepth - 0.01);
    float4 color = FetchParam(TranslucencyDesc).baseColor;
    return float4(color.rgb * color.a * transmittance,1);
}


#endif
```
