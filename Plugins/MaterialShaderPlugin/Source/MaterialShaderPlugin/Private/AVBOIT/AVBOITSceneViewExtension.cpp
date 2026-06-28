// Copyright Voxel Plugin SAS. All Rights Reserved.

#include "AVBOITSceneViewExtension.h"

#include "AVBOITSmokeSettings.h"

#include "GlobalShader.h"
#include "PostProcess/PostProcessMaterialInputs.h"
#include "RenderGraphBuilder.h"
#include "RenderGraphUtils.h"
#include "SceneRenderTargetParameters.h"
#include "ScreenPass.h"
#include "ShaderParameterStruct.h"

DECLARE_GPU_STAT_NAMED(AVBOITSmoke, TEXT("AVBOITSmoke"));

namespace
{
	class FAVBOITSmokeClearCS : public FGlobalShader
	{
	public:
		DECLARE_GLOBAL_SHADER(FAVBOITSmokeClearCS);
		SHADER_USE_PARAMETER_STRUCT(FAVBOITSmokeClearCS, FGlobalShader);

		BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
			SHADER_PARAMETER(FIntPoint, TextureExtent)
			SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<float4>, OutSmoke)
		END_SHADER_PARAMETER_STRUCT()

		static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
		{
			return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
		}
	};

	class FAVBOITSmokeFillCS : public FGlobalShader
	{
	public:
		DECLARE_GLOBAL_SHADER(FAVBOITSmokeFillCS);
		SHADER_USE_PARAMETER_STRUCT(FAVBOITSmokeFillCS, FGlobalShader);

		BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
			SHADER_PARAMETER_STRUCT_REF(FViewUniformShaderParameters, View)
			SHADER_PARAMETER_STRUCT_INCLUDE(FSceneTextureShaderParameters, SceneTextures)
			SHADER_PARAMETER(FIntPoint, TextureExtent)
			SHADER_PARAMETER(FIntPoint, ViewRectMin)
			SHADER_PARAMETER(FIntPoint, ViewRectSize)
			SHADER_PARAMETER(int32, DebugMode)
			SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<float4>, OutSmoke)
		END_SHADER_PARAMETER_STRUCT()

		static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
		{
			return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
		}
	};

	class FAVBOITSmokeCompositePS : public FGlobalShader
	{
	public:
		DECLARE_GLOBAL_SHADER(FAVBOITSmokeCompositePS);
		SHADER_USE_PARAMETER_STRUCT(FAVBOITSmokeCompositePS, FGlobalShader);

		BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
			SHADER_PARAMETER_STRUCT(FScreenPassTextureInput, SceneColor)
			SHADER_PARAMETER_STRUCT(FScreenPassTextureInput, Smoke)
			SHADER_PARAMETER(FScreenTransform, SvPositionToSceneColorUV)
			SHADER_PARAMETER(FScreenTransform, SvPositionToSmokeUV)
			SHADER_PARAMETER(float, Opacity)
			RENDER_TARGET_BINDING_SLOTS()
		END_SHADER_PARAMETER_STRUCT()

		static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
		{
			return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
		}
	};
}

IMPLEMENT_GLOBAL_SHADER(FAVBOITSmokeClearCS, "/Plugin/MaterialShaderExample/AVBOIT/AVBOITSmoke.usf", "ClearCS", SF_Compute);
IMPLEMENT_GLOBAL_SHADER(FAVBOITSmokeFillCS, "/Plugin/MaterialShaderExample/AVBOIT/AVBOITSmoke.usf", "FillCS", SF_Compute);
IMPLEMENT_GLOBAL_SHADER(FAVBOITSmokeCompositePS, "/Plugin/MaterialShaderExample/AVBOIT/AVBOITSmoke.usf", "CompositePS", SF_Pixel);

void FAVBOITSceneViewExtension::SubscribeToPostProcessingPass(
	EPostProcessingPass Pass,
	const FSceneView& InView,
	FPostProcessingPassDelegateArray& InOutPassCallbacks,
	bool bIsPassEnabled)
{
	if (!bIsPassEnabled || !AVBOIT::Smoke::IsEnabled())
	{
		return;
	}

	if (Pass != EPostProcessingPass::AfterDOF)
	{
		return;
	}

	InOutPassCallbacks.Add(FPostProcessingPassDelegate::CreateRaw(this, &FAVBOITSceneViewExtension::AddSmokePass));
}

#include "AVBOIT/Raster/AVBOITRasterRenderer.h"

void FAVBOITSceneViewExtension::PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessingInputs& Inputs)
{
	if (FAVBOITRasterRenderer::IsEnabled())
	{
		FAVBOITRasterRenderer::AddPasses(GraphBuilder, View, Inputs);
	}
}

FScreenPassTexture FAVBOITSceneViewExtension::AddSmokePass(
	FRDGBuilder& GraphBuilder,
	const FSceneView& View,
	const FPostProcessMaterialInputs& Inputs) const
{
	if (!AVBOIT::Smoke::IsEnabled())
	{
		return Inputs.ReturnUntouchedSceneColorForPostProcessing(GraphBuilder);
	}

	FScreenPassTexture SceneColor = FScreenPassTexture::CopyFromSlice(
		GraphBuilder,
		Inputs.GetInput(EPostProcessMaterialInput::SceneColor));

	if (!SceneColor.IsValid())
	{
		return Inputs.ReturnUntouchedSceneColorForPostProcessing(GraphBuilder);
	}

	RDG_EVENT_SCOPE(GraphBuilder, "AVBOIT.Smoke");
	RDG_GPU_STAT_SCOPE(GraphBuilder, AVBOITSmoke);

	const FIntRect ViewRect = SceneColor.ViewRect;
	const FIntPoint ViewRectSize = ViewRect.Size();
	if (ViewRectSize.X <= 0 || ViewRectSize.Y <= 0)
	{
		return Inputs.ReturnUntouchedSceneColorForPostProcessing(GraphBuilder);
	}

	FRDGTextureDesc SmokeDesc = FRDGTextureDesc::Create2D(
		SceneColor.Texture->Desc.Extent,
		PF_FloatRGBA,
		FClearValueBinding::Transparent,
		TexCreate_ShaderResource | TexCreate_UAV);
	SmokeDesc.Flags |= TexCreate_RenderTargetable;

	FRDGTextureRef SmokeTexture = GraphBuilder.CreateTexture(SmokeDesc, TEXT("AVBOIT.Smoke.Texture"));
	FRDGTextureUAVRef SmokeUAV = GraphBuilder.CreateUAV(SmokeTexture);

	FGlobalShaderMap* GlobalShaderMap = GetGlobalShaderMap(View.FeatureLevel);

	{
		TShaderMapRef<FAVBOITSmokeClearCS> ComputeShader(GlobalShaderMap);
		FAVBOITSmokeClearCS::FParameters* PassParameters = GraphBuilder.AllocParameters<FAVBOITSmokeClearCS::FParameters>();
		PassParameters->TextureExtent = SmokeTexture->Desc.Extent;
		PassParameters->OutSmoke = SmokeUAV;

		FComputeShaderUtils::AddPass(
			GraphBuilder,
			RDG_EVENT_NAME("AVBOIT.Smoke.Clear %dx%d", SmokeTexture->Desc.Extent.X, SmokeTexture->Desc.Extent.Y),
			ComputeShader,
			PassParameters,
			FComputeShaderUtils::GetGroupCount(SmokeTexture->Desc.Extent, 8));
	}

	{
		TShaderMapRef<FAVBOITSmokeFillCS> ComputeShader(GlobalShaderMap);
		FAVBOITSmokeFillCS::FParameters* PassParameters = GraphBuilder.AllocParameters<FAVBOITSmokeFillCS::FParameters>();
		PassParameters->View = View.ViewUniformBuffer;
		PassParameters->SceneTextures = Inputs.SceneTextures;
		PassParameters->TextureExtent = SmokeTexture->Desc.Extent;
		PassParameters->ViewRectMin = ViewRect.Min;
		PassParameters->ViewRectSize = ViewRectSize;
		PassParameters->DebugMode = AVBOIT::Smoke::GetDebugMode();
		PassParameters->OutSmoke = SmokeUAV;

		FComputeShaderUtils::AddPass(
			GraphBuilder,
			RDG_EVENT_NAME("AVBOIT.Smoke.Fill Mode=%d %dx%d", PassParameters->DebugMode, ViewRectSize.X, ViewRectSize.Y),
			ComputeShader,
			PassParameters,
			FComputeShaderUtils::GetGroupCount(ViewRectSize, 8));
	}

	FScreenPassRenderTarget Output = Inputs.OverrideOutput;
	if (!Output.IsValid())
	{
		Output = FScreenPassRenderTarget::CreateFromInput(
			GraphBuilder,
			SceneColor,
			ERenderTargetLoadAction::ENoAction,
			TEXT("AVBOIT.Smoke.Output"));
	}

	FScreenPassTexture Smoke(SmokeTexture, ViewRect);

	TShaderMapRef<FAVBOITSmokeCompositePS> PixelShader(GlobalShaderMap);
	FAVBOITSmokeCompositePS::FParameters* PassParameters = GraphBuilder.AllocParameters<FAVBOITSmokeCompositePS::FParameters>();
	PassParameters->RenderTargets[0] = Output.GetRenderTargetBinding();
	PassParameters->SceneColor = GetScreenPassTextureInput(SceneColor, TStaticSamplerState<SF_Bilinear, AM_Clamp, AM_Clamp, AM_Clamp>::GetRHI());
	PassParameters->Smoke = GetScreenPassTextureInput(Smoke, TStaticSamplerState<SF_Bilinear, AM_Clamp, AM_Clamp, AM_Clamp>::GetRHI());
	PassParameters->SvPositionToSceneColorUV = (
		FScreenTransform::ChangeTextureBasisFromTo(FScreenPassTextureViewport(Output), FScreenTransform::ETextureBasis::TexelPosition, FScreenTransform::ETextureBasis::ViewportUV) *
		FScreenTransform::ChangeTextureBasisFromTo(FScreenPassTextureViewport(SceneColor), FScreenTransform::ETextureBasis::ViewportUV, FScreenTransform::ETextureBasis::TextureUV));
	PassParameters->SvPositionToSmokeUV = (
		FScreenTransform::ChangeTextureBasisFromTo(FScreenPassTextureViewport(Output), FScreenTransform::ETextureBasis::TexelPosition, FScreenTransform::ETextureBasis::ViewportUV) *
		FScreenTransform::ChangeTextureBasisFromTo(FScreenPassTextureViewport(Smoke), FScreenTransform::ETextureBasis::ViewportUV, FScreenTransform::ETextureBasis::TextureUV));
	PassParameters->Opacity = AVBOIT::Smoke::GetOpacity();

	AddDrawScreenPass(
		GraphBuilder,
		RDG_EVENT_NAME("AVBOIT.Smoke.Composite %dx%d", Output.ViewRect.Width(), Output.ViewRect.Height()),
		View,
		FScreenPassTextureViewport(Output),
		FScreenPassTextureViewport(SceneColor),
		PixelShader,
		PassParameters);

	return MoveTemp(Output);
}
