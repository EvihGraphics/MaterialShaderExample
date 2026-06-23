#include "AVBOIT/Backend/AVBOITBackendReadback.h"
#include "RenderGraphUtils.h"
#include "RHIGPUReadback.h"

FRHIGPUTextureReadback* FAVBOITBackendReadback::EnqueueReadback(FRDGBuilder& GraphBuilder, FRDGTextureRef ResultTexture)
{
    FRHIGPUTextureReadback* Readback = new FRHIGPUTextureReadback(TEXT("AVBOITResultReadback"));
    
    AddReadbackTexturePass(
        GraphBuilder,
        RDG_EVENT_NAME("AVBOIT.Readback"),
        ResultTexture,
        [Readback, ResultTexture](FRHICommandListImmediate& RHICmdList)
        {
            Readback->EnqueueCopy(RHICmdList, ResultTexture->GetRHI(), FIntVector::ZeroValue, 0, FIntVector::ZeroValue);
        });

    return Readback;
}
