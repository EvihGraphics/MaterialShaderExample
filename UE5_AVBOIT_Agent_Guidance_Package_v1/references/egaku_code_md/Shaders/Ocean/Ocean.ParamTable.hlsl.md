# Egaku source: `Shaders/Ocean/Ocean.ParamTable.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
// This file was generated from Modules.Ocean.MaterialParamTable.OceanShaderParamTable by Egaku.Rendering.Pipeline.Common.ShaderCodeGenerator.
// Please do not modify it manually.
// Codes in this file are functioned as material parameter table, which is used to declare some uniform variables and macro constants.

#ifndef Ocean_ParamTable_hlsl
#define Ocean_ParamTable_hlsl
#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Core.hlsl"


Texture2D _OceanTerrain_terrainMap;
Texture2D _OceanTerrain_heightMap;
Texture2D _SmoothNoiseMap_tex;
Texture2D _CoastlineWaveProfile_profileMap;
Texture2DArray _DistantWave_waveSeqFrameMap;
Texture2D _WaterFoamMap_foamMap;


CBUFFER_BEGIN(PerMaterial)
	float2 _OceanTerrain_rectShapeWS;
	float2 _OceanTerrain_rectPosOriginWS;
	float _OceanTerrain_toCoastlineMaxDist;
	float _OceanTerrain_slopePredictionLength;
	float _OceanTerrain_coastlineWaveRangeWidth;
	float _CoastlineWaveProfile_waveLength;
	float _CoastlineWaveProfile_waveCount;
	float2 _CoastlineWaveProfile_waveDisplaceScale;
	float2 _CoastlineWaveProfile_waveDisplaceOffset;
	float _WaveProfileNoise_noiseTime;
	float _WaveProfileNoise_noiseScale;
	bool _WaveProfileNoise_withDetails;
	float _CoastlineSurface_neighborVtxMockLength;
	float _CoastlineSurface_velocityScale;
	float _CoastlineSurface_foamScale;
	float _CoastlineSurface_foamDistance;
	float2 _DistantWave_waveSeqFPSSliceCount;
	float _DistantWave_waveHeightScale;
	float _DistantWave_waveHeightOffset;
	float _DistantWave_foamScale;
	float _DistantWave_foamOffset;
	float2 _DistantWave_waveLengthScale;
	float2 _DistantWave_windVelocity;
	float2 _DistantWave_waveVelocityBottom;
	float2 _DistantWave_waveVelocityTop;
	float _DistantWave_mapNormalDecodeConst;
	float _WaterVolumetricShading_clarityFar;
	float _WaterVolumetricShading_clarityNear;
	float4 _WaterVolumetricShading_absorptionFar;
	float _WaterVolumetricShading_absorptionFarDist;
	float4 _WaterVolumetricShading_absorptionNear;
	float _WaterVolumetricShading_absorptionNearDist;
	float4 _WaterVolumetricShading_scatteringFar;
	float _WaterVolumetricShading_scatteringFarDist;
	float4 _WaterVolumetricShading_scatteringNear;
	float _WaterVolumetricShading_scatteringNearDist;
	float _WaterVolumetricShading_scatteringFoamScale;
	float _WaterVolumetricShading_subscatteringDetail;
	float _WaterVolumetricShading_subscatteringPower;
	float _WaterVolumetricShading_subscatteringScale;
	float4 _WaterVolumetricShading_behindColorFar;
	float4 _WaterVolumetricShading_behindColorNear;
	float4 _WaterSurfaceShading_overlayColorFar;
	float4 _WaterSurfaceShading_overlayColorNear;
	float _WaterSurfaceShading_directLightSpecular;
	float _WaterSurfaceShading_reflectionSpecular;
	float4 _WaterSurfaceShading_foamBaseColor;
	float4 _WaterSurfaceShading_foamDetailColor;
	float _WaterSurfaceShading_softFoamScale;
	float2 _WaterFoamMap_uvScale;
	float _WaterFoamMap_advectionSpeedScale;
CBUFFER_END




INSTANCING_BUFFER_BEGIN(PerInstance)
	DEFINE_INSTANCED_PROP(float4, _Mesh_pageStitchingMask)
INSTANCING_BUFFER_END(PerInstance)


#endif // Ocean_ParamTable_hlsl
```
