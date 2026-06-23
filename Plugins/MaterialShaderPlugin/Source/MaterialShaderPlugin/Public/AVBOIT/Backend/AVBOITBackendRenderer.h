#pragma once

#include "CoreMinimal.h"
#include "RenderGraph.h"
#include "AVBOIT/Backend/AVBOITBackendTypes.h"

class FAVBOITBackendRenderer
{
public:
    static FRHIGPUTextureReadback* Execute(FRDGBuilder& GraphBuilder, const FAVBOITBackendSettings& Settings, const TArray<FAVBOITInjectedFragment>& InjectedFragments);
};
