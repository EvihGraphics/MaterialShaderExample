# Egaku source: `Shaders/Ocean/Modules/Coastline/CoastlineWater.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef COASTLINE_WATER_HLSL
#define COASTLINE_WATER_HLSL
#include "CoastlineWaveProfile.hlsl"
#include "../Utils.hlsl"



struct CoastlineSurfaceData
{
    float3 normal;
    float3 displacementVec;
    float2 velocity;
    float foam;
    float mask;
    float divergence;
    float4 debug;
};



float3 CalcCoastlineWaterDisplacement(CoastlineWaveProfileData waveProfileData,OceanTerrainContext terrainContext)
{
    float factor = saturate((terrainContext.toCoastlineDist - 5)/terrainContext.slopePredictionLength);
    float3 forwardVec = lerp(terrainContext.slopeDir, Make3DFromHorizontal(terrainContext.gradientDir,0), factor);
    float3 upCompensateVec = lerp(0.8*WorldVerticalDir() * terrainContext.height,0,factor);
    float upFactor = saturate((terrainContext.toCoastlineDist - 0.1)/0.1);
    return waveProfileData.forwardDisplacement * forwardVec + upCompensateVec + WorldVerticalDir()* waveProfileData.upwardDisplacement * factor; // * upFactor;
}


float3 CalcCoastlineWaterNormal(float2 positionWS_Horizontal,float3 displacement,OceanTerrainContext terrainContext)
{
    float neighborVtxMockLength = FetchParam(CoastlineSurfaceDesc).neighborVtxMockLength;
    float2 neighborPosOffset1 = float2(1,0)*neighborVtxMockLength;
    float2 neighborPosOffset2 = float2(0,1)*neighborVtxMockLength;
    OceanTerrainContext neighborContext1 = MockNeighborTerrainContext(terrainContext,neighborPosOffset1);
    OceanTerrainContext neighborContext2 = MockNeighborTerrainContext(terrainContext,neighborPosOffset2);
    CoastlineWaveProfileData neighborProfile1 = SampleCoastlineWaveProfileMap(positionWS_Horizontal + neighborPosOffset1,neighborContext1);
    CoastlineWaveProfileData neighborProfile2 = SampleCoastlineWaveProfileMap(positionWS_Horizontal + neighborPosOffset2,neighborContext2);
    float3 neighborDisplace1 = CalcCoastlineWaterDisplacement(neighborProfile1,neighborContext1);
    float3 neighborDisplace2 = CalcCoastlineWaterDisplacement(neighborProfile2,neighborContext2);
    float3 p0 = HorizontalAdd(displacement,positionWS_Horizontal);
    float3 p1 = HorizontalAdd(neighborDisplace1,neighborPosOffset1);
    float3 p2 = HorizontalAdd(neighborDisplace2,neighborPosOffset2);
    float3 normal = TriangleNormal(p0,p1,p2);
    if (GetVertical(normal) < 0)
    {
        normal = -normal;//Make3DFromHorizontal(GetHorizontal(normal),-GetVertical(normal));
    }
    return normal;
}

float CalcCoastlineFoam(float foamIntensity,OceanTerrainContext terrainContext)
{
    DeclareParamFetch(CoastlineSurfaceDesc,surfaceDesc);
    float factor =  1.0f - saturate(terrainContext.toCoastlineDist / surfaceDesc.foamDistance);
    float foam = surfaceDesc.foamScale * factor + max(foamIntensity * 1.3f - 0.5f, -0.1f);
    return saturate(foam);
}

float CalcCoastlineMask()
{
    return 0;
}

float2 CalcCoastlineVelocity(float3 displacementVec,OceanTerrainContext terrainContext)
{
    float waveVelocityFactor = (saturate(terrainContext.toCoastlineDist * 0.4f) - 0.08f) * FetchParam(CoastlineSurfaceDesc).velocityScale;
    float2 waveVelocity = GetHorizontal(displacementVec) * waveVelocityFactor;
    return waveVelocity;
}

float CalcCoastlineDivergence(CoastlineWaveProfileData waveProfileData,OceanTerrainContext terrainContext)
{
    float divergence = lerp( 0.1f, saturate(waveProfileData.upwardDisplacement * 2.5f + 0.35f), saturate(terrainContext.toCoastlineDist * 0.1f) );
    divergence *= saturate(1.f - (waveProfileData.foamIntensity * 3.0f));
    return divergence;
}

CoastlineSurfaceData CalcCoastlineWaterSurfaceData(float2 positionWS_Horizon,OceanTerrainContext terrainContext)
{
    CoastlineSurfaceData surfaceData;
    CoastlineWaveProfileData waveProfileData = SampleCoastlineWaveProfileMap(positionWS_Horizon,terrainContext);
    float3 displacementVec = CalcCoastlineWaterDisplacement(waveProfileData,terrainContext);
    
    surfaceData.normal = CalcCoastlineWaterNormal(positionWS_Horizon,displacementVec,terrainContext);
    surfaceData.foam = CalcCoastlineFoam(waveProfileData.foamIntensity,terrainContext);
    surfaceData.displacementVec = displacementVec; 
    surfaceData.mask = CalcCoastlineMask();
    surfaceData.velocity = CalcCoastlineVelocity(displacementVec,terrainContext);
    surfaceData.divergence = CalcCoastlineDivergence(waveProfileData,terrainContext);
    surfaceData.debug = waveProfileData.debug;
    return surfaceData;
}

#endif
```
