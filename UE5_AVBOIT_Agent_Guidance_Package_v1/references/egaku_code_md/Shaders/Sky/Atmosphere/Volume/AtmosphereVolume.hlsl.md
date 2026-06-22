# Egaku source: `Shaders/Sky/Atmosphere/Volume/AtmosphereVolume.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef ATMOSPHERE_VOLUME_HLSL
#define ATMOSPHERE_VOLUME_HLSL

#include "Assets/Shaders/Sky/Basic/Utils.hlsl"
#include "Assets/Shaders/Sky/Basic/VolumeBase/Volume.hlsl"

/*struct VoxelFraction
{
    float dustPercentage;
    float airPercentage;
    float ozonePercentage;
    float density;
};*/

BoundingSphere GetAtmosphereVolumeBoundingSphere()
{
    BoundingSphere sphere;
    const float earthRadius = 6371000.f;
    const float atmosphereBottom = earthRadius;
    const float atmosphereTop = earthRadius + 40000.f;
    sphere.center = -ViewVerticalDir() * earthRadius;
    sphere.radiusMin = atmosphereBottom; // earth
    sphere.radiusMax = atmosphereTop;
    return sphere;
}

float GetAltitudeFromPositionWS(float3 positionWS)
{
    BoundingSphere sphere = GetAtmosphereVolumeBoundingSphere();
    return length(positionWS - sphere.center) - sphere.radiusMin;
}


/*VoxelFraction SampleAtmosphereVoxelFraction(SamplePoint samplePoint)
{
    float altitude = GetAltitudeFromPositionWS(samplePoint.position);
    float dust = GetDustVolumeDensityHeightAttenuation(altitude,1200.0f);
    float air = GetAirVolumeDensityHeightAttenuation(altitude,8500.0f);
    float ozone = GetOzoneHeightDistribution(altitude,25000.0f,15000.0f);
    float total = dust + air + ozone;
    VoxelFraction fract;
    fract.dustPercentage = dust / total;
    fract.airPercentage = air / total;
    fract.ozonePercentage = ozone / total;
    fract.density = total;
    return fract;
}*/




#endif
```
