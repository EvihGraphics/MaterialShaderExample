# Egaku source: `Shaders/Sky/Cloud/Volumetric/Passes/PreTransmittedOctahedronRayMarchingPass.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef PRETRANSMITTED_OCTAHEDRON_RAY_MARCHING_PASS_HLSL
#define PRETRANSMITTED_OCTAHEDRON_RAY_MARCHING_PASS_HLSL


#include "Assets/Shaders/Sky/Basic/VolumeBase/PerspectiveFrustum.hlsl"
#include "../Utils/TilePixelSelector.hlsl"
#include "../Shading/RayMarching/CloudVolumeRayMarching.hlsl"
#include "../Shading/VolumeLighting/CloudVolumeLighting.CommonScattering.hlsl"
#define VOLUMETRIC_CLOUD_USE_DIRECTION_OCCUPANCY_LUT_SRV
#include "../Buffers/DirectionOccupancyLut.hlsl"
#define VOLUMETRIC_CLOUD_USE_VOXEL_META_LUT_SRV
#include "../Buffers/VoxelMetaLut.hlsl"
#include "../Volume/OctahedronVolume.hlsl"

#pragma enable_d3d11_debug_symbols

float4x4 _ViewProjectionMatrix;
uint _VolumetricCloud_SegmentTestNormalIteration;

float2 OctahedronVolume_DirectionDimensions()
{
    return VoxelMetaLut_GetDimensions().xy;
}

float OctahedronVolume_DirectionSliceCount()
{
    return VoxelMetaLut_GetDimensions().z;
}



struct Attributes {
    float4 vertex : POSITION;
    float2 uv : TEXCOORD0;
};

struct Varyings {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 mainLightTransmittance0 : TEXCOORD1;
    float4 mainLightTransmittance1 : TEXCOORD2;
    float4 mainLightTransmittance2 : TEXCOORD3;
};

struct DirectionMainLightTransmittance
{
    float transmittance[0];
};


RayMarchingResult RayMarchingSampleMainLightTransmittance(RayMarchingContext rayMarchingContext)
{
    int iterationCount = rayMarchingContext.iterationCount;
    SamplePoint samplePoint;
    samplePoint.position = rayMarchingContext.entryPosition;
    samplePoint.stepSize = rayMarchingContext.stepSize;
    samplePoint.direction = rayMarchingContext.direction;
    RayMarchingResult result;
    result.density = 0;
    result.distance = 0;
    result.luminance = 0;
    result.transmittance = 1;
    
    [loop]
    for (int i = 0; i < iterationCount ; i++)
    {
        if (result.distance > rayMarchingContext.maxDistance)
        {
            break;
        }
   
        VoxelData voxelData = Volume_SampleVoxelData(samplePoint);
        float3 luminance = VolumeRayMarching_CalcVoxelLuminance(voxelData,samplePoint,rayMarchingContext,result);
        result = VolumeRayMarching_UpdateResult(result,voxelData,samplePoint,rayMarchingContext,luminance);
        samplePoint = VolumeRayMarching_CalcNextSamplePoint(samplePoint, voxelData,rayMarchingContext, result);
    }
    return result;
}


Varyings Vert(Attributes input) {
    Varyings output;
    output.position = TransformObjectToHClip(input.vertex);
    output.uv = input.uv;
    
    return output;
}

float4 ShadeVolumetricCloudHemiOctahedron(float3 cameraPosWS,float3 viewDirection)
{
    Ray ray;
    ray.origin = cameraPosWS;
    ray.direction = viewDirection;
    ray.hitPoint = viewDirection*10000000;
    BoundingSphere sphere = GetCloudVolumeBoundingSphere();
    RayMarchingResult result = CloudVolumeRayMarching(ray,sphere);
    return PackVolumetricCloudRayMarchingResult(result,viewDirection);
}


float4 ShadeVolumetricCloudHemiOctahedronStratified(float3 cameraPosWS,float3 viewDirection)
{
    Ray ray;
    ray.origin = cameraPosWS;
    ray.direction = viewDirection;
    ray.hitPoint = viewDirection*10000000;

    BoundingSphere sphere = GetCloudVolumeBoundingSphere();
    BoundsHitResult boundsHit = CalcBoundsHit(sphere,ray);
    RayMarchingResult result;
    result.luminance = 0;
    result.transmittance = 1;
    result.density = 0;
    result.distance = 0;
    uint segmentCount = min(VoxelMetaLut_GetDimensions().z,100);

 
    uint normalSampleCount = FetchParam(VolumeRayMarchingDesc).viewDirectionIteration;
    uint targetMaxSampleCount = FetchParam(VolumeRayMarchingDesc).viewDirectionMaxSampleCount;
    SphereVolumeSampleConfig sampleCfg = GetSphereVolumeSampleConfig(boundsHit,sphere,normalSampleCount,targetMaxSampleCount);

    float3 direction = OctahedronVolume_GetDirectionFromSampleCoords(OctahedronVolume_GetDirectionSampleCoords(viewDirection));
    float2 directionCoords = OctahedronVolume_GetDirectionCoords(direction);
    float2 directionSampleUV = OctahedronVolume_GetDirectionSampleCoords(viewDirection);
    uint segmentTestNormalSampleCount = _VolumetricCloud_SegmentTestNormalIteration;
    Ray ray2 = ray;
    ray2.direction = direction;
    BoundsHitResult boundsHit2 = CalcBoundsHit(sphere,ray2);
    SphereVolumeSampleConfig segmentPrescanSampleCfg = GetSphereVolumeSampleConfig(boundsHit,sphere,segmentTestNormalSampleCount,segmentTestNormalSampleCount*30);
    float segmentPrescanMaxMultiplier = segmentPrescanSampleCfg.directionSampleCount;
    float segmentUnitLength = segmentPrescanSampleCfg.directionSampleStepSize;
    
    
    
    [loop]
    for (uint i = 0; i < segmentCount; i++)
    {
      //  VoxelMeta thisSegMeta = VoxelMetaLut_Read(uint3(directionCoords,i));
        float dw = rcp(VoxelMetaLut_GetDimensions().z);
        VoxelMeta thisSegMeta = VoxelMetaLut_Sample(float3(directionSampleUV, (i + 0.5f) * dw));
        float segmentBegin = thisSegMeta.segmentBegin * segmentPrescanMaxMultiplier;
        float segmentEnd = thisSegMeta.segmentEnd * segmentPrescanMaxMultiplier;
        float segmentLengthMultiplier = segmentEnd - segmentBegin;
        if (segmentLengthMultiplier < 0.002)
        {
            break;
        }
 
        float segmentLength = segmentLengthMultiplier * segmentUnitLength;
        uint segmentSampleCount = max(0,ceil(segmentLength * rcp(sampleCfg.directionSampleStepSize)));
        BoundsHitResult segmentBoundsHit;
        segmentBoundsHit.distToBounds = boundsHit.distToBounds + segmentUnitLength * segmentBegin;
        segmentBoundsHit.distInsideBounds = segmentLength;
        RayMarchingContext rayMarchingContext = GetRayMarchingContext(ray,segmentBoundsHit,segmentSampleCount);
        rayMarchingContext = JitterRayMarchingEntryPoint(rayMarchingContext);
        RayMarchingResult segmentResult = CloudVolumeRayMarching(rayMarchingContext);
        result.luminance += result.transmittance*segmentResult.luminance;
        result.transmittance *= segmentResult.transmittance;
        result.density += segmentResult.density;
        result.distance += segmentResult.distance;
        if (result.transmittance.r < 0.01)
        {
            break;
        }
    }
    return PackVolumetricCloudRayMarchingResult(result,viewDirection);
}

float4 Frag(Varyings input) : SV_Target  {
    /*float hasDensity = DirectionOccupancyLut_Sample(input.uv);
    if (hasDensity.r < 0.1){
        return 0;
    }*/
    float3 viewDirectionShading = OctahedronVolume_GetDirectionFromSampleCoords(input.uv);
    float3 cameraPosWS = float3(0,0,0);
    if (FrustumCullVector(_ViewProjectionMatrix,viewDirectionShading)){
        if (IsPixelSelectedInTile(input.position) == false){
            discard;
        }
    }
    if (viewDirectionShading.y < 0.02){
        return 0;
    }
    float4 cloudColor = ShadeVolumetricCloudHemiOctahedron(cameraPosWS, viewDirectionShading);
    return cloudColor;
}



#endif
```
