# Egaku source: `Shaders/Sky/Skybox/Skybox.VolumetricCloudTexture.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef SKYBOX_VOLUMETRIC_CLOUD_TEXTURE_HLSL
#define SKYBOX_VOLUMETRIC_CLOUD_TEXTURE_HLSL
#include "Assets/Shaders/Sky/Basic/RaySpace/OctahedronConcentric.hlsl"
#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Texture.hlsl"
#include "Assets/Shaders/Sky/Cloud/Volumetric/Filters/BilateralFilter.hlsl"

Texture2D _Cloud;
float _BilateralFilter_KernelSize;
float4 _BilateralFilter_ColorSigma;
float _BilateralFilter_SpaceSigma;
float4x4 _ViewProjectionMatrix;

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


float4 SampleVolumetricCloudBilateralFiltered(float3 direction)
{
    if (direction.y < 0)
    {
        return 0;
    }
    float4 cloud;
    float2 uv = OctahedronConcentric_GetHemiDirectionSampleCoords(direction);
    if (direction.y > 0.45)
    {
        uint x,y;
        _Cloud.GetDimensions(x,y);
        float2 duv = float2(1.0f / x,1.0f / y);
        BilateralFilterDesc bfDesc;
        bfDesc.kernelSize = _BilateralFilter_KernelSize;
        bfDesc.valueSigma = _BilateralFilter_ColorSigma;
        bfDesc.spaceSigma = _BilateralFilter_SpaceSigma;
        cloud = SampleTextureBilateralBlurred(_Cloud,sampler_LinearClamp,uv,duv,bfDesc);
    }else{
        cloud = SampleTexture(_Cloud,sampler_LinearClamp,uv);
    }
    return cloud;
}

float4 SampleVolumetricCloud(float3 direction)
{
    if (direction.y < 0)
    {
        return 0;
    }
    float2 uv = OctahedronConcentric_GetHemiDirectionSampleCoords(direction);
    float4 cloud = SampleTexture(_Cloud,sampler_LinearClamp,uv);
    return cloud;
}

#endif
```
