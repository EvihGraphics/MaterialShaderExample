# Egaku source: `Shaders/Sky/Cloud/Volumetric/Filters/TemporalFilter.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef TEMPORAL_FILTER_HLSL
#define TEMPORAL_FILTER_HLSL

#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Texture.hlsl"
#include "BilateralFilter.hlsl"


struct TemporalFilterInput
{
    Texture2D currentBuffer;
    Texture2D historyBuffer;
    Texture2D velocityBuffer;
    SamplerState currentSampler;
    SamplerState historySampler;
    float2 currPixelUV;
    float blendFactor;
};


struct NoMotionTemporalFilterInput
{
    Texture2D currentBuffer;
    Texture2D historyBuffer;
    SamplerState currentSampler;
    SamplerState historySampler;
    float2 currPixelUV;
    float2 historyPixelUV;
    float blendFactor;
};

struct ColorAABB
{
    float4 boundsMin;
    float4 boundsMax;
};


float4 GetTemporalFilteredResult(TemporalFilterInput input)
{
    float4 currentColor = SampleTexture(input.currentBuffer,input.currentSampler,input.currPixelUV);
    float4 motionVector = SampleTexture(input.velocityBuffer,input.currentSampler,input.currPixelUV);
    float2 historyUV = input.currPixelUV + motionVector;
    [branch] if (any(historyUV < float2(0.0,0.0)) || any(historyUV > float2(1.0,1.0)))
    {
        return currentColor;
    }
    float4 historyColor = SampleTexture(input.historyBuffer,input.historySampler,historyUV);

    return lerp(historyColor,currentColor,input.blendFactor);
}


float4 GetTemporalFilteredResult(NoMotionTemporalFilterInput input)
{
    float4 currentColor = SampleTexture(input.currentBuffer,input.currentSampler,input.currPixelUV);
    uint x,y;
    input.historyBuffer.GetDimensions(x,y);
    float du = rcp(x);
    float dv = rcp(y);
    // float4 historyColor = SampleTextureBilateralBlurred(input.historyBuffer,input.historySampler,historyUV,float2(du,dv),bilateralDesc);
    float4 historyColor = SampleTexture(input.historyBuffer,input.historySampler,input.historyPixelUV);
    ColorAABB s;
    float4 a = SampleTexture(input.currentBuffer,input.currentSampler,input.currPixelUV + float2(-du,0.0));
    float4 b = SampleTexture(input.currentBuffer,input.currentSampler,input.currPixelUV + float2(0.0,-dv));
    float4 c = SampleTexture(input.currentBuffer,input.currentSampler,input.currPixelUV + float2(du,0.0));
    float4 d = SampleTexture(input.currentBuffer,input.currentSampler,input.currPixelUV + float2(0.0,dv));
    s.boundsMin = min(min(min(a,b),c),d);
    s.boundsMax = max(max(max(a,b),c),d);
   // historyColor = clamp(historyColor,s.boundsMin,s.boundsMax);
    float4 color = lerp(historyColor,currentColor,input.blendFactor);
    return color;
}

float4 GetTemporalFilteredResult(NoMotionTemporalFilterInput input,BilateralFilterDesc bilateralDesc)
{
    uint x,y;
    input.historyBuffer.GetDimensions(x,y);
    float du = rcp(x);
    float dv = rcp(y);
    float4 currentColor = SampleTexture(input.currentBuffer,input.currentSampler,input.currPixelUV);
  //  float4 historyColor = SampleTexture(input.historyBuffer,input.historySampler,input.historyPixelUV);
    float4 historyColor = SampleTexture(input.historyBuffer,input.historySampler,input.historyPixelUV);
    if (all(historyColor < float4(0.001,0.001,0.001,0.001)))
    {
        input.blendFactor = 1.0;
    }
    /*ColorAABB s;
    float2 historyUV = input.historyPixelUV;
    float4 a = SampleTexture(input.historyBuffer,input.historySampler,historyUV + float2(-du,0.0));
    float4 b = SampleTexture(input.historyBuffer,input.historySampler,historyUV + float2(0.0,-dv));
    float4 c = SampleTexture(input.historyBuffer,input.historySampler,historyUV + float2(du,0.0));
    float4 d = SampleTexture(input.historyBuffer,input.historySampler,historyUV + float2(0.0,dv));
    s.boundsMin = min(min(min(a,b),c),d);
    s.boundsMax = max(max(max(a,b),c),d);*/
    
    /*if (all(historyColor < float4(0.01,0.01,0.01,0.01)))
    {
        return currentColor;
    }*/
    float4 color = lerp(historyColor,currentColor,input.blendFactor);
 //   color = clamp(color,s.boundsMin,s.boundsMax);
    return color;
}

#endif
```
