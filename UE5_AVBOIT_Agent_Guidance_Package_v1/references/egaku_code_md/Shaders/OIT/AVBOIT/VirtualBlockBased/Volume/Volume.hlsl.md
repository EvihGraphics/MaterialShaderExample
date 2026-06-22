# Egaku source: `Shaders/OIT/AVBOIT/VirtualBlockBased/Volume/Volume.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef VOLUME_HLSL
#define VOLUME_HLSL
#include "Configs/VolumeConfig.hlsl"
#include "FrustumVolume.hlsl"
#include "VirtualVolume.hlsl"
#include "PhysicalVolume.hlsl"
#include "VolumeUtils.hlsl"

#ifdef AVBOIT_GET_DIMENSIONS_FROM_SHADER_CONSTANTS
uint3 PhysicalVolumeConfig_GetVolumeDimensions()
{
    return AVBOIT_GetVolumeConfig().physicalVolumeDimensions;
}

uint3 VirtualVolumeConfig_GetVolumeBlockDimensions()
{
    return AVBOIT_GetVolumeConfig().volumeBlockDimensions;
}

uint3 PhysicalVolumeConfig_GetVolumeBlockDimensions()
{
    return VirtualVolumeConfig_GetVolumeBlockDimensions();
}

uint3 VirtualVolumeConfig_GetVolumeDimensionsInBlock()
{
    return AVBOIT_GetVolumeConfig().volumeBlockDivisor;
}

#endif



#endif
```
