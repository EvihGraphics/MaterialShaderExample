// Copyright

#pragma once

#include "CoreMinimal.h"
#include "AVBOIT/Core/AVBOITFrameConfig.h"
#include "AVBOIT/Core/AVBOITPrimitivePacket.h"
#include "RenderGraphBuilder.h"
#include "ScreenPass.h"

class FSceneView;
struct FPostProcessingInputs;

struct FAVBOITRasterDrawData
{
	FMatrix LocalToWorld;
	FLinearColor Color;
	float Alpha;
	FBufferRHIRef VertexBufferRHI;
	FBufferRHIRef IndexBufferRHI;
	FVertexDeclarationRHIRef VertexDeclaration;
	uint32 SubmissionOrder = 0;
	uint32 VertexCount = 4;
	uint32 IndexCount = 6;
	uint32 PrimitiveCount = 2;

	FAVBOITPrimitivePacket ToPrimitivePacket() const
	{
		FAVBOITPrimitivePacket Packet;
		Packet.LocalToWorld = FMatrix44f(LocalToWorld);
		Packet.LinearColor = Color;
		Packet.Alpha = Alpha;
		Packet.SubmissionOrder = SubmissionOrder;
		Packet.VertexBuffer = VertexBufferRHI;
		Packet.IndexBuffer = IndexBufferRHI;
		Packet.VertexDeclaration = VertexDeclaration;
		Packet.VertexCount = VertexCount;
		Packet.IndexCount = IndexCount;
		Packet.PrimitiveCount = PrimitiveCount;
		return Packet;
	}
};

struct FAVBOITRasterPassInputs
{
	FAVBOITFrameConfig Config;
	FIntPoint TextureExtent;
	FIntRect ViewRect;
	FMatrix44f WorldToView;
	FMatrix44f WorldToClip;
	float ZNear;
	float ZFar;
	FRDGTextureRef SceneDepth;
	FRDGTextureRef SceneColor;
	FRDGBufferRef FragmentCoverageCounter = nullptr; // Testing
	FRDGBufferRef RasterDebugPixelBuffer = nullptr; // Testing
	FIntPoint DebugPixel = FIntPoint(-1, -1); // Testing
	TArray<FAVBOITRasterDrawData> DrawData;
};

struct FAVBOITRasterPassOutputs
{
	FRDGBufferRef ExtinctionVolume = nullptr;
	FRDGTextureRef TransmittanceVolume;
	FRDGTextureRef ColorAccumulation;
	FRDGTextureRef AlphaAccumulation;
	FRDGBufferRef OverflowCounter = nullptr;
	FRDGBufferRef OutOfBoundsCounter = nullptr;
	FRDGTextureRef CompositeOutput;
};


#if WITH_EDITOR || WITH_DEV_AUTOMATION_TESTS

enum class EAVBOITRasterSkipReason
{
	Disabled,
	NoProxies,
	InvalidSceneTextures,
	ViewRejected,
	InvalidViewRect,
	Executed
};

class FRHIGPUBufferReadback;

struct FAVBOITRasterExecutionProbe
{
	uint32 FrameNumber = 0;
	FString WorldName;
	FString Context;
	FIntRect ViewRect;
	FIntPoint TextureExtent;
	
	bool bRasterEnabled = false;
	int32 RegistryProxyCount = 0;
	int32 AcceptedProxyCount = 0;
	
	bool bClearPassScheduled = false;
	bool bSplatPassScheduled = false;
	bool bIntegratePassScheduled = false;
	bool bForwardPassScheduled = false;
	bool bCompositePassScheduled = false;
	
	int32 SplatDrawCount = 0;
	int32 ForwardDrawCount = 0;
	int32 CompositeDrawCount = 0;
	
	bool bDebugReadbackScheduled = false;
	EAVBOITRasterSkipReason SkipReason = EAVBOITRasterSkipReason::Disabled;

	int32 ObservedViewCount = 0;
	int32 RejectedViewCount = 0;
	int32 AcceptedViewCount = 0;
	FIntRect AcceptedViewRect;

	// Request specific pixel to readback
	FIntPoint RequestedDebugPixel = FIntPoint(-1, -1);
	bool bReadbackReady = false;
	struct FAVBOITRasterDebugPayload* ReadbackPayload = nullptr;
	FRHIGPUBufferReadback* PendingReadback = nullptr;
};

extern MATERIALSHADEREXAMPLE_API FAVBOITRasterExecutionProbe* GAVBOITRasterProbe;

#endif

class MATERIALSHADEREXAMPLE_API FAVBOITRasterRenderer
{
public:
	static bool IsEnabled();

	static void AddPasses(
		FRDGBuilder& GraphBuilder,
		const FSceneView& View,
		const FPostProcessingInputs& Inputs);

	static FAVBOITRasterPassOutputs AddCorePasses(
		FRDGBuilder& GraphBuilder,
		const FAVBOITRasterPassInputs& Inputs);
};

