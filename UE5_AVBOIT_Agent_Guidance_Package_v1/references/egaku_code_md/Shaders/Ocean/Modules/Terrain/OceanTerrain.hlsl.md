# Egaku source: `Shaders/Ocean/Modules/Terrain/OceanTerrain.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef OCEAN_TERRAIN_HLSL
#define OCEAN_TERRAIN_HLSL
#include "Ocean.ParamStruct.hlsl"

struct OceanTerrainContext{
    float2 gradientDir;
    float3 slopeDir;
    float gradientScale;
    float slopePredictionLength;
    float toCoastlineDist;
    float waveScale;
    float height;
    float4 debugTerrain;
};

float2 WorldPosToOceanTerrainMapUV(float2 positionWS_Horizontal)
{
    DeclareParamFetch(OceanTerrainDesc,mapDesc);
    return (positionWS_Horizontal - mapDesc.rectPosOriginWS)/ mapDesc.rectShapeWS;
}

OceanTerrainContext DecodeOceanTerrainSampleValue(float4 terrainMapValue,float heightMapValue)
{
    DeclareParamFetch(OceanTerrainDesc,mapDesc);
    float2 grad = 0.5f - terrainMapValue.gb;
   // grad.x = grad.x; // this is necessary
    grad.y = -grad.y;
    float gradLen = length(grad);
    float2 gradDir = grad / gradLen;
    // value.r is the sdf distance to coastline which's in [0,1]
    // = 0.5: coastline with zero altitude
    // < 0.5: area with seawater
    // > 0.5: area with land
   
    //float toCoastlineDist = 2.0f * (0.5f - value.r) * mapDesc.toCoastlineMaxDist;
    float toCoastlineDist = 2.0f * (0.5f - terrainMapValue.r) * mapDesc.toCoastlineMaxDist;
    OceanTerrainContext data = (OceanTerrainContext)0;
    data.gradientDir = gradDir;
    data.toCoastlineDist = toCoastlineDist;
    data.waveScale = terrainMapValue.a;
    data.gradientScale = gradLen;
    data.height = heightMapValue;
    return data;
}

float3 CalcCoastlineTerrainSlopeVector(float2 positionWS_Horizontal,OceanTerrainContext terrainContext,float predictionLength)
{
    DeclareParamFetch(OceanTerrainDesc,mapDesc);
    float2 predictedPosWS_Horizontal = positionWS_Horizontal + terrainContext.gradientDir * predictionLength;
   // float predictedTerrainHeight;// = mapDesc.heightMap.SampleLevel(sampler_PointClamp,predictedPosWS_Horizontal,0.f);
    //predictedTerrainHeight = SAMPLE_TEXTURE2D_LOD(mapDesc.heightMap,sampler_PointClamp,predictedPosWS_Horizontal,0.f);
    float2 uv = WorldPosToOceanTerrainMapUV(predictedPosWS_Horizontal);
    float predictedTerrainHeight = SampleTextureLevel(mapDesc.heightMap,sampler_LinearClamp,uv,0.f).r;
    float3 slopeVector;
    slopeVector.xz = terrainContext.gradientDir;
    slopeVector.y = predictedTerrainHeight - terrainContext.height;
    return normalize(slopeVector);
}

OceanTerrainContext SampleOceanTerrainMap(float2 positionWS_Horizontal)
{
    DeclareParamFetch(OceanTerrainDesc,mapDesc);
    float2 uv = WorldPosToOceanTerrainMapUV(positionWS_Horizontal);
    float4 terrain = SampleTextureLevel(mapDesc.terrainMap,sampler_LinearClamp,uv,0);
    float terrainHeight = SampleTextureLevel(mapDesc.heightMap,sampler_LinearClamp,uv,0);

    // decode texel value
    OceanTerrainContext context =  DecodeOceanTerrainSampleValue(terrain,terrainHeight);
    context.debugTerrain = 0.5 - terrain.r;
    float3 slopeDir = CalcCoastlineTerrainSlopeVector(positionWS_Horizontal,context,mapDesc.slopePredictionLength);
    context.slopeDir = slopeDir;
    context.slopePredictionLength = mapDesc.slopePredictionLength;
    return context;
}

OceanTerrainContext MockNeighborTerrainContext(OceanTerrainContext context,float2 positionOffset_Horizontal)
{
    OceanTerrainContext neightborContext = context;
    neightborContext.toCoastlineDist = context.toCoastlineDist - dot(positionOffset_Horizontal,context.gradientDir);
    return neightborContext;
}

float CalcWaterColorBlendFactor(OceanTerrainContext terrainContext)
{
    float _CoastlineScattringHeight = 0.0588;
    float _CoastlineScattringDistance = 0.019;
    float scatteringHeight = saturate(-_CoastlineScattringHeight * terrainContext.height);
    float scatteringDistance = saturate(terrainContext.toCoastlineDist * _CoastlineScattringDistance);
    float blendFactor = 1 - max(scatteringHeight, scatteringDistance);
   // blendFactor = terrainContext.height;
    return blendFactor;
}


#endif
```
