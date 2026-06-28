// Copyright

#pragma once

#include "CoreMinimal.h"
#include "RenderGraphBuilder.h"
#include "ScreenPass.h"

class FSceneView;
struct FPostProcessingInputs;

class FAVBOITRasterRenderer
{
public:
	static void AddPasses(
		FRDGBuilder& GraphBuilder,
		const FSceneView& View,
		const FPostProcessingInputs& Inputs,
		FScreenPassRenderTarget& Output);
};
