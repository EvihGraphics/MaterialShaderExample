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

BEGIN_SHADER_PARAMETER_STRUCT(FAVBOITRasterTargets, )
	RENDER_TARGET_BINDING_SLOTS()
END_SHADER_PARAMETER_STRUCT()

// For testing purposes, we define a CVar
static TAutoConsoleVariable<int32> CVarAVBOITRasterEnable(
	TEXT("r.AVBOIT.Raster.Enable"),
	0,
	TEXT("AVBOIT Raster Enable\n")
	TEXT(" 0: Disabled\n")
	TEXT(" 1: Raster Validation\n"),
	ECVF_RenderThreadSafe
);

bool FAVBOITRasterRenderer::IsEnabled()
{
	return CVarAVBOITRasterEnable.GetValueOnRenderThread() > 0;
}

void FAVBOITRasterRenderer::AddPasses(
	FRDGBuilder& GraphBuilder,
	const FSceneView& View,
	const FPostProcessingInputs& Inputs)
{
	if (!IsEnabled())
	{
		return;
	}

	const TArray<FAVBOITTestMeshSceneProxy*>& Proxies = FAVBOITRasterSceneData::Get().GetProxies();
	if (Proxies.IsEmpty())
	{
		return;
	}

	FAVBOITRasterPassInputs PassInputs;
	PassInputs.TextureExtent = Inputs.SceneTextures->GetParameters()->SceneDepthTexture->Desc.Extent;
	PassInputs.ViewRect = View.UnconstrainedViewRect;
	PassInputs.WorldToView = FMatrix44f(View.ViewMatrices.GetViewMatrix());
	PassInputs.WorldToClip = FMatrix44f(View.ViewMatrices.GetViewProjectionMatrix());
	PassInputs.ZNear = View.NearClippingDistance;
	PassInputs.ZFar = View.NearClippingDistance + 10000.0f;
	PassInputs.SceneDepth = Inputs.SceneTextures->GetParameters()->SceneDepthTexture;
	PassInputs.SceneColor = Inputs.SceneTextures->GetParameters()->SceneColorTexture;
	PassInputs.FragmentCoverageCounter = nullptr;

	PassInputs.DrawData.Reserve(Proxies.Num());
	for (FAVBOITTestMeshSceneProxy* Proxy : Proxies)
	{
		FAVBOITRasterDrawData Data;
		Data.LocalToWorld = Proxy->GetLocalToWorld();
		Data.Color = Proxy->MaterialParams.Color;
		Data.Alpha = Proxy->MaterialParams.Alpha;
		Data.VertexBufferRHI = Proxy->VertexBuffer.VertexBufferRHI;
		Data.IndexBufferRHI = Proxy->IndexBuffer.IndexBufferRHI;
		Data.VertexDeclaration = Proxy->VertexDeclaration;
		PassInputs.DrawData.Add(Data);
	}

	AddCorePasses(GraphBuilder, PassInputs);
}

FAVBOITRasterPassOutputs FAVBOITRasterRenderer::AddCorePasses(
	FRDGBuilder& GraphBuilder,
	const FAVBOITRasterPassInputs& Inputs)
{
	FAVBOITRasterPassOutputs Outputs;

	FRDGTextureRef SceneDepthTexture = Inputs.SceneDepth;
	FRDGTextureRef SceneColorTexture = Inputs.SceneColor;
	
	if (!SceneDepthTexture || !SceneColorTexture)
	{
		return Outputs;
	}

	RDG_EVENT_SCOPE(GraphBuilder, "AVBOIT.Raster");

	// 1. Allocate RDG Textures
	// We allocate matching SceneDepth extent to prevent ViewRectMin offset issues from causing out of bounds
	FIntPoint TextureExtent = SceneDepthTexture->Desc.Extent;

	FRDGBufferDesc ExtinctionDesc = FRDGBufferDesc::CreateStructuredDesc(4, TextureExtent.X * TextureExtent.Y * 64);
	Outputs.ExtinctionVolume = GraphBuilder.CreateBuffer(ExtinctionDesc, TEXT("AVBOIT.Outputs.ExtinctionVolume"));

	FRDGTextureDesc TransmittanceDesc = FRDGTextureDesc::Create2DArray(
		TextureExtent,
		PF_R32_FLOAT,
		FClearValueBinding::None,
		TexCreate_ShaderResource | TexCreate_UAV,
		64);
	Outputs.TransmittanceVolume = GraphBuilder.CreateTexture(TransmittanceDesc, TEXT("AVBOIT.Outputs.TransmittanceVolume"));

	FRDGTextureDesc ColorDesc = FRDGTextureDesc::Create2D(
		TextureExtent,
		PF_FloatRGBA,
		FClearValueBinding::Transparent,
		TexCreate_ShaderResource | TexCreate_RenderTargetable | TexCreate_UAV);
	Outputs.ColorAccumulation = GraphBuilder.CreateTexture(ColorDesc, TEXT("AVBOIT.Outputs.ColorAccumulation"));

	FIntRect ViewRect = Inputs.ViewRect;
	FIntVector4 ViewRectMin(ViewRect.Min.X, ViewRect.Min.Y, 0, 0);

	// Pass 1: Clear
	{
		auto* PassParameters = GraphBuilder.AllocParameters<FAVBOITClearCS::FParameters>();
		PassParameters->ViewResolution = FVector2f(ViewRect.Width(), ViewRect.Height());
		PassParameters->OutExtinctionVolume = GraphBuilder.CreateUAV(Outputs.ExtinctionVolume);
		PassParameters->OutTransmittanceVolume = GraphBuilder.CreateUAV(Outputs.TransmittanceVolume);
		PassParameters->OutResultTexture = GraphBuilder.CreateUAV(Outputs.ColorAccumulation);

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
	}

	// Prepare common states
	FMatrix44f WorldToClip = Inputs.WorldToClip;
	FMatrix44f WorldToView = Inputs.WorldToView;

	float ZNear = Inputs.ZNear;
	float ZFar = Inputs.ZFar; // Typical empirical or from view
	// Pass 2: Splat
	{

		
		TArray<FAVBOITRasterSplatPS::FParameters*> SplatPSParams;
		TArray<FAVBOITRasterSplatVS::FParameters*> SplatVSParams;
		for (const FAVBOITRasterDrawData& Data : Inputs.DrawData)
		{
			auto* PSParams = GraphBuilder.AllocParameters<FAVBOITRasterSplatPS::FParameters>();
			PSParams->ZNear = ZNear;
			PSParams->ZFar = ZFar;
			PSParams->ViewResolution = FVector2f(TextureExtent.X, TextureExtent.Y);
			PSParams->ColorAndAlpha = FVector4f(Data.Color.R, Data.Color.G, Data.Color.B, Data.Alpha);
			PSParams->ViewRectMin = ViewRectMin;
			PSParams->OutExtinctionVolume = GraphBuilder.CreateUAV(Outputs.ExtinctionVolume);
			PSParams->RenderTargets.DepthStencil = FDepthStencilBinding(SceneDepthTexture, ERenderTargetLoadAction::ELoad, FExclusiveDepthStencil::DepthRead_StencilNop);
			SplatPSParams.Add(PSParams);

			auto* VSParams = GraphBuilder.AllocParameters<FAVBOITRasterSplatVS::FParameters>();
			VSParams->LocalToWorld = FMatrix44f(Data.LocalToWorld);
			VSParams->WorldToClip = WorldToClip;
			VSParams->WorldToView = WorldToView;
			VSParams->ViewRectMin = ViewRectMin;
			SplatVSParams.Add(VSParams);
		}

		auto* PassParameters = GraphBuilder.AllocParameters<FAVBOITRasterSplatPS::FParameters>();
		if (SplatPSParams.Num() > 0)
		{
			*PassParameters = *SplatPSParams[0]; // For RDG dependency tracking
		}

		bool bTestCoverage = Inputs.FragmentCoverageCounter != nullptr;
		TArray<FAVBOITRasterDrawData> DrawDataArray = Inputs.DrawData;

		TShaderMapRef<FAVBOITRasterSplatVS> VertexShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
		FAVBOITRasterSplatPS::FPermutationDomain PermutationVector;
		PermutationVector.Set<FAVBOITRasterSplatPS::FTestCoverageDim>(bTestCoverage);
		TShaderMapRef<FAVBOITRasterSplatPS> PixelShader(GetGlobalShaderMap(GMaxRHIFeatureLevel), PermutationVector);

		ClearUnusedGraphResources(PixelShader, PassParameters);

		PassParameters->RenderTargets[0] = FRenderTargetBinding(Outputs.ColorAccumulation, ERenderTargetLoadAction::ELoad);
		PassParameters->RenderTargets.DepthStencil = FDepthStencilBinding(SceneDepthTexture, ERenderTargetLoadAction::ELoad, FExclusiveDepthStencil::DepthRead_StencilNop);

		GraphBuilder.AddPass(
			RDG_EVENT_NAME("AVBOIT.Raster.Splat"),
			PassParameters,
			ERDGPassFlags::Raster,
			[SplatVSParams, SplatPSParams, ViewRect, DrawDataArray, bTestCoverage, VertexShader, PixelShader](FRHICommandList& RHICmdList)
		{
			RHICmdList.SetViewport(ViewRect.Min.X, ViewRect.Min.Y, 0.0f, ViewRect.Max.X, ViewRect.Max.Y, 1.0f);

			FGraphicsPipelineStateInitializer GraphicsPSOInit;
			RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);

			GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
			GraphicsPSOInit.RasterizerState = TStaticRasterizerState<FM_Solid, CM_None>::GetRHI();
			// Depth Test Enabled, Write Disabled. Reverse-Z means GreaterEqual.
			GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_DepthNearOrEqual>::GetRHI();

			for (int32 i = 0; i < DrawDataArray.Num(); ++i)
			{
				GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = DrawDataArray[i].VertexDeclaration;
				GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
				GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
				GraphicsPSOInit.PrimitiveType = PT_TriangleList;

				SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit, 0);

				SetShaderParameters(RHICmdList, VertexShader, VertexShader.GetVertexShader(), *SplatVSParams[i]);
				SetShaderParameters(RHICmdList, PixelShader, PixelShader.GetPixelShader(), *SplatPSParams[i]);

				RHICmdList.SetStreamSource(0, DrawDataArray[i].VertexBufferRHI, 0);
				RHICmdList.DrawIndexedPrimitive(DrawDataArray[i].IndexBufferRHI, 0, 0, 4, 0, 2, 1);
			}
		});
	}

	// Pass 3: Integrate
	{
		auto* PassParameters = GraphBuilder.AllocParameters<FAVBOITIntegrateCS::FParameters>();
		PassParameters->InExtinctionVolume = GraphBuilder.CreateSRV(Outputs.ExtinctionVolume);
		PassParameters->OutTransmittanceVolume = GraphBuilder.CreateUAV(Outputs.TransmittanceVolume);

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
		TArray<FAVBOITRasterForwardPS::FParameters*> ForwardPSParams;
		TArray<FAVBOITRasterForwardVS::FParameters*> ForwardVSParams;
		for (const FAVBOITRasterDrawData& Data : Inputs.DrawData)
		{
			auto* PSParams = GraphBuilder.AllocParameters<FAVBOITRasterForwardPS::FParameters>();
			PSParams->ZNear = ZNear;
			PSParams->ZFar = ZFar;
			PSParams->ColorAndAlpha = FVector4f(Data.Color.R, Data.Color.G, Data.Color.B, Data.Alpha);
			PSParams->ReferenceBrightnessMultiplier = 1.0f; // Fixed parameter
			PSParams->ViewRectMin = ViewRectMin;
			PSParams->TransmittanceVolume = GraphBuilder.CreateSRV(Outputs.TransmittanceVolume);
			PSParams->RenderTargets[0] = FRenderTargetBinding(Outputs.ColorAccumulation, ERenderTargetLoadAction::ELoad);
			PSParams->RenderTargets.DepthStencil = FDepthStencilBinding(SceneDepthTexture, ERenderTargetLoadAction::ELoad, FExclusiveDepthStencil::DepthRead_StencilNop);
			ForwardPSParams.Add(PSParams);

			auto* VSParams = GraphBuilder.AllocParameters<FAVBOITRasterForwardVS::FParameters>();
			VSParams->LocalToWorld = FMatrix44f(Data.LocalToWorld);
			VSParams->WorldToClip = WorldToClip;
			VSParams->WorldToView = WorldToView;
			VSParams->ViewRectMin = ViewRectMin;
			ForwardVSParams.Add(VSParams);
		}

		auto* PassParameters = GraphBuilder.AllocParameters<FAVBOITRasterForwardPS::FParameters>();
		if (ForwardPSParams.Num() > 0)
		{
			*PassParameters = *ForwardPSParams[0]; // For RDG dependency tracking
		}

		TArray<FAVBOITRasterDrawData> DrawDataArray = Inputs.DrawData;
		
		TShaderMapRef<FAVBOITRasterForwardVS> VertexShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
		TShaderMapRef<FAVBOITRasterForwardPS> PixelShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
		
		ClearUnusedGraphResources(PixelShader, PassParameters);

		PassParameters->RenderTargets[0] = FRenderTargetBinding(Outputs.ColorAccumulation, ERenderTargetLoadAction::ELoad);
		PassParameters->RenderTargets.DepthStencil = FDepthStencilBinding(SceneDepthTexture, ERenderTargetLoadAction::ELoad, FExclusiveDepthStencil::DepthRead_StencilNop);

		GraphBuilder.AddPass(
			RDG_EVENT_NAME("AVBOIT.Raster.ForwardShade"),
			PassParameters,
			ERDGPassFlags::Raster,
			[ForwardVSParams, ForwardPSParams, ViewRect, DrawDataArray, VertexShader, PixelShader](FRHICommandList& RHICmdList)
		{
			RHICmdList.SetViewport(ViewRect.Min.X, ViewRect.Min.Y, 0.0f, ViewRect.Max.X, ViewRect.Max.Y, 1.0f);

			FGraphicsPipelineStateInitializer GraphicsPSOInit;
			RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);

			// Additive Blend
			GraphicsPSOInit.BlendState = TStaticBlendState<CW_RGB, BO_Add, BF_One, BF_One, BO_Add, BF_One, BF_One>::GetRHI();
			GraphicsPSOInit.RasterizerState = TStaticRasterizerState<FM_Solid, CM_None>::GetRHI();
			GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_DepthNearOrEqual>::GetRHI();

			for (int32 i = 0; i < DrawDataArray.Num(); ++i)
			{
				GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = DrawDataArray[i].VertexDeclaration;
				GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
				GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
				GraphicsPSOInit.PrimitiveType = PT_TriangleList;

				SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit, 0);

				SetShaderParameters(RHICmdList, VertexShader, VertexShader.GetVertexShader(), *ForwardVSParams[i]);
				SetShaderParameters(RHICmdList, PixelShader, PixelShader.GetPixelShader(), *ForwardPSParams[i]);

				RHICmdList.SetStreamSource(0, DrawDataArray[i].VertexBufferRHI, 0);
				RHICmdList.DrawIndexedPrimitive(DrawDataArray[i].IndexBufferRHI, 0, 0, 4, 0, 2, 1);
			}
		});
	}

	// Pass 5: Composite
	// In-place modification of SceneColor using Hardware Blend
	{
		auto* PassParameters = GraphBuilder.AllocParameters<FAVBOITRasterCompositePS::FParameters>();
		PassParameters->ViewRectMin = ViewRectMin;
		PassParameters->ColorAccumulation = Outputs.ColorAccumulation;
		PassParameters->TransmittanceVolume = GraphBuilder.CreateSRV(Outputs.TransmittanceVolume);
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
		Outputs.CompositeOutput = PassParameters->RenderTargets[0].GetTexture();
	}
	
	return Outputs;
}
