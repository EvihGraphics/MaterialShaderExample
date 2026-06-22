# Egaku source: `Shaders/Sky/Cloud/Volumetric/Passes/FilterPass.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef VOLUMETRIC_CLOUD_TEMPORAL_FILTER_HLSL
#define VOLUMETRIC_CLOUD_TEMPORAL_FILTER_HLSL
#include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Lighting.hlsl"
#include "../Filters/TemporalFilter.hlsl"

#pragma enable_d3d11_debug_symbols


Texture2D _BilateralFilteredCloud;
Texture2D _CurrentFrameTex;
Texture2D _HistoryFrameTex;
float _TemporalBlendFactor;
float _TemporalBlendFactor_OutView;
float _BilateralFilter_KernelSize;
float4 _BilateralFilter_ColorSigma;
float _BilateralFilter_SpaceSigma;
float4x4 _ViewProjectionMatrix;
float4x4 _PreviousViewProjectionMatrix;

struct Attributes {
    float4 vertex : POSITION;
    float2 uv : TEXCOORD0;
};

struct Varyings {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

Varyings Vert(Attributes input) {
    Varyings output;
    output.position = TransformObjectToHClip(input.vertex);
    output.uv = input.uv;
    return output;
}

float4x4 GetFrustumPlanesWSNoNearFar(float4x4 projectMatrix)
{
    float4x4 planes;
    planes[0] = projectMatrix[3] - projectMatrix[0];
    planes[1] = projectMatrix[3] + projectMatrix[0];
    planes[2] = projectMatrix[3] - projectMatrix[1];
    planes[3] = projectMatrix[3] + projectMatrix[1];
    return planes;
}

bool FrustumCullVector(float3 vectorVS)
{
    float4x4 planes = GetFrustumPlanesWSNoNearFar(_ViewProjectionMatrix);
    return dot(planes[0], vectorVS) < 0.0f ||
        dot(planes[1], vectorVS) < 0.0f ||
        dot(planes[2], vectorVS) < 0.0f ||
        dot(planes[3], vectorVS) < 0.0f;
}

bool FrustumCullVector(float3 vectorVS,float4x4 viewProjectionMatrix)
{
    float4x4 planes = GetFrustumPlanesWSNoNearFar(viewProjectionMatrix);
    return dot(planes[0], vectorVS) < 0.0f ||
        dot(planes[1], vectorVS) < 0.0f ||
        dot(planes[2], vectorVS) < 0.0f ||
        dot(planes[3], vectorVS) < 0.0f;
}


float4 Frag(Varyings input) : SV_Target {
    BilateralFilterDesc bfDesc;
    bfDesc.kernelSize = _BilateralFilter_KernelSize;
    bfDesc.valueSigma = _BilateralFilter_ColorSigma;
    bfDesc.spaceSigma = _BilateralFilter_SpaceSigma;
    NoMotionTemporalFilterInput tfInput;
    tfInput.currentBuffer = _CurrentFrameTex;
    tfInput.historyBuffer = _HistoryFrameTex;
    tfInput.historyPixelUV = input.uv;
    tfInput.currPixelUV = input.uv;
    tfInput.blendFactor = _TemporalBlendFactor;
    tfInput.currentSampler = sampler_LinearClamp;
    tfInput.historySampler = sampler_LinearClamp;
    float4 color = GetTemporalFilteredResult(tfInput, bfDesc);
    return color;
}

#endif
```
