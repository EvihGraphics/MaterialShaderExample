// Copyright

#pragma once

#include "CoreMinimal.h"
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
};

struct FAVBOITRasterPassInputs
{
	FIntPoint TextureExtent;
	FIntRect ViewRect;
	FMatrix44f WorldToView;
	FMatrix44f WorldToClip;
	float ZNear;
	float ZFar;
	FRDGTextureRef SceneDepth;
	FRDGTextureRef SceneColor;
	FRDGBufferRef FragmentCoverageCounter; // Testing
	TArray<FAVBOITRasterDrawData> DrawData;
};

struct FAVBOITRasterPassOutputs
{
	FRDGBufferRef ExtinctionVolume = nullptr;
	FRDGTextureRef TransmittanceVolume;
	FRDGTextureRef ColorAccumulation;
	FRDGTextureRef CompositeOutput;
};

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

