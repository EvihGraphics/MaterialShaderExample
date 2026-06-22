# Egaku source: `Shaders/Sky/Skybox/Skybox.AtmospherePrt.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef SKYBOX_ATMOSPHERE_PRT_HLSL
#define SKYBOX_ATMOSPHERE_PRT_HLSL
#include "Assets/Shaders/Sky/Basic/RaySpace/OctahedronSpace.hlsl"
#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Texture.hlsl"


Texture2D _AtmospherePrt;

float4 SampleAtmosphereOctahedronPrt(float3 direction)
{
    float2 uv = OctahedronSpace_GetHemiDirectionSampleCoords(direction);
    float4 v = SampleTexture(_AtmospherePrt,sampler_LinearClamp,uv);
    return v;
}

#endif
```
