# Egaku source: `Shaders/Sky/Cloud/Volumetric/CloudDensity/DensityNoise3D.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef DENSITY_NOISE_3D_HLSL
#define DENSITY_NOISE_3D_HLSL
#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Texture.hlsl"

struct DensityNoiseSampleDesc
{
    Texture3D tex;
    float volumeScale;
    float4 channelWeights;
    float4 noiseScale;
    float4 noiseOffset;
};

float SampleDensityNoise3D(DensityNoiseSampleDesc desc,float3 coords)
{
    Texture3D tex = desc.tex;
    coords = coords * rcp(desc .volumeScale);
    #ifndef VOLUMETRIC_CLOUD_USE_COMPUTE_SHADER
    float4 sampleValue = SampleTexture3DLevel(tex,sampler_LinearRepeat,coords,0);
    #else
    float4 sampleValue = SampleTexture3DLevel(tex,sampler_LinearRepeat,coords,0);
    #endif
    sampleValue = saturate(sampleValue) * desc.noiseScale + desc.noiseOffset;
    return saturate(dot(sampleValue, desc.channelWeights));
}



#endif
```
