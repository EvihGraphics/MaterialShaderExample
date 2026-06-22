# Egaku source: `Shaders/Sky/Atmosphere/Volume/MieVolume/DustVolume.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef DUST_VOLUME_HLSL
#define DUST_VOLUME_HLSL
#include "Assets/Shaders/Sky/Basic/VolumeBase/Volume.hlsl"
#include "Assets/Shaders/Sky/Atmosphere/Atmosphere.ParamStruct.hlsl"

float GetDustVolumeDensityHeightAttenuation(float altitude,float scalarHeight)
{
    return exp(-altitude / scalarHeight) * FetchParam(AtmosphereVolumeDesc).dustDensityFraction;
}


VolumeDesc GetDustVolumeDesc(float altitude = 0)
{
    VolumeDesc desc = (VolumeDesc)0;
    float3 sigma = float3(3.996, 3.996, 3.996) * 1e-6f;
    float density = GetDustVolumeDensityHeightAttenuation(altitude, 1200);
    desc.scatteringCoeff = sigma * density;
    desc.absorptionCoeff = desc.scatteringCoeff*1.11f;
    return desc;
}





#endif
```
