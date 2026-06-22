# Egaku source: `Shaders/Sky/Cloud/Volumetric/Passes/OctahedronRayMarchingPass.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef VOLUMETRIC_CLOUD_OCTAHEDRON_RAY_MARCHING_PASS_HLSL
#define VOLUMETRIC_CLOUD_OCTAHEDRON_RAY_MARCHING_PASS_HLSL


#include "Assets/Shaders/Sky/Basic/VolumeBase/PerspectiveFrustum.hlsl"
#include "../Utils/TilePixelSelector.hlsl"
#include "../Shading/RayMarching/CloudVolumeRayMarching.hlsl"
#include "../Shading/VolumeLighting/CloudVolumeLighting.CommonScattering.hlsl"
#include "Assets/Shaders/Sky/Basic/RaySpace/OctahedronConcentric.hlsl"

#pragma enable_d3d11_debug_symbols

float4x4 _ViewProjectionMatrix;
uint _VolumetricCloud_SegmentTestNormalIteration;



struct Attributes {
    float4 vertex : POSITION;
    float2 uv : TEXCOORD0;
};

struct Varyings {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

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


float4 Frag(Varyings input) : SV_Target  {
    /*float hasDensity = DirectionOccupancyLut_Sample(input.uv);
    if (hasDensity.r < 0.1){
        return 0;
    }*/
    float3 viewDirectionShading = OctahedronConcentric_GetHemiDirectionFromSampleCoords(input.uv);
    float3 cameraPosWS = float3(0,0,0);
    if (FrustumCullVector(_ViewProjectionMatrix,viewDirectionShading)){
       // discard; 
        if (IsPixelSelectedInTile(input.position,uint2(2,2)) == false){
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
