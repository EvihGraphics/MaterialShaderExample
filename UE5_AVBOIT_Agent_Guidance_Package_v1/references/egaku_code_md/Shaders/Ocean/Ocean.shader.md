# Egaku source: `Shaders/Ocean/Ocean.shader`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
Shader "MyCustom/Ocean"
{
	Properties
	{
		
		[Header(Ocean Terrain)]
		_OceanTerrain_terrainMap("Terrain Map", 2D) = "" {}
		_OceanTerrain_heightMap("Height Map", 2D) = "" {}
		_OceanTerrain_rectShapeWS("Terrain Rect Shape", Vector) = (0, 0, 0, 0)
		_OceanTerrain_rectPosOriginWS("Terrain Rect Origin", Vector) = (0, 0, 0, 0)
		_OceanTerrain_toCoastlineMaxDist("Coastline Max Distance", Float) = 0.0
		_OceanTerrain_slopePredictionLength("Slope Prediction Length", Float) = 0.0
		_OceanTerrain_coastlineWaveRangeWidth("Coastline Wave Range Width", Range(0,100)) = 0
		
		[Header(Smooth Noise Map)]
		_SmoothNoiseMap_tex("Smooth Noise Map", 2D) = "" {}
		
		[Header(Coastline Wave Profile)]
		_CoastlineWaveProfile_profileMap("Wave Profile Map", 2D) = "" {}
		_CoastlineWaveProfile_waveLength("Wave Length", Float) = 0.0
		_CoastlineWaveProfile_waveCount("Wave Count", Float) = 0.0
		_CoastlineWaveProfile_waveDisplaceScale("Wave Displacement Scale", Vector) = (0, 0, 0, 0)
		_CoastlineWaveProfile_waveDisplaceOffset("Wave Displacement Offset", Vector) = (0, 0, 0, 0)
		
		[Header(Coastline Wave Profile Noise)]
		_WaveProfileNoise_noiseTime("Wave Noise Time", Float) = 0.0
		_WaveProfileNoise_noiseScale("Wave Noise Scale", Float) = 0.0
		_WaveProfileNoise_withDetails("With Details", Integer) = 0
		
		[Header(Coastline Surface)]
		_CoastlineSurface_neighborVtxMockLength("Neighbor Vtx Mock Length", Float) = 0.0
		_CoastlineSurface_velocityScale("Velocity Scale", Float) = 0.0
		_CoastlineSurface_foamScale("Foam Scale", Float) = 0.0
		_CoastlineSurface_foamDistance("Foam Offset", Float) = 0.0
		
		[Header(Distant Wave)]
		_DistantWave_waveSeqFrameMap("Wave Sequence Frame Map", 2DArray) = "" {}
		_DistantWave_waveSeqFPSSliceCount("Wave Sequence Frame FPS and Slice Count", Vector) = (0, 0, 0, 0)
		_DistantWave_waveHeightScale("Wave Height Scale", Float) = 0.0
		_DistantWave_waveHeightOffset("Wave Height Offset", Float) = 0.0
		_DistantWave_foamScale("Foam Scale", Float) = 0.0
		_DistantWave_foamOffset("Foam Offset", Float) = 0.0
		_DistantWave_waveLengthScale("Wave Length Scale", Vector) = (0, 0, 0, 0)
		_DistantWave_windVelocity("Wind Velocity", Vector) = (0, 0, 0, 0)
		_DistantWave_waveVelocityBottom("Wave Velocity Bottom", Vector) = (0, 0, 0, 0)
		_DistantWave_waveVelocityTop("Wave Velocity Top", Vector) = (0, 0, 0, 0)
		_DistantWave_mapNormalDecodeConst("Map Normal Decode Const", Float) = 0.0
		
		[Header(Water Volume Shading)]
		_WaterVolumetricShading_clarityFar("Clarity Far", Float) = 0.0
		_WaterVolumetricShading_clarityNear("Clarity Near", Float) = 0.0
		_WaterVolumetricShading_absorptionFar("Absorption Far", Color) = (0, 0, 0, 0)
		_WaterVolumetricShading_absorptionFarDist("Absorption Far Dist", Float) = 0.0
		_WaterVolumetricShading_absorptionNear("Absorption Near", Color) = (0, 0, 0, 0)
		_WaterVolumetricShading_absorptionNearDist("Absorption Near Dist", Float) = 0.0
		_WaterVolumetricShading_scatteringFar("Scattering Far", Color) = (0, 0, 0, 0)
		_WaterVolumetricShading_scatteringFarDist("Scattering Far Dist", Float) = 0.0
		_WaterVolumetricShading_scatteringNear("Scattering Near", Color) = (0, 0, 0, 0)
		_WaterVolumetricShading_scatteringNearDist("Scattering Near Dist", Float) = 0.0
		_WaterVolumetricShading_scatteringFoamScale("Foam Scattering Scale", Float) = 0.0
		_WaterVolumetricShading_subscatteringDetail("SSS detail", Float) = 0.0
		_WaterVolumetricShading_subscatteringPower("SSS power", Float) = 0.0
		_WaterVolumetricShading_subscatteringScale("SSS scale", Float) = 0.0
		_WaterVolumetricShading_behindColorFar("Behind Water Color Far", Color) = (0, 0, 0, 0)
		_WaterVolumetricShading_behindColorNear("Behind Water Color Near", Color) = (0, 0, 0, 0)
		
		[Header(Water Surface Shading)]
		_WaterSurfaceShading_overlayColorFar("Overlay Color Far", Color) = (0, 0, 0, 0)
		_WaterSurfaceShading_overlayColorNear("Overlay Color Near", Color) = (0, 0, 0, 0)
		_WaterSurfaceShading_directLightSpecular("Direct Light Ks", Float) = 0.0
		_WaterSurfaceShading_reflectionSpecular("Reflection Ks", Float) = 0.0
		_WaterSurfaceShading_foamBaseColor("Foam Base Color", Color) = (0, 0, 0, 0)
		_WaterSurfaceShading_foamDetailColor("Foam Detail Color", Color) = (0, 0, 0, 0)
		_WaterSurfaceShading_softFoamScale("Soft Foam Scale", Float) = 0.0
		
		[Header(Water Foam Map)]
		_WaterFoamMap_foamMap("Foam Map", 2D) = "" {}
		_WaterFoamMap_uvScale("Foam Map UV Scale", Vector) = (0, 0, 0, 0)
		_WaterFoamMap_advectionSpeedScale("Foam Map Advection Speed Scale", Float) = 0.0
	}
	
	HLSLINCLUDE
		#define UNIVERSAL_RENDER_PIPELINE 
		#include "Ocean.ParamTable.hlsl"
	ENDHLSL
	
	SubShader
	{
		Tags
		{
			"RenderPipeline" = "UniversalPipeline"
			"RenderType" = "Transparent"
			"Queue" = "Transparent"
			"LightMode" = "UniversalForward"
		}
		
		Pass
		{
			
			Cull Off
			ZTest On
			ZWrite On
			
			HLSLPROGRAM
				#include "Ocean.hlsl"
				
				#pragma multi_compile_instancing
				#pragma target 4.0
				#pragma vertex Vert
				#pragma fragment Frag
			ENDHLSL
		}
		
	}
}
```
