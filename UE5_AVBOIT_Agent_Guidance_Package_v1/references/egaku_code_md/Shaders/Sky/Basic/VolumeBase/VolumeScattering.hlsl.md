# Egaku source: `Shaders/Sky/Basic/VolumeBase/VolumeScattering.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef VOLUME_SCATTERING_HLSL
#define VOLUME_SCATTERING_HLSL
#include "VolumeRayMarching.hlsl"


float3 VolumeScattering_CalcLuminanceHomogeneousApprox(VoxelData voxelData,float3 scatteredLuminance,float3 opticalDepth)
{
    float3 transmittance = BeerTransmittance(opticalDepth);
    voxelData.sigmaT = max(voxelData.sigmaT,0.000001f);
    return HomogeneousMediumScatter(scatteredLuminance,voxelData.sigmaT,transmittance);
}


#endif
```
