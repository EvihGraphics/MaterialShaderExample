# Egaku source: `Shaders/OIT/Fourier/OpacityMap/Common/FourierCoeffs.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef FOURIER_COEFFS_HLSL
#define FOURIER_COEFFS_HLSL
#include "Packages/com.unity.render-pipelines.core/ShaderLibrary/Macros.hlsl"


#ifndef FOM_FOURIER_COEFFS_HARMONICS_COUNT
#define FOM_FOURIER_COEFFS_HARMONICS_COUNT 7
#endif

#define FOM_FOURIER_COEFFS_FREQUENCY_COUNT (FOM_FOURIER_COEFFS_HARMONICS_COUNT + 1)

struct FourierCoeffs
{
	float sine[FOM_FOURIER_COEFFS_FREQUENCY_COUNT];
	float cosine[FOM_FOURIER_COEFFS_FREQUENCY_COUNT];
};

uint FOM_GetFourierHarmonicsCount()
{
	return FOM_FOURIER_COEFFS_HARMONICS_COUNT;
}

uint FOM_GetFourierFrequencyCount()
{
	return FOM_FOURIER_COEFFS_FREQUENCY_COUNT;
}


FourierCoeffs FOM_CombineFourierCoeffs(FourierCoeffs fc1,FourierCoeffs fc2)
{
	[unroll]
	for (int order = 0; order < FOM_GetFourierFrequencyCount(); order++)
	{
		fc1.cosine[order] += fc2.cosine[order];
		fc1.sine[order] += fc2.sine[order];
	}
	return fc1;
}

// d must be in [0,1]
FourierCoeffs FOM_CreateFourierCoeffs(float extinction,float d)
{
	float baseSin = sin(TWO_PI * 1 * d);
	float baseCos = cos(TWO_PI * 1 * d);
	float amplitude = 2.0 * extinction;
	
	FourierCoeffs fc;
	// DC term
	fc.cosine[0] = amplitude;
	fc.sine[0] = 0.0;

	// Base
	fc.cosine[1] = amplitude * baseCos;
	fc.sine[1] = amplitude * baseSin;
	float prevHarmonicSin = baseSin;
	float prevHarmonicCos = baseCos;

	// k-th harmonics
	// iterate trigonometrics with:
	// sin((n+1)*a) = sin(n*a) * cos(a) + cos(n*a) * sin(a)
	// cos((n+1)*a) = cos(n*a) * cos(a) - sin(n*a) * sin(a)
	[unroll]
	for (int order = 2; order <= FOM_GetFourierHarmonicsCount(); order++)
	{
		float harmonicSin = prevHarmonicSin * baseCos + prevHarmonicCos * baseSin;
		float harmonicCos = prevHarmonicCos * baseCos - prevHarmonicSin * baseSin;
		fc.cosine[order] = amplitude * harmonicCos;
		fc.sine[order] = amplitude * harmonicSin;
		prevHarmonicSin = harmonicSin;
		prevHarmonicCos = harmonicCos;
	}
	return fc;
}


#endif
```
