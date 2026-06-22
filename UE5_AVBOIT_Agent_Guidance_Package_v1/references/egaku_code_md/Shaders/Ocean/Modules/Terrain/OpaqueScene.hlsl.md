# Egaku source: `Shaders/Ocean/Modules/Terrain/OpaqueScene.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef OPAQUE_SCENE_HLSL
#define OPAQUE_SCENE_HLSL
#include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/DeclareOpaqueTexture.hlsl"

float3 SampleOpaqueSceneColor(float2 screenUV)
{
    return SampleSceneColor(screenUV);
}

float GetOpaqueSceneLinearEyeDepth(float2 screenUV)
{
    return LinearEyeDepth(GetSceneDepth(screenUV), _ZBufferParams);
}

float2 GetUVDistortion(float3 normalWS, float3 vertexNormalWS, float refraction)
{
    float3 normalVS = TransformWorldToView(normalWS);
    float3 vertexNormalVS = TransformWorldToView(vertexNormalWS);

    float2 distortion = (vertexNormalVS.xz - normalVS.xz) * (refraction-1);
    return distortion;
}

float2 GetScreenUV(float4 positionSS)
{
    return positionSS/_ScreenParams.xy;
}

#endif
```
