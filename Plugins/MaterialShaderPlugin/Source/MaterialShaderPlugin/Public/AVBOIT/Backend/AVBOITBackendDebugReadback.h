#pragma once

#include "CoreMinimal.h"
#include "RenderGraph.h"

struct FAVBOITMappingTestResult
{
    TArray<float> NormalizedDepths;
    TArray<uint32> SliceIndices;
};

struct FAVBOITSliceLineReadbacks
{
    FRHIGPUBufferReadback* Extinction = nullptr;
    FRHIGPUBufferReadback* Transmittance = nullptr;
};

class FAVBOITBackendDebugReadback
{
public:
    static FAVBOITMappingTestResult RunMappingTestSync(const TArray<float>& LinearDepths, float ZNear, float ZFar);
    static FAVBOITSliceLineReadbacks EnqueueExtractSliceLine(FRDGBuilder& GraphBuilder, FRDGBufferRef ExtinctionVolume, FRDGTextureRef TransmittanceVolume, FIntPoint CenterPixel, FIntPoint ViewResolution);
};
