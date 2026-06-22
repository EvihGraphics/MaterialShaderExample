# Egaku source: `Shaders/Sky/Cloud/Volumetric/Shading/VolumeLighting/CloudVolumeLighting.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef CLOUD_VOLUME_LIGHTING_HLSL
#define CLOUD_VOLUME_LIGHTING_HLSL
#include "Assets/Shaders/Sky/Basic/VolumeBase/VolumeScattering.hlsl"
#include "Assets/Shaders/Sky/Basic/VolumeBase/VolumeLighting.hlsl"

float3 CloudVolumeLighting_CalcCloudMainLightScatteringLuminance(VoxelData voxelData,RayMarchingResult currentViewMarchingResult,float3 mainLightVisibility,float mainLightPhaseCosine);
float3 CloudVolumeLighting_GetMainLightDirection();
float3 CloudVolumeLighting_CalcCloudAmbientScatteringLuminance(VoxelData voxelData,RayMarchingResult currentViewMarchingResult,float3 viewDirection,float3 ambientVisibility);


#endif
```
