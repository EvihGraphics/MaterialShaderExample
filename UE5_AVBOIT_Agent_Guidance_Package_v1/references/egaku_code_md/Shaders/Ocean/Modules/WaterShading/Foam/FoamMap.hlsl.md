# Egaku source: `Shaders/Ocean/Modules/WaterShading/Foam/FoamMap.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef FOAM_MAP_HLSL
#define FOAM_MAP_HLSL
#include "../../Utils.hlsl"
#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Core.hlsl"
#include "Ocean.ParamStruct.hlsl"

struct FoamData
{
    float detailFoam;
    float softFoam;
    float3 normal;
};

struct AdvectedSampleContext
{
    Texture2D map;
    SamplerState samplerState;
    float2 uv;
    float2 advectionDir;
    float speed;
    float time;
};

FoamData DecodeFoamMapSampleValue(float4 value)
{
    FoamData data;
    float2 normalXY = value.rg * 2.0 - 1.0;
    data.normal = ReconstructNormalFromXYSquare(normalXY);
    data.softFoam = value.b;
    data.detailFoam = value.a;
    return data;
}



float3 CalcAdvectedSampleOffsets(float time,float speed)
{
    return frac(frac(time)+ float3(0,1,2) * rcp(3)) * speed;
}

float3 CalcAdvectedSampleWeights(float time)
{
    float3 weights = float3(0,1,2) * rcp(3) + frac(time);
    weights = 1 - cos(TWO_PI * weights);
    return weights * rcp(3);
}


float4 SampleAdvectedMap(AdvectedSampleContext context)
{
    Texture2D map = context.map;
    SamplerState samplerState = context.samplerState;
    float3 weights = CalcAdvectedSampleWeights(context.time);
    float3 offsets = CalcAdvectedSampleOffsets(context.time, context.speed);
    float2 uv1 = context.uv + offsets.r * context.advectionDir;
    float2 uv2 = context.uv + offsets.g * context.advectionDir;
    float2 uv3 = context.uv + offsets.b * context.advectionDir;
    
    float4 value1 = SampleTexture(map, samplerState, uv1);
    float4 value2 = SampleTexture(map, samplerState, uv2);
    float4 value3 = SampleTexture(map, samplerState, uv3);
    float4 value = value1 * weights.x + value2 * weights.y + value3 * weights.z;
    return value;
}

FoamData SampleFoamMap(float2 uv,float2 velocity)
{
    DeclareParamFetch(WaterFoamMapDesc, foamMapDesc);
    AdvectedSampleContext context;
    context.time = GetTimeContext().timeInSec;
    context.map = foamMapDesc.foamMap;
    context.samplerState = sampler_LinearRepeat;
    context.uv = uv * foamMapDesc.uvScale;
    context.speed = foamMapDesc.advectionSpeedScale;
    context.advectionDir = velocity;
    float4 foamMapValue = SampleAdvectedMap(context);
    return DecodeFoamMapSampleValue(foamMapValue);
}

FoamData SampleFoamMap(float3 positionWS,float2 velocity)
{
    return SampleFoamMap(GetHorizontal(positionWS),velocity);
}

#endif
```
