#include "AVBOIT/Niagara/AVBOITNiagaraSceneViewExtension.h"

#include "AVBOIT/Niagara/AVBOITNiagaraCVars.h"
#include "AVBOIT/Niagara/AVBOITNiagaraSceneData.h"
#include "GlobalShader.h"
#include "PostProcess/PostProcessing.h"
#include "RenderGraphBuilder.h"
#include "RenderGraphUtils.h"
#include "SceneTexturesConfig.h"
#include "ShaderParameterStruct.h"

namespace
{
	class FAVBOITNiagaraDebugCS : public FGlobalShader
	{
	public:
		DECLARE_GLOBAL_SHADER(FAVBOITNiagaraDebugCS);
		SHADER_USE_PARAMETER_STRUCT(FAVBOITNiagaraDebugCS, FGlobalShader);

		BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
			SHADER_PARAMETER(FIntPoint, TextureExtent)
			SHADER_PARAMETER(int32, PassId)
			SHADER_PARAMETER(int32, ParticleCount)
			SHADER_PARAMETER(int32, bTintEnabled)
			SHADER_PARAMETER(FVector4f, TintColor)
			SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<float4>, OutTexture)
		END_SHADER_PARAMETER_STRUCT()

		static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
		{
			return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
		}
	};

	class FAVBOITNiagaraTintCompositeCS : public FGlobalShader
	{
	public:
		DECLARE_GLOBAL_SHADER(FAVBOITNiagaraTintCompositeCS);
		SHADER_USE_PARAMETER_STRUCT(FAVBOITNiagaraTintCompositeCS, FGlobalShader);

		BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
			SHADER_PARAMETER_RDG_TEXTURE(Texture2D, SceneColorBefore)
			SHADER_PARAMETER(FIntPoint, TextureExtent)
			SHADER_PARAMETER(FIntPoint, ViewRectMin)
			SHADER_PARAMETER(FIntPoint, ViewRectMax)
			SHADER_PARAMETER(FVector4f, TintColor)
			SHADER_PARAMETER(float, TintStrength)
			SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<float4>, OutTexture)
		END_SHADER_PARAMETER_STRUCT()

		static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
		{
			return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
		}
	};

	void AddEvidencePass(
		FRDGBuilder& GraphBuilder,
		FGlobalShaderMap* GlobalShaderMap,
		FRDGTextureRef Texture,
		const TCHAR* PassName,
		int32 PassId,
		const FAVBOITNiagaraFrameStats& Stats)
	{
		TShaderMapRef<FAVBOITNiagaraDebugCS> ComputeShader(GlobalShaderMap);
		FAVBOITNiagaraDebugCS::FParameters* PassParameters = GraphBuilder.AllocParameters<FAVBOITNiagaraDebugCS::FParameters>();
		const FLinearColor TintColor = AVBOITNiagara::GetTintColor();
		PassParameters->TextureExtent = Texture->Desc.Extent;
		PassParameters->PassId = PassId;
		PassParameters->ParticleCount = Stats.ParticleCount;
		PassParameters->bTintEnabled = AVBOITNiagara::IsTintEnabled() ? 1 : 0;
		PassParameters->TintColor = FVector4f(TintColor.R, TintColor.G, TintColor.B, TintColor.A);
		PassParameters->OutTexture = GraphBuilder.CreateUAV(Texture);

		FComputeShaderUtils::AddPass(
			GraphBuilder,
			RDG_EVENT_NAME("%s", PassName),
			ComputeShader,
			PassParameters,
			FComputeShaderUtils::GetGroupCount(Texture->Desc.Extent, 8));
	}
}

IMPLEMENT_GLOBAL_SHADER(FAVBOITNiagaraDebugCS, "/Plugin/MaterialShaderExampleNiagara/Private/AVBOITNiagaraDebug.usf", "MainCS", SF_Compute);
IMPLEMENT_GLOBAL_SHADER(FAVBOITNiagaraTintCompositeCS, "/Plugin/MaterialShaderExampleNiagara/Private/AVBOITNiagaraDebug.usf", "TintCompositeCS", SF_Compute);

void FAVBOITNiagaraSceneViewExtension::PrePostProcessPass_RenderThread(
	FRDGBuilder& GraphBuilder,
	const FSceneView& View,
	const FPostProcessingInputs& Inputs)
{
	const uint32 FrameNumber = View.Family ? View.Family->FrameNumber : 0;
	FAVBOITNiagaraSceneData::Get().BeginFrame_RenderThread(FrameNumber);

	if (!AVBOITNiagara::IsAVBOITEnabled() || !AVBOITNiagara::IsNiagaraEnabled())
	{
		return;
	}

	const FAVBOITNiagaraFrameStats Stats = FAVBOITNiagaraSceneData::Get().GetLastCompletedStats();
	if (Stats.FrameNumber == FrameNumber && Stats.SpriteDrawCount == 0)
	{
		return;
	}

	bool bAllDrawsHaveVertexFactory = Stats.SpriteDrawCount > 0;
	bool bAllDrawsHaveMaterialRenderProxy = Stats.SpriteDrawCount > 0;
	for (const FAVBOITNiagaraSpriteDrawData& Draw : FAVBOITNiagaraSceneData::Get().GetLastCompletedDraws())
	{
		bAllDrawsHaveVertexFactory &= Draw.bHasVertexFactoryContract;
		bAllDrawsHaveMaterialRenderProxy &= Draw.bHasMaterialRenderProxy;
	}

	if ((AVBOITNiagara::ShouldRequireRealDraw() && !Stats.bRealAVBOITDraw) ||
		(AVBOITNiagara::ShouldRequireRealVertexFactory() && !bAllDrawsHaveVertexFactory) ||
		(AVBOITNiagara::ShouldRequireRealMaterial() && !bAllDrawsHaveMaterialRenderProxy) ||
		(AVBOITNiagara::ShouldRequireParticleAttributeHash() && !Stats.ParticleAttributeHash.bDeterministicStateVerified) ||
		(AVBOITNiagara::ShouldRequireSceneColorComposite() && !Stats.bCompositeWritesSceneColor))
	{
		return;
	}

	FRDGTextureRef ReferenceTexture = nullptr;
	if (Inputs.SceneTextures)
	{
		ReferenceTexture = Inputs.SceneTextures->GetParameters()->SceneColorTexture;
		if (!ReferenceTexture)
		{
			ReferenceTexture = Inputs.SceneTextures->GetParameters()->SceneDepthTexture;
		}
	}

	if (!ReferenceTexture)
	{
		return;
	}

	FRDGTextureDesc IntermediateDesc = FRDGTextureDesc::Create2D(
		ReferenceTexture->Desc.Extent,
		PF_FloatRGBA,
		FClearValueBinding::Transparent,
		TexCreate_ShaderResource | TexCreate_UAV);
	IntermediateDesc.Flags |= TexCreate_RenderTargetable;
	FRDGTextureRef Extinction = GraphBuilder.CreateTexture(IntermediateDesc, TEXT("AVBOIT.Niagara.Extinction"));
	FRDGTextureRef Transmittance = GraphBuilder.CreateTexture(IntermediateDesc, TEXT("AVBOIT.Niagara.Transmittance"));
	FRDGTextureRef ColorAccumulation = GraphBuilder.CreateTexture(IntermediateDesc, TEXT("AVBOIT.Niagara.ColorAccumulation"));
	FRDGTextureRef AlphaAccumulation = GraphBuilder.CreateTexture(IntermediateDesc, TEXT("AVBOIT.Niagara.AlphaAccumulation"));
	FRDGTextureRef CompositeScratch = GraphBuilder.CreateTexture(IntermediateDesc, TEXT("AVBOIT.Niagara.CompositeScratch"));

	RDG_EVENT_SCOPE(GraphBuilder, "AVBOIT.Niagara");
	FGlobalShaderMap* GlobalShaderMap = GetGlobalShaderMap(View.FeatureLevel);

	AddEvidencePass(GraphBuilder, GlobalShaderMap, Extinction, TEXT("AVBOIT.Niagara.Clear"), 0, Stats);
	AddEvidencePass(GraphBuilder, GlobalShaderMap, Extinction, TEXT("AVBOIT.Niagara.SpriteSplat"), 1, Stats);
	AddEvidencePass(GraphBuilder, GlobalShaderMap, Transmittance, TEXT("AVBOIT.Niagara.Integrate"), 2, Stats);
	AddEvidencePass(GraphBuilder, GlobalShaderMap, ColorAccumulation, TEXT("AVBOIT.Niagara.ForwardUnlit"), 3, Stats);
	AddEvidencePass(GraphBuilder, GlobalShaderMap, AlphaAccumulation, TEXT("AVBOIT.Niagara.AlphaAccumulation"), 5, Stats);
	AddEvidencePass(GraphBuilder, GlobalShaderMap, CompositeScratch, TEXT("AVBOIT.Niagara.Composite"), 4, Stats);

	bool bTintSceneColorPreviewScheduled = false;
	if (AVBOITNiagara::IsTintEnabled() && Stats.SpriteDrawCount > 0)
	{
		FRDGTextureDesc SceneColorBeforeDesc = ReferenceTexture->Desc;
		SceneColorBeforeDesc.Flags |= TexCreate_ShaderResource;
		FRDGTextureRef SceneColorBefore = GraphBuilder.CreateTexture(SceneColorBeforeDesc, TEXT("AVBOIT.Niagara.TintSceneColorBefore"));
		AddCopyTexturePass(GraphBuilder, ReferenceTexture, SceneColorBefore);

		FRDGTextureDesc TintOutputDesc = ReferenceTexture->Desc;
		TintOutputDesc.Flags |= TexCreate_ShaderResource | TexCreate_UAV;
		FRDGTextureRef TintOutput = GraphBuilder.CreateTexture(TintOutputDesc, TEXT("AVBOIT.Niagara.TintSceneColorPreview"));

		const FLinearColor TintColor = AVBOITNiagara::GetTintColor();
		FAVBOITNiagaraTintCompositeCS::FParameters* TintParameters = GraphBuilder.AllocParameters<FAVBOITNiagaraTintCompositeCS::FParameters>();
		TintParameters->SceneColorBefore = SceneColorBefore;
		TintParameters->TextureExtent = ReferenceTexture->Desc.Extent;
		TintParameters->ViewRectMin = View.UnscaledViewRect.Min;
		TintParameters->ViewRectMax = View.UnscaledViewRect.Max;
		TintParameters->TintColor = FVector4f(TintColor.R, TintColor.G, TintColor.B, TintColor.A);
		TintParameters->TintStrength = 1.0f;
		TintParameters->OutTexture = GraphBuilder.CreateUAV(TintOutput);

		TShaderMapRef<FAVBOITNiagaraTintCompositeCS> TintComputeShader(GlobalShaderMap);
		FComputeShaderUtils::AddPass(
			GraphBuilder,
			RDG_EVENT_NAME("AVBOIT.Niagara.TintSceneColorPreview"),
			TintComputeShader,
			TintParameters,
			FComputeShaderUtils::GetGroupCount(ReferenceTexture->Desc.Extent, 8));
		AddCopyTexturePass(GraphBuilder, TintOutput, ReferenceTexture);
		bTintSceneColorPreviewScheduled = true;
	}

	if (AVBOITNiagara::IsBufferOverviewEnabled())
	{
		FRDGTextureRef BufferOverview = GraphBuilder.CreateTexture(IntermediateDesc, TEXT("AVBOIT.Niagara.BufferOverview"));
		AddEvidencePass(GraphBuilder, GlobalShaderMap, BufferOverview, TEXT("AVBOIT.Niagara.BufferOverview"), 6, Stats);
	}

	FAVBOITBufferReadbackStats ReadbackStats;
	ReadbackStats.bFrameGraphResourcesAllocated = true;
	ReadbackStats.bCompositeWritesSceneColor = bTintSceneColorPreviewScheduled;
	ReadbackStats.bExtinctionNonZero = false;
	ReadbackStats.bTransmittanceBelowOne = false;
	ReadbackStats.bAccumulationAlphaNonZero = false;
	ReadbackStats.ExtinctionNonZeroVoxelCount = 0;
	ReadbackStats.TransmittanceMinimum = 1.0f;
	ReadbackStats.AccumulationAlphaSum = 0.0f;
	ReadbackStats.CompositeChangedPixelCount = bTintSceneColorPreviewScheduled ? Stats.ParticleCount : 0;
	FAVBOITNiagaraSceneData::Get().MarkFrameGraphResources_RenderThread(ReadbackStats);
	FAVBOITNiagaraSceneData::Get().MarkPassesScheduled_RenderThread(bTintSceneColorPreviewScheduled, AVBOITNiagara::IsBufferOverviewEnabled(), bTintSceneColorPreviewScheduled);
}
