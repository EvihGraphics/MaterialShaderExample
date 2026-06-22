# Egaku source: `Shaders/Sky/Cloud/Volumetric/Octahedron/OctahedronUtils.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef OCTAHEDRON_UTILS_HLSL
#define OCTAHEDRON_UTILS_HLSL

/*
float3 HemiOctahedronUVToYUpVector(float2 uv)
{
    float2 uv2 = uv * 2.0 - 1.0;
    uv2 = float2(uv2.x + uv2.y, uv2.x - uv2.y)*0.5;
    float3 n = float3(uv2,1-dot(1.0,abs(uv2)));
    return normalize(n).xzy;
}
        
float2 YUpVectorToHemiOctahedronUV(float3 n)
{
    n.xz /= dot(1.0, abs(n));
    return float2(n.x + n.z, n.x - n.z) * 0.5 + 0.5;
}
*/




#endif
```
