# Egaku source: `Shaders/Sky/Skybox/Skybox.SunDisk.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef SKYBOX_SUNDISK_HLSL
#define SKYBOX_SUNDISK_HLSL


float GetSunDiskSolidAngle()
{
    return 0;
}

bool IsDirectionInSunDisk(float3 sunDiskDirection,float3 direction){
    const float pi = 3.1415926535897932384626433832795;
    float diskD = cos(0.2725 *  (pi / 180.0f));
    float d = dot(direction, sunDiskDirection);
    return saturate(d - diskD);
}



#endif
```
