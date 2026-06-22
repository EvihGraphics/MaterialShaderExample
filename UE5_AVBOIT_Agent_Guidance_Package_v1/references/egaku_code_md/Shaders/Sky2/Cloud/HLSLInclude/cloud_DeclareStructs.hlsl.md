# Egaku source: `Shaders/Sky2/Cloud/HLSLInclude/cloud_DeclareStructs.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef CLOUD_DECLARE_STRUCTS_HLSL
#define CLOUD_DECLARE_STRUCTS_HLSL


struct BoundingBox {
    float3 boundsMax;
    float3 boundsMin;
};

struct Sphere
{
    float3 center;
    float radiusMin;
    float radiusMax;
};

struct Atmosphere
{
    float3 center;
    float heightMin;
    float heightMax;
};

struct BoundsHitResult {
    float distToBox;
    float distInsideBox;
};


struct Ray
{
    float3 origin;
    float3 direction;
    float3 hitPoint;
};

struct RayMarchingDesc
{
    float distance;
    float maxStepCnt;
    float stepSize;
};

struct VoxelDensitySampleDesc
{
    SamplerState samplerState;
    float4 valueOffset;
    float4 valueScale;
    float4 coordsScale;
};

struct DensityVolume
{
    Texture3D volumeTex;
    VoxelDensitySampleDesc sampleDesc;
    float4 channelWeights;
};

struct MapTexture
{
    Texture2D mapTex;
    VoxelDensitySampleDesc sampleDesc;
    float4 channelWeights;
};

struct OctaveScatterDesc
{
    float extFactor;
    float scatterFactor;
    float phaseFactor;
    int count;
};

struct VolumeLightingDesc
{
    float3 sigmaExt;
    float3 sigmaScatter;
    float hgPhaseG1Factor;
    float hgPhaseG2Factor;
    OctaveScatterDesc octaveScatterDesc;
};

struct CloudVolumeDesc
{
    DensityVolume basicDensity;
    DensityVolume detailDensity;
    MapTexture weatherMap;
    float densityScale;
    VolumeLightingDesc lightingDesc;
};

struct CloudRayMarchingConfig
{
    float viewMarchStepSize;
    float lightMarchStepCnt;
};

struct SunLightDesc {
    float3 direction;
    float3 color;
};

struct AmbientLightDesc
{
    float3 color;
    float phase;
};




#endif
```
