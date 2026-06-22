# Egaku source: `Shaders/Sky/Atmosphere/Volume/RayleighVolume/AirVolume.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef AIR_VOLUME_HLSL
#define AIR_VOLUME_HLSL
#include "Assets/Shaders/Sky/Basic/VolumeBase/Volume.hlsl"
#include "Assets/Shaders/Sky/Atmosphere/Atmosphere.ParamStruct.hlsl"

float GetAirVolumeDensityHeightAttenuation(float altitude,float scalarHeight)
{
    return exp(-altitude / scalarHeight) * FetchParam(AtmosphereVolumeDesc).airDensityFraction;
}

//https://www.researchgate.net/profile/David-Ebert-5/publication/2900319_Efficient_Rendering_of_Atmospheric_Phenomena/links/02e7e528de775974e7000000/Efficient-Rendering-of-Atmospheric-Phenomena.pdf
//Riley K, Ebert D S, Kraus M, et al. Efficient rendering of atmospheric phenomena[J]. Rendering Techniques, 2004, 4: 374-386.
VolumeDesc GetAirVolumeDesc(float altitude = 0)
{
    VolumeDesc desc = (VolumeDesc)0;
    float3 sigma = float3(5.8, 13.5, 33.1) * 1e-6f;
    float density = GetAirVolumeDensityHeightAttenuation(altitude, 8500);
    desc.scatteringCoeff = sigma * density;
    desc.absorptionCoeff = 0;
    return desc;
}




#endif
```
