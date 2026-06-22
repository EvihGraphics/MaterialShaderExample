# Egaku source: `ShaderLibrary/Context.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef CONTEXT_HLSL
#define CONTEXT_HLSL
#include "Core.hlsl"


struct TimeContext
{
    float timeOverTwenty;
    float timeInSec;
    float doubledTime;
    float tripledTime;
};

TimeContext GetTimeContext()
{
    TimeContext time;
    time.timeOverTwenty = _Time.x;
    time.timeInSec = _Time.y;
    time.doubledTime = _Time.z;
    time.tripledTime = _Time.w;
    return time;
}

#endif
```
