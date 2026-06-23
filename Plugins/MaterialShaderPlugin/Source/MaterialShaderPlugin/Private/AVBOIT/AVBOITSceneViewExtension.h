// Copyright Voxel Plugin SAS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneViewExtension.h"

class FAVBOITSceneViewExtension : public FSceneViewExtensionBase
{
public:
	explicit FAVBOITSceneViewExtension(const FAutoRegister& AutoRegister)
		: FSceneViewExtensionBase(AutoRegister)
	{
	}

	//~ Begin ISceneViewExtension Interface
	virtual void SubscribeToPostProcessingPass(
		EPostProcessingPass Pass,
		const FSceneView& InView,
		FPostProcessingPassDelegateArray& InOutPassCallbacks,
		bool bIsPassEnabled) override;
	virtual void PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessingInputs& Inputs) override;
	//~ End ISceneViewExtension Interface

	// Required pure virtual overrides from FSceneViewExtensionBase
	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override {}
	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override {}
	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override {}
	virtual void PreRenderViewFamily_RenderThread(FRDGBuilder& GraphBuilder, FSceneViewFamily& InViewFamily) override {}
	virtual void PreRenderView_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView) override {}

private:
	FScreenPassTexture AddSmokePass(
		FRDGBuilder& GraphBuilder,
		const FSceneView& View,
		const FPostProcessMaterialInputs& Inputs) const;
};
