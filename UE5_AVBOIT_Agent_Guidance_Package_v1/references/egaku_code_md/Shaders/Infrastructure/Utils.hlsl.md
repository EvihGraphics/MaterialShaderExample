# Egaku source: `Shaders/Infrastructure/Utils.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef UTILS_HLSL
#define UTILS_HLSL
#include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/DeclareDepthTexture.hlsl"

float3 GetWorldPosition(float2 screenUV){
    /* get world space position from clip position */
    #if UNITY_REVERSED_Z
    real depth = SampleSceneDepth(screenUV);
    #else
    real depth = lerp(UNITY_NEAR_CLIP_VALUE, 1, SampleSceneDepth(uv));
    #endif
    return ComputeWorldSpacePosition(screenUV, depth, UNITY_MATRIX_I_VP);
}



float3 TriangleNormal(float3 p0, float3 p1, float3 p2)
{
    float3 normal = cross(p1 - p0, p2 - p0);
    return normalize(normal);
}


float3 Make3DFromHorizontal(float2 horizontal, float vertical)
{
    return float3(horizontal.x,vertical,horizontal.y);
}

float3 HorizontalAdd(float3 threeDim,float2 horizontal)
{
    return threeDim + Make3DFromHorizontal(horizontal,0);
}

float3 WorldVerticalDir()
{
    return float3(0,1,0);
}

float3 ViewVerticalDir()
{
    return float3(0,1,0);
}


float2 GetHorizontal(float3 threeDim)
{
    return float2(threeDim.x, threeDim.z);
}

float GetVertical(float3 threeDim)
{
    return threeDim.y;
}


float3 DecodePartialDerivativeNormal(float2 normalDerivativeXY)
{
    return normalize(float3(normalDerivativeXY,1.0f));
}

float3 ReconstructNormalFromXYSquare(float2 normalXY)
{
    float z = sqrt(1.0f - dot(normalXY, normalXY));
    return float3(normalXY, z);
}

float3 TangentSpaceDefToWorldSpaceDef(float3 vec)
{
    return vec.xzy;
}

float3 WorldSpaceDefToTangentSpaceDef(float3 vec)
{
    return vec.xzy;
}


/*
float3 HemiOctahedronUVToYUpVector(float2 uv)
{
    float2 uv2 = uv * 2.0 - 1.0;
    uv2 = float2(uv2.x + uv2.y, uv2.x - uv2.y)*0.5;
    float3 n = float3(uv2,1-dot(1.0,abs(uv2)));
    return normalize(n).xzy;
}
        
float2 YUpVectorToHemiOctahedronUV(float3 n)
{
    n.xz /= dot(1.0, abs(n));
    return float2(n.x + n.z, n.x - n.z) * 0.5 + 0.5;
}*/

#endif
```
