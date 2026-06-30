#pragma once

#include "CoreMinimal.h"
#include "SceneViewExtension.h"

class FAVBOITNiagaraSceneViewExtension final : public FSceneViewExtensionBase
{
public:
	explicit FAVBOITNiagaraSceneViewExtension(const FAutoRegister& AutoRegister)
		: FSceneViewExtensionBase(AutoRegister)
	{
	}

	virtual void PrePostProcessPass_RenderThread(
		FRDGBuilder& GraphBuilder,
		const FSceneView& View,
		const FPostProcessingInputs& Inputs) override;

	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override {}
	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override {}
	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override {}
	virtual void PreRenderViewFamily_RenderThread(FRDGBuilder& GraphBuilder, FSceneViewFamily& InViewFamily) override {}
	virtual void PreRenderView_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView) override {}
};
