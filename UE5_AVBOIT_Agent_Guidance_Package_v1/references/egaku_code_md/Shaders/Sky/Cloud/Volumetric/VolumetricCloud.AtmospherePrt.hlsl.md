# Egaku source: `Shaders/Sky/Cloud/Volumetric/VolumetricCloud.AtmospherePrt.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef VOLUMETRIC_CLOUD_ATMOSPHERE_PRT_HLSL
#define VOLUMETRIC_CLOUD_ATMOSPHERE_PRT_HLSL
#include "Assets/Shaders/Sky/Basic/RaySpace/OctahedronSpace.hlsl"
#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Texture.hlsl"


Texture2D _AtmospherePrt;

float4 SampleAtmosphereOctahedronPrt(float3 direction)
{
    float2 uv = OctahedronSpace_GetHemiDirectionFromSampleCoords(direction);
    #ifdef VOLUMETRIC_CLOUD_USE_COMPUTE_SHADER
    float4 v = SampleTextureLevel(_AtmospherePrt,sampler_LinearClamp,uv,0);
    #else
    float4 v = SampleTexture(_AtmospherePrt,sampler_LinearClamp,uv);
    #endif
    return v;
}

#endif
```
