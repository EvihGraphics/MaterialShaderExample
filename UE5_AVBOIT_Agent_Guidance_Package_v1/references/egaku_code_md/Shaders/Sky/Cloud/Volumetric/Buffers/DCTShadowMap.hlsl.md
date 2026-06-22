# Egaku source: `Shaders/Sky/Cloud/Volumetric/Buffers/DCTShadowMap.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef DCT_SHADOW_MAP_HLSL
#define DCT_SHADOW_MAP_HLSL

#define DCT_HARMONICS_COUNT 15
#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Texture.hlsl"
#include "Assets/Shaders/Infrastructure/Codec/DCT.hlsl"

struct DCTCoeffsLutEntry
{
	float4 coeffs4;
};



#ifdef VOLUMETRIC_CLOUD_USE_DCT_SHADOW_MAP_UAV

#define VOLUMETRIC_CLOUD_DCT_SHADOW_MAP_DEFINED
#ifdef VOLUMETRIC_CLOUD_USE_DCT_SHADOW_MAP_TEXARRAY
RWTexture2DArray<float4> _VolumetricCloud_DCTShadowMap;
#else
RWTexture2D<float4> _VolumetricCloud_DCTShadowMap0;
RWTexture2D<float4> _VolumetricCloud_DCTShadowMap1;
RWTexture2D<float4> _VolumetricCloud_DCTShadowMap2;
RWTexture2D<float4> _VolumetricCloud_DCTShadowMap3;
#endif

#else

#ifdef VOLUMETRIC_CLOUD_USE_DCT_SHADOW_MAP_SRV

#define VOLUMETRIC_CLOUD_DCT_SHADOW_MAP_DEFINED
#ifdef VOLUMETRIC_CLOUD_USE_DCT_SHADOW_MAP_TEXARRAY
Texture2DArray<float4> _VolumetricCloud_DCTShadowMap;
#else
Texture2D<float4> _VolumetricCloud_DCTShadowMap0;
Texture2D<float4> _VolumetricCloud_DCTShadowMap1;
Texture2D<float4> _VolumetricCloud_DCTShadowMap2;
Texture2D<float4> _VolumetricCloud_DCTShadowMap3;
#endif
#endif

#endif

uint3 DCTShadowMap_GetDimensions();

#ifdef VOLUMETRIC_CLOUD_DCT_SHADOW_MAP_DEFINED
uint3 DCTShadowMap_GetDimensions()
{
	uint x,y,z = 1;
	#ifdef VOLUMETRIC_CLOUD_USE_DCT_SHADOW_MAP_TEXARRAY
	_VolumetricCloud_DCTShadowMap.GetDimensions(x,y,z);
	#else
	_VolumetricCloud_DCTShadowMap0.GetDimensions(x,y);
	#endif
	return uint3(x,y,z);
}
#endif




DCTCoeffsLutEntry DCTShadowMap_UnpackValue(float4 value)
{
	DCTCoeffsLutEntry entry;
	entry.coeffs4 = value;
	return entry;
}

float4 DCTShadowMap_GetPackedValue(DCTCoeffsLutEntry entry)
{
	float4 packed = float4(entry.coeffs4);
	return packed;
}

#ifdef VOLUMETRIC_CLOUD_DCT_SHADOW_MAP_DEFINED

#endif


DCTCoeffsLutEntry DCTShadowMap_PackToDCTCoeffsLutEntry(DCTCoeffs coeffs, uint index)
{
	DCTCoeffsLutEntry entry = (DCTCoeffsLutEntry)0;
	if (index >= DCT_GetVectorizedElemCount())
	{
		return entry;
	}
	entry.coeffs4 = coeffs.a[index];
	return entry;
}

#ifdef VOLUMETRIC_CLOUD_USE_DCT_SHADOW_MAP_SRV

float4 DCTShadowMap_SampleSlice(float2 rayDirUV, uint slice)
{
	float4 val;
	#ifdef VOLUMETRIC_CLOUD_USE_DCT_SHADOW_MAP_TEXARRAY
	val = SampleTextureArray(_VolumetricCloud_DCTShadowMap,sampler_LinearClamp,rayDirUV,slice);
	#else
	switch (slice)
	{
	case 0:
		val = SampleTexture(_VolumetricCloud_DCTShadowMap0,sampler_LinearClamp,rayDirUV);
		break;
	case 1:
		val = SampleTexture(_VolumetricCloud_DCTShadowMap1,sampler_LinearClamp,rayDirUV);
		break;
	case 2:
		val = SampleTexture(_VolumetricCloud_DCTShadowMap2,sampler_LinearClamp,rayDirUV);
		break;
	case 3:
		val = SampleTexture(_VolumetricCloud_DCTShadowMap3,sampler_LinearClamp,rayDirUV);
		break;
	default:
		val = 0;
		break;
	}
	#endif
	return val;
}


float4 DCTShadowMap_SampleSlice(float2 rayDirUV, uint slice,float lod)
{
	float4 val;
	#ifdef VOLUMETRIC_CLOUD_USE_DCT_SHADOW_MAP_TEXARRAY
	val = SampleTextureArray(_VolumetricCloud_DCTShadowMap,sampler_LinearClamp,rayDirUV,slice);
	#else
	switch (slice)
	{
	case 0:
		val = SampleTextureLevel(_VolumetricCloud_DCTShadowMap0,sampler_LinearClamp,rayDirUV, lod);
		break;
	case 1:
		val = SampleTextureLevel(_VolumetricCloud_DCTShadowMap1,sampler_LinearClamp,rayDirUV, lod);
		break;
	case 2:
		val = SampleTextureLevel(_VolumetricCloud_DCTShadowMap2,sampler_LinearClamp,rayDirUV, lod);
		break;
	case 3:
		val = SampleTextureLevel(_VolumetricCloud_DCTShadowMap3,sampler_LinearClamp,rayDirUV, lod);
		break;
	default:
		val = 0;
		break;
	}
	#endif
	return val;
}

DCTCoeffsLutEntry DCTShadowMap_SampleDCTCoeffsLutEntry(float2 rayDirUV, uint sliceIndex)
{
	float4 val = DCTShadowMap_SampleSlice(rayDirUV,sliceIndex);
	DCTCoeffsLutEntry entry = DCTShadowMap_UnpackValue(val);
	return entry;
}

DCTCoeffsLutEntry DCTShadowMap_SampleDCTCoeffsLutEntry(float2 rayDirUV, uint sliceIndex,float lod)
{
	float4 val = DCTShadowMap_SampleSlice(rayDirUV,sliceIndex, lod);
	DCTCoeffsLutEntry entry = DCTShadowMap_UnpackValue(val);
	return entry;
}

DCTCoeffs DCTShadowMap_SampleDCTCoeffs(float2 rayDirUV)
{
	DCTCoeffs coeffs;
	[unroll]
	for (uint i = 0; i < DCT_GetVectorizedElemCount(); i++)
	{
		DCTCoeffsLutEntry entry = DCTShadowMap_SampleDCTCoeffsLutEntry(rayDirUV,i, 0);
		coeffs.a[i] = entry.coeffs4;
	}
	return coeffs;
}

#endif

#endif
```
