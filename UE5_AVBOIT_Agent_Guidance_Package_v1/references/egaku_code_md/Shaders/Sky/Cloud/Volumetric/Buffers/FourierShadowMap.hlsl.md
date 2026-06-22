# Egaku source: `Shaders/Sky/Cloud/Volumetric/Buffers/FourierShadowMap.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef FOURIER_SHADOW_MAP_HLSL
#define FOURIER_SHADOW_MAP_HLSL

#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Texture.hlsl"
#include "Assets/Shaders/OIT/Fourier/OpacityMap/Common/FourierCoeffs.hlsl"

struct FourierCoeffsLutEntry
{
	float cos0;
	float sin0;
	float cos1;
	float sin1;
};



#ifdef VOLUMETRIC_CLOUD_USE_FSM_FOURIER_COEFFS_LUT_UAV

#define VOLUMETRIC_CLOUD_FSM_FOURIER_COEFFS_LUT_DEFINED
#ifdef VOLUMETRIC_CLOUD_USE_FOURIER_COEFFS_LUT_TEXARRAY
RWTexture2DArray<float4> _VolumetricCloud_FSM_FourierCoeffsLut;
#else
RWTexture2D<float4> _VolumetricCloud_FSM_FourierCoeffsLut0;
RWTexture2D<float4> _VolumetricCloud_FSM_FourierCoeffsLut1;
RWTexture2D<float4> _VolumetricCloud_FSM_FourierCoeffsLut2;
RWTexture2D<float4> _VolumetricCloud_FSM_FourierCoeffsLut3;
#endif

#else

#ifdef VOLUMETRIC_CLOUD_USE_FSM_FOURIER_COEFFS_LUT_SRV

#define VOLUMETRIC_CLOUD_FSM_FOURIER_COEFFS_LUT_DEFINED
#ifdef VOLUMETRIC_CLOUD_USE_FOURIER_COEFFS_LUT_TEXARRAY
Texture2DArray<float4> _VolumetricCloud_FSM_FourierCoeffsLut;
#else
Texture2D<float4> _VolumetricCloud_FSM_FourierCoeffsLut0;
Texture2D<float4> _VolumetricCloud_FSM_FourierCoeffsLut1;
Texture2D<float4> _VolumetricCloud_FSM_FourierCoeffsLut2;
Texture2D<float4> _VolumetricCloud_FSM_FourierCoeffsLut3;
#endif
#endif

#endif

uint3 FSM_GetDimensions();

#ifdef VOLUMETRIC_CLOUD_FSM_FOURIER_COEFFS_LUT_DEFINED
uint3 FSM_GetDimensions()
{
	uint x,y,z = 1;
	#ifdef VOLUMETRIC_CLOUD_USE_FOURIER_COEFFS_LUT_TEXARRAY
	_VolumetricCloud_FSM_FourierCoeffsLut.GetDimensions(x,y,z);
	#else
	_VolumetricCloud_FSM_FourierCoeffsLut0.GetDimensions(x,y);
	#endif
	return uint3(x,y,z);
}
#endif




FourierCoeffsLutEntry FSM_UnpackValue(float4 value)
{
	FourierCoeffsLutEntry entry;
	entry.cos0 = value.x;
	entry.sin0 = value.y;
	entry.cos1 = value.z;
	entry.sin1 = value.w;
	return entry;
}

float4 FSM_GetPackedValue(FourierCoeffsLutEntry entry)
{
	float4 packed = float4(entry.cos0, entry.sin0, entry.cos1, entry.sin1);
	return packed;
}

#ifdef VOLUMETRIC_CLOUD_FSM_FOURIER_COEFFS_LUT_DEFINED

#endif


FourierCoeffsLutEntry FSM_PackToFourierCoeffsLutEntry(FourierCoeffs coeffs, uint harmonicIndex)
{
	FourierCoeffsLutEntry entry;
	entry.cos0 = coeffs.cosine[harmonicIndex];
	entry.sin0 = coeffs.sine[harmonicIndex];
	entry.cos1 = coeffs.cosine[harmonicIndex + 1];
	entry.sin1 = coeffs.sine[harmonicIndex + 1];
	return entry;
}

#ifdef VOLUMETRIC_CLOUD_USE_FSM_FOURIER_COEFFS_LUT_SRV

float4 FSM_SampleSlice(float2 rayDirUV, uint slice)
{
	float4 val;
	#ifdef VOLUMETRIC_CLOUD_USE_FOURIER_COEFFS_LUT_TEXARRAY
	val = SampleTextureArray(_VolumetricCloud_FSM_FourierCoeffsLut,sampler_LinearClamp,rayDirUV,slice);
	#else
	switch (slice)
	{
	case 0:
		val = SampleTexture(_VolumetricCloud_FSM_FourierCoeffsLut0,sampler_LinearClamp,rayDirUV);
		break;
	case 1:
		val = SampleTexture(_VolumetricCloud_FSM_FourierCoeffsLut1,sampler_LinearClamp,rayDirUV);
		break;
	case 2:
		val = SampleTexture(_VolumetricCloud_FSM_FourierCoeffsLut2,sampler_LinearClamp,rayDirUV);
		break;
	case 3:
		val = SampleTexture(_VolumetricCloud_FSM_FourierCoeffsLut3,sampler_LinearClamp,rayDirUV);
		break;
	default:
		val = 0;
		break;
	}
	#endif
	return val;
}


FourierCoeffsLutEntry FSM_SampleFourierCoeffsLutEntry(float2 rayDirUV, uint harmonicIndex)
{
	uint slice = harmonicIndex / 2;
	float4 val = FSM_SampleSlice(rayDirUV,slice);
	FourierCoeffsLutEntry entry = FSM_UnpackValue(val);
	return entry;
}

FourierCoeffs FSM_SampleFourierCoeffs(float2 rayDirUV)
{
	FourierCoeffs coeffs;
	[unroll]
	for (uint i = 0; i < FOM_GetFourierFrequencyCount(); i += 2)
	{
		FourierCoeffsLutEntry entry = FSM_SampleFourierCoeffsLutEntry(rayDirUV,i);
		coeffs.cosine[i] = entry.cos0;
		coeffs.sine[i] = entry.sin0;
		coeffs.cosine[i+1] = entry.cos1;
		coeffs.sine[i+1] = entry.sin1;
	}
	return coeffs;
}

#endif

#endif
```
