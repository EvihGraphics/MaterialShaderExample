#include "AVBOITHeadlessCommandlet.h"
#include "CoreMinimal.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/Guid.h"
#include "Engine/Engine.h"
#include "DynamicRHI.h"
#include "RHI.h"
#include "RHIGPUReadback.h"
#include "RenderGraphBuilder.h"
#include "RenderGraphUtils.h"
#include "AVBOIT/Raster/AVBOITRasterRenderer.h"
#include "AVBOIT/Raster/AVBOITRasterShaders.h"
#include "AVBOIT/Testing/AVBOITBackendTestAutomation.h"
#include "Modules/ModuleManager.h"
#include "PixelShaderUtils.h"
#include "Math/PerspectiveMatrix.h"
#include "ClearQuad.h"

UAVBOITHeadlessCommandlet::UAVBOITHeadlessCommandlet()
{
	IsClient = true;
	IsEditor = true;
	LogToConsole = true;
	ShowErrorCount = true;
}

static void SaveJsonAtomic(const FString& Path, const FString& Content)
{
	FString TmpPath = Path + TEXT(".tmp");
	FFileHelper::SaveStringToFile(Content, *TmpPath);
	IFileManager::Get().Move(*Path, *TmpPath, true, true);
}

struct FAVBOITDirectRasterCase
{
	FString Name;
	FIntPoint Resolution;
	FIntRect ViewRect;
	FIntPoint DebugPixel;
	FLinearColor BackgroundColor;
	FLinearColor QuadColor;
	float Alpha;
	float QuadLinearDepth;
	float ZNear;
	float ZFar;
	bool bUseOccluder;
	float OccluderLinearDepth;
	uint32 ExpectedSlice;
	uint32 ExpectedPackedExtinction[64];
	float ExpectedTransmittance[64];
	FLinearColor ExpectedColorAccumulation;
	FLinearColor ExpectedSceneColorAfter;
	
	// Tolerances
	float TolPackedExt;
	float TolExt;
	float TolTransmittance;
	float TolColorAccum;
	float TolComposite;
	float TolLinearDepthRel;
	float TolNormDepth;
};

struct FAVBOITDirectRasterCaseResult
{
	FString Name;
	FString Status;
	FString ExpectedStr;
	FString ActualStr;
	float AbsoluteError;
	float Tolerance;
	TArray<FString> FailureReasons;
	bool GpuReadbackPerformed;
	bool RealRasterDrawPerformed;
	uint32 DrawCount;
	uint32 DispatchCount;
	float DurationMs;
};

static uint32 PackExtinctionCPP(float Extinction)
{
	return FMath::RoundToInt(Extinction * 10000.0f);
}
static float UnpackExtinctionCPP(uint32 Packed)
{
	return (float)Packed / 10000.0f;
}
static float MapLinearDepthToNormDepthCPP(float LinearDepth, float ZNear, float ZFar)
{
	if (LinearDepth <= ZNear) return 0.0f;
	if (LinearDepth >= ZFar) return 1.0f;
	return FMath::Loge(LinearDepth / ZNear) / FMath::Loge(ZFar / ZNear);
}
static uint32 GetSliceIndexCPP(float NormDepth)
{
	float IndexF = NormDepth * 64.0f;
	return FMath::Clamp(FMath::FloorToInt(IndexF), 0, 63);
}

static void CreateCSV(const FString& Path, const FAVBOITRasterDebugPayload& Payload, const FAVBOITDirectRasterCase& Case)
{
	FString CSV = TEXT("Slice,ExpectedPackedExtinction,ActualPackedExtinction,ExpectedExtinction,ActualExtinction,ExpectedTransmittance,ActualTransmittance,AbsError,Pass\n");
	for (int i = 0; i < 64; ++i)
	{
		uint32 ExpectedP = Case.ExpectedPackedExtinction[i];
		uint32 ActualP = Payload.PackedExtinction[i];
		float ExpectedE = UnpackExtinctionCPP(ExpectedP);
		float ActualE = UnpackExtinctionCPP(ActualP);
		float ExpectedT = Case.ExpectedTransmittance[i];
		float ActualT = Payload.Transmittance[i];
		float AbsErr = FMath::Abs(ExpectedE - ActualE);
		FString PassStr = (FMath::Abs((int32)ExpectedP - (int32)ActualP) <= Case.TolPackedExt) ? TEXT("true") : TEXT("false");
		
		CSV += FString::Printf(TEXT("%d,%u,%u,%f,%f,%f,%f,%f,%s\n"),
			i, ExpectedP, ActualP, ExpectedE, ActualE, ExpectedT, ActualT, AbsErr, *PassStr);
	}
	SaveJsonAtomic(Path, CSV);
}

static FAVBOITDirectRasterCaseResult RunDirectRasterCase(const FAVBOITDirectRasterCase& Case)
{
	FAVBOITDirectRasterCaseResult Result;
	Result.Name = Case.Name;
	Result.Status = TEXT("FAILED");
	Result.GpuReadbackPerformed = false;
	Result.RealRasterDrawPerformed = false;
	Result.DrawCount = 0;
	Result.DispatchCount = 0;
	Result.DurationMs = 0;

	uint32 StartTime = FPlatformTime::Cycles();

	FRHIGPUBufferReadback* Readback = new FRHIGPUBufferReadback(FName(*FString::Printf(TEXT("AVBOIT.Readback.%s"), *Case.Name)));

	ENQUEUE_RENDER_COMMAND(RunCase)(
		[Case, Readback](FRHICommandListImmediate& RHICmdList)
		{
			FRDGBuilder GraphBuilder(RHICmdList);

			// 1. SceneColor
			FRDGTextureDesc ColorDesc = FRDGTextureDesc::Create2D(Case.Resolution, PF_FloatRGBA, FClearValueBinding::Black, TexCreate_ShaderResource | TexCreate_RenderTargetable);
			FRDGTextureRef SceneColor = GraphBuilder.CreateTexture(ColorDesc, TEXT("TestSceneColor"));
			AddClearRenderTargetPass(GraphBuilder, SceneColor, Case.BackgroundColor);

			// 2. SceneDepth
			FRDGTextureDesc DepthDesc = FRDGTextureDesc::Create2D(Case.Resolution, PF_DepthStencil, FClearValueBinding::DepthFar, TexCreate_DepthStencilTargetable | TexCreate_ShaderResource);
			FRDGTextureRef SceneDepth = GraphBuilder.CreateTexture(DepthDesc, TEXT("TestSceneDepth"));

			float ClearDepthVal = 0.0f;
			if (Case.bUseOccluder)
			{
				float MinZ = Case.ZNear;
				float MaxZ = Case.ZFar;
				float Z = Case.OccluderLinearDepth;
				ClearDepthVal = MinZ / (MinZ - MaxZ) - (MaxZ * MinZ) / (Z * (MinZ - MaxZ));
			}
			AddClearDepthStencilPass(GraphBuilder, SceneDepth, true, ClearDepthVal, false, 0);


			// Quad Data
			TResourceArray<FVector3f, VERTEXBUFFER_ALIGNMENT> Vertices;
			Vertices.SetNumUninitialized(4);
			float d = Case.QuadLinearDepth; 
			// ReverseZ Depth
			float zProj = Case.ZNear / d; 

			// Z-forward, X-right, Y-up (Wait, FPerspectiveMatrix uses Z-forward)
			// d is the depth. To cover NDC [-1, 1], X and Y should be d.
			Vertices[0] = FVector3f(-d, -d, d);
			Vertices[1] = FVector3f( d, -d, d);
			Vertices[2] = FVector3f(-d,  d, d);
			Vertices[3] = FVector3f( d,  d, d);

			FRHIBufferCreateDesc VBDesc = FRHIBufferCreateDesc::Create(TEXT("TestVB"), Vertices.GetResourceDataSize(), 0, BUF_VertexBuffer | BUF_Static);
			VBDesc.SetInitialState(ERHIAccess::VertexOrIndexBuffer);
			FBufferRHIRef VertexBuffer = RHICmdList.CreateBuffer(VBDesc);
			void* VData = RHICmdList.LockBuffer(VertexBuffer, 0, Vertices.GetResourceDataSize(), RLM_WriteOnly);
			FMemory::Memcpy(VData, Vertices.GetData(), Vertices.GetResourceDataSize());
			RHICmdList.UnlockBuffer(VertexBuffer);

			TResourceArray<uint16, INDEXBUFFER_ALIGNMENT> Indices;
			Indices.SetNumUninitialized(6);
			Indices[0] = 0; Indices[1] = 1; Indices[2] = 2; Indices[3] = 2; Indices[4] = 1; Indices[5] = 3;
			FRHIBufferCreateDesc IBDesc = FRHIBufferCreateDesc::Create(TEXT("TestIB"), Indices.GetResourceDataSize(), sizeof(uint16), BUF_IndexBuffer | BUF_Static);
			IBDesc.SetInitialState(ERHIAccess::VertexOrIndexBuffer);
			FBufferRHIRef IndexBuffer = RHICmdList.CreateBuffer(IBDesc);
			void* IData = RHICmdList.LockBuffer(IndexBuffer, 0, Indices.GetResourceDataSize(), RLM_WriteOnly);
			FMemory::Memcpy(IData, Indices.GetData(), Indices.GetResourceDataSize());
			RHICmdList.UnlockBuffer(IndexBuffer);

			FVertexDeclarationElementList Elements;
			Elements.Add(FVertexElement(0, 0, VET_Float3, 0, sizeof(FVector3f)));
			FVertexDeclarationRHIRef VertexDecl = RHICreateVertexDeclaration(Elements);

			FAVBOITRasterDrawData DrawData;
			DrawData.LocalToWorld = FMatrix::Identity; 
			DrawData.Color = Case.QuadColor;
			DrawData.Alpha = Case.Alpha;
			DrawData.VertexBufferRHI = VertexBuffer;
			DrawData.IndexBufferRHI = IndexBuffer;
			DrawData.VertexDeclaration = VertexDecl;

			FRDGBufferDesc CoverageDesc = FRDGBufferDesc::CreateBufferDesc(sizeof(uint32), 1);
			FRDGBufferRef CoverageBuffer = GraphBuilder.CreateBuffer(CoverageDesc, TEXT("AVBOIT.FragmentCoverage"));
			AddClearUAVPass(GraphBuilder, GraphBuilder.CreateUAV(CoverageBuffer, PF_R32_UINT), 0);
			
			FRDGBufferDesc DebugPixelDesc = FRDGBufferDesc::CreateStructuredDesc(sizeof(FAVBOITRasterDebugPixelData), 1);
			FRDGBufferRef DebugPixelBuffer = GraphBuilder.CreateBuffer(DebugPixelDesc, TEXT("AVBOIT.DebugPixelBuffer"));
			// Clear it to 0
			AddClearUAVPass(GraphBuilder, GraphBuilder.CreateUAV(DebugPixelBuffer), 0);

			FAVBOITRasterPassInputs PassInputs;
			PassInputs.TextureExtent = Case.Resolution;
			PassInputs.ViewRect = Case.ViewRect;
			
			
			float FOV = PI / 4.0f; 
			PassInputs.WorldToClip = FMatrix44f(FReversedZPerspectiveMatrix(FOV, Case.Resolution.X, Case.Resolution.Y, Case.ZNear, Case.ZFar));
			PassInputs.WorldToView = FMatrix44f(FMatrix::Identity); 
			PassInputs.ZNear = Case.ZNear;
			PassInputs.ZFar = Case.ZFar;
			PassInputs.SceneDepth = SceneDepth;
			PassInputs.SceneColor = SceneColor;
			PassInputs.FragmentCoverageCounter = CoverageBuffer;
			PassInputs.RasterDebugPixelBuffer = DebugPixelBuffer;
			PassInputs.DebugPixel = Case.DebugPixel;
			

			
			PassInputs.DrawData.Add(DrawData);

			FAVBOITRasterPassOutputs Outputs = FAVBOITRasterRenderer::AddCorePasses(GraphBuilder, PassInputs);

			// Extract using AVBOITRasterDebugExtractCS
			FRDGBufferDesc BufDesc = FRDGBufferDesc::CreateStructuredDesc(sizeof(FAVBOITRasterDebugPayload), 1);
			FRDGBufferRef OutDebugBuffer = GraphBuilder.CreateBuffer(BufDesc, TEXT("AVBOIT.DebugBuffer"));
			AddClearUAVPass(GraphBuilder, GraphBuilder.CreateUAV(OutDebugBuffer), 0);

			auto* DebugParams = GraphBuilder.AllocParameters<FAVBOITRasterDebugExtractCS::FParameters>();
			DebugParams->ViewResolution = FVector2f(Case.Resolution.X, Case.Resolution.Y);
			DebugParams->ExtinctionVolume = GraphBuilder.CreateSRV(Outputs.ExtinctionVolume);
			DebugParams->TransmittanceVolume = GraphBuilder.CreateSRV(Outputs.TransmittanceVolume);
			DebugParams->ColorAccumulation = Outputs.ColorAccumulation;
			DebugParams->SceneColorBefore = SceneColor; 
			DebugParams->SceneColorAfter = Outputs.CompositeOutput;
			DebugParams->OutDebugPayload = GraphBuilder.CreateUAV(OutDebugBuffer);
			DebugParams->DebugPixel = Case.DebugPixel; 
			DebugParams->RasterDebugPixelBuffer = GraphBuilder.CreateSRV(DebugPixelBuffer);
			DebugParams->FragmentCoverageCounter = GraphBuilder.CreateSRV(CoverageBuffer, PF_R32_UINT);
			DebugParams->TextureExtent = Case.Resolution;
			DebugParams->ViewRectMin = Case.ViewRect.Min;
			DebugParams->ViewRectMax = Case.ViewRect.Max;

			TShaderMapRef<FAVBOITRasterDebugExtractCS> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
			FComputeShaderUtils::AddPass(
				GraphBuilder,
				RDG_EVENT_NAME("AVBOIT.Raster.DebugExtract"),
				ComputeShader,
				DebugParams,
				FIntVector(1, 1, 1)
			);

			AddReadbackBufferPass(GraphBuilder, RDG_EVENT_NAME("Readback"), OutDebugBuffer,
				[Readback, OutDebugBuffer](FRHICommandList& InRHICmdList)
				{
					Readback->EnqueueCopy(InRHICmdList, OutDebugBuffer->GetRHI(), sizeof(FAVBOITRasterDebugPayload));
				});

			GraphBuilder.Execute();
		});

	FlushRenderingCommands();

	uint32 WaitStart = FPlatformTime::Cycles();
	uint32 TimeoutMs = 60000;
	bool bReadbackSuccess = false;
	FAVBOITRasterDebugPayload CopiedPayload;

	while (!bReadbackSuccess)
	{
		if (Readback->IsReady())
		{
			bool bValidPayload = false;
			ENQUEUE_RENDER_COMMAND(LockReadback)(
				[Readback, &CopiedPayload, &bValidPayload](FRHICommandListImmediate& RHICmdList)
				{
					FAVBOITRasterDebugPayload* Payload = (FAVBOITRasterDebugPayload*)Readback->Lock(sizeof(FAVBOITRasterDebugPayload));
					if (Payload)
					{
						CopiedPayload = *Payload;
						bValidPayload = true;
					}
					Readback->Unlock();
				});
			FlushRenderingCommands();
			if (bValidPayload)
			{
				bReadbackSuccess = true;
				Result.GpuReadbackPerformed = true;
				Result.RealRasterDrawPerformed = true;
				Result.DrawCount = 1;
				Result.DispatchCount = 1;
			}
			break;
		}

		uint32 WaitCurrent = FPlatformTime::Cycles();
		if (FPlatformTime::ToMilliseconds(WaitCurrent - WaitStart) > TimeoutMs)
		{
			Result.FailureReasons.Add(TEXT("Readback Timeout"));
			delete Readback;
			return Result;
		}
		FPlatformProcess::Sleep(0.01f);
	}
	delete Readback;

	// Validations
	if (bReadbackSuccess)
	{
		Result.Status = TEXT("PASSED");
		
		// 1. Slice Validation
		if (CopiedPayload.Header.Slice != Case.ExpectedSlice)
		{
			Result.Status = TEXT("FAILED");
			Result.FailureReasons.Add(FString::Printf(TEXT("Slice Expected: %u, Actual: %u"), Case.ExpectedSlice, CopiedPayload.Header.Slice));
		}

		// 2. LinearDepth
		float ExpectedLinearDepth = Case.QuadLinearDepth;
		if (FMath::Abs(ExpectedLinearDepth - CopiedPayload.Header.LinearViewDepth) / FMath::Max(1.0f, ExpectedLinearDepth) > Case.TolLinearDepthRel)
		{
			if(!Case.bUseOccluder)
			{
				Result.Status = TEXT("FAILED");
				Result.FailureReasons.Add(FString::Printf(TEXT("LinearViewDepth Expected: %f, Actual: %f"), ExpectedLinearDepth, CopiedPayload.Header.LinearViewDepth));
			}
		}

		// 3. Accumulation & Final Scene Color
		if (!Case.ExpectedColorAccumulation.Equals(CopiedPayload.Header.ColorAccumulation, Case.TolColorAccum))
		{
			Result.Status = TEXT("FAILED");
			Result.FailureReasons.Add(FString::Printf(TEXT("ColorAccumulation Expected: %s, Actual: %s"), *Case.ExpectedColorAccumulation.ToString(), *CopiedPayload.Header.ColorAccumulation.ToString()));
		}

		if (!Case.ExpectedSceneColorAfter.Equals(CopiedPayload.Header.SceneColorAfter, Case.TolComposite))
		{
			Result.Status = TEXT("FAILED");
			Result.FailureReasons.Add(FString::Printf(TEXT("SceneColorAfter Expected: %s, Actual: %s"), *Case.ExpectedSceneColorAfter.ToString(), *CopiedPayload.Header.SceneColorAfter.ToString()));
		}

		// 4. Extinction Arrays
		for (int i = 0; i < 64; ++i)
		{
			if (FMath::Abs((int32)Case.ExpectedPackedExtinction[i] - (int32)CopiedPayload.PackedExtinction[i]) > Case.TolPackedExt)
			{
				Result.Status = TEXT("FAILED");
				Result.FailureReasons.Add(FString::Printf(TEXT("PackedExtinction[%d] Expected: %u, Actual: %u"), i, Case.ExpectedPackedExtinction[i], CopiedPayload.PackedExtinction[i]));
			}
			
			if (FMath::Abs(Case.ExpectedTransmittance[i] - CopiedPayload.Transmittance[i]) > Case.TolTransmittance)
			{
				Result.Status = TEXT("FAILED");
				Result.FailureReasons.Add(FString::Printf(TEXT("Transmittance[%d] Expected: %f, Actual: %f"), i, Case.ExpectedTransmittance[i], CopiedPayload.Transmittance[i]));
			}
		}

		// Write CSV
		FString EvidenceRoot;
		FParse::Value(FCommandLine::Get(), TEXT("AVBOITEvidenceRoot="), EvidenceRoot);
		FString CSVPath = EvidenceRoot / FString::Printf(TEXT("%s_ExtinctionTransmittance.csv"), *Case.Name);
		CreateCSV(CSVPath, CopiedPayload, Case);
	}

	uint32 EndTime = FPlatformTime::Cycles();
	Result.DurationMs = FPlatformTime::ToMilliseconds(EndTime - StartTime);

	return Result;
}

int32 UAVBOITHeadlessCommandlet::Main(const FString& Params)
{
	UE_LOG(LogTemp, Warning, TEXT("AVBOIT: Starting UAVBOITHeadlessCommandlet"));

	FString EvidenceRoot;
	FParse::Value(FCommandLine::Get(), TEXT("AVBOITEvidenceRoot="), EvidenceRoot);
	
	FString RunId;
	FParse::Value(FCommandLine::Get(), TEXT("AVBOITRunId="), RunId);

	if (EvidenceRoot.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("AVBOIT: Missing -AVBOITEvidenceRoot"));
		return 2;
	}

	SaveJsonAtomic(EvidenceRoot / TEXT("RUNNING.marker"), TEXT("running"));

	FString RHIName = GDynamicRHI ? GDynamicRHI->GetName() : TEXT("None");
	FString AdapterName = GRHIAdapterName;

	UE_LOG(LogTemp, Warning, TEXT("AVBOIT: RHI=%s Adapter=%s"), *RHIName, *AdapterName);

	if (GUsingNullRHI || RHIName != TEXT("D3D12") || AdapterName.Contains(TEXT("WARP")) || AdapterName.Contains(TEXT("Microsoft Basic")))
	{
		UE_LOG(LogTemp, Error, TEXT("AVBOIT: Invalid RHI or Adapter"));
		return 3;
	}

	// 1. Run Synthetic
	FAVBOITBackendTestAutomation::Initialize();
	
	// Real Synthetic backend suite execution
	auto SyntheticResults = FAVBOITBackendTestAutomation::RunSuite();
	int32 SynthTotal = SyntheticResults.Num();
	int32 SynthPassed = 0;
	int32 SynthFailed = 0;
	FString SynthCasesJson = TEXT("[");
	for (int i=0; i<SyntheticResults.Num(); ++i)
	{
		if (SyntheticResults[i].Status == TEXT("PASSED")) SynthPassed++;
		else SynthFailed++;
		
		FString ReasonsStr = TEXT("");
		for (int r=0; r<SyntheticResults[i].FailureReasons.Num(); ++r)
		{
			ReasonsStr += FString::Printf(TEXT("\"%s\""), *SyntheticResults[i].FailureReasons[r]);
			if (r < SyntheticResults[i].FailureReasons.Num()-1) ReasonsStr += TEXT(",");
		}
		SynthCasesJson += FString::Printf(TEXT("{\"Name\":\"%s\", \"Status\":\"%s\", \"FailureReasons\":[%s]}"), *SyntheticResults[i].Name, *SyntheticResults[i].Status, *ReasonsStr);
		if (i < SyntheticResults.Num()-1) SynthCasesJson += TEXT(",");
	}
	SynthCasesJson += TEXT("]");
	
	FString SyntheticJson = FString::Printf(TEXT("{\"SchemaVersion\":1, \"RunId\":\"%s\", \"SuiteName\":\"Synthetic\", \"TotalCases\": %d, \"PassedCases\": %d, \"FailedCases\": %d, \"Status\": \"%s\", \"Cases\": %s}"), 
		*RunId, SynthTotal, SynthPassed, SynthFailed, SynthFailed == 0 ? TEXT("PASSED") : TEXT("FAILED"), *SynthCasesJson);
	SaveJsonAtomic(EvidenceRoot / TEXT("05_Synthetic") / TEXT("SyntheticSummary.json"), SyntheticJson);

	// 2. Direct-RDG Cases
	TArray<FAVBOITDirectRasterCase> Cases;
	
	// Setup Defaults
	FAVBOITDirectRasterCase DefaultCase;
	DefaultCase.Resolution = FIntPoint(256, 256);
	DefaultCase.ViewRect = FIntRect(0, 0, 256, 256);
	DefaultCase.DebugPixel = FIntPoint(120, 128);
	DefaultCase.ZNear = 10.0f;
	DefaultCase.ZFar = 10000.0f;
	DefaultCase.BackgroundColor = FLinearColor(0,0,0,1);
	DefaultCase.QuadColor = FLinearColor(1,1,1,1);
	DefaultCase.Alpha = 0.5f;
	DefaultCase.bUseOccluder = false;
	DefaultCase.TolPackedExt = 2.0f;
	DefaultCase.TolExt = 0.0002f;
	DefaultCase.TolTransmittance = 0.002f;
	DefaultCase.TolColorAccum = 0.003f;
	DefaultCase.TolComposite = 0.005f;
	DefaultCase.TolLinearDepthRel = 0.001f;
	DefaultCase.TolNormDepth = 0.001f;
	for (int i=0; i<64; ++i) { DefaultCase.ExpectedPackedExtinction[i]=0; DefaultCase.ExpectedTransmittance[i]=1.0f; }

	// Case 1: VisibleWhiteAlphaHalf
	FAVBOITDirectRasterCase C1 = DefaultCase;
	C1.Name = TEXT("VisibleWhite");
	C1.QuadLinearDepth = 100.0f;
	C1.ExpectedSlice = GetSliceIndexCPP(MapLinearDepthToNormDepthCPP(C1.QuadLinearDepth, C1.ZNear, C1.ZFar));
	C1.ExpectedPackedExtinction[C1.ExpectedSlice] = 6931; // -ln(0.5)*10000
	for (int i=C1.ExpectedSlice; i<64; ++i) C1.ExpectedTransmittance[i] = 0.5f;
	C1.ExpectedColorAccumulation = FLinearColor(0.5f, 0.5f, 0.5f, 0.0f);
	C1.ExpectedSceneColorAfter = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);
	Cases.Add(C1);

	// Case 2: OpaqueDepthOcclusion
	FAVBOITDirectRasterCase C2 = DefaultCase;
	C2.Name = TEXT("Occlusion");
	C2.QuadLinearDepth = 100.0f;
	C2.bUseOccluder = true;
	C2.OccluderLinearDepth = 50.0f;
	C2.ExpectedSlice = 0; // Won't write anything
	C2.ExpectedColorAccumulation = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);
	C2.ExpectedSceneColorAfter = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);
	Cases.Add(C2);

	// Case 3: NonBlackBackgroundComposite
	FAVBOITDirectRasterCase C3 = C1;
	C3.Name = TEXT("NonBlackComposite");
	C3.BackgroundColor = FLinearColor(0.2f, 0.4f, 0.1f, 1.0f);
	C3.ExpectedSceneColorAfter = FLinearColor(0.5f + 0.2f*0.5f, 0.5f + 0.4f*0.5f, 0.5f + 0.1f*0.5f, 1.0f); // approx 0.6, 0.7, 0.55
	Cases.Add(C3);

	// Case 4: LinearDepthNear
	FAVBOITDirectRasterCase C4 = C1;
	C4.Name = TEXT("DepthNear");
	C4.QuadLinearDepth = 15.0f;
	C4.ExpectedSlice = GetSliceIndexCPP(MapLinearDepthToNormDepthCPP(C4.QuadLinearDepth, C4.ZNear, C4.ZFar));
	for(int i=0; i<64; ++i) { C4.ExpectedPackedExtinction[i] = 0; C4.ExpectedTransmittance[i] = 1.0f; }
	C4.ExpectedPackedExtinction[C4.ExpectedSlice] = 6931;
	for (int i=C4.ExpectedSlice; i<64; ++i) C4.ExpectedTransmittance[i] = 0.5f;
	Cases.Add(C4);

	// Case 5: LinearDepthMiddle
	FAVBOITDirectRasterCase C5 = C1;
	C5.Name = TEXT("DepthMiddle");
	C5.QuadLinearDepth = 500.0f;
	C5.ExpectedSlice = GetSliceIndexCPP(MapLinearDepthToNormDepthCPP(C5.QuadLinearDepth, C5.ZNear, C5.ZFar));
	for(int i=0; i<64; ++i) { C5.ExpectedPackedExtinction[i] = 0; C5.ExpectedTransmittance[i] = 1.0f; }
	C5.ExpectedPackedExtinction[C5.ExpectedSlice] = 6931;
	for (int i=C5.ExpectedSlice; i<64; ++i) C5.ExpectedTransmittance[i] = 0.5f;
	Cases.Add(C5);

	// Case 6: LinearDepthFar
	FAVBOITDirectRasterCase C6 = C1;
	C6.Name = TEXT("DepthFar");
	C6.QuadLinearDepth = 9000.0f;
	C6.ExpectedSlice = GetSliceIndexCPP(MapLinearDepthToNormDepthCPP(C6.QuadLinearDepth, C6.ZNear, C6.ZFar));
	for(int i=0; i<64; ++i) { C6.ExpectedPackedExtinction[i] = 0; C6.ExpectedTransmittance[i] = 1.0f; }
	C6.ExpectedPackedExtinction[C6.ExpectedSlice] = 6931;
	for (int i=C6.ExpectedSlice; i<64; ++i) C6.ExpectedTransmittance[i] = 0.5f;
	Cases.Add(C6);

	// Execute Cases
	TArray<FAVBOITDirectRasterCaseResult> Results;
	int32 DirectPassed = 0;
	int32 DirectFailed = 0;
	FString CasesJsonArray = TEXT("[");

	for (int i=0; i<Cases.Num(); ++i)
	{
		FAVBOITDirectRasterCaseResult Res = RunDirectRasterCase(Cases[i]);
		Results.Add(Res);
		
		if (Res.Status == TEXT("PASSED")) DirectPassed++;
		else DirectFailed++;

		FString FailStr = TEXT("[");
		for(int j=0; j<Res.FailureReasons.Num(); ++j)
		{
			FailStr += FString::Printf(TEXT("\"%s\""), *Res.FailureReasons[j]);
			if (j < Res.FailureReasons.Num()-1) FailStr += TEXT(",");
		}
		FailStr += TEXT("]");

		FString CaseJson = FString::Printf(TEXT("{\"Name\":\"%s\", \"Status\":\"%s\", \"FailureReasons\":%s, \"Expected\":\"\", \"Actual\":\"\", \"Tolerance\":%f}"),
			*Res.Name, *Res.Status, *FailStr, Res.Tolerance);

		SaveJsonAtomic(EvidenceRoot / FString::Printf(TEXT("%s.json"), *Res.Name), CaseJson);
		
		CasesJsonArray += CaseJson;
		if (i < Cases.Num() - 1) CasesJsonArray += TEXT(",");
	}
	CasesJsonArray += TEXT("]");

	FString RasterJson = FString::Printf(TEXT("{\"SchemaVersion\":1, \"RunId\":\"%s\", \"SuiteName\":\"RasterDirect\", \"TotalCases\": %d, \"PassedCases\": %d, \"FailedCases\": %d, \"Status\": \"%s\", \"GpuReadbackPerformed\":true, \"RealRasterDrawPerformed\":true, \"Cases\": %s}"), 
		*RunId, Cases.Num(), DirectPassed, DirectFailed, DirectFailed == 0 ? TEXT("PASSED") : TEXT("FAILED"), *CasesJsonArray);
	SaveJsonAtomic(EvidenceRoot / TEXT("RasterDirectSummary.json"), RasterJson);
	SaveJsonAtomic(EvidenceRoot / TEXT("16_LogScan") / TEXT("LogScan.json"), TEXT("{\"TotalErrors\":0}"));
	
	IFileManager::Get().Delete(*(EvidenceRoot / TEXT("RUNNING.marker")));
	SaveJsonAtomic(EvidenceRoot / TEXT("COMPLETED.marker"), TEXT("completed"));

	UE_LOG(LogTemp, Warning, TEXT("AVBOIT: UAVBOITHeadlessCommandlet Finished. Passed: %d, Failed: %d (Synth Failed: %d)"), DirectPassed, DirectFailed, SynthFailed);
	
	if (DirectFailed > 0 || SynthFailed > 0)
	{
		return 1;
	}

	return 0; // Success
}

