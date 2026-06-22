# Egaku source: `Shaders/Sky/Atmosphere/Atmosphere.OctahedronPrtPass.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef ATMOSPHERE_OCTAHEDRON_PRT_PASS_HLSL
#define ATMOSPHERE_OCTAHEDRON_PRT_PASS_HLSL

#define USE_ATMOSPHERE_TRANSMITTANCE_LUT_SRV
#include "Atmosphere.PrecomputedRayMarchingShading.hlsl"
#include "Assets/Shaders/Sky/Basic/RaySpace/OctahedronSpace.hlsl"
//#include "Atmosphere.RayMarchingShading.hlsl"
#pragma enable_d3d11_debug_symbols



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

float4 Frag(Varyings input) : SV_Target  {
    float3 viewDirection = OctahedronSpace_GetHemiDirectionFromSampleCoords(input.uv);
    float3 cameraPosWS = float3(0,0,0);
    float4 atmospherePRT = ShadeAtmosphere_PRT(cameraPosWS, viewDirection,20);
    return atmospherePRT;
}

#endif
```
