# Egaku source: `Shaders/Ocean/Modules/Terrain/OnshoreWater.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
#ifndef ONSHORE_WATER_HLSL
#define ONSHORE_WATER_HLSL
#include "OceanTerrain.hlsl"

struct OnshoreSurfaceData
{
    float2 velocity;
    float foam;
};

float CalcOnshoreFoamIntensity(OceanTerrainContext terrainContext)
{
    return min(1.0, (terrainContext.toCoastlineDist * (-0.5f) + 0.6)) * 0.6f;
}

float2 CalcOnshoreVelocity(OceanTerrainContext terrainContext)
{
    float scale = 0.4f * (saturate(terrainContext.toCoastlineDist * -0.5f) - 0.5f);
    float2 dir =  terrainContext.gradientDir;
    return scale*dir;
}

OnshoreSurfaceData CalcOnshoreWaterSurfaceData(OceanTerrainContext terrainContext)
{
    OnshoreSurfaceData data;
    data.velocity = CalcOnshoreVelocity(terrainContext);
    data.foam = CalcOnshoreFoamIntensity(terrainContext);
    return data;
}

#endif
```
