# Egaku source: `Shaders/Sky/Cloud/Volumetric/VolumetricCloud.BlueNoise.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef VOLUMETRIC_CLOUD_BLUE_NOISE_HLSL
#define VOLUMETRIC_CLOUD_BLUE_NOISE_HLSL
#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Texture.hlsl"

Texture2D  _BlueNoise;

float4 SampleBlueNoise(float2 uv)
{
    #ifndef VOLUMETRIC_CLOUD_USE_COMPUTE_SHADER
    return SampleTexture(_BlueNoise,sampler_PointRepeat,uv);
    #else
    return SampleTextureLevel(_BlueNoise,sampler_PointRepeat,uv,0);
    #endif
}




#endif
```
