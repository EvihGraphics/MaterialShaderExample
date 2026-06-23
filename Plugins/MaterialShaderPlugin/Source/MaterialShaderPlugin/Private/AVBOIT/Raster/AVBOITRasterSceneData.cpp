// Copyright

#include "AVBOITRasterSceneData.h"
#include "AVBOIT/Raster/AVBOITTestMeshSceneProxy.h"

FAVBOITRasterSceneData& FAVBOITRasterSceneData::Get()
{
	static FAVBOITRasterSceneData Instance;
	return Instance;
}

void FAVBOITRasterSceneData::RegisterProxy(FAVBOITTestMeshSceneProxy* Proxy)
{
	check(IsInRenderingThread());
	ActiveProxies.AddUnique(Proxy);
}

void FAVBOITRasterSceneData::UnregisterProxy(FAVBOITTestMeshSceneProxy* Proxy)
{
	check(IsInRenderingThread());
	ActiveProxies.RemoveSingle(Proxy);
}
