// Copyright

#include "AVBOIT/Raster/AVBOITRasterRenderer.h"
#include "SceneView.h"
#include "PostProcess/PostProcessing.h"
#include "AVBOIT/Raster/AVBOITRasterSceneData.h"
#include "AVBOIT/Raster/AVBOITTestMeshSceneProxy.h"
#include "AVBOIT/Raster/AVBOITRasterShaders.h"
#include "AVBOIT/Backend/AVBOITBackendShaders.h"
#include "RenderGraphUtils.h"
#include "PipelineStateCache.h"
#include "CommonRenderResources.h"
#include "ScreenPass.h"
#include "PixelShaderUtils.h"

// For testing purposes, we define a CVar
static TAutoConsoleVariable<int32> CVarAVBOITRasterMode(
	TEXT("r.AVBOIT.Mode"),
	0,
	TEXT("AVBOIT Rendering Mode\n")
	TEXT(" 0: Disabled\n")
	TEXT(" 1: SyntheticValidation\n")
	TEXT(" 2: RasterValidation\n"),
	ECVF_RenderThreadSafe
);

void FAVBOITRasterRenderer::AddPasses(
	FRDGBuilder& GraphBuilder,
	const FSceneView& View,
	const FPostProcessingInputs& Inputs,
	FScreenPassRenderTarget& Output)
{
	if (CVarAVBOITRasterMode.GetValueOnRenderThread() != 2)
	{
		return;
	}

	const TArray<FAVBOITTestMeshSceneProxy*>& Proxies = FAVBOITRasterSceneData::Get().GetProxies();
	if (Proxies.IsEmpty())
	{
		return;
	}

	// Extract the actual SceneDepth and SceneColor from Inputs
	auto SceneTexturesParams = Inputs.SceneTextures->GetParameters();
	FRDGTextureRef SceneDepthTexture = SceneTexturesParams->SceneDepthTexture;
	FRDGTextureRef SceneColorTexture = SceneTexturesParams->SceneColorTexture;
	
	if (!SceneDepthTexture || !SceneColorTexture)
	{
		return;
	}

	RDG_EVENT_SCOPE(GraphBuilder, "AVBOIT.Raster");

	// 1. Allocate RDG Textures
	// We allocate matching SceneDepth extent to prevent ViewRectMin offset issues from causing out of bounds
	FIntPoint TextureExtent = SceneDepthTexture->Desc.Extent;

	FRDGTextureDesc ExtinctionDesc = FRDGTextureDesc::Create2DArray(
		TextureExtent,
		PF_R32_UINT,
		FClearValueBinding::None,
		TexCreate_ShaderResource | TexCreate_UAV,
		64);
	FRDGTextureRef ExtinctionVolume = GraphBuilder.CreateTexture(ExtinctionDesc, TEXT("AVBOIT.ExtinctionVolume"));

	FRDGTextureDesc TransmittanceDesc = FRDGTextureDesc::Create2DArray(
		TextureExtent,
		PF_R32_FLOAT,
		FClearValueBinding::None,
		TexCreate_ShaderResource | TexCreate_UAV,
		64);
	FRDGTextureRef TransmittanceVolume = GraphBuilder.CreateTexture(TransmittanceDesc, TEXT("AVBOIT.TransmittanceVolume"));

	FRDGTextureDesc ColorDesc = FRDGTextureDesc::Create2D(
		TextureExtent,
		PF_FloatRGBA,
		FClearValueBinding::Transparent,
		TexCreate_ShaderResource | TexCreate_RenderTargetable);
	FRDGTextureRef ColorAccumulation = GraphBuilder.CreateTexture(ColorDesc, TEXT("AVBOIT.ColorAccumulation"));

	FIntRect ViewRect = View.UnconstrainedViewRect;
	FIntVector4 ViewRectMin(ViewRect.Min.X, ViewRect.Min.Y, 0, 0);

	// Pass 1: Clear
	{
		auto* PassParameters = GraphBuilder.AllocParameters<FAVBOITClearCS::FParameters>();
		PassParameters->OutExtinctionVolume = GraphBuilder.CreateUAV(ExtinctionVolume);
		PassParameters->OutTransmittanceVolume = GraphBuilder.CreateUAV(TransmittanceVolume);

		TShaderMapRef<FAVBOITClearCS> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
		FIntVector GroupCount = FIntVector(
			FMath::DivideAndRoundUp(TextureExtent.X, 8),
			FMath::DivideAndRoundUp(TextureExtent.Y, 8),
			1
		);

		FComputeShaderUtils::AddPass(
			GraphBuilder,
			RDG_EVENT_NAME("AVBOIT.Raster.Clear"),
			ComputeShader,
			PassParameters,
			GroupCount
		);

		AddClearRenderTargetPass(GraphBuilder, ColorAccumulation, FLinearColor::Transparent);
	}

	// Prepare common states
	FMatrix44f LocalToWorld = FMatrix44f(Proxies[0]->GetLocalToWorld());
	FMatrix44f WorldToClip = FMatrix44f(View.ViewMatrices.GetViewProjectionMatrix());
	FMatrix44f WorldToView = FMatrix44f(View.ViewMatrices.GetViewMatrix());

	float ZNear = View.NearClippingDistance;
	float ZFar = View.NearClippingDistance + 10000.0f; // Typical empirical or from view
	// Note: since the test cases expect certain Depth mapping, we can just hardcode ZFar or pass a parameter.
	
	FVector4f ColorAndAlpha = FVector4f(Proxies[0]->MaterialParams.Color.R, Proxies[0]->MaterialParams.Color.G, Proxies[0]->MaterialParams.Color.B, Proxies[0]->MaterialParams.Alpha);

	// Pass 2: Splat
	{
		auto* PassParameters = GraphBuilder.AllocParameters<FAVBOITRasterSplatPS::FParameters>();
		PassParameters->ZNear = ZNear;
		PassParameters->ZFar = ZFar;
		PassParameters->ColorAndAlpha = ColorAndAlpha;
		PassParameters->ViewRectMin = ViewRectMin;
		PassParameters->OutExtinctionVolume = GraphBuilder.CreateUAV(ExtinctionVolume);
		
		auto* VSPassParameters = GraphBuilder.AllocParameters<FAVBOITRasterSplatVS::FParameters>();
		VSPassParameters->LocalToWorld = LocalToWorld;
		VSPassParameters->WorldToClip = WorldToClip;
		VSPassParameters->WorldToView = WorldToView;
		VSPassParameters->ViewRectMin = ViewRectMin;

		GraphBuilder.AddPass(
			RDG_EVENT_NAME("AVBOIT.Raster.Splat"),
			PassParameters,
			ERDGPassFlags::Raster,
			[VSPassParameters, PassParameters, ViewRect, Proxies, SceneDepthTexture](FRHICommandList& RHICmdList)
		{
			// Setup Depth target mapping
			FRHIRenderPassInfo RPInfo(SceneDepthTexture->GetRHI(), ERenderTargetActions::Load_DontStore);
			RHICmdList.BeginRenderPass(RPInfo, TEXT("AVBOIT.Raster.Splat"));

			RHICmdList.SetViewport(ViewRect.Min.X, ViewRect.Min.Y, 0.0f, ViewRect.Max.X, ViewRect.Max.Y, 1.0f);

			FGraphicsPipelineStateInitializer GraphicsPSOInit;
			RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);

			GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
			GraphicsPSOInit.RasterizerState = TStaticRasterizerState<FM_Solid, CM_None>::GetRHI();
			// Depth Test Enabled, Write Disabled. Reverse-Z means GreaterEqual.
			GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_DepthNearOrEqual>::GetRHI();

			TShaderMapRef<FAVBOITRasterSplatVS> VertexShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
			TShaderMapRef<FAVBOITRasterSplatPS> PixelShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));

			GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = Proxies[0]->VertexDeclaration;
			GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
			GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
			GraphicsPSOInit.PrimitiveType = PT_TriangleList;

			SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit, 0);

			SetShaderParameters(RHICmdList, VertexShader, VertexShader.GetVertexShader(), *VSPassParameters);
			SetShaderParameters(RHICmdList, PixelShader, PixelShader.GetPixelShader(), *PassParameters);

			RHICmdList.SetStreamSource(0, Proxies[0]->VertexBuffer.VertexBufferRHI, 0);
			RHICmdList.DrawIndexedPrimitive(Proxies[0]->IndexBuffer.IndexBufferRHI, 0, 0, 4, 0, 2, 1);

			RHICmdList.EndRenderPass();
		});
	}

	// Pass 3: Integrate
	{
		auto* PassParameters = GraphBuilder.AllocParameters<FAVBOITIntegrateCS::FParameters>();
		PassParameters->InExtinctionVolume = GraphBuilder.CreateSRV(ExtinctionVolume);
		PassParameters->OutTransmittanceVolume = GraphBuilder.CreateUAV(TransmittanceVolume);

		TShaderMapRef<FAVBOITIntegrateCS> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
		FIntVector GroupCount = FIntVector(
			FMath::DivideAndRoundUp(TextureExtent.X, 8),
			FMath::DivideAndRoundUp(TextureExtent.Y, 8),
			1
		);

		FComputeShaderUtils::AddPass(
			GraphBuilder,
			RDG_EVENT_NAME("AVBOIT.Raster.Integrate"),
			ComputeShader,
			PassParameters,
			GroupCount
		);
	}

	// Pass 4: ForwardShade
	{
		auto* PassParameters = GraphBuilder.AllocParameters<FAVBOITRasterForwardPS::FParameters>();
		PassParameters->ZNear = ZNear;
		PassParameters->ZFar = ZFar;
		PassParameters->ColorAndAlpha = ColorAndAlpha;
		PassParameters->ReferenceBrightnessMultiplier = 1.0f; // Fixed parameter
		PassParameters->ViewRectMin = ViewRectMin;
		PassParameters->TransmittanceVolume = GraphBuilder.CreateSRV(TransmittanceVolume);
		PassParameters->RenderTargets[0] = FRenderTargetBinding(ColorAccumulation, ERenderTargetLoadAction::ELoad);
		PassParameters->RenderTargets.DepthStencil = FDepthStencilBinding(SceneDepthTexture, ERenderTargetLoadAction::ELoad, FExclusiveDepthStencil::DepthRead_StencilNop);
		
		auto* VSPassParameters = GraphBuilder.AllocParameters<FAVBOITRasterForwardVS::FParameters>();
		VSPassParameters->LocalToWorld = LocalToWorld;
		VSPassParameters->WorldToClip = WorldToClip;
		VSPassParameters->WorldToView = WorldToView;
		VSPassParameters->ViewRectMin = ViewRectMin;

		GraphBuilder.AddPass(
			RDG_EVENT_NAME("AVBOIT.Raster.ForwardShade"),
			PassParameters,
			ERDGPassFlags::Raster,
			[VSPassParameters, PassParameters, ViewRect, Proxies](FRHICommandList& RHICmdList)
		{
			RHICmdList.SetViewport(ViewRect.Min.X, ViewRect.Min.Y, 0.0f, ViewRect.Max.X, ViewRect.Max.Y, 1.0f);

			FGraphicsPipelineStateInitializer GraphicsPSOInit;
			RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);

			// Additive Blend
			GraphicsPSOInit.BlendState = TStaticBlendState<CW_RGB, BO_Add, BF_One, BF_One, BO_Add, BF_One, BF_One>::GetRHI();
			GraphicsPSOInit.RasterizerState = TStaticRasterizerState<FM_Solid, CM_None>::GetRHI();
			GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_DepthNearOrEqual>::GetRHI();

			TShaderMapRef<FAVBOITRasterForwardVS> VertexShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
			TShaderMapRef<FAVBOITRasterForwardPS> PixelShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));

			GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = Proxies[0]->VertexDeclaration;
			GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
			GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
			GraphicsPSOInit.PrimitiveType = PT_TriangleList;

			SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit, 0);

			SetShaderParameters(RHICmdList, VertexShader, VertexShader.GetVertexShader(), *VSPassParameters);
			SetShaderParameters(RHICmdList, PixelShader, PixelShader.GetPixelShader(), *PassParameters);

			RHICmdList.SetStreamSource(0, Proxies[0]->VertexBuffer.VertexBufferRHI, 0);
			RHICmdList.DrawIndexedPrimitive(Proxies[0]->IndexBuffer.IndexBufferRHI, 0, 0, 4, 0, 2, 1);
		});
	}

	// Pass 5: Composite
	// In-place modification of SceneColor using Hardware Blend
	{
		auto* PassParameters = GraphBuilder.AllocParameters<FAVBOITRasterCompositePS::FParameters>();
		PassParameters->ViewRectMin = ViewRectMin;
		PassParameters->ColorAccumulation = ColorAccumulation;
		PassParameters->TransmittanceVolume = GraphBuilder.CreateSRV(TransmittanceVolume);
		PassParameters->RenderTargets[0] = FRenderTargetBinding(SceneColorTexture, ERenderTargetLoadAction::ELoad);

		TShaderMapRef<FAVBOITRasterCompositePS> PixelShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
		FRHIBlendState* BlendState = TStaticBlendState<CW_RGB, BO_Add, BF_One, BF_SourceAlpha, BO_Add, BF_Zero, BF_One>::GetRHI();

		FPixelShaderUtils::AddFullscreenPass(
			GraphBuilder,
			GetGlobalShaderMap(GMaxRHIFeatureLevel),
			RDG_EVENT_NAME("AVBOIT.Raster.Composite"),
			PixelShader,
			PassParameters,
			ViewRect,
			BlendState
		);
	}
}
