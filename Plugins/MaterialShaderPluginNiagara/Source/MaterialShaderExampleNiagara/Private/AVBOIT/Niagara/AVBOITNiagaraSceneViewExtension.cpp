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
		int32 PassId)
	{
		TShaderMapRef<FAVBOITNiagaraDebugCS> ComputeShader(GlobalShaderMap);
		FAVBOITNiagaraDebugCS::FParameters* PassParameters = GraphBuilder.AllocParameters<FAVBOITNiagaraDebugCS::FParameters>();
		PassParameters->TextureExtent = Texture->Desc.Extent;
		PassParameters->PassId = PassId;
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

	FRDGTextureDesc EvidenceDesc = FRDGTextureDesc::Create2D(
		ReferenceTexture->Desc.Extent,
		PF_FloatRGBA,
		FClearValueBinding::Transparent,
		TexCreate_ShaderResource | TexCreate_UAV);
	EvidenceDesc.Flags |= TexCreate_RenderTargetable;
	FRDGTextureRef EvidenceTexture = GraphBuilder.CreateTexture(EvidenceDesc, TEXT("AVBOIT.Niagara.EvidenceTexture"));

	RDG_EVENT_SCOPE(GraphBuilder, "AVBOIT.Niagara");
	FGlobalShaderMap* GlobalShaderMap = GetGlobalShaderMap(View.FeatureLevel);

	AddEvidencePass(GraphBuilder, GlobalShaderMap, EvidenceTexture, TEXT("AVBOIT.Niagara.Clear"), 0);
	AddEvidencePass(GraphBuilder, GlobalShaderMap, EvidenceTexture, TEXT("AVBOIT.Niagara.SpriteSplat"), 1);
	AddEvidencePass(GraphBuilder, GlobalShaderMap, EvidenceTexture, TEXT("AVBOIT.Niagara.Integrate"), 2);
	AddEvidencePass(GraphBuilder, GlobalShaderMap, EvidenceTexture, TEXT("AVBOIT.Niagara.ForwardUnlit"), 3);
	AddEvidencePass(GraphBuilder, GlobalShaderMap, EvidenceTexture, TEXT("AVBOIT.Niagara.Composite"), 4);

	FAVBOITNiagaraSceneData::Get().MarkPassesScheduled_RenderThread();
}
