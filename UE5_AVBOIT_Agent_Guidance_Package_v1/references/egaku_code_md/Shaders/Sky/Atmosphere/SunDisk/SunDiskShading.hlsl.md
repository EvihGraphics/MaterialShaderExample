# Egaku source: `Shaders/Sky/Atmosphere/SunDisk/SunDiskShading.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef SUN_DISK_SHADING_HLSL
#define SUN_DISK_SHADING_HLSL

static float3 sunDiskDirection;

void SetSunDiskDirection(float3 direction)
{
    sunDiskDirection = direction;
}

float GetSunDiskSolidAngle()
{
    return 0;
}

bool IsDirectionInSunDisk(float3 direction){
    const float pi = 3.1415926535897932384626433832795;
    float diskD = cos(10*0.2725 *  (pi / 180.0f));
    float d = dot(direction, sunDiskDirection);
    return saturate(d - diskD);
}

float3 GetSunDiskColor(float3 direction)
{
    float3 luminance = 1;
    return IsDirectionInSunDisk(direction) * luminance;
}



#endif
```
