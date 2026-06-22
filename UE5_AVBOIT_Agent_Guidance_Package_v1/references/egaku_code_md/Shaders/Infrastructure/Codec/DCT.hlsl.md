# Egaku source: `Shaders/Infrastructure/Codec/DCT.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef DCT_HLSL
#define DCT_HLSL
#include "Packages/com.unity.render-pipelines.core/ShaderLibrary/Macros.hlsl"


#ifndef DCT_HARMONICS_COUNT
#define DCT_HARMONICS_COUNT 7
#endif

#define DCT_FREQUENCY_COUNT (DCT_HARMONICS_COUNT + 1)

#define DCT_CEIL(x,d) (x+d-1)/d  

struct DCTBasis
{
    
};
 
struct DCTCoeffs
{
    float4 a[DCT_CEIL(DCT_FREQUENCY_COUNT,4)];
};

uint DCT_GetHarmonicsCount()
{
    return DCT_HARMONICS_COUNT;
}

uint DCT_GetFrequencyCount()
{
    return DCT_FREQUENCY_COUNT;
}

uint DCT_GetVectorizedElemCount()
{
    return DCT_CEIL(DCT_FREQUENCY_COUNT,4);
}

uint DCT_GetVectorizedElemIndex(uint index)
{
    return index/4;
}

DCTCoeffs DCT_CombineDCTCoeffs(DCTCoeffs c1,DCTCoeffs c2)
{
    [unroll]
    for (int i = 0; i < DCT_GetVectorizedElemCount(); i++)
    {
        c1.a[i] += c2.a[i];
    }
    return c1;
}

float DCT_CalcBaseAngle(float n,float sampleCount)
{
    return PI * (n + 0.5f) * rcp(sampleCount);
}


DCTCoeffs DCT_NormalizeCoeffs(DCTCoeffs coeffs,float sampleCount)
{
    float amplitudeDC = rcp(sampleCount);
    float amplitudeAC = amplitudeDC * 2;
    
    coeffs.a[0].r *= amplitudeDC;
    coeffs.a[0].gba *= amplitudeAC;
    
    [unroll]
    for (int i = 1; i < DCT_GetVectorizedElemCount(); i++)
    {
        coeffs.a[i] *= amplitudeAC;
    }
    
    return coeffs;
}

// d must be in [0,1]
DCTCoeffs DCT_CreateDCTCoeffs(float sampleValue,float baseAngle)
{
    float baseSin = sin(baseAngle);
    float baseCos = cos(baseAngle);
    
    DCTCoeffs c;
    
    // k-th harmonics
    // iterate trigonometrics with:
    // sin((n+1)*a) = sin(n*a) * cos(a) + cos(n*a) * sin(a)
    // cos((n+1)*a) = cos(n*a) * cos(a) - sin(n*a) * sin(a)
 
    float4 thisCos = 0;
    float4 thisSin = 0;
    
    thisCos[0] = 1;
    
    thisSin[1] = thisSin[0] * baseCos + thisCos[0] * baseSin;
    thisCos[1] = thisCos[0] * baseCos - thisSin[0] * baseSin;

    thisSin[2] = thisSin[1] * baseCos + thisCos[1] * baseSin;
    thisCos[2] = thisCos[1] * baseCos - thisSin[1] * baseSin;
    
    thisSin[3] = thisSin[2] * baseCos + thisCos[2] * baseSin;
    thisCos[3] = thisCos[2] * baseCos - thisSin[2] * baseSin;

  //  thisCos.gba = cos(float3(baseAngle,2*baseAngle,3*baseAngle));
    c.a[0] = sampleValue * thisCos;
    
    float prevSin = thisSin[3];
    float prevCos = thisCos[3];
    
    [unroll]
    for (int i = 1; i < DCT_GetVectorizedElemCount(); i++)
    {
        thisSin[0] = prevSin * baseCos + prevCos * baseSin;
        thisCos[0] = prevCos * baseCos - prevSin * baseSin;
    
        thisSin[1] = thisSin[0] * baseCos + thisCos[0] * baseSin;
        thisCos[1] = thisCos[0] * baseCos - thisSin[0] * baseSin;

        thisSin[2] = thisSin[1] * baseCos + thisCos[1] * baseSin;
        thisCos[2] = thisCos[1] * baseCos - thisSin[1] * baseSin;
    
        thisSin[3] = thisSin[2] * baseCos + thisCos[2] * baseSin;
        thisCos[3] = thisCos[2] * baseCos - thisSin[2] * baseSin;

       // thisCos = cos(float4(baseAngle*i*4,baseAngle*(4*i+1),baseAngle*(4*i+2),baseAngle*(4*i+3)));
        c.a[i] = sampleValue * thisCos;

        prevCos = thisCos[3];
        prevSin = thisSin[3];
    }
    return c;
}

// d must be in [0,1]
DCTCoeffs DCT_CreateDCTCoeffs(float4 sampleValue,float4 baseAngle)
{
    float4 baseSin = sin(baseAngle);
    float4 baseCos = cos(baseAngle);
    
    DCTCoeffs c;
    
    // k-th harmonics
    // iterate trigonometrics with:
    // sin((n+1)*a) = sin(n*a) * cos(a) + cos(n*a) * sin(a)
    // cos((n+1)*a) = cos(n*a) * cos(a) - sin(n*a) * sin(a)
 
    float4x4 thisCos = 0;
    float4x4 thisSin = 0;
    
    thisCos[0] = float4(1,1,1,1);
    
    thisSin[1] = thisSin[0] * baseCos + thisCos[0] * baseSin;
    thisCos[1] = thisCos[0] * baseCos - thisSin[0] * baseSin;

    thisSin[2] = thisSin[1] * baseCos + thisCos[1] * baseSin;
    thisCos[2] = thisCos[1] * baseCos - thisSin[1] * baseSin;
    
    thisSin[3] = thisSin[2] * baseCos + thisCos[2] * baseSin;
    thisCos[3] = thisCos[2] * baseCos - thisSin[2] * baseSin;

    c.a[0] = mul(thisCos,sampleValue);
    
    float4 prevSin = thisSin[3];
    float4 prevCos = thisCos[3];
    
    [unroll]
    for (int i = 1; i < DCT_GetVectorizedElemCount(); i++)
    {
        thisSin[0] = prevSin * baseCos + prevCos * baseSin;
        thisCos[0] = prevCos * baseCos - prevSin * baseSin;
    
        thisSin[1] = thisSin[0] * baseCos + thisCos[0] * baseSin;
        thisCos[1] = thisCos[0] * baseCos - thisSin[0] * baseSin;

        thisSin[2] = thisSin[1] * baseCos + thisCos[1] * baseSin;
        thisCos[2] = thisCos[1] * baseCos - thisSin[1] * baseSin;
    
        thisSin[3] = thisSin[2] * baseCos + thisCos[2] * baseSin;
        thisCos[3] = thisCos[2] * baseCos - thisSin[2] * baseSin;
        
        c.a[i] = mul(thisCos,sampleValue);

        prevSin = thisSin[3];
        prevCos = thisCos[3];
    }
    return c;
}


// d must be in [0,1]
DCTCoeffs DCT_CreateDCTCoeffs_Fast(float sampleValue,float baseAngle)
{
    float baseSin = sin(baseAngle);
    float baseCos = cos(baseAngle);
    
    DCTCoeffs c;
    // DC term
    c.a[0] = sampleValue;
    
    float prevHarmonicSin = 0;
    float prevHarmonicCos = 1;

    // k-th harmonics
    // iterate trigonometrics with:
    // sin((n+1)*a) = sin(n*a) * cos(a) + cos(n*a) * sin(a)
    // cos((n+1)*a) = cos(n*a) * cos(a) - sin(n*a) * sin(a)
    [unroll]
    for (int order = 1; order <= DCT_GetHarmonicsCount(); order++)
    {
        float harmonicSin = prevHarmonicSin * baseCos + prevHarmonicCos * baseSin;
        float harmonicCos = prevHarmonicCos * baseCos - prevHarmonicSin * baseSin;
        c.a[order] = sampleValue * harmonicCos;
        prevHarmonicSin = harmonicSin;
        prevHarmonicCos = harmonicCos;
    }
    return c;
}

float DCT_Decode(DCTCoeffs c,float baseAngle)
{
    float baseSin = sin(baseAngle);
    float baseCos = cos(baseAngle);

    float sampleValue = 0;
    // k-th harmonics
    // iterate trigonometrics with:
    // sin((n+1)*a) = sin(n*a) * cos(a) + cos(n*a) * sin(a)
    // cos((n+1)*a) = cos(n*a) * cos(a) - sin(n*a) * sin(a)
 
    float4 thisCos = 0;
    float4 thisSin = 0;
    
    thisCos[0] = 1;
    
    thisSin[1] = thisSin[0] * baseCos + thisCos[0] * baseSin;
    thisCos[1] = thisCos[0] * baseCos - thisSin[0] * baseSin;

    thisSin[2] = thisSin[1] * baseCos + thisCos[1] * baseSin;
    thisCos[2] = thisCos[1] * baseCos - thisSin[1] * baseSin;
    
    thisSin[3] = thisSin[2] * baseCos + thisCos[2] * baseSin;
    thisCos[3] = thisCos[2] * baseCos - thisSin[2] * baseSin;

    sampleValue += dot(c.a[0], thisCos);
    
    float prevSin = thisSin[3];
    float prevCos = thisCos[3];
    
    [unroll]
    for (int i = 1; i < DCT_GetVectorizedElemCount(); i++)
    {
        thisSin[0] = prevSin * baseCos + prevCos * baseSin;
        thisCos[0] = prevCos * baseCos - prevSin * baseSin;
    
        thisSin[1] = thisSin[0] * baseCos + thisCos[0] * baseSin;
        thisCos[1] = thisCos[0] * baseCos - thisSin[0] * baseSin;

        thisSin[2] = thisSin[1] * baseCos + thisCos[1] * baseSin;
        thisCos[2] = thisCos[1] * baseCos - thisSin[1] * baseSin;
    
        thisSin[3] = thisSin[2] * baseCos + thisCos[2] * baseSin;
        thisCos[3] = thisCos[2] * baseCos - thisSin[2] * baseSin;
        
        sampleValue += dot(c.a[i],thisCos);

        prevSin = thisSin[3];
        prevCos = thisCos[3];
    }
    return sampleValue;
}

#endif
```
