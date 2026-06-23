// Copyright

#pragma once

#include "CoreMinimal.h"
#include "PrimitiveSceneProxy.h"
#include "AVBOIT/Raster/AVBOITTestMeshComponent.h"
#include "MaterialShared.h"

class FAVBOITTestMeshSceneProxy : public FPrimitiveSceneProxy
{
public:
	FAVBOITTestMeshSceneProxy(UAVBOITTestMeshComponent* Component);

	virtual ~FAVBOITTestMeshSceneProxy() override;

	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const override;
	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override;
	virtual uint32 GetMemoryFootprint() const override;
	virtual SIZE_T GetTypeHash() const override;

	FAVBOITRasterMaterialParameters MaterialParams;
	bool bIsTransparent;
	int32 SubmissionOrder;
};
