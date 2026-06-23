#pragma once

#include "CoreMinimal.h"
#include "RenderGraph.h"

struct FAVBOITReadbackResult
{
    bool bReady = false;
    TArray<FFloat16Color> Pixels;
    FIntPoint Size;
};

class FAVBOITBackendReadback
{
public:
    static FRHIGPUTextureReadback* EnqueueReadback(FRDGBuilder& GraphBuilder, FRDGTextureRef ResultTexture);
};
