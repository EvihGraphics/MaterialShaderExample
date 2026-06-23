// Copyright

#pragma once

#include "CoreMinimal.h"

class FAVBOITTestMeshSceneProxy;

class FAVBOITRasterSceneData
{
public:
	static FAVBOITRasterSceneData& Get();

	void RegisterProxy(FAVBOITTestMeshSceneProxy* Proxy);
	void UnregisterProxy(FAVBOITTestMeshSceneProxy* Proxy);

	const TArray<FAVBOITTestMeshSceneProxy*>& GetProxies() const { return ActiveProxies; }

private:
	TArray<FAVBOITTestMeshSceneProxy*> ActiveProxies;
};
