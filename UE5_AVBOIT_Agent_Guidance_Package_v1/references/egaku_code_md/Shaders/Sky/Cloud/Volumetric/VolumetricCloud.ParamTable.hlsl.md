# Egaku source: `Shaders/Sky/Cloud/Volumetric/VolumetricCloud.ParamTable.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
// This file was generated from Modules.Sky.Cloud.Volumetric.Material.VolumetricCloudShaderParamTable by Egaku.Rendering.Pipeline.Common.ShaderCodeGenerator.
// Please do not modify it manually.
// Codes in this file are functioned as material parameter table, which is used to declare some uniform variables and macro constants.

#ifndef VolumetricCloud_ParamTable_hlsl
#define VolumetricCloud_ParamTable_hlsl
#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Core.hlsl"


CBUFFER_BEGIN(PerMaterial)
	float _CloudVolume_earthRadius;
	float _CloudVolume_heightLowerBound;
	float _CloudVolume_heightUpperBound;
	float _CloudVolume_scatteringCoeff;
	float _CloudVolume_absorptionCoeff;
	float _CloudVolume_phaseG1;
	float _CloudVolume_phaseG2;
	float _CloudVolume_densityScale;
	float _CloudVolume_ambientLightGroundFactor;
	float _CloudVolume_ambientLightSkyFactor;
	float _CloudVolume_mainLightFactor;
	float _CloudVolume_powderEffectFactor;
	float _CloudVolume_viewZenithGradientStart;
	float _CloudVolume_viewZenithGradientEnd;
	float _CloudVolume_distantViewZenithGradientStart;
	float _CloudVolume_distantViewZenithGradientEnd;
	int _VolumeRayMarching_viewDirectionIteration;
	int _VolumeRayMarching_viewDirectionMaxSampleCount;
	int _VolumeRayMarching_mainLightIteration;
	int _VolumeRayMarching_mainLightMaxSampleCount;
	float _VolumeRayMarching_jitterRange;
	float4 _CloudBasicDensity_channelWeights;
	float4 _CloudBasicDensity_noiseScale;
	float4 _CloudBasicDensity_noiseOffset;
	float _CloudBasicDensity_volumeScale;
	float4 _CloudDetailDensity_channelWeights;
	float4 _CloudDetailDensity_noiseScale;
	float4 _CloudDetailDensity_noiseOffset;
	float _CloudDetailDensity_volumeScale;
	float4 _DistantCloudBasicDensity_channelWeights;
	float4 _DistantCloudBasicDensity_noiseScale;
	float4 _DistantCloudBasicDensity_noiseOffset;
	float _DistantCloudBasicDensity_volumeScale;
	float4 _DistantCloudDetailDensity_channelWeights;
	float4 _DistantCloudDetailDensity_noiseScale;
	float4 _DistantCloudDetailDensity_noiseOffset;
	float _DistantCloudDetailDensity_volumeScale;
	float4 _WeatherMap_scale;
	float4 _WeatherMap_offset;
	float _WeatherMap_mapScale;
	float4 _Wind_direction;
	float _Wind_speed;
	float _CloudOctaveMultiScatterApprox_extinctionFactor;
	float _CloudOctaveMultiScatterApprox_scatteringFactor;
	float _CloudOctaveMultiScatterApprox_phaseFactor;
	int _CloudOctaveMultiScatterApprox_approxCount;
CBUFFER_END


Texture3D _CloudBasicDensity_noiseTex;
Texture3D _CloudDetailDensity_noiseTex;
Texture3D _DistantCloudBasicDensity_noiseTex;
Texture3D _DistantCloudDetailDensity_noiseTex;
Texture2D _WeatherMap_weatherMap;






#endif // VolumetricCloud_ParamTable_hlsl
```
