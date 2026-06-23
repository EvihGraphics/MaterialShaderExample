#pragma once

#include "CoreMinimal.h"
#include "RenderGraph.h"
#include "AVBOIT/Backend/AVBOITBackendTypes.h"

struct FAVBOITBackendReadbacks
{
    FRHIGPUTextureReadback* Result = nullptr;
    FRHIGPUBufferReadback* ExtinctionLine = nullptr;
    FRHIGPUBufferReadback* TransmittanceLine = nullptr;
};

class FAVBOITBackendRenderer
{
public:
    static FAVBOITBackendReadbacks Execute(FRDGBuilder& GraphBuilder, const FAVBOITBackendSettings& Settings, const TArray<FAVBOITInjectedFragment>& InjectedFragments);
};
