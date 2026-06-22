# Egaku source: `Shaders/HLSLInclude/ScreenSpaceUtils.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef SCREEN_SPACE_UTILS_HLSL
#define SCREEN_SPACE_UTILS_HLSL

#include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/DeclareDepthTexture.hlsl"
#ifndef UNITY_COMMON_INCLUDED
#warning "UNITY_HLSL_FILES_NEEDED"
#endif


float GetSceneDepth(float3 positionSS)
{
    float2 uv = positionSS.xy / _ScaledScreenParams.xy;
    #if UNITY_REVERSED_Z
    float depth = SampleSceneDepth(uv);
    #else
    float depth = lerp(UNITY_NEAR_CLIP_VALUE, 1, SampleSceneDepth(uv));
    #endif
    return depth;
}

float GetSceneDepth(float2 screenUV)
{
    #if UNITY_REVERSED_Z
    float depth = SampleSceneDepth(screenUV);
    #else
    float depth = lerp(UNITY_NEAR_CLIP_VALUE, 1, SampleSceneDepth(uv));
    #endif
    return depth;
}

float3 GetWorldPosition(float2 screenUV){
    /* get world space position from clip position */
    #if UNITY_REVERSED_Z
    real depth = SampleSceneDepth(screenUV);
    #else
    real depth = lerp(UNITY_NEAR_CLIP_VALUE, 1, SampleSceneDepth(uv));
    #endif
    return ComputeWorldSpacePosition(screenUV, depth, UNITY_MATRIX_I_VP);
}


        

#endif
```
