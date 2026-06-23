// Copyright

#include "AVBOIT/Raster/AVBOITRasterRenderer.h"
#include "AVBOIT/Raster/AVBOITRasterSceneData.h"
#include "AVBOIT/Raster/AVBOITTestMeshSceneProxy.h"
#include "RenderGraphUtils.h"
#include "SceneView.h"

void FAVBOITRasterRenderer::AddPasses(
	FRDGBuilder& GraphBuilder,
	const FSceneView& View,
	const FPostProcessingInputs& Inputs,
	FScreenPassRenderTarget& Output)
{
	const TArray<FAVBOITTestMeshSceneProxy*>& Proxies = FAVBOITRasterSceneData::Get().GetProxies();
	if (Proxies.IsEmpty())
	{
		return;
	}

	RDG_EVENT_SCOPE(GraphBuilder, "AVBOIT.Raster");

	// For now, just a stub that proves we reached this point.
}
