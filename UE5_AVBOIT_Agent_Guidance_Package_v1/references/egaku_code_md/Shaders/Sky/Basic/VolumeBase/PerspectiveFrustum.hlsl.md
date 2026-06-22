# Egaku source: `Shaders/Sky/Basic/VolumeBase/PerspectiveFrustum.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef PERSPECTIVE_FRUSTUM_HLSL
#define PERSPECTIVE_FRUSTUM_HLSL


float4x4 GetFrustumPlanesWSNoNearFar(float4x4 projectMatrix)
{
    float4x4 planes;
    planes[0] = projectMatrix[3] - projectMatrix[0];
    planes[1] = projectMatrix[3] + projectMatrix[0];
    planes[2] = projectMatrix[3] - projectMatrix[1];
    planes[3] = projectMatrix[3] + projectMatrix[1];
    return planes;
}

bool FrustumCullVector(float4x4 mat,float3 vectorVS)
{
    float4x4 planes = GetFrustumPlanesWSNoNearFar(mat);
    return dot(planes[0], vectorVS) < 0.0f ||
        dot(planes[1], vectorVS) < 0.0f ||
        dot(planes[2], vectorVS) < 0.0f ||
        dot(planes[3], vectorVS) < 0.0f;
}

#endif
```
