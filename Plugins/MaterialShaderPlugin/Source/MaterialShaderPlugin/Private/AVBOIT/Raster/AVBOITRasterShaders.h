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

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER(float, ZNear)
		SHADER_PARAMETER(float, ZFar)
		SHADER_PARAMETER(FVector4f, ColorAndAlpha)
		SHADER_PARAMETER(FIntVector4, ViewRectMin)
		SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2DArray<uint>, OutExtinctionVolume)
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
