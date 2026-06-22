# Egaku source: `Shaders/Sky/Cloud/Volumetric/Filters/BilateralFilter.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef BILATERAL_FILTER_HLSL
#define BILATERAL_FILTER_HLSL
#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Texture.hlsl"

#define PI 3.141592653

/*float3 BilateralFilter_GetValue(float2 coords);

float3 GaussianBlur(float2 coords, float kernelSize,float sigma){
    float weightSum = 0;
    float3 valueSum = 0;

    float3 color = BilateralFilter_GetValue(coords);
    
    for(int i = -kernelSize; i < kernelSize; i++){
        for(int j = -kernelSize; j < kernelSize; j++){
            float factor = i * i + j * j;
            factor = (-factor) / (2 * sigma * sigma);
            float weight = 1/(sigma * sigma * 2 * PI) * exp(factor);
            weightSum += weight;
            valueSum += BilateralFilter_GetValue(coords + float2(i, j)) * weight;
        }
    }

    if(weightSum > 0){
        color = valueSum / weightSum;
    }

    return color;
}*/

struct BilateralFilterDesc{
    float kernelSize;
    float spaceSigma;
    float4 valueSigma;
};


float4 SampleTextureBilateralBlurred(Texture2D tex,SamplerState texSampler,float2 uv,float2 duv,BilateralFilterDesc desc){
    float4 weightSum = 0;
    float4 valueSum = 0;

    float4 centerValue = SampleTexture(tex,texSampler,uv);
    int kernelSize = desc.kernelSize;
    float spaceSigma = desc.spaceSigma;
    float4 valueSigma = desc.valueSigma;
    [loop]
    for(int i = -kernelSize; i < kernelSize; i++){
        [loop]
        for(int j = -kernelSize; j < kernelSize; j++){
            float spaceFactor = i * i + j * j;
            spaceFactor = (-spaceFactor) / (2 * spaceSigma * spaceSigma);
            float spaceWeight = 1/(spaceSigma * spaceSigma * 2 * PI) * exp(spaceFactor);
            
            float4 valueNeighbor = SampleTexture(tex,sampler_LinearClamp,uv + duv*float2(i, j));
            float4 valueDiff = valueNeighbor - centerValue;
            float4 valueFactor = valueDiff * valueDiff ;
            valueFactor = (-valueFactor) / (2 * valueSigma * valueSigma);
            float4 valueWeight = (1 / (2 * PI * valueSigma)) * exp(valueFactor);

            float4 weight = spaceWeight;// * valueWeight;
            weightSum += weight;
            valueSum += valueNeighbor * weight;
        }
    }
    
    float4 value = centerValue;
    if(all(weightSum > 0)){
        value = valueSum / weightSum;
    }
    return value;
}

#endif
```
