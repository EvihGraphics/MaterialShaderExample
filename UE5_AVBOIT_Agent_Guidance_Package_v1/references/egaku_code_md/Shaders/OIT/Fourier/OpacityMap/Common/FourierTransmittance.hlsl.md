# Egaku source: `Shaders/OIT/Fourier/OpacityMap/Common/FourierTransmittance.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef  FOURIER_TRANSMITTANCE_HLSL
#define  FOURIER_TRANSMITTANCE_HLSL
#include "FourierCoeffs.hlsl"


// d must be in [0,1]
float FOM_EvalTransmittance(FourierCoeffs coeffs,float d)
{
    float compSin = 0;
    float compCos = 0;
    // DC
    compSin += coeffs.cosine[0] * d * 0.5f;
    
    float baseCos = cos(TWO_PI * d);
    float baseSin = sin(TWO_PI * d);
    float amplitude = INV_TWO_PI;

    // Base
    // According to integrated result, we need to use cosine coeffs to multiply with sine and vice versa.
    compSin += amplitude * coeffs.cosine[1] * baseSin;
    compCos += amplitude * coeffs.sine[1] * (1 - baseCos);
    float prevHarmonicCos = baseCos;
    float prevHarmonicSin = baseSin;

    // k-th harmonics
    // iterate trigonometrics with:
    // sin((n+1)*a) = sin(n*a) * cos(a) + cos(n*a) * sin(a)
    // cos((n+1)*a) = cos(n*a) * cos(a) - sin(n*a) * sin(a)
    [unroll]
    for(int i = 2; i <= FOM_GetFourierHarmonicsCount();i++)
    {
        float harmonicCos = prevHarmonicCos * baseCos - prevHarmonicSin * baseSin;
        float harmonicSin = prevHarmonicSin * baseCos + prevHarmonicCos * baseSin;
        amplitude = INV_TWO_PI * rcp(i);
        compSin += amplitude * coeffs.cosine[i] * harmonicSin;
        compCos += amplitude * coeffs.sine[i] * (1 - harmonicCos);
        prevHarmonicCos = harmonicCos;
        prevHarmonicSin = harmonicSin;
    }
    float opticalDepth = max(0,compSin + compCos);
    return exp(-opticalDepth);
}

#endif
```
