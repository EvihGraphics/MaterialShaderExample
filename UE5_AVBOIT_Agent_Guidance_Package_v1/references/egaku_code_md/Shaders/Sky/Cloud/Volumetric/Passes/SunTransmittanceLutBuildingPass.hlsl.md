# Egaku source: `Shaders/Sky/Cloud/Volumetric/Passes/SunTransmittanceLutBuildingPass.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef SUN_TRANSMITTANCE_LUT_BUILDING_PASS_HLSL
#define SUN_TRANSMITTANCE_LUT_BUILDING_PASS_HLSL
#include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Lighting.hlsl"
#include "Assets/Shaders/Sky/Basic/VolumeBase/VolumeRayMarching.hlsl"
#include "../CloudVolume/CloudVolume.hlsl"
#include "Assets/Shaders/Sky/Basic/Utils.hlsl"
#define VOLUMETRIC_CLOUD_USE_SUN_TRANSMITTANCE_LUT_UAV
#include "../Buffers/SunTransmittanceLut.hlsl"
#include "Volume/OctahedronVolume.hlsl"
//#pragma enable_d3d11_debug_symbols


struct Attributes {
    float4 vertex : POSITION;
    float2 uv : TEXCOORD0;
};

struct Varyings {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD1;
};

Varyings Vert(Attributes input) {
    Varyings output;
    output.position = TransformObjectToHClip(input.vertex);
    output.uv = input.uv;
    return output;
}

static BoundsHitResult currentDirectionBoundsHit;
static uint segmentCount;

float2 OctahedronVolume_DirectionDimensions()
{
    return SunTransmittanceLut_GetDimensions().xy;
}

float OctahedronVolume_DirectionSliceCount()
{
    return SunTransmittanceLut_GetDimensions().z;
}

float OctahedronVolume_SliceThickness()
{
    BoundingSphere sphere = GetCloudVolumeBoundingSphere();
    return (sphere.radiusMax - sphere.radiusMin) / segmentCount;
}

float OctahedronVolume_LowerSliceBound()
{
    return currentDirectionBoundsHit.distToBounds;
}


float CalcSunDirectionTransmittance(SamplePoint samplePoint,BoundingSphere sphere)
{
    Ray sunRay;
    sunRay.direction = GetMainLight().direction;
    sunRay.origin = samplePoint.position;
    sunRay.hitPoint = sunRay.origin + sunRay.direction * 1000000;
    BoundsHitResult boundsHit = CalcBoundsHit(sphere,sunRay);
    RayMarchingContext sunDirMarchingContext = GetRayMarchingContext(sunRay,boundsHit,10);
    float sunTransmittance = VolumeRayMarching_CalcTransmittanceOnly(sunDirMarchingContext).transmittance;
    return sunTransmittance;
}

float CalcSunDirectionDensity(SamplePoint samplePoint,BoundingSphere sphere)
{
    Ray sunRay;
    sunRay.direction = GetMainLight().direction;
    sunRay.origin = samplePoint.position;
    sunRay.hitPoint = sunRay.origin + sunRay.direction * 1000000;
    BoundsHitResult boundsHit = CalcBoundsHit(sphere,sunRay);
    RayMarchingContext sunDirMarchingContext = GetRayMarchingContext(sunRay,boundsHit,20,0.5);

    float sunDensity = -log(VolumeRayMarching_CalcTransmittanceOnly(sunDirMarchingContext).transmittance);
    return  sunDensity;
}


SamplePoint VolumeRayMarching_CalcNextSamplePoint(SamplePoint samplePoint,VoxelData voxelData,RayMarchingContext context,RayMarchingResult currentResult)
{
    samplePoint.position += samplePoint.direction * samplePoint.stepSize;
    return samplePoint;
}



void RayMarching(uint3 voxelPosBegin,Ray ray,BoundingSphere sphere)
{
    BoundsHitResult boundsHit = CalcBoundsHit(sphere,ray);
    currentDirectionBoundsHit = boundsHit;
    segmentCount = 8;
    uint viewSampleCount = GetSphereVolumeSampleConfig(boundsHit,sphere,segmentCount,segmentCount*3).directionSampleCount;
    
    uint samplePerSeg = 1;
    uint totalSampleCount = viewSampleCount;
    RayMarchingContext rayMarchingContext = GetRayMarchingContext(ray,boundsHit,totalSampleCount);
    //rayMarchingContext.entryPosition += rayMarchingContext.direction * rayMarchingContext.stepSize * 0.5;

    SamplePoint samplePoint;
    samplePoint.position = rayMarchingContext.entryPosition;
    samplePoint.stepSize = rayMarchingContext.stepSize;
    samplePoint.direction = rayMarchingContext.direction;
    RayMarchingResult result;
    result.density = 0;
    result.distance = 0;
    result.luminance = 0;
    result.transmittance = 1;
    float weight = 1;rcp(8);
    
    [loop]
    for (int i = 0; i < viewSampleCount; i++)
    {
        uint3 voxelPos2 = voxelPosBegin + uint3(0,0,i);
        /*VoxelMeta entry;
        entry.flag = 0;
        entry.density = 0;
        entry.viewRadianceTransfer = 0;
        entry.sunTransmittance = 0;*/
        
        for (int j = 0; j < samplePerSeg; j++)
        {
            VoxelData voxelData = Volume_SampleVoxelData(samplePoint);
            float3 sunTransmittance = CalcSunDirectionTransmittance(samplePoint,sphere);
            result.luminance += sunTransmittance * samplePoint.stepSize;
            result.density += voxelData.density * samplePoint.stepSize;
            result.transmittance *= BeerTransmittance(voxelData.sigmaT * samplePoint.stepSize);
            result.distance += samplePoint.stepSize;
            
            float3 voxelPos = OctahedronVolume_GetVoxelCoords(samplePoint.position);
            /*if (all(uint3(voxelPos) == voxelPos2))
            {
                entry.flag += 0.1;
            }else
            {
                entry.flag += 0;
            }
            entry.density += weight*voxelData.density;
            entry.viewRadianceTransfer += weight*result.transmittance;
            entry.sunTransmittance += weight*sunTransmittance.r;*/
            SunTransmittanceLut_Write(voxelPos,sunTransmittance.r);
            samplePoint.position += samplePoint.direction * samplePoint.stepSize;
        }
        
    }
  
}


float4 Frag(Varyings input) : SV_Target {
    float3 viewHit = GetWorldPosition(input.uv);
    BoundingSphere sphere = GetCloudVolumeBoundingSphere();
    Ray ray;
    ray.direction = OctahedronVolume_GetDirectionFromSampleCoords(input.uv);
    ray.origin = OctahedronVolume_CenterPoint();
    ray.hitPoint = viewHit;
    uint3 voxelPosBegin = uint3(input.position.xy,0);
    RayMarching(voxelPosBegin,ray, sphere);

    float2 duv = rcp(OctahedronVolume_DirectionDimensions());
    float2 uv1 = input.uv + float2(0.25,0.25)*duv;
    float2 uv2 = input.uv + float2(-0.25,0.25)*duv;
    float2 uv3 = input.uv + float2(0.25,-0.25)*duv;
    float2 uv4 = input.uv + float2(-0.25,-0.25)*duv;
    
    /*ray.direction = OctahedronVolume_GetDirectionFromSampleCoords(uv1);
    RayMarching(voxelPosBegin,ray, sphere);
    ray.direction = OctahedronVolume_GetDirectionFromSampleCoords(uv2);
    RayMarching(voxelPosBegin,ray, sphere);
    ray.direction = OctahedronVolume_GetDirectionFromSampleCoords(uv3);
    RayMarching(voxelPosBegin,ray, sphere);
    ray.direction = OctahedronVolume_GetDirectionFromSampleCoords(uv4);
    RayMarching(voxelPosBegin,ray, sphere);*/
    
    
    
    return 1;
}

#endif
```
