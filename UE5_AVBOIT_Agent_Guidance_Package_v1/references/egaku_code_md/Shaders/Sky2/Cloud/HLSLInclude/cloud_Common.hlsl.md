# Egaku source: `Shaders/Sky2/Cloud/HLSLInclude/cloud_Common.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef CLOUD_COMMON_HLSL
#define CLOUD_COMMON_HLSL

#include "Assets/Shaders/HLSLInclude/ScreenSpaceUtils.hlsl"
#include "cloud_DeclareStructs.hlsl"
#include "cloud_DeclareVars.hlsl"



Ray GetWorldSpaceRay(float2 screenUV)
{
 
    float3 rayOriginWS;
    float3 rayDirWS;
    float3 rayHitPoint;
    
    float3 positionWS = GetWorldPosition(screenUV);
    rayHitPoint =  positionWS;
    rayOriginWS = _WorldSpaceCameraPos;
    rayDirWS = normalize(positionWS - rayOriginWS);

    Ray ray;
    ray.origin = rayOriginWS;
    ray.direction = rayDirWS;
    ray.hitPoint = rayHitPoint;
    return ray;
}

BoundingBox GetBoundingBox()
{
    BoundingBox box;
    box.boundsMax = _CloudBounds_Max;
    box.boundsMin = _CloudBounds_Min;
    return box;
}

CloudRayMarchingConfig GetCloudRayMarchingConfig()
{
    CloudRayMarchingConfig config;
    config.lightMarchStepCnt = _RayMarching_LightMarchStepCount;
    config.viewMarchStepSize = _RayMarching_ViewMarchStepSize;
    return config;
}

DensityVolume GetBasicCloudDensityVolume()
{
    DensityVolume volume;
    volume.volumeTex = _BasicCloud_DensityVolumeTex;
    volume.channelWeights = _BasicCloud_ChannelWeights;
    VoxelDensitySampleDesc sampleDesc;
    sampleDesc.coordsScale = _BasicCloud_CoordsScale;
    sampleDesc.samplerState = sampler_BasicCloud_DensityVolumeTex;
    sampleDesc.valueOffset = _BasicCloud_ValueOffset;
    sampleDesc.valueScale = _BasicCloud_ValueScale;
    volume.sampleDesc = sampleDesc;
    return volume;
}

DensityVolume GetDetailCloudDensityVolume()
{
    DensityVolume volume;
    volume.volumeTex = _DetailCloud_DensityVolumeTex;
    volume.channelWeights = _DetailCloud_ChannelWeights;
    VoxelDensitySampleDesc sampleDesc;
    sampleDesc.coordsScale = _DetailCloud_CoordsScale;
    sampleDesc.samplerState = sampler_DetailCloud_DensityVolumeTex;
    sampleDesc.valueOffset = _DetailCloud_ValueOffset;
    sampleDesc.valueScale = _DetailCloud_ValueScale;
    volume.sampleDesc = sampleDesc;
    return volume;
}

MapTexture GetCloudWeatherMap()
{
    MapTexture map;
    map.mapTex = _WeatherMap_Tex;
    map.channelWeights = _WeatherMap_ChannelWeights;
    VoxelDensitySampleDesc sampleDesc;
    sampleDesc.coordsScale = _WeatherMap_CoordsScale;
    sampleDesc.samplerState = sampler_WeatherMap_Tex;
    sampleDesc.valueOffset = _WeatherMap_ValueOffset;
    sampleDesc.valueScale = _WeatherMap_ValueScale;
    map.sampleDesc = sampleDesc;
    return map;
}

OctaveScatterDesc GetOctaveScatterDesc()
{
    OctaveScatterDesc desc;
    desc.extFactor = _Lighting_OctaveMultiScatter_SigmaExtFactor;
    desc.scatterFactor = _Lighting_OctaveMultiScatter_SigmaScatterFactor;
    desc.phaseFactor = _Lighting_OctaveMultiScatter_PhaseFactor;
    desc.count = _Lighting_OctaveMultiScatter_OctaveCount;
    return desc;
}

VolumeLightingDesc GetLightingDesc()
{
    VolumeLightingDesc desc;
    desc.sigmaScatter = _Lighting_SigmaScatter;
    desc.sigmaExt = _Lighting_SigmaExt;
    desc.hgPhaseG1Factor = _Lighting_PhaseG1;
    desc.hgPhaseG2Factor = _Lighting_PhaseG2;
    desc.octaveScatterDesc = GetOctaveScatterDesc();
    return desc;
}

CloudVolumeDesc GetCloudVolumeDesc()
{
    CloudVolumeDesc desc;
    desc.basicDensity = GetBasicCloudDensityVolume();
    desc.detailDensity = GetDetailCloudDensityVolume();
    desc.weatherMap = GetCloudWeatherMap();
    desc.densityScale = _Lighting_DensityScale;
    desc.lightingDesc = GetLightingDesc();
    return desc;
}



AmbientLightDesc GetAmbientLightDesc()
{
    AmbientLightDesc desc;
    desc.color = _Lighting_AmbientLightColor;
    desc.phase = _Lighting_AmbientLightPhase;
    return desc;
}

float4 GetBackgroundColor(float2 screenUV)
{
    return _MainTex.SampleLevel(sampler_MainTex,screenUV,0);
}

float remap(float x, float L0, float H0, float Ln, float Hn){
    return Ln + (x - L0) * (Hn - Ln) / (H0 - L0);
}


#endif
```
