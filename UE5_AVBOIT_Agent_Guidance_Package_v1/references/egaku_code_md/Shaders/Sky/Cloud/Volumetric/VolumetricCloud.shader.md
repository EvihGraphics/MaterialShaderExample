# Egaku source: `Shaders/Sky/Cloud/Volumetric/VolumetricCloud.shader`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
Shader "MyCustom/Sky/Cloud/Volumetric"
{
	Properties
	{
		
		[Header(Cloud Volume Settings)]
		_CloudVolume_earthRadius("Earth Radius", Float) = 0.0
		_CloudVolume_heightLowerBound("Cloud Height Lower Bound", Float) = 0.0
		_CloudVolume_heightUpperBound("Cloud Height Upper Bound", Float) = 0.0
		_CloudVolume_scatteringCoeff("Scattering Coefficient", Float) = 0.0
		_CloudVolume_absorptionCoeff("Absorption Coefficient", Float) = 0.0
		_CloudVolume_phaseG1("Phase G1", Range(-1,1)) = -1
		_CloudVolume_phaseG2("Phase G2", Range(-1,1)) = -1
		_CloudVolume_densityScale("Density Scale", Range(0,10)) = 0
		_CloudVolume_ambientLightGroundFactor("Cloud Ambient Lighting Ground Factor", Float) = 0.0
		_CloudVolume_ambientLightSkyFactor("Cloud Ambient Lighting Sky Factor", Float) = 0.0
		_CloudVolume_mainLightFactor("Cloud Main Lighting Factor", Float) = 0.0
		_CloudVolume_powderEffectFactor("Cloud Powder Effect Factor", Float) = 0.0
		_CloudVolume_viewZenithGradientStart("View Zenith Gradient Start", Float) = 0.0
		_CloudVolume_viewZenithGradientEnd("View Zenith Gradient End", Float) = 0.0
		_CloudVolume_distantViewZenithGradientStart("Distant View Zenith Gradient Start", Float) = 0.0
		_CloudVolume_distantViewZenithGradientEnd("Distant View Zenith Gradient End", Float) = 0.0
		
		[Header(Volume Ray Marching Settings)]
		_VolumeRayMarching_viewDirectionIteration("View Direction Iteration", Integer) = 0
		_VolumeRayMarching_viewDirectionMaxSampleCount("View Direction Max Sample Count", Integer) = 0
		_VolumeRayMarching_mainLightIteration("Main Light Direction Iteration", Integer) = 0
		_VolumeRayMarching_mainLightMaxSampleCount("Main Light Direction Max Sample Count", Integer) = 0
		_VolumeRayMarching_jitterRange("Sample Point Jitter Range", Range(0,0.8)) = 0
		
		[Header( Cloud Basic Density Settings)]
		_CloudBasicDensity_noiseTex("Noise Texture", 3D) = "" {}
		_CloudBasicDensity_channelWeights("Noise Channel Weights", Vector) = (0, 0, 0, 0)
		_CloudBasicDensity_noiseScale("Density Noise Scale", Vector) = (0, 0, 0, 0)
		_CloudBasicDensity_noiseOffset("Density Noise Offset", Vector) = (0, 0, 0, 0)
		_CloudBasicDensity_volumeScale("Density Volume Scale", Float) = 0.0
		
		[Header(Cloud Detail Density Settings)]
		_CloudDetailDensity_noiseTex("Noise Texture", 3D) = "" {}
		_CloudDetailDensity_channelWeights("Noise Channel Weights", Vector) = (0, 0, 0, 0)
		_CloudDetailDensity_noiseScale("Density Noise Scale", Vector) = (0, 0, 0, 0)
		_CloudDetailDensity_noiseOffset("Density Noise Offset", Vector) = (0, 0, 0, 0)
		_CloudDetailDensity_volumeScale("Density Volume Scale", Float) = 0.0
		
		[Header(Distant Cloud Basic Density Settings)]
		_DistantCloudBasicDensity_noiseTex("Noise Texture", 3D) = "" {}
		_DistantCloudBasicDensity_channelWeights("Noise Channel Weights", Vector) = (0, 0, 0, 0)
		_DistantCloudBasicDensity_noiseScale("Density Noise Scale", Vector) = (0, 0, 0, 0)
		_DistantCloudBasicDensity_noiseOffset("Density Noise Offset", Vector) = (0, 0, 0, 0)
		_DistantCloudBasicDensity_volumeScale("Density Volume Scale", Float) = 0.0
		
		[Header(Distant Cloud Detail Density Settings)]
		_DistantCloudDetailDensity_noiseTex("Noise Texture", 3D) = "" {}
		_DistantCloudDetailDensity_channelWeights("Noise Channel Weights", Vector) = (0, 0, 0, 0)
		_DistantCloudDetailDensity_noiseScale("Density Noise Scale", Vector) = (0, 0, 0, 0)
		_DistantCloudDetailDensity_noiseOffset("Density Noise Offset", Vector) = (0, 0, 0, 0)
		_DistantCloudDetailDensity_volumeScale("Density Volume Scale", Float) = 0.0
		
		[Header(Weather Map Settings)]
		_WeatherMap_weatherMap("Texture", 2D) = "" {}
		_WeatherMap_scale("Scale", Vector) = (0, 0, 0, 0)
		_WeatherMap_offset("Offset", Vector) = (0, 0, 0, 0)
		_WeatherMap_mapScale("Map Scale", Float) = 0.0
		
		[Header(Wind Settings)]
		_Wind_direction("Direction", Vector) = (0, 0, 0, 0)
		_Wind_speed("Speed", Float) = 0.0
		
		[Header(Cloud Octave Multi Scattering Approximation Settings)]
		_CloudOctaveMultiScatterApprox_extinctionFactor("Extinction Factor", Range(0,1)) = 0
		_CloudOctaveMultiScatterApprox_scatteringFactor("Scattering Factor", Range(0,1)) = 0
		_CloudOctaveMultiScatterApprox_phaseFactor("Phase Factor", Range(0,1)) = 0
		_CloudOctaveMultiScatterApprox_approxCount("Approx Count", Integer) = 0
	}
	
	HLSLINCLUDE
		#define UNIVERSAL_RENDER_PIPELINE 
		#include "VolumetricCloud.ParamTable.hlsl"
	ENDHLSL
	
	SubShader
	{
		Tags
		{
			"RenderPipeline" = "UniversalPipeline"
		}
		
		Pass
		{
			Name "OctahedronMarching"
			
			Cull Off
			ZWrite Off
			ZTest Off
			
			HLSLPROGRAM
				#include "Passes/OctahedronRayMarchingPass.hlsl"
				
				#pragma target 5.0
				#pragma vertex Vert
				#pragma fragment Frag
			ENDHLSL
		}
		
		Pass
		{
			Name "TemporalFilter"
			
			Cull Off
			ZWrite Off
			ZTest Off
			
			HLSLPROGRAM
				#include "Passes/FilterPass.hlsl"
				
				#pragma target 5.0
				#pragma vertex Vert
				#pragma fragment Frag
			ENDHLSL
		}
		
		Pass
		{
			Name "SunTransmittanceLutBuilding"
			
			Cull Off
			ZWrite Off
			ZTest Off
			ColorMask 0
			
			HLSLPROGRAM
				#include "Passes/SunTransmittanceLutBuildingPass.hlsl"
				
				#pragma target 5.0
				#pragma vertex Vert
				#pragma fragment Frag
			ENDHLSL
		}
		
		Pass
		{
			Name "VoxelMetaLutBuilding"
			
			Cull Off
			ZWrite Off
			ZTest Off
			ColorMask 0
			
			HLSLPROGRAM
				#include "Passes/VoxelMetaLutBuildingPass.hlsl"
				
				#pragma target 5.0
				#pragma vertex Vert
				#pragma fragment Frag
			ENDHLSL
		}
		
		Pass
		{
			Name "DirectionOccupancyLutBuilding"
			
			Cull Off
			ZWrite Off
			ZTest Off
			
			HLSLPROGRAM
				#include "Passes/DirectionOccupancyLutBuildingPass.hlsl"
				
				#pragma target 5.0
				#pragma vertex Vert
				#pragma fragment Frag
			ENDHLSL
		}
		
		Pass
		{
			Name "FourierShadowMapBuilding"
			
			Cull Off
			ZWrite Off
			ZTest Off
			
			HLSLPROGRAM
				#include "Passes/FourierShadowMapBuildingPass.hlsl"
				
				#pragma target 5.0
				#pragma vertex Vert
				#pragma fragment Frag
			ENDHLSL
		}
		
		Pass
		{
			Name "DCTShadowMapBuilding"
			
			Cull Off
			ZWrite Off
			ZTest Off
			
			HLSLPROGRAM
				#include "Passes/DCTShadowMapBuildingPass.hlsl"
				
				#pragma target 5.0
				#pragma vertex Vert
				#pragma fragment Frag
			ENDHLSL
		}
		
	}
}
```
