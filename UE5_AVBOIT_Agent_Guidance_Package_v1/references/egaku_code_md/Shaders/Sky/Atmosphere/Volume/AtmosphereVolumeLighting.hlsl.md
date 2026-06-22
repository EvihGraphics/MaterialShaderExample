# Egaku source: `Shaders/Sky/Atmosphere/Volume/AtmosphereVolumeLighting.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef ATMOSPHERE_VOLUME_LIGHTING_HLSL
#define ATMOSPHERE_VOLUME_LIGHTING_HLSL

#include "Assets/Shaders/Sky/Basic/VolumeBase/VolumeLighting.hlsl"

float GetPhase_Rayleigh(float cosTheta)
{
    return RayleighPhase(cosTheta);
}

float GetPhase_Mie(float cosTheta,float anisotropy)
{
    return HGPhase(anisotropy,cosTheta);
}


#endif
```
