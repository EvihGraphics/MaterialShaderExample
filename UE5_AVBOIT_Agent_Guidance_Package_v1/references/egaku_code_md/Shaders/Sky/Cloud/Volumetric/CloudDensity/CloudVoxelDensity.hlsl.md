# Egaku source: `Shaders/Sky/Cloud/Volumetric/CloudDensity/CloudVoxelDensity.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef VOLUMETRIC_CLOUD_VOXEL_DENSITY_HLSL
#define VOLUMETRIC_CLOUD_VOXEL_DENSITY_HLSL
#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Texture.hlsl"
#include "../VolumetricCloud.ParamStruct.hlsl"
#include "Assets/Shaders/Sky/Basic/Utils.hlsl"
#include "DensityNoise3D.hlsl"


float SampleCloudVoxelBasicDensity_Near(float3 coords)
{
    DeclareParamFetch(CloudBasicDensityDesc,desc);
    DensityNoiseSampleDesc sampleDesc;
    sampleDesc.tex = desc.noiseTex;
    sampleDesc.channelWeights = desc.channelWeights;
    sampleDesc.noiseOffset = desc.noiseOffset;
    sampleDesc.noiseScale = desc.noiseScale;
    sampleDesc.volumeScale = desc.volumeScale;
    return SampleDensityNoise3D(sampleDesc,coords);
}

float SampleCloudVoxelDetailDensity_Near(float3 coords)
{
    DeclareParamFetch(CloudDetailDensityDesc,desc);
    DensityNoiseSampleDesc sampleDesc;
    sampleDesc.tex = desc.noiseTex;
    sampleDesc.channelWeights = desc.channelWeights;
    sampleDesc.noiseOffset = desc.noiseOffset;
    sampleDesc.noiseScale = desc.noiseScale;
    sampleDesc.volumeScale = desc.volumeScale;
    return SampleDensityNoise3D(sampleDesc,coords);
}

float SampleCloudVoxelBasicDensity_Distant(float3 coords)
{
    DeclareParamFetch(DistantCloudBasicDensityDesc,desc);
    DensityNoiseSampleDesc sampleDesc;
    sampleDesc.tex = desc.noiseTex;
    sampleDesc.channelWeights = desc.channelWeights;
    sampleDesc.noiseOffset = desc.noiseOffset;
    sampleDesc.noiseScale = desc.noiseScale;
    sampleDesc.volumeScale = desc.volumeScale;
    return SampleDensityNoise3D(sampleDesc,coords);
}


float SampleCloudVoxelDetailDensity_Distant(float3 coords)
{
    DeclareParamFetch(DistantCloudDetailDensityDesc,desc);
    DensityNoiseSampleDesc sampleDesc;
    sampleDesc.tex = desc.noiseTex;
    sampleDesc.channelWeights = desc.channelWeights;
    sampleDesc.noiseOffset = desc.noiseOffset;
    sampleDesc.noiseScale = desc.noiseScale;
    sampleDesc.volumeScale = desc.volumeScale;
    return SampleDensityNoise3D(sampleDesc,coords);
}

float3 SampleWeatherMap(float2 uv)
{
    DeclareParamFetch(WeatherMapDesc,desc);
    float2 coords = uv;
    float3 sampledVal = SampleTexture(desc.weatherMap,sampler_LinearRepeat,coords);
    sampledVal = sampledVal * desc.scale + desc.offset;
    return saturate(sampledVal);
}

float3 SampleWeatherMap(float3 positionWS)
{
    DeclareParamFetch(WeatherMapDesc,desc);
    float2 coords = GetHorizontal(positionWS) * rcp(desc.mapScale);
    float3 sampledVal = SampleTextureLevel(desc.weatherMap,sampler_LinearRepeat,coords,0);
    sampledVal = sampledVal * desc.scale + desc.offset;
    return saturate(sampledVal);
}





#endif
```
