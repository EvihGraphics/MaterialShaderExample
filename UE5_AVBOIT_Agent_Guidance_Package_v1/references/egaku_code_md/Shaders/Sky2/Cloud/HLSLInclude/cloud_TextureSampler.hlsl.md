# Egaku source: `Shaders/Sky2/Cloud/HLSLInclude/cloud_TextureSampler.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef CLOUD_TEXTURE_SAMPLER_HLSL
#define CLOUD_TEXTURE_SAMPLER_HLSL
#include "cloud_DeclareStructs.hlsl"
#include "cloud_Common.hlsl"

float4 SampleTexture(Texture2D tex,float2 coords,VoxelDensitySampleDesc sampleDesc)
{
    float4 sampleValue = tex.SampleLevel(sampleDesc.samplerState,coords/sampleDesc.coordsScale,0);
    return sampleValue * sampleDesc.valueScale + sampleDesc.valueOffset;
}

float4 SampleTexture(Texture3D tex,float3 coords,VoxelDensitySampleDesc sampleDesc)
{
    float4 sampleValue = tex.SampleLevel(sampleDesc.samplerState,coords/sampleDesc.coordsScale,0);
    return sampleValue * sampleDesc.valueScale + sampleDesc.valueOffset;
}


float SampleDensityVolume(DensityVolume vol,float3 coords)
{
    Texture3D densityVolTex = vol.volumeTex;
    VoxelDensitySampleDesc sampleDesc = vol.sampleDesc;
    float4 noise = SampleTexture(densityVolTex,coords,sampleDesc);
    return dot(noise,vol.channelWeights);
    float fbm = dot( noise.gba,vol.channelWeights.gba);
    float density = remap( noise.r,fbm-1,1,0,1);
    return density;
}

float SampleErosionVolume(DensityVolume vol,float3 coords)
{
    Texture3D densityVolTex = vol.volumeTex;
    VoxelDensitySampleDesc sampleDesc = vol.sampleDesc;
    float4 noise = SampleTexture(densityVolTex,coords,sampleDesc);
    return dot(noise,vol.channelWeights);
}

float SampleWeatherMap(MapTexture weatherMap,float2 coords)
{
    float3 sampledVal = SampleTexture(weatherMap.mapTex,coords,weatherMap.sampleDesc);
    return dot(sampledVal,weatherMap.channelWeights);
}

float SampleCloudVolume(CloudVolumeDesc cloud,BoundingBox box,float3 coords)
{
    float basicDensity = SampleDensityVolume(cloud.basicDensity,coords);
    float erosionDensity = SampleErosionVolume(cloud.detailDensity,coords);
    MapTexture weatherMap = cloud.weatherMap;
    float weather = SampleWeatherMap(weatherMap,coords.xz);
    float density = saturate((basicDensity - erosionDensity)*weather*cloud.densityScale);
 
    float factor = 0.5;
    float cloudLayerHeight = box.boundsMax.y - box.boundsMin.y;
    float sampleHeight = coords.y - box.boundsMin.y;
    float shape = density * smoothstep(0, cloudLayerHeight * factor, sampleHeight);
    shape = shape * smoothstep(0, cloudLayerHeight * factor, cloudLayerHeight - sampleHeight);
    
    float result = pow(shape, 1.5);
    result = lerp(result,saturate(result),0.5);
    return result;
}

#endif
```
