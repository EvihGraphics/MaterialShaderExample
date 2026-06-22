# Egaku source: `Shaders/Ocean/Ocean.SH.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
#pragma once
#ifndef OCEAN_SH_HLSL
#define OCEAN_SH_HLSL
#include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Lighting.hlsl"

float4 _EnvLight_SHAr;
float4 _EnvLight_SHAg;
float4 _EnvLight_SHAb;
float4 _EnvLight_SHBr;
float4 _EnvLight_SHBg;
float4 _EnvLight_SHBb;
float4 _EnvLight_SHC;

float3 SH_DiffuseIndirect(float3 normalWS)
{
    float4 SHCoefficients[7];
    SHCoefficients[0] = _EnvLight_SHAr;
    SHCoefficients[1] = _EnvLight_SHAg;
    SHCoefficients[2] = _EnvLight_SHAb;
    SHCoefficients[3] = _EnvLight_SHBr;
    SHCoefficients[4] = _EnvLight_SHBg;
    SHCoefficients[5] = _EnvLight_SHBb;
    SHCoefficients[6] = _EnvLight_SHC;
    float3 Color = SampleSH9(SHCoefficients,normalWS);
    return max(0,Color);
}

#endif
```
