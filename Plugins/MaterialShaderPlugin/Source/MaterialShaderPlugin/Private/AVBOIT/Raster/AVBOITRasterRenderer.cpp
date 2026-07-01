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
#include "HAL/IConsoleManager.h"

BEGIN_SHADER_PARAMETER_STRUCT(FAVBOITRasterTargets, )
	RENDER_TARGET_BINDING_SLOTS()
END_SHADER_PARAMETER_STRUCT()

namespace AVBOIT::Foundation
{
	static TAutoConsoleVariable<int32> CVarEnable(
		TEXT("r.AVBOIT.Foundation.Enable"),
		0,
		TEXT("Enable AVBOIT Foundation raster validation path."),
		ECVF_RenderThreadSafe);

	static TAutoConsoleVariable<int32> CVarMode(
		TEXT("r.AVBOIT.Foundation.Mode"),
		3,
		TEXT("0=EngineDefault 1=UESortedPixelsOIT 2=PluginIdentity 3=PluginAVBOIT 4=ExactReference 5=BufferOverview"),
		ECVF_RenderThreadSafe);

	static TAutoConsoleVariable<int32> CVarScene(
		TEXT("r.AVBOIT.Foundation.Scene"),
		1,
		TEXT("0=SingleLayerIdentity 1=TwoIntersectingQuads 2=ThreeLayerStress"),
		ECVF_RenderThreadSafe);

	static TAutoConsoleVariable<int32> CVarSubmissionOrder(
		TEXT("r.AVBOIT.Foundation.SubmissionOrder"),
		0,
		TEXT("0=AB 1=BA 2=ABC 3=CBA 4/5/6=RandomSeed1/2/3"),
		ECVF_RenderThreadSafe);

	static TAutoConsoleVariable<int32> CVarRandomSeed(
		TEXT("r.AVBOIT.Foundation.RandomSeed"),
		1,
		TEXT("Random seed used by Foundation submission-order tests."),
		ECVF_RenderThreadSafe);

	static TAutoConsoleVariable<int32> CVarDebugPixelX(
		TEXT("r.AVBOIT.Foundation.DebugPixelX"),
		-1,
		TEXT("Foundation debug readback pixel X."),
		ECVF_RenderThreadSafe);

	static TAutoConsoleVariable<int32> CVarDebugPixelY(
		TEXT("r.AVBOIT.Foundation.DebugPixelY"),
		-1,
		TEXT("Foundation debug readback pixel Y."),
		ECVF_RenderThreadSafe);

	static TAutoConsoleVariable<int32> CVarDownsampleFactor(
		TEXT("r.AVBOIT.Foundation.DownsampleFactor"),
		8,
		TEXT("Foundation low-resolution AVBOIT XY downsample factor."),
		ECVF_RenderThreadSafe);

	static TAutoConsoleVariable<int32> CVarNumSlices(
		TEXT("r.AVBOIT.Foundation.NumSlices"),
		64,
		TEXT("Foundation AVBOIT depth slice count. Clamped to 1..64."),
		ECVF_RenderThreadSafe);

	static TAutoConsoleVariable<float> CVarNearDepthCm(
		TEXT("r.AVBOIT.Foundation.NearDepthCm"),
		100.0f,
		TEXT("Foundation positive linear near depth in centimeters."),
		ECVF_RenderThreadSafe);

	static TAutoConsoleVariable<float> CVarFarDepthCm(
		TEXT("r.AVBOIT.Foundation.FarDepthCm"),
		400000.0f,
		TEXT("Foundation positive linear far depth in centimeters."),
		ECVF_RenderThreadSafe);

	static TAutoConsoleVariable<int32> CVarRasterEnableAlias(
		TEXT("r.AVBOIT.Raster.Enable"),
		0,
		TEXT("Deprecated alias for r.AVBOIT.Foundation.Enable."),
		ECVF_RenderThreadSafe);
}

#if WITH_EDITOR || WITH_DEV_AUTOMATION_TESTS
FAVBOITRasterExecutionProbe* GAVBOITRasterProbe = nullptr;
#endif

bool FAVBOITRasterRenderer::IsEnabled()
{
	return AVBOIT::Foundation::CVarEnable.GetValueOnRenderThread() > 0
		|| AVBOIT::Foundation::CVarRasterEnableAlias.GetValueOnRenderThread() > 0;
}

void FAVBOITRasterRenderer::AddPasses(
	FRDGBuilder& GraphBuilder,
	const FSceneView& View,
	const FPostProcessingInputs& Inputs)
{
#if WITH_EDITOR || WITH_DEV_AUTOMATION_TESTS
	if (GAVBOITRasterProbe)
	{
		GAVBOITRasterProbe->ObservedViewCount++;
		// Filter view: Must be Level Editor Viewport or PIE Viewport (exclude thumbnails, captures, etc.)
		bool bIsMainView = (View.bIsGameView || View.bIsViewInfo) && !View.bIsSceneCapture && !View.bIsReflectionCapture && !View.bIsPlanarReflection;
		if (!bIsMainView)
		{
			GAVBOITRasterProbe->RejectedViewCount++;
			return; // Don't run in thumbnails
		}
		
		GAVBOITRasterProbe->AcceptedViewCount++;
		GAVBOITRasterProbe->AcceptedViewRect = View.UnconstrainedViewRect;
		
		UWorld* World = View.Family ? View.Family->Scene->GetWorld() : nullptr;
		if (World)
		{
			GAVBOITRasterProbe->WorldName = World->GetName();
			GAVBOITRasterProbe->Context = (World->WorldType == EWorldType::PIE) ? TEXT("PIE") : TEXT("Editor");
		}
		
		GAVBOITRasterProbe->FrameNumber = View.Family ? View.Family->FrameNumber : 0;
		GAVBOITRasterProbe->ViewRect = View.UnconstrainedViewRect;
		GAVBOITRasterProbe->TextureExtent = Inputs.SceneTextures->GetParameters()->SceneDepthTexture->Desc.Extent;
		GAVBOITRasterProbe->bRasterEnabled = IsEnabled();
	}
#endif

	if (!IsEnabled())
	{
#if WITH_EDITOR || WITH_DEV_AUTOMATION_TESTS
		if (GAVBOITRasterProbe && GAVBOITRasterProbe->SkipReason == EAVBOITRasterSkipReason::Disabled)
		{
			GAVBOITRasterProbe->SkipReason = EAVBOITRasterSkipReason::Disabled;
		}
#endif
		return;
	}

	const TArray<FAVBOITTestMeshSceneProxy*>& Proxies = FAVBOITRasterSceneData::Get().GetProxies();
	
#if WITH_EDITOR || WITH_DEV_AUTOMATION_TESTS
	if (GAVBOITRasterProbe)
	{
		GAVBOITRasterProbe->RegistryProxyCount = Proxies.Num();
		GAVBOITRasterProbe->AcceptedProxyCount = Proxies.Num();
	}
#endif

	if (Proxies.IsEmpty())
	{
#if WITH_EDITOR || WITH_DEV_AUTOMATION_TESTS
		if (GAVBOITRasterProbe) GAVBOITRasterProbe->SkipReason = EAVBOITRasterSkipReason::NoProxies;
#endif
		return;
	}

	FAVBOITRasterPassInputs PassInputs;
	PassInputs.TextureExtent = Inputs.SceneTextures->GetParameters()->SceneDepthTexture->Desc.Extent;
	PassInputs.ViewRect = View.UnconstrainedViewRect;
	PassInputs.Config = FAVBOITFrameConfig::Build(
		PassInputs.ViewRect,
		PassInputs.TextureExtent,
		AVBOIT::Foundation::CVarNearDepthCm.GetValueOnRenderThread(),
		AVBOIT::Foundation::CVarFarDepthCm.GetValueOnRenderThread(),
		static_cast<uint32>(FMath::Max(AVBOIT::Foundation::CVarDownsampleFactor.GetValueOnRenderThread(), 1)),
		static_cast<uint32>(FMath::Clamp(AVBOIT::Foundation::CVarNumSlices.GetValueOnRenderThread(), 1, 64)));
	PassInputs.WorldToView = FMatrix44f(View.ViewMatrices.GetViewMatrix());
	PassInputs.WorldToClip = FMatrix44f(View.ViewMatrices.GetViewProjectionMatrix());
	PassInputs.ZNear = PassInputs.Config.NearDepthCm;
	PassInputs.ZFar = PassInputs.Config.FarDepthCm;
	PassInputs.SceneDepth = Inputs.SceneTextures->GetParameters()->SceneDepthTexture;
	PassInputs.SceneColor = Inputs.SceneTextures->GetParameters()->SceneColorTexture;
	PassInputs.FragmentCoverageCounter = nullptr;
	PassInputs.RasterDebugPixelBuffer = nullptr;

#if WITH_EDITOR || WITH_DEV_AUTOMATION_TESTS
	if (GAVBOITRasterProbe && GAVBOITRasterProbe->RequestedDebugPixel.X >= 0)
	{
		PassInputs.DebugPixel = GAVBOITRasterProbe->RequestedDebugPixel;
		PassInputs.FragmentCoverageCounter = GraphBuilder.CreateBuffer(FRDGBufferDesc::CreateBufferDesc(4, 1), TEXT("AVBOIT.FragmentCoverageCounter"));
		PassInputs.RasterDebugPixelBuffer = GraphBuilder.CreateBuffer(FRDGBufferDesc::CreateStructuredDesc(sizeof(FAVBOITRasterDebugPixelData), 1), TEXT("AVBOIT.RasterDebugPixelBuffer"));
	}
#endif

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
		Data.SubmissionOrder = static_cast<uint32>(FMath::Max(Proxy->SubmissionOrder, 0));
		Data.VertexCount = 4;
		Data.IndexCount = 6;
		Data.PrimitiveCount = 2;
		PassInputs.DrawData.Add(Data);
	}
	PassInputs.DrawData.Sort([](const FAVBOITRasterDrawData& A, const FAVBOITRasterDrawData& B)
	{
		return A.SubmissionOrder < B.SubmissionOrder;
	});

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

	RDG_EVENT_SCOPE(GraphBuilder, "AVBOIT.Foundation");

	// 1. Allocate RDG Textures
	// We allocate matching SceneDepth extent to prevent ViewRectMin offset issues from causing out of bounds
	FIntPoint TextureExtent = SceneDepthTexture->Desc.Extent;
	if (TextureExtent.X <= 0 || TextureExtent.Y <= 0)
	{
		return Outputs;
	}

	const FIntRect ViewRect = Inputs.ViewRect.Width() > 0 && Inputs.ViewRect.Height() > 0
		? Inputs.ViewRect
		: FIntRect(FIntPoint::ZeroValue, TextureExtent);
	FAVBOITFrameConfig Config = Inputs.Config.IsValid()
		? Inputs.Config
		: FAVBOITFrameConfig::Build(
			ViewRect,
			TextureExtent,
			Inputs.ZNear > 0.0f ? Inputs.ZNear : 100.0f,
			Inputs.ZFar > Inputs.ZNear ? Inputs.ZFar : 400000.0f,
			8,
			64);

	TextureExtent = Config.TextureExtent;
	const FIntPoint VolumeExtent = Config.VolumeExtent;
	const uint32 DownsampleFactor = Config.DownsampleFactor;
	const uint32 NumSlices = Config.NumSlices;
	const FIntVector4 ViewRectMin(Config.ViewRect.Min.X, Config.ViewRect.Min.Y, 0, 0);
	const FIntVector4 ViewRectSize(Config.ViewRect.Width(), Config.ViewRect.Height(), 0, 0);

	uint64 ExtinctionElements = (uint64)VolumeExtent.X * (uint64)VolumeExtent.Y * NumSlices;
	if (ExtinctionElements == 0) ExtinctionElements = 1;

	FRDGBufferDesc ExtinctionDesc = FRDGBufferDesc::CreateStructuredDesc(4, ExtinctionElements);

	Outputs.ExtinctionVolume = GraphBuilder.CreateBuffer(ExtinctionDesc, TEXT("AVBOIT.Outputs.ExtinctionVolume"));
	if (!Outputs.ExtinctionVolume)
	{
		UE_LOG(LogTemp, Fatal, TEXT("AVBOIT: ExtinctionVolume Buffer is NULL!"));
	}
	FRDGBufferUAVRef ExtUAV = GraphBuilder.CreateUAV(Outputs.ExtinctionVolume);
	if (!ExtUAV)
	{
		UE_LOG(LogTemp, Fatal, TEXT("AVBOIT: ExtinctionVolume UAV is NULL!"));
	}

	Outputs.OverflowCounter = GraphBuilder.CreateBuffer(FRDGBufferDesc::CreateBufferDesc(sizeof(uint32), 1), TEXT("AVBOIT.Foundation.OverflowCounter"));
	Outputs.OutOfBoundsCounter = GraphBuilder.CreateBuffer(FRDGBufferDesc::CreateBufferDesc(sizeof(uint32), 1), TEXT("AVBOIT.Foundation.OutOfBoundsCounter"));
	AddClearUAVPass(GraphBuilder, GraphBuilder.CreateUAV(Outputs.OverflowCounter, PF_R32_UINT), 0);
	AddClearUAVPass(GraphBuilder, GraphBuilder.CreateUAV(Outputs.OutOfBoundsCounter, PF_R32_UINT), 0);

	FRDGBufferRef DummyCoverage = GraphBuilder.CreateBuffer(FRDGBufferDesc::CreateBufferDesc(4, 1), TEXT("DummyCoverage"));
	FRDGBufferRef DummyDebugPixel = GraphBuilder.CreateBuffer(FRDGBufferDesc::CreateStructuredDesc(sizeof(FAVBOITRasterDebugPixelData), 1), TEXT("DummyDebugPixel"));
	FRDGBufferUAVRef DummyCoverageUAV = GraphBuilder.CreateUAV(DummyCoverage, PF_R32_UINT);
	FRDGBufferUAVRef DummyDebugPixelUAV = GraphBuilder.CreateUAV(DummyDebugPixel);
	AddClearUAVPass(GraphBuilder, DummyCoverageUAV, 0);
	AddClearUAVPass(GraphBuilder, DummyDebugPixelUAV, 0);
	if (!DummyCoverageUAV || !DummyDebugPixelUAV)
	{
		UE_LOG(LogTemp, Fatal, TEXT("AVBOIT: Dummy UAVs are NULL!"));
	}

	FRDGTextureDesc TransmittanceDesc = FRDGTextureDesc::Create2DArray(
		VolumeExtent,
		PF_R32_FLOAT,
		FClearValueBinding::None,
		TexCreate_ShaderResource | TexCreate_UAV,
		NumSlices);
	Outputs.TransmittanceVolume = GraphBuilder.CreateTexture(TransmittanceDesc, TEXT("AVBOIT.Outputs.TransmittanceVolume"));

	FRDGTextureDesc ColorDesc = FRDGTextureDesc::Create2D(
		TextureExtent,
		PF_FloatRGBA,
		FClearValueBinding::Transparent,
		TexCreate_ShaderResource | TexCreate_RenderTargetable | TexCreate_UAV);
	Outputs.ColorAccumulation = GraphBuilder.CreateTexture(ColorDesc, TEXT("AVBOIT.Outputs.ColorAccumulation"));
	Outputs.AlphaAccumulation = GraphBuilder.CreateTexture(ColorDesc, TEXT("AVBOIT.Outputs.AlphaAccumulation"));

	// Pass 1: Clear
	{
		auto* PassParameters = GraphBuilder.AllocParameters<FAVBOITClearCS::FParameters>();
		PassParameters->ViewResolution = FVector2f(TextureExtent.X, TextureExtent.Y);
		PassParameters->VolumeResolution = FVector2f(VolumeExtent.X, VolumeExtent.Y);
		PassParameters->ZNear = Config.NearDepthCm;
		PassParameters->ZFar = Config.FarDepthCm;
		PassParameters->NumSlices = NumSlices;
		PassParameters->FragmentCount = 0;
		PassParameters->OutExtinctionVolume = ExtUAV;
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
			RDG_EVENT_NAME("AVBOIT.Foundation.Clear"),
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
	if (Inputs.DrawData.Num() > 0)
	{

		
		TArray<FAVBOITRasterSplatPS::FParameters*> SplatPSParams;
		TArray<FAVBOITRasterSplatVS::FParameters*> SplatVSParams;
		for (const FAVBOITRasterDrawData& Data : Inputs.DrawData)
		{
			auto* PSParams = GraphBuilder.AllocParameters<FAVBOITRasterSplatPS::FParameters>();
			PSParams->ZNear = ZNear;
			PSParams->ZFar = ZFar;
			PSParams->ViewResolution = FVector2f(TextureExtent.X, TextureExtent.Y);
			PSParams->VolumeResolution = FVector2f(VolumeExtent.X, VolumeExtent.Y);
			PSParams->DownsampleFactor = DownsampleFactor;
			PSParams->NumSlices = NumSlices;
			PSParams->ColorAndAlpha = FVector4f(Data.Color.R, Data.Color.G, Data.Color.B, Data.Alpha);
			PSParams->ViewRectMin = ViewRectMin;
			PSParams->ViewRectSize = ViewRectSize;
			PSParams->OutExtinctionVolume = GraphBuilder.CreateUAV(Outputs.ExtinctionVolume);
			PSParams->OverflowCounter = GraphBuilder.CreateUAV(Outputs.OverflowCounter, PF_R32_UINT);
			PSParams->OutOfBoundsCounter = GraphBuilder.CreateUAV(Outputs.OutOfBoundsCounter, PF_R32_UINT);
			PSParams->DebugPixel = Inputs.DebugPixel;
			
			if (Inputs.FragmentCoverageCounter != nullptr && Inputs.RasterDebugPixelBuffer != nullptr)
			{
				PSParams->FragmentCoverageCounter = GraphBuilder.CreateUAV(Inputs.FragmentCoverageCounter, PF_R32_UINT);
				PSParams->OutDebugPixelBuffer = GraphBuilder.CreateUAV(Inputs.RasterDebugPixelBuffer);
			}
			else
			{
				PSParams->FragmentCoverageCounter = DummyCoverageUAV;
				PSParams->OutDebugPixelBuffer = DummyDebugPixelUAV;
			}

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

		FRDGTextureDesc DummyDesc = FRDGTextureDesc::Create2D(
			Outputs.ColorAccumulation->Desc.Extent,
			PF_R8G8B8A8,
			FClearValueBinding::None,
			TexCreate_RenderTargetable | TexCreate_ShaderResource
		);
		FRDGTextureRef DummyRT = GraphBuilder.CreateTexture(DummyDesc, TEXT("AVBOIT.DummySplatRT"));

		PassParameters->RenderTargets[0] = FRenderTargetBinding(DummyRT, ERenderTargetLoadAction::ENoAction);
		PassParameters->RenderTargets.DepthStencil = FDepthStencilBinding(SceneDepthTexture, ERenderTargetLoadAction::ELoad, FExclusiveDepthStencil::DepthRead_StencilNop);

		GraphBuilder.AddPass(
			RDG_EVENT_NAME("AVBOIT.Foundation.Splat"),
			PassParameters,
			ERDGPassFlags::Raster,
			[SplatVSParams, SplatPSParams, ViewRect, DrawDataArray, bTestCoverage, VertexShader, PixelShader](FRHICommandList& RHICmdList)
		{
			RHICmdList.SetViewport(ViewRect.Min.X, ViewRect.Min.Y, 0.0f, ViewRect.Max.X, ViewRect.Max.Y, 1.0f);

			FGraphicsPipelineStateInitializer GraphicsPSOInit;
			RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);

			GraphicsPSOInit.BlendState = TStaticBlendState<CW_NONE>::GetRHI();
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
				RHICmdList.DrawIndexedPrimitive(DrawDataArray[i].IndexBufferRHI, 0, 0, DrawDataArray[i].VertexCount, 0, DrawDataArray[i].PrimitiveCount, 1);
			}
		});
	}

	// Pass 3: Integrate
	{
		auto* PassParameters = GraphBuilder.AllocParameters<FAVBOITIntegrateCS::FParameters>();
		PassParameters->ViewResolution = FVector2f(TextureExtent.X, TextureExtent.Y);
		PassParameters->VolumeResolution = FVector2f(VolumeExtent.X, VolumeExtent.Y);
		PassParameters->ZNear = Config.NearDepthCm;
		PassParameters->ZFar = Config.FarDepthCm;
		PassParameters->NumSlices = NumSlices;
		PassParameters->FragmentCount = 0;
		PassParameters->InExtinctionVolume = GraphBuilder.CreateSRV(Outputs.ExtinctionVolume);
		if (!PassParameters->InExtinctionVolume)
		{
			UE_LOG(LogTemp, Fatal, TEXT("AVBOIT: InExtinctionVolume SRV is NULL!"));
		}
		PassParameters->OutTransmittanceVolume = GraphBuilder.CreateUAV(Outputs.TransmittanceVolume);

		TShaderMapRef<FAVBOITIntegrateCS> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
		FIntVector GroupCount = FIntVector(
			FMath::DivideAndRoundUp(VolumeExtent.X, 8),
			FMath::DivideAndRoundUp(VolumeExtent.Y, 8),
			1
		);

		FComputeShaderUtils::AddPass(
			GraphBuilder,
			RDG_EVENT_NAME("AVBOIT.Foundation.Integrate"),
			ComputeShader,
			PassParameters,
			GroupCount
		);
	}

	// Pass 3.5: Resolved alpha. AlphaAccumulation stores 1 - far transmittance.
	{
		auto* PassParameters = GraphBuilder.AllocParameters<FAVBOITRasterResolvedAlphaCS::FParameters>();
		PassParameters->TextureExtent = TextureExtent;
		PassParameters->VolumeExtent = VolumeExtent;
		PassParameters->ViewRectMin = ViewRectMin;
		PassParameters->ViewRectSize = ViewRectSize;
		PassParameters->DownsampleFactor = DownsampleFactor;
		PassParameters->NumSlices = NumSlices;
		PassParameters->TransmittanceVolume = GraphBuilder.CreateSRV(Outputs.TransmittanceVolume);
		PassParameters->OutResolvedAlpha = GraphBuilder.CreateUAV(Outputs.AlphaAccumulation);

		TShaderMapRef<FAVBOITRasterResolvedAlphaCS> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
		FComputeShaderUtils::AddPass(
			GraphBuilder,
			RDG_EVENT_NAME("AVBOIT.Foundation.ResolvedAlpha"),
			ComputeShader,
			PassParameters,
			FComputeShaderUtils::GetGroupCount(TextureExtent, 8));
	}

	// Pass 4: ForwardShade
	if (Inputs.DrawData.Num() > 0)
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
			PSParams->VolumeResolution = FVector2f(VolumeExtent.X, VolumeExtent.Y);
			PSParams->DownsampleFactor = DownsampleFactor;
			PSParams->NumSlices = NumSlices;
			PSParams->ViewRectMin = ViewRectMin;
			PSParams->ViewRectSize = ViewRectSize;
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
			RDG_EVENT_NAME("AVBOIT.Foundation.ForwardUnlit"),
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
				RHICmdList.DrawIndexedPrimitive(DrawDataArray[i].IndexBufferRHI, 0, 0, DrawDataArray[i].VertexCount, 0, DrawDataArray[i].PrimitiveCount, 1);
			}
		});
	}

#if WITH_EDITOR || WITH_DEV_AUTOMATION_TESTS
	if (GAVBOITRasterProbe)
	{
		GAVBOITRasterProbe->bClearPassScheduled = true;
		GAVBOITRasterProbe->bSplatPassScheduled = true;
		GAVBOITRasterProbe->bIntegratePassScheduled = true;
		GAVBOITRasterProbe->bForwardPassScheduled = true;
		GAVBOITRasterProbe->SplatDrawCount = Inputs.DrawData.Num();
		GAVBOITRasterProbe->ForwardDrawCount = Inputs.DrawData.Num();
		GAVBOITRasterProbe->SkipReason = EAVBOITRasterSkipReason::Executed;
	}
#endif


	FRDGTextureRef SceneColorBefore = nullptr;
#if WITH_EDITOR || WITH_DEV_AUTOMATION_TESTS
	if (GAVBOITRasterProbe && GAVBOITRasterProbe->RequestedDebugPixel.X >= 0 && Inputs.FragmentCoverageCounter != nullptr && Inputs.RasterDebugPixelBuffer != nullptr && Inputs.DrawData.Num() > 0)
	{
		FRDGTextureDesc CopyDesc = SceneColorTexture->Desc;
		CopyDesc.Flags |= TexCreate_RenderTargetable;
		SceneColorBefore = GraphBuilder.CreateTexture(CopyDesc, TEXT("AVBOIT.SceneColorBefore"));
		AddCopyTexturePass(GraphBuilder, SceneColorTexture, SceneColorBefore);
	}
#endif

	// Pass 5: Composite
	// In-place modification of SceneColor using Hardware Blend
	{
		auto* PassParameters = GraphBuilder.AllocParameters<FAVBOITRasterCompositePS::FParameters>();
		PassParameters->ViewRectMin = ViewRectMin;
		PassParameters->ViewRectSize = ViewRectSize;
		PassParameters->VolumeExtent = VolumeExtent;
		PassParameters->ColorAccumulation = Outputs.ColorAccumulation;
		PassParameters->TransmittanceVolume = GraphBuilder.CreateSRV(Outputs.TransmittanceVolume);
		PassParameters->DownsampleFactor = DownsampleFactor;
		PassParameters->NumSlices = NumSlices;
		PassParameters->RenderTargets[0] = FRenderTargetBinding(SceneColorTexture, ERenderTargetLoadAction::ELoad);

		TShaderMapRef<FAVBOITRasterCompositePS> PixelShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
		FRHIBlendState* BlendState = TStaticBlendState<CW_RGB, BO_Add, BF_One, BF_SourceAlpha, BO_Add, BF_Zero, BF_One>::GetRHI();

		FPixelShaderUtils::AddFullscreenPass(
			GraphBuilder,
			GetGlobalShaderMap(GMaxRHIFeatureLevel),
			RDG_EVENT_NAME("AVBOIT.Foundation.Composite"),
			PixelShader,
			PassParameters,
			ViewRect,
			BlendState
		);

		Outputs.CompositeOutput = PassParameters->RenderTargets[0].GetTexture();
	}

#if WITH_EDITOR || WITH_DEV_AUTOMATION_TESTS
	if (GAVBOITRasterProbe && GAVBOITRasterProbe->RequestedDebugPixel.X >= 0 && Inputs.FragmentCoverageCounter != nullptr && Inputs.RasterDebugPixelBuffer != nullptr && Inputs.DrawData.Num() > 0)
	{
		GAVBOITRasterProbe->bCompositePassScheduled = true;
		GAVBOITRasterProbe->CompositeDrawCount = 1;
		GAVBOITRasterProbe->bDebugReadbackScheduled = true;

		FRDGBufferDesc BufDesc = FRDGBufferDesc::CreateStructuredDesc(sizeof(FAVBOITRasterDebugPayload), 1);
		FRDGBufferRef OutDebugBuffer = GraphBuilder.CreateBuffer(BufDesc, TEXT("AVBOIT.DebugBuffer"));
		// AddClearUAVPass(GraphBuilder, GraphBuilder.CreateUAV(OutDebugBuffer), 0);

		auto* DebugParams = GraphBuilder.AllocParameters<FAVBOITRasterDebugExtractCS::FParameters>();
		DebugParams->ViewResolution = FVector2f(TextureExtent.X, TextureExtent.Y);
		DebugParams->ExtinctionVolume = GraphBuilder.CreateSRV(Outputs.ExtinctionVolume);
		DebugParams->TransmittanceVolume = GraphBuilder.CreateSRV(Outputs.TransmittanceVolume);
		DebugParams->ColorAccumulation = Outputs.ColorAccumulation;
		DebugParams->SceneColorBefore = SceneColorBefore ? SceneColorBefore : SceneColorTexture;
		DebugParams->SceneColorAfter = SceneColorTexture;
		// The struct may expect DebugPixel instead of TargetPixel:
		DebugParams->DebugPixel = GAVBOITRasterProbe->RequestedDebugPixel;
		DebugParams->FragmentCoverageCounter = GraphBuilder.CreateSRV(Inputs.FragmentCoverageCounter ? Inputs.FragmentCoverageCounter : GraphBuilder.CreateBuffer(FRDGBufferDesc::CreateBufferDesc(4, 1), TEXT("Dummy")), PF_R32_UINT);
		DebugParams->RasterDebugPixelBuffer = GraphBuilder.CreateSRV(Inputs.RasterDebugPixelBuffer ? Inputs.RasterDebugPixelBuffer : GraphBuilder.CreateBuffer(FRDGBufferDesc::CreateStructuredDesc(sizeof(FAVBOITRasterDebugPixelData), 1), TEXT("Dummy")));
		DebugParams->OutDebugPayload = GraphBuilder.CreateUAV(OutDebugBuffer);
			DebugParams->TextureExtent = TextureExtent;
			DebugParams->VolumeExtent = VolumeExtent;
			DebugParams->ViewRectMin = Inputs.ViewRect.Min;
			DebugParams->ViewRectMax = Inputs.ViewRect.Max;
			DebugParams->NumSlices = NumSlices;
			DebugParams->DownsampleFactor = DownsampleFactor;
			DebugParams->OverflowCounter = GraphBuilder.CreateSRV(Outputs.OverflowCounter, PF_R32_UINT);
			DebugParams->OutOfBoundsCounter = GraphBuilder.CreateSRV(Outputs.OutOfBoundsCounter, PF_R32_UINT);

		TShaderMapRef<FAVBOITRasterDebugExtractCS> DebugComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
		FComputeShaderUtils::AddPass(
			GraphBuilder,
			RDG_EVENT_NAME("AVBOIT.Foundation.DebugExtract"),
			DebugComputeShader,
			DebugParams,
			FIntVector(1, 1, 1)
		);

		FRHIGPUBufferReadback* Readback = new FRHIGPUBufferReadback(TEXT("AVBOITRasterReadback"));
		AddEnqueueCopyPass(GraphBuilder, Readback, OutDebugBuffer, sizeof(FAVBOITRasterDebugPayload));

		FAVBOITRasterExecutionProbe* LocalProbe = GAVBOITRasterProbe;
		if (LocalProbe)
		{
			// Instead of locking inside the RDG pass, we store the readback to poll on the RenderThread later.
			if (LocalProbe->PendingReadback)
			{
				delete LocalProbe->PendingReadback;
			}
			LocalProbe->PendingReadback = Readback;
		}
	}
#endif
	
	return Outputs;
}
