# Egaku source: `Shaders/OIT/Fourier/OpacityMap/Passes/BackgroundResolutionPass.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef BACKGROUND_RESOLUTION_PASS_HLSL
#define BACKGROUND_RESOLUTION_PASS_HLSL

#define FOM_FOURIER_COEFFS_HARMONICS_COUNT 7
#include "../Common/FourierCoeffs.hlsl"
#include "../Common/FourierTransmittance.hlsl"
#define FOM_USE_FOURIER_COEFFS_LUT_SRV
#include "../Buffers/FourierCoeffsLut.hlsl"
#include "Assets/Shaders/Infrastructure/ScreenSpaceUtils.hlsl"
#include "../FourierOpacityMap.ParamStruct.hlsl"
#include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/DeclareOpaqueTexture.hlsl"

#pragma enable_d3d11_debug_symbols


struct Attributes {
    float4 vertex : POSITION;
};

struct Varyings {
    float4 position : SV_POSITION;
};


float2 GetScreenUVFromHClipPosition(float4 positionHCS)
{
    float2 screenUV = positionHCS.xy / positionHCS.w;
    screenUV.x = screenUV.x * 0.5 + 0.5;
    screenUV.y = -screenUV.y * 0.5 + 0.5;
    return screenUV;
}

Varyings Vert(Attributes input) {
    Varyings output;
    output.position = TransformObjectToHClip(input.vertex);
    return output;
}

float4 Frag(Varyings input) : SV_Target {
    float2 screenUV = GetNormalizedScreenSpaceUV(input.position);
    float3 backgroundLuminance = SampleSceneColor(screenUV);
    float viewDepth = 0.99;
    FourierCoeffs coeffs = FOM_SampleFourierCoeffs(screenUV);
    float transmittance = FOM_EvalTransmittance(coeffs,viewDepth);
    float3 color =  backgroundLuminance;
    return float4(color.rgb  * transmittance,1);
}




#endif
```
