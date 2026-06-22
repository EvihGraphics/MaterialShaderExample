# Egaku source: `Shaders/Sky/Cloud/Volumetric/Passes/FourierShadowMapBuildingPass.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef FOURIER_SHADOW_MAP_BUILDING_PASS_HLSL
#define FOURIER_SHADOW_MAP_BUILDING_PASS_HLSL

#include "Assets/Shaders/Sky/Basic/VolumeBase/VolumeRayMarching.hlsl"
#include "../Shading/RayMarching/CloudVolumeRayMarching.hlsl"
#include "../Shading/VolumeLighting/CloudVolumeLighting.CommonScattering.hlsl"
#include "Assets/Shaders/Sky/Basic/RaySpace/OctahedronSpace.hlsl"
#include "../CloudVolume/CloudVolume.hlsl"
#define FOM_FOURIER_COEFFS_HARMONICS_COUNT 7
#include "../Buffers/FourierShadowMap.hlsl"


#pragma enable_d3d11_debug_symbols


struct Attributes {
    float4 vertex : POSITION;
    float2 uv : TEXCOORD0;
};

struct Varyings {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

struct PixelOutput
{
    float4 output0 : SV_Target0;
    float4 output1 : SV_Target1;
    float4 output2 : SV_Target2;
    float4 output3 : SV_Target3;
};

Varyings Vert(Attributes input) {
    Varyings output;
    output.position = input.vertex;
    output.uv = input.uv;
    return output;
}



FourierCoeffs RayMarchingCalcTransmittanceFourier(RayMarchingContext rayMarchingContext)
{
    SamplePoint samplePoint;
    samplePoint.position = rayMarchingContext.entryPosition;
    samplePoint.stepSize = rayMarchingContext.stepSize;
    samplePoint.direction = rayMarchingContext.direction;
    float dist = 0;
    FourierCoeffs sumFC = (FourierCoeffs)0;
    [loop]
    for (uint i = 0; i < rayMarchingContext.iterationCount ; i++)
    {
        VoxelData voxelData = Volume_SampleVoxelData(samplePoint);
        if (voxelData.density > 0.0001)
        {
            float t = dist * rcp(rayMarchingContext.maxDistance);
            FourierCoeffs fc = FOM_CreateFourierCoeffs(voxelData.sigmaT * samplePoint.stepSize,t);
            sumFC = FOM_CombineFourierCoeffs(sumFC,fc);
        }
        samplePoint.position += samplePoint.direction * samplePoint.stepSize;
        dist += samplePoint.stepSize;
    }
    return sumFC;
}



PixelOutput Frag(Varyings input)  {
    float3 viewHit = GetWorldPosition(input.uv);
    BoundingSphere sphere = GetCloudVolumeBoundingSphere();
    float centralAngleRange = acos(GetEarthRadius() * rcp(sphere.radiusMax));
    float2 dirDiskPolar = OctahedronConcentric_SquareToConcentricDiskPolar(input.uv);
    float dirZenith = OctahedronConcentric_GetDirectionZenithAngleFromConcentricDiscPolar(dirDiskPolar);
    float dirZenithMapped = Remap(dirZenith,0,HALF_PI,0,centralAngleRange);
    float3 dirToVolume = float3(cos(dirDiskPolar.y),cos(dirZenithMapped), sin(dirDiskPolar.y));
    
    Ray rayToVolume;
    rayToVolume.direction = dirToVolume; //OctahedronSpace_GetHemiDirectionFromSampleCoords(input.uv);
    rayToVolume.origin = sphere.center; //0
    rayToVolume.hitPoint = viewHit;
    Sphere lower = {sphere.center,sphere.radiusMin};
    Sphere upper = {sphere.center,sphere.radiusMax};
    SphereIntersectionResult result = CalcSphereIntersection(upper, rayToVolume);
    
    Ray rayFromSun;
    rayFromSun.direction = -GetMainLight().direction;
    rayFromSun.origin = rayToVolume.origin + rayToVolume.direction * result.t1;
    rayFromSun.hitPoint = rayFromSun.direction*1000000;
    SphereIntersectionResult result2 = CalcSphereIntersection(lower, rayFromSun);
    BoundsHitResult boundsHit;
    boundsHit.distInsideBounds = result2.t0;
    boundsHit.distToBounds = 0;
    
    uint iterationNormal = FetchParam(VolumeRayMarchingDesc).mainLightIteration;
    uint maxSampleCount = FetchParam(VolumeRayMarchingDesc).mainLightMaxSampleCount;
    SphereVolumeSampleConfig sampleConfig = GetSphereVolumeSampleConfig(boundsHit,sphere,iterationNormal,maxSampleCount);
    uint viewSampleCount = sampleConfig.directionSampleCount;
    RayMarchingContext rayMarchingContext = GetRayMarchingContext(rayFromSun,boundsHit,viewSampleCount);
    
    FourierCoeffs coeffs = RayMarchingCalcTransmittanceFourier(rayMarchingContext);

    FourierCoeffsLutEntry entry0 = FSM_PackToFourierCoeffsLutEntry(coeffs, 0);
    FourierCoeffsLutEntry entry1 = FSM_PackToFourierCoeffsLutEntry(coeffs, 2);
    FourierCoeffsLutEntry entry2 = FSM_PackToFourierCoeffsLutEntry(coeffs, 4);
    FourierCoeffsLutEntry entry3 = FSM_PackToFourierCoeffsLutEntry(coeffs, 6);
    
    PixelOutput output;
    output.output0 = FSM_GetPackedValue(entry0);
    output.output1 = FSM_GetPackedValue(entry1);
    output.output2 = FSM_GetPackedValue(entry2);
    output.output3 = FSM_GetPackedValue(entry3);
    return output;
}



#endif
```
