# Egaku source: `Shaders/Ocean/Modules/Noise/SmoothNoise.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef SMOOTH_NOISE_HLSL
#define SMOOTH_NOISE_HLSL
#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Texture.hlsl"
#include "Ocean.ParamStruct.hlsl"


float SampleSmoothNoiseMap(float2 uv)
{
    float noise = SampleTextureLevel(FetchParam(SmoothNoiseMapDesc).tex, sampler_LinearRepeat, uv, 0).r;
    return noise;
}

#endif
```
