# Egaku source: `Shaders/Sky/Atmosphere/Atmosphere.TransmittanceLutUtil.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef ATMOSPHERE_TRANSMITTANCE_LUT_UTIL_HLSL
#define ATMOSPHERE_TRANSMITTANCE_LUT_UTIL_HLSL


struct TransmittanceLUTRayMarchingDesc
{
    float directionZenithCosine;
    float radius;
    float dist;
};

TransmittanceLUTRayMarchingDesc GetTransmittanceLUTRayMarchingDirection0(float2 uv,float bottomRadius, float topRadius)
{
    float maxH = sqrt(topRadius * topRadius - bottomRadius * bottomRadius);
    float h = uv.y * maxH;
    float radius = sqrt(h * h + bottomRadius * bottomRadius);
    float maxD = h + maxH;
    float minD = topRadius - radius;
    float d = uv.x * maxD + (1 - uv.x) * minD;
    float directionCosine = (topRadius*topRadius - radius * radius - d*d)/(2*radius*d);
    TransmittanceLUTRayMarchingDesc desc;
    desc.directionZenithCosine = directionCosine;
    desc.radius = radius;
    desc.dist = d;
    return desc;
}

TransmittanceLUTRayMarchingDesc GetTransmittanceLUTRayMarchingDirection(float2 uv,float bottomRadius, float topRadius)
{
    float maxH = sqrt((topRadius - bottomRadius) * (topRadius + bottomRadius));
    float h = uv.y * maxH;
    float radius = sqrt(h * h + bottomRadius * bottomRadius);
    float maxD = h + maxH;
    float minD = topRadius - radius;
    float d = uv.x * maxD + (1 - uv.x) * minD;
    float directionCosine = 0.5 * ((topRadius/radius) * (topRadius/d) - radius / d - d/radius);
    TransmittanceLUTRayMarchingDesc desc;
    desc.directionZenithCosine = directionCosine;
    desc.radius = radius;
    desc.dist = d;
    return desc;
}

float2 GetTransmittanceLUTSampleUV(TransmittanceLUTRayMarchingDesc desc,float bottomRadius, float topRadius)
{
    float radius = desc.radius;
    float directionCosine = desc.directionZenithCosine;
    float minH = 0;
    float maxH = sqrt(topRadius * topRadius - bottomRadius * bottomRadius);
    float h = sqrt(max(0,radius * radius - bottomRadius * bottomRadius));
    float discriminant = radius * radius * (directionCosine * directionCosine - 1) + topRadius * topRadius;
    float d = max(0.0f, -radius * directionCosine + sqrt(discriminant));
    float maxD = maxH + h;
    float minD = topRadius - radius;
    float u = (d - minD) / (maxD - minD);
    float v = (h - minH) / (maxH - minH);
    return float2(u, v);
}

#endif
```
