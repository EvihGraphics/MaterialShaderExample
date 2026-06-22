# Egaku source: `Shaders/Ocean/Modules/OceanSurface.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
#ifndef OCEAN_SURFACE_HLSL
#define OCEAN_SURFACE_HLSL
#include "Coastline/CoastlineWater.hlsl"
#include "Distant/DistantWater.hlsl"
#include "Terrain/OnshoreWater.hlsl"

struct OceanSurfaceData
{
    float3 normal;
    float3 displacementVec;
    float2 velocity;
    float foam;
    float mask;
    float divergence;
    float4 debug;
};

OceanSurfaceData BlendWaterSurfaceData(CoastlineSurfaceData coastlineData, DistantSurfaceData distantData,OnshoreSurfaceData onshoreData,float alpha)
{
    OceanSurfaceData data = (OceanSurfaceData)0;
    data.normal = normalize(lerp(coastlineData.normal,clamp(-1,1,distantData.normal), alpha));

    data.displacementVec = lerp(coastlineData.displacementVec,distantData.displacementVec, pow(alpha,10));
    data.velocity = lerp(coastlineData.velocity + onshoreData.velocity,clamp(-1,1,distantData.velocity), alpha);
    data.foam = lerp(max(coastlineData.foam,onshoreData.foam),distantData.foam, alpha);
    data.mask = lerp(coastlineData.mask,distantData.mask, alpha);
    data.divergence = lerp(coastlineData.divergence,distantData.divergence, alpha);
    return data;
}

float CalcBlendFactor(OceanTerrainContext terrainContext)
{
    // magic operation
    float basicFactor = saturate(terrainContext.gradientScale * -4.54545f + 2.159091f);
    DeclareParamFetch(OceanTerrainDesc,mapDesc);
    float coastlinePercentage = saturate(terrainContext.toCoastlineDist/mapDesc.coastlineWaveRangeWidth);
    float blendFactor = min(1.0f,coastlinePercentage +  basicFactor);
    return blendFactor;
}


OceanSurfaceData CalcOceanSurfaceData(float2 positionWS_Horizontal,OceanTerrainContext terrainContext)
{
    float blendFactor = CalcBlendFactor(terrainContext);
    CoastlineSurfaceData coastlineData = CalcCoastlineWaterSurfaceData(positionWS_Horizontal,terrainContext);
    DistantSurfaceData distantData = CalcDistantWaterSurfaceData(positionWS_Horizontal,terrainContext);
    OnshoreSurfaceData onshoreData = CalcOnshoreWaterSurfaceData(terrainContext);
    //blendAlpha = 0;
    //distantData = (DistantSurfaceData)0;
    OceanSurfaceData data =  BlendWaterSurfaceData(coastlineData,distantData, onshoreData,blendFactor);
    data.debug = float4(distantData.normal,1);
    return data;
}



#endif
```
