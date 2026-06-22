# Egaku source: `Shaders/Sky/Cloud/Volumetric/VolumetricCloud.OctahedronStratified.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef VOLUMETRIC_CLOUD_OCTAHEDRON_STRATIFIED_HLSL
#define VOLUMETRIC_CLOUD_OCTAHEDRON_STRATIFIED_HLSL

#include "VolumetricCloud.RayMarchingShading.hlsl"
#include "Assets/Shaders/Sky/Basic/Utils.hlsl"
#include "VolumetricCloud.TilePixelSelector.hlsl"
#pragma enable_d3d11_debug_symbols

float4x4 _ViewProjectionMatrix;


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

float4x4 GetFrustumPlanesWSNoNearFar(float4x4 projectMatrix)
{
    float4x4 planes;
    planes[0] = projectMatrix[3] - projectMatrix[0];
    planes[1] = projectMatrix[3] + projectMatrix[0];
    planes[2] = projectMatrix[3] - projectMatrix[1];
    planes[3] = projectMatrix[3] + projectMatrix[1];
    return planes;
}

bool FrustumCullVector(float3 vectorVS)
{
    float4x4 planes = GetFrustumPlanesWSNoNearFar(_ViewProjectionMatrix);
    return dot(planes[0], vectorVS) < 0.0f ||
        dot(planes[1], vectorVS) < 0.0f ||
        dot(planes[2], vectorVS) < 0.0f ||
        dot(planes[3], vectorVS) < 0.0f;
}


float4 Frag(Varyings input) : SV_Target  {
    float3 viewDirectionShading = HemiOctahedronUVToYUpVector(input.uv);
    float3 cameraPosWS = float3(0,0,0);
    if (FrustumCullVector(viewDirectionShading)){
        if (IsPixelSelectedInTile(input.position) == false){
            discard;
        }
    }
    float4 cloudColor = ShadeVolumetricCloudHemiOctahedronStratified(cameraPosWS, viewDirectionShading);
    return cloudColor;
}



#endif
```
