# Egaku source: `Shaders/Ocean/Modules/WaterShading/WaterScattering.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef WATER_SCATTERING_HLSL
#define WATER_SCATTERING_HLSL
#include "../Utils.hlsl"
#include "WaterShadingHelperStructs.hlsl"

float3 CalcScatteringFactor(WaterShadingContext context)
{
    DeclareParamFetch(WaterVolumetricShadingDesc,shadingDesc);
    float vertexNoV = dot(context.vertexNormalWS, context.viewDir);
    float pixelNoV = dot(context.pixelNormalWS, context.viewDir);
    float3 scatter = lerp(vertexNoV, abs(pixelNoV),  shadingDesc.subscatteringDetail);
    scatter -= GetVertical(context.viewDir);
    scatter = saturate(scatter);
    scatter = pow(scatter, shadingDesc.subscatteringPower);
    scatter = saturate(scatter * shadingDesc.subscatteringScale);
    return scatter;
}

#endif
```
