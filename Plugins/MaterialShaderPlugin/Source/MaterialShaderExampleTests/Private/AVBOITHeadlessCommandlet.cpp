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
#include "ImageUtils.h"
#include "RHITypes.h"

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

static FString JsonEscape(const FString& In)
{
	FString Out = In;
	Out.ReplaceInline(TEXT("\\"), TEXT("\\\\"));
	Out.ReplaceInline(TEXT("\""), TEXT("\\\""));
	Out.ReplaceInline(TEXT("\r"), TEXT("\\r"));
	Out.ReplaceInline(TEXT("\n"), TEXT("\\n"));
	return Out;
}

static void EnsureParentDirectory(const FString& Path)
{
	IFileManager::Get().MakeDirectory(*FPaths::GetPath(Path), true);
}

static bool SaveColorPng(const FString& Path, const TArray<FColor>& Pixels, const FIntPoint Size)
{
	if (Pixels.Num() != Size.X * Size.Y || Size.X <= 0 || Size.Y <= 0)
	{
		return false;
	}
	EnsureParentDirectory(Path);
	const FImageView Image(Pixels.GetData(), Size.X, Size.Y);
	return FImageUtils::SaveImageByExtension(*Path, Image);
}

struct FAVBOITFoundationImageReadback
{
	FString Name;
	FIntPoint Size = FIntPoint::ZeroValue;
	TArray<FColor> Pixels;
	bool bPerformed = false;
};

static void AddTextureReadbackPass(
	FRDGBuilder& GraphBuilder,
	FRDGTextureRef Texture,
	const FIntRect& Rect,
	const TSharedRef<FAVBOITFoundationImageReadback, ESPMode::ThreadSafe>& Result)
{
	AddReadbackTexturePass(
		GraphBuilder,
		RDG_EVENT_NAME("AVBOIT.Foundation.FullImageReadback"),
		Texture,
		[Texture, Rect, Result](FRHICommandListImmediate& RHICmdList)
	{
		FReadSurfaceDataFlags ReadFlags(RCM_UNorm);
		ReadFlags.SetLinearToGamma(true);
		Result->Pixels.Reset();
		RHICmdList.ReadSurfaceData(Texture->GetRHI(), Rect, Result->Pixels, ReadFlags);
		Result->Size = Rect.Size();
		Result->bPerformed = Result->Pixels.Num() == Result->Size.X * Result->Size.Y;
	});
}

enum class EAVBOITFoundationDirectScene : uint8
{
	SingleLayerIdentity,
	TwoIntersectingQuads
};

enum class EAVBOITFoundationDirectOrder : uint8
{
	AB,
	BA,
	RandomSeed1,
	RandomSeed2,
	RandomSeed3
};

struct FAVBOITFoundationDirectPrimitive
{
	FString Id;
	FLinearColor Color;
	float Alpha = 1.0f;
	uint32 SubmissionOrder = 0;
	FVector3f Vertices[4];
};

struct FAVBOITFoundationDirectScene
{
	EAVBOITFoundationDirectScene Scene = EAVBOITFoundationDirectScene::TwoIntersectingQuads;
	TArray<FAVBOITFoundationDirectPrimitive> Primitives;
	FIntPoint Resolution = FIntPoint(512, 512);
	FIntRect ViewRect = FIntRect(0, 0, 512, 512);
	FIntRect RoiRect = FIntRect(64, 64, 448, 448);
	float NearDepthCm = 100.0f;
	float FarDepthCm = 400000.0f;
};

static FMatrix44f MakeFoundationClipMatrix()
{
	FMatrix44f Matrix = FMatrix44f::Identity;
	Matrix.M[2][2] = 0.0f;
	Matrix.M[3][2] = 0.5f;
	Matrix.M[3][3] = 1.0f;
	return Matrix;
}

static FAVBOITFoundationDirectPrimitive MakeFoundationQuad(
	const FString& Id,
	const FLinearColor& Color,
	float Alpha,
	float LeftDepth,
	float RightDepth,
	uint32 SubmissionOrder)
{
	FAVBOITFoundationDirectPrimitive Primitive;
	Primitive.Id = Id;
	Primitive.Color = Color;
	Primitive.Alpha = Alpha;
	Primitive.SubmissionOrder = SubmissionOrder;
	Primitive.Vertices[0] = FVector3f(-0.75f, -0.65f, LeftDepth);
	Primitive.Vertices[1] = FVector3f( 0.75f, -0.65f, RightDepth);
	Primitive.Vertices[2] = FVector3f(-0.75f,  0.65f, LeftDepth);
	Primitive.Vertices[3] = FVector3f( 0.75f,  0.65f, RightDepth);
	return Primitive;
}

static FAVBOITFoundationDirectScene BuildFoundationDirectScene(
	EAVBOITFoundationDirectScene Scene,
	EAVBOITFoundationDirectOrder Order,
	FIntPoint Resolution)
{
	FAVBOITFoundationDirectScene Out;
	Out.Scene = Scene;
	Out.Resolution = Resolution;
	Out.ViewRect = FIntRect(0, 0, Resolution.X, Resolution.Y);
	Out.RoiRect = FIntRect(Resolution.X / 8, Resolution.Y / 8, Resolution.X - Resolution.X / 8, Resolution.Y - Resolution.Y / 8);

	auto OrderFor = [Order](int32 PhysicalIndex) -> uint32
	{
		switch (Order)
		{
		case EAVBOITFoundationDirectOrder::BA:
			return PhysicalIndex == 0 ? 1u : 0u;
		case EAVBOITFoundationDirectOrder::RandomSeed1:
			return PhysicalIndex == 0 ? 1u : 0u;
		case EAVBOITFoundationDirectOrder::RandomSeed2:
			return PhysicalIndex == 0 ? 0u : 1u;
		case EAVBOITFoundationDirectOrder::RandomSeed3:
			return PhysicalIndex == 0 ? 1u : 0u;
		case EAVBOITFoundationDirectOrder::AB:
		default:
			return static_cast<uint32>(PhysicalIndex);
		}
	};

	if (Scene == EAVBOITFoundationDirectScene::SingleLayerIdentity)
	{
		Out.Primitives.Add(MakeFoundationQuad(TEXT("A_White"), FLinearColor::White, 0.5f, 1000.0f, 1000.0f, 0));
	}
	else
	{
		Out.Primitives.Add(MakeFoundationQuad(TEXT("A_Green"), FLinearColor(0.0f, 1.0f, 0.0f, 1.0f), 0.55f, 700.0f, 1300.0f, OrderFor(0)));
		Out.Primitives.Add(MakeFoundationQuad(TEXT("B_Cyan"), FLinearColor(0.0f, 0.75f, 1.0f, 1.0f), 0.55f, 1300.0f, 700.0f, OrderFor(1)));
	}

	Out.Primitives.Sort([](const FAVBOITFoundationDirectPrimitive& A, const FAVBOITFoundationDirectPrimitive& B)
	{
		return A.SubmissionOrder < B.SubmissionOrder;
	});
	return Out;
}

static FString FoundationOrderToString(EAVBOITFoundationDirectOrder Order)
{
	switch (Order)
	{
	case EAVBOITFoundationDirectOrder::BA: return TEXT("BA");
	case EAVBOITFoundationDirectOrder::RandomSeed1: return TEXT("RandomSeed1");
	case EAVBOITFoundationDirectOrder::RandomSeed2: return TEXT("RandomSeed2");
	case EAVBOITFoundationDirectOrder::RandomSeed3: return TEXT("RandomSeed3");
	case EAVBOITFoundationDirectOrder::AB:
	default: return TEXT("AB");
	}
}

static FString FoundationSceneToString(EAVBOITFoundationDirectScene Scene)
{
	return Scene == EAVBOITFoundationDirectScene::SingleLayerIdentity ? TEXT("SingleLayerIdentity") : TEXT("TwoIntersectingQuads");
}

static FString ActualOrderString(const FAVBOITFoundationDirectScene& Scene)
{
	FString Out;
	for (int32 Index = 0; Index < Scene.Primitives.Num(); ++Index)
	{
		if (Index > 0)
		{
			Out += TEXT(",");
		}
		Out += Scene.Primitives[Index].Id;
	}
	return Out;
}

static FAVBOITRasterDrawData CreateRasterDrawData(
	FRHICommandListImmediate& RHICmdList,
	const FAVBOITFoundationDirectPrimitive& Primitive)
{
	TResourceArray<FVector3f, VERTEXBUFFER_ALIGNMENT> Vertices;
	Vertices.SetNumUninitialized(4);
	for (int32 Index = 0; Index < 4; ++Index)
	{
		Vertices[Index] = Primitive.Vertices[Index];
	}

	FRHIBufferCreateDesc VBDesc = FRHIBufferCreateDesc::Create(*FString::Printf(TEXT("Foundation.%s.VB"), *Primitive.Id), Vertices.GetResourceDataSize(), 0, BUF_VertexBuffer | BUF_Static);
	VBDesc.SetInitialState(ERHIAccess::VertexOrIndexBuffer);
	FBufferRHIRef VertexBuffer = RHICmdList.CreateBuffer(VBDesc);
	void* VertexData = RHICmdList.LockBuffer(VertexBuffer, 0, Vertices.GetResourceDataSize(), RLM_WriteOnly);
	FMemory::Memcpy(VertexData, Vertices.GetData(), Vertices.GetResourceDataSize());
	RHICmdList.UnlockBuffer(VertexBuffer);

	TResourceArray<uint16, INDEXBUFFER_ALIGNMENT> Indices;
	Indices.SetNumUninitialized(6);
	Indices[0] = 0; Indices[1] = 1; Indices[2] = 2; Indices[3] = 2; Indices[4] = 1; Indices[5] = 3;
	FRHIBufferCreateDesc IBDesc = FRHIBufferCreateDesc::Create(*FString::Printf(TEXT("Foundation.%s.IB"), *Primitive.Id), Indices.GetResourceDataSize(), sizeof(uint16), BUF_IndexBuffer | BUF_Static);
	IBDesc.SetInitialState(ERHIAccess::VertexOrIndexBuffer);
	FBufferRHIRef IndexBuffer = RHICmdList.CreateBuffer(IBDesc);
	void* IndexData = RHICmdList.LockBuffer(IndexBuffer, 0, Indices.GetResourceDataSize(), RLM_WriteOnly);
	FMemory::Memcpy(IndexData, Indices.GetData(), Indices.GetResourceDataSize());
	RHICmdList.UnlockBuffer(IndexBuffer);

	FVertexDeclarationElementList Elements;
	Elements.Add(FVertexElement(0, 0, VET_Float3, 0, sizeof(FVector3f)));
	FVertexDeclarationRHIRef VertexDecl = RHICreateVertexDeclaration(Elements);

	FAVBOITRasterDrawData DrawData;
	DrawData.LocalToWorld = FMatrix::Identity;
	DrawData.Color = Primitive.Color;
	DrawData.Alpha = Primitive.Alpha;
	DrawData.VertexBufferRHI = VertexBuffer;
	DrawData.IndexBufferRHI = IndexBuffer;
	DrawData.VertexDeclaration = VertexDecl;
	DrawData.SubmissionOrder = Primitive.SubmissionOrder;
	DrawData.VertexCount = 4;
	DrawData.IndexCount = 6;
	DrawData.PrimitiveCount = 2;
	return DrawData;
}

static void AddFoundationIdentityPass(
	FRDGBuilder& GraphBuilder,
	FRDGTextureRef SceneDepth,
	FRDGTextureRef SceneColor,
	const FIntRect& ViewRect,
	const FIntVector4& ViewRectMin,
	const FIntVector4& ViewRectSize,
	const FMatrix44f& WorldToClip,
	const FMatrix44f& WorldToView,
	const TArray<FAVBOITRasterDrawData>& DrawDataArray)
{
	if (!SceneDepth || !SceneColor || DrawDataArray.IsEmpty())
	{
		return;
	}

	TArray<FAVBOITRasterIdentityPS::FParameters*> PSParamsArray;
	TArray<FAVBOITRasterIdentityVS::FParameters*> VSParamsArray;
	for (const FAVBOITRasterDrawData& DrawData : DrawDataArray)
	{
		auto* PSParams = GraphBuilder.AllocParameters<FAVBOITRasterIdentityPS::FParameters>();
		PSParams->ColorAndAlpha = FVector4f(DrawData.Color.R, DrawData.Color.G, DrawData.Color.B, DrawData.Alpha);
		PSParams->ViewRectMin = ViewRectMin;
		PSParams->ViewRectSize = ViewRectSize;
		PSParams->RenderTargets[0] = FRenderTargetBinding(SceneColor, ERenderTargetLoadAction::ELoad);
		PSParams->RenderTargets.DepthStencil = FDepthStencilBinding(SceneDepth, ERenderTargetLoadAction::ELoad, FExclusiveDepthStencil::DepthRead_StencilNop);
		PSParamsArray.Add(PSParams);

		auto* VSParams = GraphBuilder.AllocParameters<FAVBOITRasterIdentityVS::FParameters>();
		VSParams->LocalToWorld = FMatrix44f(DrawData.LocalToWorld);
		VSParams->WorldToClip = WorldToClip;
		VSParams->WorldToView = WorldToView;
		VSParams->ViewRectMin = ViewRectMin;
		VSParamsArray.Add(VSParams);
	}

	auto* PassParameters = GraphBuilder.AllocParameters<FAVBOITRasterIdentityPS::FParameters>();
	*PassParameters = *PSParamsArray[0];

	TShaderMapRef<FAVBOITRasterIdentityVS> VertexShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
	TShaderMapRef<FAVBOITRasterIdentityPS> PixelShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
	ClearUnusedGraphResources(PixelShader, PassParameters);
	const TArray<FAVBOITRasterDrawData> DrawDataCopy = DrawDataArray;

	GraphBuilder.AddPass(
		RDG_EVENT_NAME("AVBOIT.Foundation.PluginIdentity"),
		PassParameters,
		ERDGPassFlags::Raster,
		[VSParamsArray, PSParamsArray, ViewRect, DrawDataCopy, VertexShader, PixelShader](FRHICommandList& RHICmdList)
	{
		RHICmdList.SetViewport(ViewRect.Min.X, ViewRect.Min.Y, 0.0f, ViewRect.Max.X, ViewRect.Max.Y, 1.0f);

		FGraphicsPipelineStateInitializer GraphicsPSOInit;
		RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);
		GraphicsPSOInit.BlendState = TStaticBlendState<CW_RGB, BO_Add, BF_One, BF_InverseSourceAlpha, BO_Add, BF_Zero, BF_One>::GetRHI();
		GraphicsPSOInit.RasterizerState = TStaticRasterizerState<FM_Solid, CM_None>::GetRHI();
		GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_DepthNearOrEqual>::GetRHI();

		for (int32 Index = 0; Index < DrawDataCopy.Num(); ++Index)
		{
			GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = DrawDataCopy[Index].VertexDeclaration;
			GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
			GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
			GraphicsPSOInit.PrimitiveType = PT_TriangleList;

			SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit, 0);
			SetShaderParameters(RHICmdList, VertexShader, VertexShader.GetVertexShader(), *VSParamsArray[Index]);
			SetShaderParameters(RHICmdList, PixelShader, PixelShader.GetPixelShader(), *PSParamsArray[Index]);
			RHICmdList.SetStreamSource(0, DrawDataCopy[Index].VertexBufferRHI, 0);
			RHICmdList.DrawIndexedPrimitive(DrawDataCopy[Index].IndexBufferRHI, 0, 0, DrawDataCopy[Index].VertexCount, 0, DrawDataCopy[Index].PrimitiveCount, 1);
		}
	});
}

static bool PixelInFoundationCoverage(const FAVBOITFoundationDirectScene& Scene, int32 X, int32 Y)
{
	return Scene.RoiRect.Contains(FIntPoint(X, Y));
}

static float FoundationDepthAtPixel(const FAVBOITFoundationDirectPrimitive& Primitive, const FAVBOITFoundationDirectScene& Scene, int32 X)
{
	const float T = FMath::Clamp((X - Scene.RoiRect.Min.X) / FMath::Max(1.0f, static_cast<float>(Scene.RoiRect.Width())), 0.0f, 1.0f);
	return FMath::Lerp(Primitive.Vertices[0].Z, Primitive.Vertices[1].Z, T);
}

static TArray<FColor> GenerateExactReferencePixels(const FAVBOITFoundationDirectScene& Scene, int32* OutAFrontCount = nullptr, int32* OutBFrontCount = nullptr)
{
	if (OutAFrontCount) *OutAFrontCount = 0;
	if (OutBFrontCount) *OutBFrontCount = 0;

	TArray<FColor> Pixels;
	Pixels.SetNumZeroed(Scene.Resolution.X * Scene.Resolution.Y);

	for (int32 Y = 0; Y < Scene.Resolution.Y; ++Y)
	{
		for (int32 X = 0; X < Scene.Resolution.X; ++X)
		{
			FLinearColor Accum(0, 0, 0, 1);
			if (PixelInFoundationCoverage(Scene, X, Y))
			{
				TArray<const FAVBOITFoundationDirectPrimitive*> Layers;
				for (const FAVBOITFoundationDirectPrimitive& Primitive : Scene.Primitives)
				{
					Layers.Add(&Primitive);
				}
				Layers.Sort([&Scene, X](const FAVBOITFoundationDirectPrimitive& A, const FAVBOITFoundationDirectPrimitive& B)
				{
					return FoundationDepthAtPixel(A, Scene, X) < FoundationDepthAtPixel(B, Scene, X);
				});

				if (Layers.Num() >= 2)
				{
					if (Layers[0]->Id.StartsWith(TEXT("A_")) && OutAFrontCount) ++(*OutAFrontCount);
					if (Layers[0]->Id.StartsWith(TEXT("B_")) && OutBFrontCount) ++(*OutBFrontCount);
				}

				FVector3f Color = FVector3f::Zero();
				float RemainingT = 1.0f;
				for (const FAVBOITFoundationDirectPrimitive* Layer : Layers)
				{
					Color += FVector3f(Layer->Color.R, Layer->Color.G, Layer->Color.B) * Layer->Alpha * RemainingT;
					RemainingT *= (1.0f - Layer->Alpha);
				}
				Accum = FLinearColor(Color.X, Color.Y, Color.Z, 1.0f - RemainingT);
			}
			Pixels[Y * Scene.Resolution.X + X] = Accum.ToFColor(true);
			Pixels[Y * Scene.Resolution.X + X].A = 255;
		}
	}
	return Pixels;
}

static void CompareImages(
	const TArray<FColor>& A,
	const TArray<FColor>& B,
	FIntPoint Size,
	const FIntRect& Region,
	double& OutMae,
	double& OutMaxAbs,
	double& OutP95)
{
	TArray<double> Errors;
	double Sum = 0.0;
	double MaxAbs = 0.0;
	for (int32 Y = Region.Min.Y; Y < Region.Max.Y; ++Y)
	{
		for (int32 X = Region.Min.X; X < Region.Max.X; ++X)
		{
			const int32 Index = Y * Size.X + X;
			if (!A.IsValidIndex(Index) || !B.IsValidIndex(Index))
			{
				continue;
			}
			const double ER = FMath::Abs((double)A[Index].R - (double)B[Index].R) / 255.0;
			const double EG = FMath::Abs((double)A[Index].G - (double)B[Index].G) / 255.0;
			const double EB = FMath::Abs((double)A[Index].B - (double)B[Index].B) / 255.0;
			const double E = (ER + EG + EB) / 3.0;
			Sum += E;
			MaxAbs = FMath::Max(MaxAbs, FMath::Max3(ER, EG, EB));
			Errors.Add(E);
		}
	}
	Errors.Sort();
	OutMae = Errors.Num() > 0 ? Sum / Errors.Num() : 1.0;
	OutMaxAbs = MaxAbs;
	OutP95 = Errors.Num() > 0 ? Errors[FMath::Clamp(FMath::RoundToInt((Errors.Num() - 1) * 0.95f), 0, Errors.Num() - 1)] : 1.0;
}

static TArray<FColor> GenerateDifferencePixels(const TArray<FColor>& A, const TArray<FColor>& B, FIntPoint Size, bool bHeatmap)
{
	TArray<FColor> Out;
	Out.SetNumZeroed(Size.X * Size.Y);
	for (int32 Index = 0; Index < Out.Num(); ++Index)
	{
		if (!A.IsValidIndex(Index) || !B.IsValidIndex(Index))
		{
			continue;
		}
		const uint8 DR = static_cast<uint8>(FMath::Abs((int32)A[Index].R - (int32)B[Index].R));
		const uint8 DG = static_cast<uint8>(FMath::Abs((int32)A[Index].G - (int32)B[Index].G));
		const uint8 DB = static_cast<uint8>(FMath::Abs((int32)A[Index].B - (int32)B[Index].B));
		if (bHeatmap)
		{
			const uint8 V = FMath::Max3(DR, DG, DB);
			Out[Index] = FColor(V, 0, 255 - V, 255);
		}
		else
		{
			Out[Index] = FColor(DR, DG, DB, 255);
		}
	}
	return Out;
}

static TArray<FColor> GenerateMaskPixels(const FAVBOITFoundationDirectScene& Scene, bool bEqualDepthMask)
{
	TArray<FColor> Out;
	Out.SetNumZeroed(Scene.Resolution.X * Scene.Resolution.Y);
	const int32 CenterX = (Scene.RoiRect.Min.X + Scene.RoiRect.Max.X) / 2;
	for (int32 Y = 0; Y < Scene.Resolution.Y; ++Y)
	{
		for (int32 X = 0; X < Scene.Resolution.X; ++X)
		{
			const bool bCoverage = PixelInFoundationCoverage(Scene, X, Y);
			const bool bEqualBand = bCoverage && FMath::Abs(X - CenterX) <= 3;
			const bool bOn = bEqualDepthMask ? bEqualBand : bCoverage;
			Out[Y * Scene.Resolution.X + X] = bOn ? FColor::White : FColor::Black;
			Out[Y * Scene.Resolution.X + X].A = 255;
		}
	}
	return Out;
}

static void SaveFoundationImageManifest(
	const FString& PngPath,
	const FString& Mode,
	const FString& Scene,
	const FString& Order,
	const FString& ActualDrawOrder,
	const FString& Source,
	FIntPoint Resolution)
{
	const FString ManifestPath = FPaths::ChangeExtension(PngPath, TEXT(".Manifest.json"));
	const FString Json = FString::Printf(
		TEXT("{\"SchemaVersion\":1,\"ScreenshotSource\":\"%s\",\"DesktopScreenshotUsed\":false,\"Resolution\":{\"X\":%d,\"Y\":%d},\"ViewRect\":{\"MinX\":0,\"MinY\":0,\"MaxX\":%d,\"MaxY\":%d},\"Mode\":\"%s\",\"Scene\":\"%s\",\"SubmissionOrder\":\"%s\",\"ActualDrawOrder\":\"%s\",\"RHI\":\"%s\",\"GPU\":\"%s\"}"),
		*JsonEscape(Source),
		Resolution.X,
		Resolution.Y,
		Resolution.X,
		Resolution.Y,
		*JsonEscape(Mode),
		*JsonEscape(Scene),
		*JsonEscape(Order),
		*JsonEscape(ActualDrawOrder),
		GDynamicRHI ? *JsonEscape(GDynamicRHI->GetName()) : TEXT("None"),
		*JsonEscape(GRHIAdapterName));
	SaveJsonAtomic(ManifestPath, Json);
}

struct FAVBOITFoundationRenderResult
{
	bool bSucceeded = false;
	FString FailureReason;
	TArray<FColor> CompositePixels;
	TArray<FColor> ColorAccumulationPixels;
	TArray<FColor> ResolvedAlphaPixels;
	FIntPoint Size = FIntPoint::ZeroValue;
};

static FAVBOITFoundationRenderResult RenderFoundationDirectMode(
	const FAVBOITFoundationDirectScene& Scene,
	bool bPluginIdentity)
{
	FAVBOITFoundationRenderResult Result;
	TSharedRef<FAVBOITFoundationImageReadback, ESPMode::ThreadSafe> CompositeReadback = MakeShared<FAVBOITFoundationImageReadback, ESPMode::ThreadSafe>();
	TSharedRef<FAVBOITFoundationImageReadback, ESPMode::ThreadSafe> ColorReadback = MakeShared<FAVBOITFoundationImageReadback, ESPMode::ThreadSafe>();
	TSharedRef<FAVBOITFoundationImageReadback, ESPMode::ThreadSafe> AlphaReadback = MakeShared<FAVBOITFoundationImageReadback, ESPMode::ThreadSafe>();

	ENQUEUE_RENDER_COMMAND(RenderFoundationDirectMode)(
		[Scene, bPluginIdentity, CompositeReadback, ColorReadback, AlphaReadback](FRHICommandListImmediate& RHICmdList)
	{
		FRDGBuilder GraphBuilder(RHICmdList);

		FRDGTextureDesc ColorDesc = FRDGTextureDesc::Create2D(Scene.Resolution, PF_FloatRGBA, FClearValueBinding::Black, TexCreate_ShaderResource | TexCreate_RenderTargetable | TexCreate_UAV);
		FRDGTextureRef SceneColor = GraphBuilder.CreateTexture(ColorDesc, TEXT("Foundation.SceneColor"));
		AddClearRenderTargetPass(GraphBuilder, SceneColor, FLinearColor::Black);

		FRDGTextureDesc DepthDesc = FRDGTextureDesc::Create2D(Scene.Resolution, PF_DepthStencil, FClearValueBinding::DepthFar, TexCreate_DepthStencilTargetable | TexCreate_ShaderResource);
		FRDGTextureRef SceneDepth = GraphBuilder.CreateTexture(DepthDesc, TEXT("Foundation.SceneDepth"));
		AddClearDepthStencilPass(GraphBuilder, SceneDepth, true, 0.0f, false, 0);

		TArray<FAVBOITRasterDrawData> DrawDataArray;
		DrawDataArray.Reserve(Scene.Primitives.Num());
		for (const FAVBOITFoundationDirectPrimitive& Primitive : Scene.Primitives)
		{
			DrawDataArray.Add(CreateRasterDrawData(RHICmdList, Primitive));
		}

		const FMatrix44f WorldToClip = MakeFoundationClipMatrix();
		const FMatrix44f WorldToView = FMatrix44f(FMatrix::Identity);
		const FIntVector4 ViewRectMin(Scene.ViewRect.Min.X, Scene.ViewRect.Min.Y, 0, 0);
		const FIntVector4 ViewRectSize(Scene.ViewRect.Width(), Scene.ViewRect.Height(), 0, 0);

		FRDGTextureRef CompositeTexture = SceneColor;
		FRDGTextureRef ColorAccumulationTexture = nullptr;
		FRDGTextureRef ResolvedAlphaTexture = nullptr;
		if (bPluginIdentity)
		{
			AddFoundationIdentityPass(GraphBuilder, SceneDepth, SceneColor, Scene.ViewRect, ViewRectMin, ViewRectSize, WorldToClip, WorldToView, DrawDataArray);
		}
		else
		{
			FAVBOITRasterPassInputs PassInputs;
			PassInputs.TextureExtent = Scene.Resolution;
			PassInputs.ViewRect = Scene.ViewRect;
			PassInputs.Config = FAVBOITFrameConfig::Build(Scene.ViewRect, Scene.Resolution, Scene.NearDepthCm, Scene.FarDepthCm, 8, 64);
			PassInputs.WorldToClip = WorldToClip;
			PassInputs.WorldToView = WorldToView;
			PassInputs.ZNear = Scene.NearDepthCm;
			PassInputs.ZFar = Scene.FarDepthCm;
			PassInputs.SceneDepth = SceneDepth;
			PassInputs.SceneColor = SceneColor;
			PassInputs.DrawData = DrawDataArray;
			FAVBOITRasterPassOutputs Outputs = FAVBOITRasterRenderer::AddCorePasses(GraphBuilder, PassInputs);
			CompositeTexture = Outputs.CompositeOutput ? Outputs.CompositeOutput : SceneColor;
			ColorAccumulationTexture = Outputs.ColorAccumulation;
			ResolvedAlphaTexture = Outputs.AlphaAccumulation;
		}

		AddTextureReadbackPass(GraphBuilder, CompositeTexture, Scene.ViewRect, CompositeReadback);
		if (ColorAccumulationTexture)
		{
			AddTextureReadbackPass(GraphBuilder, ColorAccumulationTexture, Scene.ViewRect, ColorReadback);
		}
		if (ResolvedAlphaTexture)
		{
			AddTextureReadbackPass(GraphBuilder, ResolvedAlphaTexture, Scene.ViewRect, AlphaReadback);
		}

		GraphBuilder.Execute();
	});
	FlushRenderingCommands();

	if (!CompositeReadback->bPerformed)
	{
		Result.FailureReason = TEXT("Composite full-image readback did not complete");
		return Result;
	}

	Result.bSucceeded = true;
	Result.CompositePixels = CompositeReadback->Pixels;
	Result.ColorAccumulationPixels = ColorReadback->Pixels;
	Result.ResolvedAlphaPixels = AlphaReadback->Pixels;
	Result.Size = CompositeReadback->Size;
	return Result;
}

static int32 RunFoundationVisualSuite(const FString& EvidenceRoot, const FString& RunId)
{
	int32 ResX = 1920;
	int32 ResY = 1080;
	FParse::Value(FCommandLine::Get(), TEXT("resx="), ResX);
	FParse::Value(FCommandLine::Get(), TEXT("resy="), ResY);
	const FIntPoint Resolution(FMath::Max(64, ResX), FMath::Max(64, ResY));

	IFileManager::Get().MakeDirectory(*(EvidenceRoot / TEXT("Raw")), true);
	IFileManager::Get().MakeDirectory(*(EvidenceRoot / TEXT("Derived")), true);
	IFileManager::Get().MakeDirectory(*(EvidenceRoot / TEXT("Metrics")), true);
	IFileManager::Get().MakeDirectory(*(EvidenceRoot / TEXT("Readback")), true);

	TArray<FString> BlockingReasons;
	FString MetricsJson = FString::Printf(TEXT("{\"SchemaVersion\":1,\"RunId\":\"%s\",\"SuiteName\":\"FoundationVisual\",\"Images\":["), *JsonEscape(RunId));

	auto AddImageRecord = [&MetricsJson](const FString& Name, bool bReadback, const FString& Mode, const FString& Order, const FString& ActualOrder)
	{
		if (!MetricsJson.EndsWith(TEXT("[")))
		{
			MetricsJson += TEXT(",");
		}
		MetricsJson += FString::Printf(TEXT("{\"Name\":\"%s\",\"Readback\":%s,\"Mode\":\"%s\",\"SubmissionOrder\":\"%s\",\"ActualDrawOrder\":\"%s\"}"),
			*JsonEscape(Name), bReadback ? TEXT("true") : TEXT("false"), *JsonEscape(Mode), *JsonEscape(Order), *JsonEscape(ActualOrder));
	};

	const FAVBOITFoundationDirectScene ExactScene = BuildFoundationDirectScene(EAVBOITFoundationDirectScene::TwoIntersectingQuads, EAVBOITFoundationDirectOrder::AB, Resolution);
	int32 AFrontCount = 0;
	int32 BFrontCount = 0;
	const TArray<FColor> ExactPixels = GenerateExactReferencePixels(ExactScene, &AFrontCount, &BFrontCount);
	const FString ExactPath = EvidenceRoot / TEXT("Raw/00_ExactReference_AB.png");
	SaveColorPng(ExactPath, ExactPixels, Resolution);
	SaveFoundationImageManifest(ExactPath, TEXT("ExactReference"), FoundationSceneToString(ExactScene.Scene), TEXT("AB"), TEXT("CPUDepthSort"), TEXT("CPUExactReference"), Resolution);
	AddImageRecord(TEXT("00_ExactReference_AB.png"), false, TEXT("ExactReference"), TEXT("AB"), TEXT("CPUDepthSort"));

	TMap<FString, TArray<FColor>> Captured;
	auto CaptureMode = [&](const FString& FileName, bool bIdentity, EAVBOITFoundationDirectOrder Order)
	{
		const FAVBOITFoundationDirectScene Scene = BuildFoundationDirectScene(EAVBOITFoundationDirectScene::TwoIntersectingQuads, Order, Resolution);
		const FString ActualOrder = ActualOrderString(Scene);
		FAVBOITFoundationRenderResult Render = RenderFoundationDirectMode(Scene, bIdentity);
		const FString ModeName = bIdentity ? TEXT("PluginIdentity") : TEXT("PluginAVBOIT");
		const FString OrderName = FoundationOrderToString(Order);
		if (!Render.bSucceeded)
		{
			BlockingReasons.Add(FString::Printf(TEXT("%s failed: %s"), *FileName, *Render.FailureReason));
			AddImageRecord(FileName, false, ModeName, OrderName, ActualOrder);
			return;
		}

		const FString RawPath = EvidenceRoot / TEXT("Raw") / FileName;
		SaveColorPng(RawPath, Render.CompositePixels, Render.Size);
		SaveFoundationImageManifest(RawPath, ModeName, FoundationSceneToString(Scene.Scene), OrderName, ActualOrder, TEXT("RDGTextureReadback"), Render.Size);
		Captured.Add(FileName, Render.CompositePixels);
		AddImageRecord(FileName, true, ModeName, OrderName, ActualOrder);

		if (!bIdentity && Order == EAVBOITFoundationDirectOrder::AB)
		{
			if (Render.ColorAccumulationPixels.Num() == Render.Size.X * Render.Size.Y)
			{
				const FString ColorPath = EvidenceRoot / TEXT("Raw/14_ColorAccumulationOverview.png");
				SaveColorPng(ColorPath, Render.ColorAccumulationPixels, Render.Size);
				SaveFoundationImageManifest(ColorPath, TEXT("BufferOverview"), FoundationSceneToString(Scene.Scene), OrderName, ActualOrder, TEXT("RDGColorAccumulationReadback"), Render.Size);
				AddImageRecord(TEXT("14_ColorAccumulationOverview.png"), true, TEXT("BufferOverview"), OrderName, ActualOrder);
			}
			if (Render.ResolvedAlphaPixels.Num() == Render.Size.X * Render.Size.Y)
			{
				const FString AlphaPath = EvidenceRoot / TEXT("Raw/15_ResolvedAlphaOverview.png");
				SaveColorPng(AlphaPath, Render.ResolvedAlphaPixels, Render.Size);
				SaveFoundationImageManifest(AlphaPath, TEXT("BufferOverview"), FoundationSceneToString(Scene.Scene), OrderName, ActualOrder, TEXT("RDGResolvedAlphaReadback"), Render.Size);
				AddImageRecord(TEXT("15_ResolvedAlphaOverview.png"), true, TEXT("BufferOverview"), OrderName, ActualOrder);
			}
		}
	};

	CaptureMode(TEXT("05_PluginIdentity_AB.png"), true, EAVBOITFoundationDirectOrder::AB);
	CaptureMode(TEXT("06_PluginIdentity_BA.png"), true, EAVBOITFoundationDirectOrder::BA);
	CaptureMode(TEXT("07_PluginAVBOIT_AB.png"), false, EAVBOITFoundationDirectOrder::AB);
	CaptureMode(TEXT("08_PluginAVBOIT_BA.png"), false, EAVBOITFoundationDirectOrder::BA);
	CaptureMode(TEXT("09_PluginAVBOIT_RandomSeed1.png"), false, EAVBOITFoundationDirectOrder::RandomSeed1);
	CaptureMode(TEXT("10_PluginAVBOIT_RandomSeed2.png"), false, EAVBOITFoundationDirectOrder::RandomSeed2);
	CaptureMode(TEXT("11_PluginAVBOIT_RandomSeed3.png"), false, EAVBOITFoundationDirectOrder::RandomSeed3);

	SaveColorPng(EvidenceRoot / TEXT("Derived/CoverageMask.png"), GenerateMaskPixels(ExactScene, false), Resolution);
	SaveColorPng(EvidenceRoot / TEXT("Derived/EqualDepthExclusionMask.png"), GenerateMaskPixels(ExactScene, true), Resolution);

	double AvboitExactMae = 1.0;
	double AvboitExactMax = 1.0;
	double AvboitExactP95 = 1.0;
	double OrderMae = 1.0;
	double OrderMax = 1.0;
	double OrderP95 = 1.0;
	if (const TArray<FColor>* AvboitAB = Captured.Find(TEXT("07_PluginAVBOIT_AB.png")))
	{
		CompareImages(*AvboitAB, ExactPixels, Resolution, ExactScene.RoiRect, AvboitExactMae, AvboitExactMax, AvboitExactP95);
		SaveColorPng(EvidenceRoot / TEXT("Derived/17_AVBOIT_vs_Exact_Difference.png"), GenerateDifferencePixels(*AvboitAB, ExactPixels, Resolution, false), Resolution);
		SaveColorPng(EvidenceRoot / TEXT("Derived/18_AVBOIT_vs_Exact_Heatmap.png"), GenerateDifferencePixels(*AvboitAB, ExactPixels, Resolution, true), Resolution);
	}
	if (const TArray<FColor>* AvboitAB = Captured.Find(TEXT("07_PluginAVBOIT_AB.png")))
	{
		if (const TArray<FColor>* AvboitBA = Captured.Find(TEXT("08_PluginAVBOIT_BA.png")))
		{
			CompareImages(*AvboitAB, *AvboitBA, Resolution, ExactScene.RoiRect, OrderMae, OrderMax, OrderP95);
			SaveColorPng(EvidenceRoot / TEXT("Derived/19_OrderAB_vs_BA_Difference.png"), GenerateDifferencePixels(*AvboitAB, *AvboitBA, Resolution, false), Resolution);
		}
	}

	MetricsJson += FString::Printf(
		TEXT("],\"AFrontPixelCount\":%d,\"BFrontPixelCount\":%d,\"AVBOIT_vs_Exact\":{\"RGB_MAE\":%.9f,\"MaxAbs\":%.9f,\"P95Abs\":%.9f},\"OrderAB_vs_BA\":{\"RGB_MAE\":%.9f,\"MaxAbs\":%.9f,\"P95Abs\":%.9f},\"BlockingReasons\":["),
		AFrontCount,
		BFrontCount,
		AvboitExactMae,
		AvboitExactMax,
		AvboitExactP95,
		OrderMae,
		OrderMax,
		OrderP95);
	for (int32 Index = 0; Index < BlockingReasons.Num(); ++Index)
	{
		if (Index > 0)
		{
			MetricsJson += TEXT(",");
		}
		MetricsJson += FString::Printf(TEXT("\"%s\""), *JsonEscape(BlockingReasons[Index]));
	}
	MetricsJson += TEXT("]}");
	SaveJsonAtomic(EvidenceRoot / TEXT("Metrics/FoundationVisualMetrics.json"), MetricsJson);

	return BlockingReasons.Num() == 0 ? 0 : 1;
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
	Result.AbsoluteError = 0.0f;
	Result.Tolerance = 0.0f;
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
			PassInputs.Config = FAVBOITFrameConfig::Build(Case.ViewRect, Case.Resolution, Case.ZNear, Case.ZFar, 1, 64);
			
			
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
			DebugParams->VolumeExtent = Case.Resolution;
			DebugParams->ViewRectMin = Case.ViewRect.Min;
			DebugParams->ViewRectMax = Case.ViewRect.Max;
			DebugParams->NumSlices = 64;
			DebugParams->DownsampleFactor = 1;
			DebugParams->OverflowCounter = GraphBuilder.CreateSRV(Outputs.OverflowCounter, PF_R32_UINT);
			DebugParams->OutOfBoundsCounter = GraphBuilder.CreateSRV(Outputs.OutOfBoundsCounter, PF_R32_UINT);

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
	FString SuiteName;
	FParse::Value(FCommandLine::Get(), TEXT("AVBOITSuite="), SuiteName);

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

	int32 FoundationVisualExit = 0;
	if (SuiteName == TEXT("FoundationQuad"))
	{
		FoundationVisualExit = RunFoundationVisualSuite(EvidenceRoot, RunId);
	}
	
	IFileManager::Get().Delete(*(EvidenceRoot / TEXT("RUNNING.marker")));
	SaveJsonAtomic(EvidenceRoot / TEXT("SUITE_FINISHED.marker"), TEXT("suite-finished"));

	const bool bPassed = DirectFailed == 0 && SynthFailed == 0 && FoundationVisualExit == 0;
	if (!bPassed)
	{
		SaveJsonAtomic(EvidenceRoot / TEXT("FAILED.marker"), TEXT("failed"));
	}

	UE_LOG(LogTemp, Warning, TEXT("AVBOIT: UAVBOITHeadlessCommandlet Finished. Passed: %d, Failed: %d (Synth Failed: %d, FoundationVisualExit: %d)"), DirectPassed, DirectFailed, SynthFailed, FoundationVisualExit);
	
	if (!bPassed)
	{
		return 1;
	}

	return 0; // Success
}

