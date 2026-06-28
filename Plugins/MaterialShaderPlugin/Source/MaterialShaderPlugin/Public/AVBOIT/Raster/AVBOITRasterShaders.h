// Copyright

#pragma once

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "RenderGraphUtils.h"

//
// Splat Shaders
//

class FAVBOITRasterSplatVS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FAVBOITRasterSplatVS);
	SHADER_USE_PARAMETER_STRUCT(FAVBOITRasterSplatVS, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER(FMatrix44f, LocalToWorld)
		SHADER_PARAMETER(FMatrix44f, WorldToClip)
		SHADER_PARAMETER(FMatrix44f, WorldToView)
		SHADER_PARAMETER(FIntVector4, ViewRectMin)
	END_SHADER_PARAMETER_STRUCT()

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters) { return true; }
};

class FAVBOITRasterSplatPS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FAVBOITRasterSplatPS);
	SHADER_USE_PARAMETER_STRUCT(FAVBOITRasterSplatPS, FGlobalShader);

	class FTestCoverageDim : SHADER_PERMUTATION_BOOL("AVBOIT_TEST_COVERAGE");
	using FPermutationDomain = TShaderPermutationDomain<FTestCoverageDim>;

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER(float, ZNear)
		SHADER_PARAMETER(float, ZFar)
		SHADER_PARAMETER(FVector2f, ViewResolution)
		SHADER_PARAMETER(FVector4f, ColorAndAlpha)
		SHADER_PARAMETER(FIntVector4, ViewRectMin)
		SHADER_PARAMETER_RDG_BUFFER_UAV(RWStructuredBuffer<uint>, OutExtinctionVolume)
		SHADER_PARAMETER_RDG_BUFFER_UAV(RWBuffer<uint>, FragmentCoverageCounter)
		RENDER_TARGET_BINDING_SLOTS()
	END_SHADER_PARAMETER_STRUCT()

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters) { return true; }
};

//
// Forward Shaders
//

class FAVBOITRasterForwardVS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FAVBOITRasterForwardVS);
	SHADER_USE_PARAMETER_STRUCT(FAVBOITRasterForwardVS, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER(FMatrix44f, LocalToWorld)
		SHADER_PARAMETER(FMatrix44f, WorldToClip)
		SHADER_PARAMETER(FMatrix44f, WorldToView)
		SHADER_PARAMETER(FIntVector4, ViewRectMin)
	END_SHADER_PARAMETER_STRUCT()

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters) { return true; }
};

class FAVBOITRasterForwardPS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FAVBOITRasterForwardPS);
	SHADER_USE_PARAMETER_STRUCT(FAVBOITRasterForwardPS, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER(float, ZNear)
		SHADER_PARAMETER(float, ZFar)
		SHADER_PARAMETER(FVector4f, ColorAndAlpha)
		SHADER_PARAMETER(float, ReferenceBrightnessMultiplier)
		SHADER_PARAMETER(FIntVector4, ViewRectMin)
		SHADER_PARAMETER_RDG_TEXTURE_SRV(Texture2DArray<float>, TransmittanceVolume)
		RENDER_TARGET_BINDING_SLOTS()
	END_SHADER_PARAMETER_STRUCT()

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters) { return true; }
};

//
// Composite Shaders
//

class FAVBOITRasterCompositePS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FAVBOITRasterCompositePS);
	SHADER_USE_PARAMETER_STRUCT(FAVBOITRasterCompositePS, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER(FIntVector4, ViewRectMin)
		SHADER_PARAMETER_RDG_TEXTURE(Texture2D<float4>, ColorAccumulation)
		SHADER_PARAMETER_RDG_TEXTURE_SRV(Texture2DArray<float>, TransmittanceVolume)
		RENDER_TARGET_BINDING_SLOTS()
	END_SHADER_PARAMETER_STRUCT()

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters) { return true; }
};

struct FAVBOITRasterDebugHeader
{
	FVector4f ColorAccumulation;
	FVector4f SceneColorBefore;
	FVector4f SceneColorAfter;
	float LinearViewDepth;
	float NormalizedDepth;
	uint32 Slice;
	uint32 FragmentCoverageCount;
	FIntPoint TextureExtent;
	FIntPoint ViewRectMin;
	FIntPoint ViewRectMax;
	FIntPoint Padding;
};

struct FAVBOITRasterDebugPayload
{
	FAVBOITRasterDebugHeader Header;
	uint32 PackedExtinction[64];
	float Transmittance[64];
};

class MATERIALSHADEREXAMPLE_API FAVBOITRasterDebugExtractCS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FAVBOITRasterDebugExtractCS);
	SHADER_USE_PARAMETER_STRUCT(FAVBOITRasterDebugExtractCS, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER(FVector2f, ViewResolution)
		SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<uint>, ExtinctionVolume)
		SHADER_PARAMETER_RDG_TEXTURE_SRV(Texture2DArray<float>, TransmittanceVolume)
		SHADER_PARAMETER_RDG_TEXTURE(Texture2D<float4>, ColorAccumulation)
		SHADER_PARAMETER_RDG_TEXTURE(Texture2D<float4>, SceneColorBefore)
		SHADER_PARAMETER_RDG_TEXTURE(Texture2D<float4>, SceneColorAfter)
		SHADER_PARAMETER_RDG_BUFFER_UAV(RWStructuredBuffer<FAVBOITRasterDebugPayload>, OutDebugPayload)

		SHADER_PARAMETER(FIntPoint, DebugPixel)
		SHADER_PARAMETER(float, LinearViewDepth)
		SHADER_PARAMETER(float, NormalizedDepth)
		SHADER_PARAMETER(uint32, Slice)
		SHADER_PARAMETER_RDG_BUFFER_SRV(Buffer<uint>, FragmentCoverageCounter)
		SHADER_PARAMETER(FIntPoint, TextureExtent)
		SHADER_PARAMETER(FIntPoint, ViewRectMin)
		SHADER_PARAMETER(FIntPoint, ViewRectMax)
	END_SHADER_PARAMETER_STRUCT()

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters) { return true; }
};
