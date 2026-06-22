# Egaku source: `Shaders/Sky/Atmosphere/Volume/RayleighVolume/OzoneVolume.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef OZONE_VOLUME_HLSL
#define OZONE_VOLUME_HLSL
#include "Assets/Shaders/Sky/Basic/VolumeBase/Volume.hlsl"
#include "Assets/Shaders/Sky/Atmosphere/Atmosphere.ParamStruct.hlsl"

float GetOzoneHeightDistribution(float altitude,float centerAltitude,float thickness)
{
    return max(0,1 - abs(altitude - centerAltitude) / thickness) * FetchParam(AtmosphereVolumeDesc).ozoneDensityFraction;
}

VolumeDesc GetOzoneVolumeDesc(float altitude = 0)
{
    VolumeDesc coeffs = (VolumeDesc)0;
    float3 sigma = float3(0.650,1.881,0.085) * 1e-6f;
    float density = GetOzoneHeightDistribution(altitude, 25000, 15000);
    coeffs.scatteringCoeff = 0;
    coeffs.absorptionCoeff = sigma * density;
    return coeffs;
}


#endif
```
