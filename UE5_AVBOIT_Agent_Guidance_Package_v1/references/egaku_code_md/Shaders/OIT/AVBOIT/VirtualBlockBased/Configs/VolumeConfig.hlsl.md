# Egaku source: `Shaders/OIT/AVBOIT/VirtualBlockBased/Configs/VolumeConfig.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef VOLUME_CONFIG_HLSL
#define VOLUME_CONFIG_HLSL
#define AVBOIT_GET_DIMENSIONS_FROM_SHADER_CONSTANTS

float3 _AVBOIT_PhysicalVolumeDimensions;
float3 _AVBOIT_VolumeBlockDimensions;
float3 _AVBOIT_VirtualVolumeDimensions;
float3 _AVBOIT_VolumeBlockDivisor;

struct VolumeConfig
{
    float3 physicalVolumeDimensions;
    float3 volumeBlockDimensions;
    float3 virtualVolumeDimensions;
    float3 volumeBlockDivisor;
};

VolumeConfig AVBOIT_GetVolumeConfig()
{
    VolumeConfig config;
    config.physicalVolumeDimensions = _AVBOIT_PhysicalVolumeDimensions;
    config.volumeBlockDimensions = _AVBOIT_VolumeBlockDimensions;
    config.virtualVolumeDimensions = _AVBOIT_VirtualVolumeDimensions;
    config.volumeBlockDivisor = _AVBOIT_VolumeBlockDivisor;
    return config;
}


#endif
```
