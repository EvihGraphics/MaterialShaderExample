# Egaku source: `Shaders/Sky2/Cloud/HLSLInclude/cloud_RayJitterUtils.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef CLOUD_RAY_JITTER_UTILS_HLSL
#define CLOUD_RAY_JITTER_UTILS_HLSL

struct JitterDesc
{
    bool jitter;
};

float3 JitterPoint(float3 p,JitterDesc desc)
{
    return p;
}

#endif
```
