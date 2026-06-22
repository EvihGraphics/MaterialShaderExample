# Egaku source: `Shaders/Ocean/Modules/Reflection/ReflectionTexture.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef REFLECTION_TEXTURE_HLSL
#define REFLECTION_TEXTURE_HLSL

Texture2D _ReflectionTex;

float4 SampleReflectionTexture(float2 uv)
{
    return SampleTexture(_ReflectionTex,sampler_LinearClamp,uv);
}

#endif
```
