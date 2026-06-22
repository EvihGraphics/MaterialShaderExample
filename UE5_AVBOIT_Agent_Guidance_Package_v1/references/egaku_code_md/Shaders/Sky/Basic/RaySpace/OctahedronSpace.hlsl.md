# Egaku source: `Shaders/Sky/Basic/RaySpace/OctahedronSpace.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef OCTAHEDRON_SPACE_HLSL
#define OCTAHEDRON_SPACE_HLSL

float3 OctahedronSpace_GetHemiDirectionFromSampleCoords(float2 uv)
{
    float2 uv2 = uv * 2.0 - 1.0;
    uv2 = float2(uv2.x + uv2.y, uv2.x - uv2.y)*0.5;
    float3 n = float3(uv2,1-dot(1.0,abs(uv2)));
    return normalize(n).xzy;
}

float2 OctahedronSpace_GetHemiDirectionSampleCoords(float3 direction)
{
    direction.xz /= dot(1.0, abs(direction));
    return float2(direction.x +direction.z, direction.x - direction.z) * 0.5 + 0.5;
}


#endif
```
