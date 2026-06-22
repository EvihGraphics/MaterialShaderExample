# Egaku source: `Shaders/Ocean/Ocean.ParamStruct.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
// This file was generated from Modules.Ocean.MaterialParamTable.OceanShaderParamTable by Egaku.Rendering.Pipeline.Common.ShaderCodeGenerator.
// Please do not modify it manually.

#ifndef Ocean_ParamStruct_hlsl
#define Ocean_ParamStruct_hlsl
#include "Ocean.ParamTable.hlsl"


#define FetchParam(structName) Get##structName()
#define DeclareParamFetch(structName, var) structName var = FetchParam(structName)


// Declare material parameter struct MeshDesc
struct MeshDesc
{
	float4 pageStitchingMask;
};


// Getter for struct MeshDesc
MeshDesc GetMeshDesc()
{
	MeshDesc result;
	result.pageStitchingMask = ACCESS_INSTANCED_PROP(PerInstance, _Mesh_pageStitchingMask);
	return result;
}


// Declare material parameter struct OceanTerrainDesc
struct OceanTerrainDesc
{
	Texture2D terrainMap;
	Texture2D heightMap;
	float2 rectShapeWS;
	float2 rectPosOriginWS;
	float toCoastlineMaxDist;
	float slopePredictionLength;
	float coastlineWaveRangeWidth;
};


// Getter for struct OceanTerrainDesc
OceanTerrainDesc GetOceanTerrainDesc()
{
	OceanTerrainDesc result;
	result.terrainMap = _OceanTerrain_terrainMap;
	result.heightMap = _OceanTerrain_heightMap;
	result.rectShapeWS = _OceanTerrain_rectShapeWS;
	result.rectPosOriginWS = _OceanTerrain_rectPosOriginWS;
	result.toCoastlineMaxDist = _OceanTerrain_toCoastlineMaxDist;
	result.slopePredictionLength = _OceanTerrain_slopePredictionLength;
	result.coastlineWaveRangeWidth = _OceanTerrain_coastlineWaveRangeWidth;
	return result;
}


// Declare material parameter struct SmoothNoiseMapDesc
struct SmoothNoiseMapDesc
{
	Texture2D tex;
};


// Getter for struct SmoothNoiseMapDesc
SmoothNoiseMapDesc GetSmoothNoiseMapDesc()
{
	SmoothNoiseMapDesc result;
	result.tex = _SmoothNoiseMap_tex;
	return result;
}


// Declare material parameter struct CoastlineWaveProfileDesc
struct CoastlineWaveProfileDesc
{
	Texture2D profileMap;
	float waveLength;
	float waveCount;
	float2 waveDisplaceScale;
	float2 waveDisplaceOffset;
};


// Getter for struct CoastlineWaveProfileDesc
CoastlineWaveProfileDesc GetCoastlineWaveProfileDesc()
{
	CoastlineWaveProfileDesc result;
	result.profileMap = _CoastlineWaveProfile_profileMap;
	result.waveLength = _CoastlineWaveProfile_waveLength;
	result.waveCount = _CoastlineWaveProfile_waveCount;
	result.waveDisplaceScale = _CoastlineWaveProfile_waveDisplaceScale;
	result.waveDisplaceOffset = _CoastlineWaveProfile_waveDisplaceOffset;
	return result;
}


// Declare material parameter struct WaveProfileNoiseDesc
struct WaveProfileNoiseDesc
{
	float noiseTime;
	float noiseScale;
	bool withDetails;
};


// Getter for struct WaveProfileNoiseDesc
WaveProfileNoiseDesc GetWaveProfileNoiseDesc()
{
	WaveProfileNoiseDesc result;
	result.noiseTime = _WaveProfileNoise_noiseTime;
	result.noiseScale = _WaveProfileNoise_noiseScale;
	result.withDetails = _WaveProfileNoise_withDetails;
	return result;
}


// Declare material parameter struct CoastlineSurfaceDesc
struct CoastlineSurfaceDesc
{
	float neighborVtxMockLength;
	float velocityScale;
	float foamScale;
	float foamDistance;
};


// Getter for struct CoastlineSurfaceDesc
CoastlineSurfaceDesc GetCoastlineSurfaceDesc()
{
	CoastlineSurfaceDesc result;
	result.neighborVtxMockLength = _CoastlineSurface_neighborVtxMockLength;
	result.velocityScale = _CoastlineSurface_velocityScale;
	result.foamScale = _CoastlineSurface_foamScale;
	result.foamDistance = _CoastlineSurface_foamDistance;
	return result;
}


// Declare material parameter struct DistantWaveDesc
struct DistantWaveDesc
{
	Texture2DArray waveSeqFrameMap;
	float2 waveSeqFPSSliceCount;
	float waveHeightScale;
	float waveHeightOffset;
	float foamScale;
	float foamOffset;
	float2 waveLengthScale;
	float2 windVelocity;
	float2 waveVelocityBottom;
	float2 waveVelocityTop;
	float mapNormalDecodeConst;
};


// Getter for struct DistantWaveDesc
DistantWaveDesc GetDistantWaveDesc()
{
	DistantWaveDesc result;
	result.waveSeqFrameMap = _DistantWave_waveSeqFrameMap;
	result.waveSeqFPSSliceCount = _DistantWave_waveSeqFPSSliceCount;
	result.waveHeightScale = _DistantWave_waveHeightScale;
	result.waveHeightOffset = _DistantWave_waveHeightOffset;
	result.foamScale = _DistantWave_foamScale;
	result.foamOffset = _DistantWave_foamOffset;
	result.waveLengthScale = _DistantWave_waveLengthScale;
	result.windVelocity = _DistantWave_windVelocity;
	result.waveVelocityBottom = _DistantWave_waveVelocityBottom;
	result.waveVelocityTop = _DistantWave_waveVelocityTop;
	result.mapNormalDecodeConst = _DistantWave_mapNormalDecodeConst;
	return result;
}


// Declare material parameter struct WaterVolumetricShadingDesc
struct WaterVolumetricShadingDesc
{
	float clarityFar;
	float clarityNear;
	float4 absorptionFar;
	float absorptionFarDist;
	float4 absorptionNear;
	float absorptionNearDist;
	float4 scatteringFar;
	float scatteringFarDist;
	float4 scatteringNear;
	float scatteringNearDist;
	float scatteringFoamScale;
	float subscatteringDetail;
	float subscatteringPower;
	float subscatteringScale;
	float4 behindColorFar;
	float4 behindColorNear;
};


// Getter for struct WaterVolumetricShadingDesc
WaterVolumetricShadingDesc GetWaterVolumetricShadingDesc()
{
	WaterVolumetricShadingDesc result;
	result.clarityFar = _WaterVolumetricShading_clarityFar;
	result.clarityNear = _WaterVolumetricShading_clarityNear;
	result.absorptionFar = _WaterVolumetricShading_absorptionFar;
	result.absorptionFarDist = _WaterVolumetricShading_absorptionFarDist;
	result.absorptionNear = _WaterVolumetricShading_absorptionNear;
	result.absorptionNearDist = _WaterVolumetricShading_absorptionNearDist;
	result.scatteringFar = _WaterVolumetricShading_scatteringFar;
	result.scatteringFarDist = _WaterVolumetricShading_scatteringFarDist;
	result.scatteringNear = _WaterVolumetricShading_scatteringNear;
	result.scatteringNearDist = _WaterVolumetricShading_scatteringNearDist;
	result.scatteringFoamScale = _WaterVolumetricShading_scatteringFoamScale;
	result.subscatteringDetail = _WaterVolumetricShading_subscatteringDetail;
	result.subscatteringPower = _WaterVolumetricShading_subscatteringPower;
	result.subscatteringScale = _WaterVolumetricShading_subscatteringScale;
	result.behindColorFar = _WaterVolumetricShading_behindColorFar;
	result.behindColorNear = _WaterVolumetricShading_behindColorNear;
	return result;
}


// Declare material parameter struct WaterSurfaceShadingDesc
struct WaterSurfaceShadingDesc
{
	float4 overlayColorFar;
	float4 overlayColorNear;
	float directLightSpecular;
	float reflectionSpecular;
	float4 foamBaseColor;
	float4 foamDetailColor;
	float softFoamScale;
};


// Getter for struct WaterSurfaceShadingDesc
WaterSurfaceShadingDesc GetWaterSurfaceShadingDesc()
{
	WaterSurfaceShadingDesc result;
	result.overlayColorFar = _WaterSurfaceShading_overlayColorFar;
	result.overlayColorNear = _WaterSurfaceShading_overlayColorNear;
	result.directLightSpecular = _WaterSurfaceShading_directLightSpecular;
	result.reflectionSpecular = _WaterSurfaceShading_reflectionSpecular;
	result.foamBaseColor = _WaterSurfaceShading_foamBaseColor;
	result.foamDetailColor = _WaterSurfaceShading_foamDetailColor;
	result.softFoamScale = _WaterSurfaceShading_softFoamScale;
	return result;
}


// Declare material parameter struct WaterFoamMapDesc
struct WaterFoamMapDesc
{
	Texture2D foamMap;
	float2 uvScale;
	float advectionSpeedScale;
};


// Getter for struct WaterFoamMapDesc
WaterFoamMapDesc GetWaterFoamMapDesc()
{
	WaterFoamMapDesc result;
	result.foamMap = _WaterFoamMap_foamMap;
	result.uvScale = _WaterFoamMap_uvScale;
	result.advectionSpeedScale = _WaterFoamMap_advectionSpeedScale;
	return result;
}




#endif // Ocean_ParamStruct_hlsl
```
