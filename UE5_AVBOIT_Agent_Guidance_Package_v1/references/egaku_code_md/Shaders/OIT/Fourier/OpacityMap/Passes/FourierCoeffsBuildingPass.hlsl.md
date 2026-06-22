# Egaku source: `Shaders/OIT/Fourier/OpacityMap/Passes/FourierCoeffsBuildingPass.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef FOURIER_COEFFS_BUILDING_PASS_HLSL
#define FOURIER_COEFFS_BUILDING_PASS_HLSL


#define FOM_FOURIER_COEFFS_HARMONICS_COUNT 7
#include "../Common/FourierCoeffs.hlsl"
#include "../FourierOpacityMap.ParamStruct.hlsl"
#include "../Buffers/FourierCoeffsLut.hlsl"
#include "Assets/Shaders/Infrastructure/ScreenSpaceUtils.hlsl"
#pragma enable_d3d11_debug_symbols


struct Attributes {
    float4 vertex : POSITION;
};

struct Varyings {
    float4 position : SV_POSITION;
};

struct PixelOutput
{
    float4 output0 : SV_Target0;
    float4 output1 : SV_Target1;
    float4 output2 : SV_Target2;
    float4 output3 : SV_Target3;
};

Varyings Vert(Attributes input) {
    Varyings output;
    output.position = TransformObjectToHClip(input.vertex);
    return output;
}


PixelOutput Frag(Varyings input)  {
    float4 color = FetchParam(TranslucencyDesc).baseColor;
    float extinction = -log(1 - color.a);
    float viewDepth = GetLinear01DepthFromRawZ(input.position.z);
    FourierCoeffs coeffs = FOM_CreateFourierCoeffs(extinction,viewDepth);

    FourierCoeffsLutEntry entry0 = FOM_PackToFourierCoeffsLutEntry(coeffs, 0);
    FourierCoeffsLutEntry entry1 = FOM_PackToFourierCoeffsLutEntry(coeffs, 2);
    FourierCoeffsLutEntry entry2 = FOM_PackToFourierCoeffsLutEntry(coeffs, 4);
    FourierCoeffsLutEntry entry3 = FOM_PackToFourierCoeffsLutEntry(coeffs, 6);
    
    PixelOutput output;
    output.output0 = FOM_GetPackedValue(entry0);
    output.output1 = FOM_GetPackedValue(entry1);
    output.output2 = FOM_GetPackedValue(entry2);
    output.output3 = FOM_GetPackedValue(entry3);
    return output;
}



#endif
```
