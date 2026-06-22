# Egaku source: `ShaderLibrary/Modules/Blit/Blit.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef BLIT_HLSL
#define BLIT_HLSL
#include "Blit.ParamTable.hlsl"

struct Attributes
{
    float3 positionOS : POSITION;
    float2 uv : TEXCOORD0;
};

struct Varyings
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

Varyings Vertex(Attributes a)
{
    Varyings o;
    o.position = TransformObjectToHClip(a.positionOS);
    o.uv = a.uv;
    return o;
}


float4 Fragment(Varyings v) : SV_TARGET
{
    float4 color = _MainTex.Sample(sampler_MainTex,v.uv);
    color += _Color;
    return color;
}

#endif
```
