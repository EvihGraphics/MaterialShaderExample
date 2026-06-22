# Egaku source: `Shaders/OIT/AVBOIT/VirtualBlockBased/Volume/PhysicalVolumeConfigInterface.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef PHYSICAL_VOLUME_CONFIG_INTERFACE_HLSL
#define PHYSICAL_VOLUME_CONFIG_INTERFACE_HLSL

uint3 PhysicalVolumeConfig_GetVolumeDimensions();
uint3 PhysicalVolumeConfig_GetVolumeBlockDimensions();


float PhysicalVolume_TransformVirtualVolumeBlockIndexToPhysical(float virtualVolumeBlockIndex);

#endif
```
