// Copyright

#pragma once

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "RenderGraphUtils.h"

//
// Splat Shaders
//

struct FAVBOITRasterDebugPixelData
{
	float LinearViewDepth;
	float NormalizedDepth;
	uint32 Slice;
	uint32 WriteFlag;
};

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
		SHADER_PARAMETER(FVector2f, VolumeResolution)
		SHADER_PARAMETER(uint32, DownsampleFactor)
		SHADER_PARAMETER(uint32, NumSlices)
		SHADER_PARAMETER(FVector4f, ColorAndAlpha)
		SHADER_PARAMETER(FIntVector4, ViewRectMin)
		SHADER_PARAMETER(FIntVector4, ViewRectSize)
		SHADER_PARAMETER(FIntPoint, DebugPixel)
		SHADER_PARAMETER_RDG_BUFFER_UAV(RWStructuredBuffer<uint>, OutExtinctionVolume)
		SHADER_PARAMETER_RDG_BUFFER_UAV(RWBuffer<uint>, OverflowCounter)
		SHADER_PARAMETER_RDG_BUFFER_UAV(RWBuffer<uint>, OutOfBoundsCounter)
		SHADER_PARAMETER_RDG_BUFFER_UAV(RWBuffer<uint>, FragmentCoverageCounter)
		SHADER_PARAMETER_RDG_BUFFER_UAV(RWStructuredBuffer<FAVBOITRasterDebugPixelData>, OutDebugPixelBuffer)
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
		SHADER_PARAMETER(FVector2f, VolumeResolution)
		SHADER_PARAMETER(uint32, DownsampleFactor)
		SHADER_PARAMETER(uint32, NumSlices)
		SHADER_PARAMETER(FIntVector4, ViewRectMin)
		SHADER_PARAMETER(FIntVector4, ViewRectSize)
		SHADER_PARAMETER_RDG_TEXTURE_SRV(Texture2DArray<float>, TransmittanceVolume)
		RENDER_TARGET_BINDING_SLOTS()
	END_SHADER_PARAMETER_STRUCT()

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters) { return true; }
};

//
// Plugin Identity Shaders
//

class MATERIALSHADEREXAMPLE_API FAVBOITRasterIdentityVS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FAVBOITRasterIdentityVS);
	SHADER_USE_PARAMETER_STRUCT(FAVBOITRasterIdentityVS, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER(FMatrix44f, LocalToWorld)
		SHADER_PARAMETER(FMatrix44f, WorldToClip)
		SHADER_PARAMETER(FMatrix44f, WorldToView)
		SHADER_PARAMETER(FIntVector4, ViewRectMin)
	END_SHADER_PARAMETER_STRUCT()

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters) { return true; }
};

class MATERIALSHADEREXAMPLE_API FAVBOITRasterIdentityPS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FAVBOITRasterIdentityPS);
	SHADER_USE_PARAMETER_STRUCT(FAVBOITRasterIdentityPS, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER(FVector4f, ColorAndAlpha)
		SHADER_PARAMETER(FIntVector4, ViewRectMin)
		SHADER_PARAMETER(FIntVector4, ViewRectSize)
		RENDER_TARGET_BINDING_SLOTS()
	END_SHADER_PARAMETER_STRUCT()

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters) { return true; }
};

//
// Resolved Alpha Shaders
//

class FAVBOITRasterResolvedAlphaCS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FAVBOITRasterResolvedAlphaCS);
	SHADER_USE_PARAMETER_STRUCT(FAVBOITRasterResolvedAlphaCS, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER(FIntPoint, TextureExtent)
		SHADER_PARAMETER(FIntPoint, VolumeExtent)
		SHADER_PARAMETER(FIntVector4, ViewRectMin)
		SHADER_PARAMETER(FIntVector4, ViewRectSize)
		SHADER_PARAMETER(uint32, DownsampleFactor)
		SHADER_PARAMETER(uint32, NumSlices)
		SHADER_PARAMETER_RDG_TEXTURE_SRV(Texture2DArray<float>, TransmittanceVolume)
		SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<float4>, OutResolvedAlpha)
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
		SHADER_PARAMETER_RDG_TEXTURE(Texture2D<float4>, ColorAccumulation)
		SHADER_PARAMETER_RDG_TEXTURE_SRV(Texture2DArray<float>, TransmittanceVolume)
		SHADER_PARAMETER(FIntVector4, ViewRectMin)
		SHADER_PARAMETER(FIntVector4, ViewRectSize)
		SHADER_PARAMETER(FIntPoint, VolumeExtent)
		SHADER_PARAMETER(uint32, DownsampleFactor)
		SHADER_PARAMETER(uint32, NumSlices)
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
	uint32 NumSlices;
	uint32 OverflowCount;
	uint32 OutOfBoundsCount;
	FIntPoint TextureExtent;
	FIntPoint VolumeExtent;
	FIntPoint ViewRectMin;
	FIntPoint ViewRectMax;
	FIntPoint Padding;
	uint32 HeaderPadding[3];
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
		SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<FAVBOITRasterDebugPixelData>, RasterDebugPixelBuffer)
		SHADER_PARAMETER_RDG_BUFFER_SRV(Buffer<uint>, FragmentCoverageCounter)
		SHADER_PARAMETER(FIntPoint, TextureExtent)
		SHADER_PARAMETER(FIntPoint, VolumeExtent)
		SHADER_PARAMETER(FIntPoint, ViewRectMin)
		SHADER_PARAMETER(FIntPoint, ViewRectMax)
		SHADER_PARAMETER(uint32, NumSlices)
		SHADER_PARAMETER(uint32, DownsampleFactor)
		SHADER_PARAMETER_RDG_BUFFER_SRV(Buffer<uint>, OverflowCounter)
		SHADER_PARAMETER_RDG_BUFFER_SRV(Buffer<uint>, OutOfBoundsCounter)
	END_SHADER_PARAMETER_STRUCT()

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters) { return true; }
};
