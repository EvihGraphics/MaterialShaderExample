# Egaku source: `Shaders/Sky/Skybox/Skybox.AtmosphereTransmittanceLut.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef SKYBOX_ATMOSPHERE_TRANSMITTANCE_LUT_HLSL
#define SKYBOX_ATMOSPHERE_TRANSMITTANCE_LUT_HLSL

#define USE_ATMOSPHERE_TRANSMITTANCE_LUT_SRV
#include "Assets/Shaders/Sky/Atmosphere/Atmosphere.TransmittanceLut.hlsl"
#include "Assets/Shaders/Sky/Atmosphere/Volume/AtmosphereVolume.hlsl"


float3 LookupAtmosphereDirectionTransmittance(float3 position,float3 direction)
{
   
    BoundingSphere bounds = GetAtmosphereVolumeBoundingSphere();
    float3 positionVec = position - bounds.center;
    float directionZenithCosine = dot(normalize(positionVec), direction);
    float radius = length(positionVec);
    float4 t = SampleAtmosphereTransmittanceLut(radius, directionZenithCosine,bounds.radiusMin, bounds.radiusMax);
    return t;
}


float3 LookupCloudAtmosphereTransmittance(float3 position,float3 direction)
{
    float3 t1 = LookupAtmosphereDirectionTransmittance(position, direction);
    float3 t2 = LookupAtmosphereDirectionTransmittance(position + direction * 1000.0f, direction);
    return t1 * rcp(t2);
}

#endif
```
