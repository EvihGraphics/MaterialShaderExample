# Egaku source: `Shaders/OIT/AVBOIT/VirtualBlockBased/Volume/FrustumVolume.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef FRUSTUM_VOLUME_HLSL
#define FRUSTUM_VOLUME_HLSL

float _AVBOIT_FrustumFarPlane;
int _AVBOIT_FrustumSliceCurveFactor;
uint _AVBOIT_FrustumSliceCount;

float FrustumVolume_RemapFrustumDepth(float viewDepth)
{
    float farPlane = _AVBOIT_FrustumFarPlane;
    float linearFactor = _AVBOIT_FrustumSliceCurveFactor;
    return log(viewDepth/linearFactor+1.0)*rcp(log(farPlane/linearFactor+1.0));
}

uint FrustumVolume_GetVolumeSliceCount()
{
    return _AVBOIT_FrustumSliceCount;
}

float FrustumVolume_GetVolumeSliceIndex(float viewDepth,uint sliceCount)
{
    return (sliceCount - 1) * FrustumVolume_RemapFrustumDepth(viewDepth);
}

float FrustumVolume_GetVolumeSliceIndex(float viewDepth)
{
    return FrustumVolume_GetVolumeSliceIndex(viewDepth, FrustumVolume_GetVolumeSliceCount());
}


#endif
```
