// Copyright

#include "AVBOIT/Raster/AVBOITTestMeshSceneProxy.h"
#include "Materials/Material.h"

FAVBOITTestMeshSceneProxy::FAVBOITTestMeshSceneProxy(UAVBOITTestMeshComponent* Component)
	: FPrimitiveSceneProxy(Component)
	, MaterialParams(Component->MaterialParams)
	, bIsTransparent(Component->bIsTransparent)
	, SubmissionOrder(Component->SubmissionOrder)
{
	bWillEverBeLit = false;
}

FAVBOITTestMeshSceneProxy::~FAVBOITTestMeshSceneProxy()
{
}

void FAVBOITTestMeshSceneProxy::GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const
{
	// We don't draw via standard UE paths! We just provide a proxy for AVBOIT subsystem to collect and draw manually in RDG passes.
	// But we must exist in the scene to be tracked by our own custom pass.
}

FPrimitiveViewRelevance FAVBOITTestMeshSceneProxy::GetViewRelevance(const FSceneView* View) const
{
	FPrimitiveViewRelevance Result;
	Result.bDrawRelevance = IsShown(View);
	Result.bShadowRelevance = IsShadowCast(View);
	Result.bDynamicRelevance = true;
	Result.bRenderInMainPass = false; // We draw in our custom pass!
	// But wait, if it's an opaque occluder, maybe we draw it in main pass?
	if (!bIsTransparent)
	{
		Result.bRenderInMainPass = true;
		Result.bOpaque = true;
	}
	else
	{
		Result.bNormalTranslucency = true; // Just to be safe for visibility
	}
	return Result;
}

uint32 FAVBOITTestMeshSceneProxy::GetMemoryFootprint() const
{
	return sizeof(*this) + GetAllocatedSize();
}

SIZE_T FAVBOITTestMeshSceneProxy::GetTypeHash() const
{
	static size_t UniquePointer;
	return reinterpret_cast<size_t>(&UniquePointer);
}
