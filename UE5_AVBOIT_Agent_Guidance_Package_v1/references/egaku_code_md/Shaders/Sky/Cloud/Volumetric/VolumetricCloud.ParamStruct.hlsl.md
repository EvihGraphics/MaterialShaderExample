# Egaku source: `Shaders/Sky/Cloud/Volumetric/VolumetricCloud.ParamStruct.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
// This file was generated from Modules.Sky.Cloud.Volumetric.Material.VolumetricCloudShaderParamTable by Egaku.Rendering.Pipeline.Common.ShaderCodeGenerator.
// Please do not modify it manually.

#ifndef VolumetricCloud_ParamStruct_hlsl
#define VolumetricCloud_ParamStruct_hlsl
#include "VolumetricCloud.ParamTable.hlsl"


#define FetchParam(structName) Get##structName()
#define DeclareParamFetch(structName, var) structName var = FetchParam(structName)


// Declare material parameter struct CloudVolumeDesc
struct CloudVolumeDesc
{
	float earthRadius;
	float heightLowerBound;
	float heightUpperBound;
	float scatteringCoeff;
	float absorptionCoeff;
	float phaseG1;
	float phaseG2;
	float densityScale;
	float ambientLightGroundFactor;
	float ambientLightSkyFactor;
	float mainLightFactor;
	float powderEffectFactor;
	float viewZenithGradientStart;
	float viewZenithGradientEnd;
	float distantViewZenithGradientStart;
	float distantViewZenithGradientEnd;
};


// Getter for struct CloudVolumeDesc
CloudVolumeDesc GetCloudVolumeDesc()
{
	CloudVolumeDesc result;
	result.earthRadius = _CloudVolume_earthRadius;
	result.heightLowerBound = _CloudVolume_heightLowerBound;
	result.heightUpperBound = _CloudVolume_heightUpperBound;
	result.scatteringCoeff = _CloudVolume_scatteringCoeff;
	result.absorptionCoeff = _CloudVolume_absorptionCoeff;
	result.phaseG1 = _CloudVolume_phaseG1;
	result.phaseG2 = _CloudVolume_phaseG2;
	result.densityScale = _CloudVolume_densityScale;
	result.ambientLightGroundFactor = _CloudVolume_ambientLightGroundFactor;
	result.ambientLightSkyFactor = _CloudVolume_ambientLightSkyFactor;
	result.mainLightFactor = _CloudVolume_mainLightFactor;
	result.powderEffectFactor = _CloudVolume_powderEffectFactor;
	result.viewZenithGradientStart = _CloudVolume_viewZenithGradientStart;
	result.viewZenithGradientEnd = _CloudVolume_viewZenithGradientEnd;
	result.distantViewZenithGradientStart = _CloudVolume_distantViewZenithGradientStart;
	result.distantViewZenithGradientEnd = _CloudVolume_distantViewZenithGradientEnd;
	return result;
}


// Declare material parameter struct VolumeRayMarchingDesc
struct VolumeRayMarchingDesc
{
	int viewDirectionIteration;
	int viewDirectionMaxSampleCount;
	int mainLightIteration;
	int mainLightMaxSampleCount;
	float jitterRange;
};


// Getter for struct VolumeRayMarchingDesc
VolumeRayMarchingDesc GetVolumeRayMarchingDesc()
{
	VolumeRayMarchingDesc result;
	result.viewDirectionIteration = _VolumeRayMarching_viewDirectionIteration;
	result.viewDirectionMaxSampleCount = _VolumeRayMarching_viewDirectionMaxSampleCount;
	result.mainLightIteration = _VolumeRayMarching_mainLightIteration;
	result.mainLightMaxSampleCount = _VolumeRayMarching_mainLightMaxSampleCount;
	result.jitterRange = _VolumeRayMarching_jitterRange;
	return result;
}


// Declare material parameter struct CloudBasicDensityDesc
struct CloudBasicDensityDesc
{
	Texture3D noiseTex;
	float4 channelWeights;
	float4 noiseScale;
	float4 noiseOffset;
	float volumeScale;
};


// Getter for struct CloudBasicDensityDesc
CloudBasicDensityDesc GetCloudBasicDensityDesc()
{
	CloudBasicDensityDesc result;
	result.noiseTex = _CloudBasicDensity_noiseTex;
	result.channelWeights = _CloudBasicDensity_channelWeights;
	result.noiseScale = _CloudBasicDensity_noiseScale;
	result.noiseOffset = _CloudBasicDensity_noiseOffset;
	result.volumeScale = _CloudBasicDensity_volumeScale;
	return result;
}


// Declare material parameter struct CloudDetailDensityDesc
struct CloudDetailDensityDesc
{
	Texture3D noiseTex;
	float4 channelWeights;
	float4 noiseScale;
	float4 noiseOffset;
	float volumeScale;
};


// Getter for struct CloudDetailDensityDesc
CloudDetailDensityDesc GetCloudDetailDensityDesc()
{
	CloudDetailDensityDesc result;
	result.noiseTex = _CloudDetailDensity_noiseTex;
	result.channelWeights = _CloudDetailDensity_channelWeights;
	result.noiseScale = _CloudDetailDensity_noiseScale;
	result.noiseOffset = _CloudDetailDensity_noiseOffset;
	result.volumeScale = _CloudDetailDensity_volumeScale;
	return result;
}


// Declare material parameter struct DistantCloudBasicDensityDesc
struct DistantCloudBasicDensityDesc
{
	Texture3D noiseTex;
	float4 channelWeights;
	float4 noiseScale;
	float4 noiseOffset;
	float volumeScale;
};


// Getter for struct DistantCloudBasicDensityDesc
DistantCloudBasicDensityDesc GetDistantCloudBasicDensityDesc()
{
	DistantCloudBasicDensityDesc result;
	result.noiseTex = _DistantCloudBasicDensity_noiseTex;
	result.channelWeights = _DistantCloudBasicDensity_channelWeights;
	result.noiseScale = _DistantCloudBasicDensity_noiseScale;
	result.noiseOffset = _DistantCloudBasicDensity_noiseOffset;
	result.volumeScale = _DistantCloudBasicDensity_volumeScale;
	return result;
}


// Declare material parameter struct DistantCloudDetailDensityDesc
struct DistantCloudDetailDensityDesc
{
	Texture3D noiseTex;
	float4 channelWeights;
	float4 noiseScale;
	float4 noiseOffset;
	float volumeScale;
};


// Getter for struct DistantCloudDetailDensityDesc
DistantCloudDetailDensityDesc GetDistantCloudDetailDensityDesc()
{
	DistantCloudDetailDensityDesc result;
	result.noiseTex = _DistantCloudDetailDensity_noiseTex;
	result.channelWeights = _DistantCloudDetailDensity_channelWeights;
	result.noiseScale = _DistantCloudDetailDensity_noiseScale;
	result.noiseOffset = _DistantCloudDetailDensity_noiseOffset;
	result.volumeScale = _DistantCloudDetailDensity_volumeScale;
	return result;
}


// Declare material parameter struct WeatherMapDesc
struct WeatherMapDesc
{
	Texture2D weatherMap;
	float4 scale;
	float4 offset;
	float mapScale;
};


// Getter for struct WeatherMapDesc
WeatherMapDesc GetWeatherMapDesc()
{
	WeatherMapDesc result;
	result.weatherMap = _WeatherMap_weatherMap;
	result.scale = _WeatherMap_scale;
	result.offset = _WeatherMap_offset;
	result.mapScale = _WeatherMap_mapScale;
	return result;
}


// Declare material parameter struct WindDesc
struct WindDesc
{
	float4 direction;
	float speed;
};


// Getter for struct WindDesc
WindDesc GetWindDesc()
{
	WindDesc result;
	result.direction = _Wind_direction;
	result.speed = _Wind_speed;
	return result;
}


// Declare material parameter struct CloudOctaveMultiScatterApproxDesc
struct CloudOctaveMultiScatterApproxDesc
{
	float extinctionFactor;
	float scatteringFactor;
	float phaseFactor;
	int approxCount;
};


// Getter for struct CloudOctaveMultiScatterApproxDesc
CloudOctaveMultiScatterApproxDesc GetCloudOctaveMultiScatterApproxDesc()
{
	CloudOctaveMultiScatterApproxDesc result;
	result.extinctionFactor = _CloudOctaveMultiScatterApprox_extinctionFactor;
	result.scatteringFactor = _CloudOctaveMultiScatterApprox_scatteringFactor;
	result.phaseFactor = _CloudOctaveMultiScatterApprox_phaseFactor;
	result.approxCount = _CloudOctaveMultiScatterApprox_approxCount;
	return result;
}




#endif // VolumetricCloud_ParamStruct_hlsl
```
