# Egaku source: `Shaders/Sky/Basic/VolumeBase/Volume.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef VOLUME_HLSL
#define VOLUME_HLSL

struct VoxelData
{
    float3 position;
    float3 sigmaSPerUnit;
    float3 sigmaAPerUnit;
    float3 sigmaTPerUnit;
    float3 sigmaS;
    float3 sigmaA;
    float3 sigmaT;
    float density;
};

struct VolumeDesc
{
    float3 scatteringCoeff;
    float3 absorptionCoeff;
    float hgPhaseG1;
    float hgPhaseG2;
};

struct BoundingBox {
    float3 boundsMax;
    float3 boundsMin;
};

struct Sphere
{
    float3 center;
    float radius;
};

struct BoundingSphere
{
    float3 center;
    float radiusMin;
    float radiusMax;
};



#endif
```
