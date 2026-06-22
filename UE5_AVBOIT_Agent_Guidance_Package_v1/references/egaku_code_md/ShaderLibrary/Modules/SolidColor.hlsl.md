# Egaku source: `ShaderLibrary/Modules/SolidColor.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef SOLID_COLOR_INCLUDED
#define SOLID_COLOR_INCLUDED
#include "SolidColor.ParamTable.hlsl"

struct Attributes
{
    float3 positionOS : POSITION;
    DECLARE_VERTEX_INPUT_INSTANCE_ID
};

struct Varyings
{
    float4 position : SV_POSITION;
};

Varyings Vertex(Attributes a)
{
    SETUP_INSTANCE_ID(a);
    Varyings o;
    o.position = TransformObjectToHClip(a.positionOS);
    return o;
}


float4 Fragment(Varyings v) : SV_TARGET
{
    return _SolidColor;
}

#endif
```
