# Egaku source: `Shaders/Ocean/Modules/Coastline/CoastlineWaveProfile.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef COASTLINE_WAVE_PROFILE_HLSL
#define COASTLINE_WAVE_PROFILE_HLSL
#include "../Terrain/OceanTerrain.hlsl"
#include "../Noise/SmoothNoise.hlsl"

struct CoastlineWaveProfileData
{
    float forwardDisplacement;
    float upwardDisplacement;
    float foamIntensity;
    float4 debug;
};

struct WaveProfileNoise
{
    float timeNoise;
    float phaseNoise;
};

WaveProfileNoise CalcBasicNoise(float2 positionWS_Horizontal,OceanTerrainContext terrainContext)
{
    WaveProfileNoise noise;
    float3 factor  = (positionWS_Horizontal + (terrainContext.toCoastlineDist * terrainContext.gradientDir)).y * float3(0.121, 0.242, 0.752);
    float sinFactor = sin(factor.y * 2 * PI);
    float cosFactor = cos(factor.z * 2 * PI);
    float timeNoise = factor.x + sinFactor * (-0.135) + cosFactor * (-0.038);
    float phaseNoise = sinFactor * (-0.25) + cosFactor * (0.15);
    noise.timeNoise = timeNoise;
    noise.phaseNoise = phaseNoise;
    return noise;
}

WaveProfileNoise CalcDetailNoise(float2 positionWS_Horizontal,OceanTerrainContext terrainContext)
{
    DeclareParamFetch(WaveProfileNoiseDesc,noiseDesc);
    WaveProfileNoise noise;
    float  noise1 = SampleSmoothNoiseMap( positionWS_Horizontal * 0.5) - 0.5;
    float  noise2 = SampleSmoothNoiseMap( positionWS_Horizontal * 1.5) - 1.0;
    noise.timeNoise = noiseDesc.withDetails * (noiseDesc.noiseTime * noise1 * saturate(terrainContext.toCoastlineDist * 0.35 - 0.05));
    noise.phaseNoise = noiseDesc.withDetails * ( noiseDesc.noiseScale * noise2);
    return noise;
}

WaveProfileNoise GenerateWaveProfileNoise(float2 positionWS_Horizontal,OceanTerrainContext terrainContext,float waveLength)
{
    positionWS_Horizontal /= waveLength;
    terrainContext.toCoastlineDist /= waveLength;
    WaveProfileNoise noise = CalcBasicNoise(positionWS_Horizontal, terrainContext);
    WaveProfileNoise detailNoise = CalcDetailNoise(positionWS_Horizontal, terrainContext);
    
 //   noise.timeNoise += detailNoise.timeNoise;
  //  noise.phaseNoise += detailNoise.phaseNoise;
    
    return noise;
}

float2 TerrainDataToCoastlineWaveProfileMapUV(float2 positionWS_Horizontal,OceanTerrainContext terrainContext)
{
    // toCoastlineDist is 0 if we are at the coastline, otherwise it's the distance to the coastline
    DeclareParamFetch(CoastlineWaveProfileDesc,mapDesc);
    WaveProfileNoise noise = GenerateWaveProfileNoise(positionWS_Horizontal, terrainContext,mapDesc.waveLength);
    float waveNum = 2.37f - terrainContext.toCoastlineDist/mapDesc.waveLength;
    float t = 0.5f*GetTimeContext().timeInSec + noise.timeNoise;
    // wavePhase is the phase of the wave within [0,1)
    float wavePhase = frac(waveNum - t);
    float waveIndex = -(waveNum - wavePhase /*+ noise.phaseNoise*/) / mapDesc.waveCount;

    return float2(wavePhase,waveIndex);    
}

CoastlineWaveProfileData DecodeCoastlineWaveProfileSampleValue(float4 value)
{
    DeclareParamFetch(CoastlineWaveProfileDesc,profileDesc);
    CoastlineWaveProfileData data = (CoastlineWaveProfileData)0;
    float2 forwardUpward = (value.rg + profileDesc.waveDisplaceOffset) * profileDesc.waveDisplaceScale * profileDesc.waveLength;
    data.foamIntensity = value.b;
    data.forwardDisplacement = forwardUpward.x;
    data.upwardDisplacement = forwardUpward.y;
    return data;
}

CoastlineWaveProfileData SampleCoastlineWaveProfileMap(float2 positionWS_Horizontal,OceanTerrainContext terrainContext)
{
    DeclareParamFetch(CoastlineWaveProfileDesc,profileDesc);
    float2 uv = TerrainDataToCoastlineWaveProfileMapUV(positionWS_Horizontal, terrainContext);
    float4 value = SampleTextureLevel(profileDesc.profileMap,sampler_LinearClamp,uv,0);
    CoastlineWaveProfileData data = DecodeCoastlineWaveProfileSampleValue(value);
    data.foamIntensity *=  terrainContext.waveScale;
    data.forwardDisplacement *= terrainContext.waveScale;
    data.upwardDisplacement *= terrainContext.waveScale;
    data.debug = float4(value);
    return data;
}

#endif
```
