#include "AVBOIT/Backend/AVBOITBackendDebugReadback.h"
#include "RenderGraphUtils.h"
#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "RHIGPUReadback.h"

class FAVBOITMappingTestCS : public FGlobalShader
{
    DECLARE_GLOBAL_SHADER(FAVBOITMappingTestCS);
    SHADER_USE_PARAMETER_STRUCT(FAVBOITMappingTestCS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(float, ZNear)
        SHADER_PARAMETER(float, ZFar)
        SHADER_PARAMETER(uint32, ItemCount)
        SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<float>, InLinearDepths)
        SHADER_PARAMETER_RDG_BUFFER_UAV(RWStructuredBuffer<float>, OutNormalizedDepths)
        SHADER_PARAMETER_RDG_BUFFER_UAV(RWStructuredBuffer<uint>, OutSliceIndices)
    END_SHADER_PARAMETER_STRUCT()
};

IMPLEMENT_GLOBAL_SHADER(FAVBOITMappingTestCS, "/Plugin/MaterialShaderExample/AVBOIT/Backend/AVBOITMappingTest.usf", "AVBOITMappingTestCS", SF_Compute);

FAVBOITMappingTestResult FAVBOITBackendDebugReadback::RunMappingTestSync(const TArray<float>& LinearDepths, float ZNear, float ZFar)
{
    FAVBOITMappingTestResult FinalResult;
    uint32 ItemCount = LinearDepths.Num();
    if (ItemCount == 0) return FinalResult;

    FinalResult.NormalizedDepths.SetNumZeroed(ItemCount);
    FinalResult.SliceIndices.SetNumZeroed(ItemCount);

    ENQUEUE_RENDER_COMMAND(RunAVBOITMappingTest)(
        [LinearDepths, ZNear, ZFar, ItemCount, &FinalResult](FRHICommandListImmediate& RHICmdList)
        {
            FRDGBuilder GraphBuilder(RHICmdList);

            FRDGBufferRef InLinearDepthsBuf = CreateUploadBuffer(GraphBuilder, TEXT("InLinearDepths"), sizeof(float), ItemCount, LinearDepths.GetData(), sizeof(float) * ItemCount);
            
            FRDGBufferDesc OutNormDesc = FRDGBufferDesc::CreateStructuredDesc(sizeof(float), ItemCount);
            FRDGBufferRef OutNormalizedDepthsBuf = GraphBuilder.CreateBuffer(OutNormDesc, TEXT("OutNormalizedDepths"));
            
            FRDGBufferDesc OutSliceDesc = FRDGBufferDesc::CreateStructuredDesc(sizeof(uint32), ItemCount);
            FRDGBufferRef OutSliceIndicesBuf = GraphBuilder.CreateBuffer(OutSliceDesc, TEXT("OutSliceIndices"));

            FAVBOITMappingTestCS::FParameters* PassParams = GraphBuilder.AllocParameters<FAVBOITMappingTestCS::FParameters>();
            PassParams->ZNear = ZNear;
            PassParams->ZFar = ZFar;
            PassParams->ItemCount = ItemCount;
            PassParams->InLinearDepths = GraphBuilder.CreateSRV(InLinearDepthsBuf, PF_R32_FLOAT);
            PassParams->OutNormalizedDepths = GraphBuilder.CreateUAV(OutNormalizedDepthsBuf, PF_R32_FLOAT);
            PassParams->OutSliceIndices = GraphBuilder.CreateUAV(OutSliceIndicesBuf, PF_R32_UINT);

            TShaderMapRef<FAVBOITMappingTestCS> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
            FIntVector GroupCount = FIntVector(FMath::DivideAndRoundUp(ItemCount, 64u), 1, 1);
            
            FComputeShaderUtils::AddPass(GraphBuilder, RDG_EVENT_NAME("AVBOIT.Test.Mapping"), ComputeShader, PassParams, GroupCount);

            FRHIGPUBufferReadback* ReadbackNorm = new FRHIGPUBufferReadback(TEXT("ReadbackNorm"));
            FRHIGPUBufferReadback* ReadbackSlice = new FRHIGPUBufferReadback(TEXT("ReadbackSlice"));

            AddReadbackBufferPass(GraphBuilder, RDG_EVENT_NAME("ReadbackNormPass"), OutNormalizedDepthsBuf,
                [ReadbackNorm, OutNormalizedDepthsBuf](FRHICommandList& RHICmdList)
                {
                    ReadbackNorm->EnqueueCopy(RHICmdList, OutNormalizedDepthsBuf->GetRHI(), OutNormalizedDepthsBuf->GetSize());
                });

            AddReadbackBufferPass(GraphBuilder, RDG_EVENT_NAME("ReadbackSlicePass"), OutSliceIndicesBuf,
                [ReadbackSlice, OutSliceIndicesBuf](FRHICommandList& RHICmdList)
                {
                    ReadbackSlice->EnqueueCopy(RHICmdList, OutSliceIndicesBuf->GetRHI(), OutSliceIndicesBuf->GetSize());
                });

            GraphBuilder.Execute();
            RHICmdList.ImmediateFlush(EImmediateFlushType::FlushRHIThread);

            // Synchronous Wait
            while (!ReadbackNorm->IsReady() || !ReadbackSlice->IsReady())
            {
                FPlatformProcess::Sleep(0.001f);
            }

            const float* NormData = (const float*)ReadbackNorm->Lock(ItemCount * sizeof(float));
            const uint32* SliceData = (const uint32*)ReadbackSlice->Lock(ItemCount * sizeof(uint32));

            FMemory::Memcpy(FinalResult.NormalizedDepths.GetData(), NormData, ItemCount * sizeof(float));
            FMemory::Memcpy(FinalResult.SliceIndices.GetData(), SliceData, ItemCount * sizeof(uint32));

            ReadbackNorm->Unlock();
            ReadbackSlice->Unlock();

            delete ReadbackNorm;
            delete ReadbackSlice;
        });

    FlushRenderingCommands();
    return FinalResult;
}

class FAVBOITDebugExtractCS : public FGlobalShader
{
    DECLARE_GLOBAL_SHADER(FAVBOITDebugExtractCS);
    SHADER_USE_PARAMETER_STRUCT(FAVBOITDebugExtractCS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(FIntVector, CenterPixel)
        SHADER_PARAMETER_RDG_TEXTURE_SRV(Texture2DArray<uint>, InExtinctionVolume)
        SHADER_PARAMETER_RDG_TEXTURE_SRV(Texture2DArray<float>, InTransmittanceVolume)
        SHADER_PARAMETER_RDG_BUFFER_UAV(RWStructuredBuffer<uint>, OutExtinctionLine)
        SHADER_PARAMETER_RDG_BUFFER_UAV(RWStructuredBuffer<float>, OutTransmittanceLine)
    END_SHADER_PARAMETER_STRUCT()
};

IMPLEMENT_GLOBAL_SHADER(FAVBOITDebugExtractCS, "/Plugin/MaterialShaderExample/AVBOIT/Backend/AVBOITDebugExtract.usf", "AVBOITDebugExtractCS", SF_Compute);

FAVBOITSliceLineReadbacks FAVBOITBackendDebugReadback::EnqueueExtractSliceLine(FRDGBuilder& GraphBuilder, FRDGTextureRef ExtinctionVolume, FRDGTextureRef TransmittanceVolume, FIntPoint CenterPixel)
{
    FAVBOITSliceLineReadbacks OutReadbacks;
    
    FRDGBufferDesc ExtDesc = FRDGBufferDesc::CreateStructuredDesc(sizeof(uint32), 64);
    FRDGBufferRef OutExtinctionLineBuf = GraphBuilder.CreateBuffer(ExtDesc, TEXT("OutExtinctionLine"));
    
    FRDGBufferDesc TransDesc = FRDGBufferDesc::CreateStructuredDesc(sizeof(float), 64);
    FRDGBufferRef OutTransmittanceLineBuf = GraphBuilder.CreateBuffer(TransDesc, TEXT("OutTransmittanceLine"));

    FAVBOITDebugExtractCS::FParameters* PassParams = GraphBuilder.AllocParameters<FAVBOITDebugExtractCS::FParameters>();
    PassParams->CenterPixel = FIntVector(CenterPixel.X, CenterPixel.Y, 0);
    PassParams->InExtinctionVolume = GraphBuilder.CreateSRV(ExtinctionVolume);
    PassParams->InTransmittanceVolume = GraphBuilder.CreateSRV(TransmittanceVolume);
    PassParams->OutExtinctionLine = GraphBuilder.CreateUAV(OutExtinctionLineBuf, PF_R32_UINT);
    PassParams->OutTransmittanceLine = GraphBuilder.CreateUAV(OutTransmittanceLineBuf, PF_R32_FLOAT);

    TShaderMapRef<FAVBOITDebugExtractCS> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
    FComputeShaderUtils::AddPass(GraphBuilder, RDG_EVENT_NAME("AVBOIT.Debug.ExtractSliceLine"), ComputeShader, PassParams, FIntVector(1, 1, 1)); // 64 threads in X

    OutReadbacks.Extinction = new FRHIGPUBufferReadback(TEXT("ReadbackExtLine"));
    OutReadbacks.Transmittance = new FRHIGPUBufferReadback(TEXT("ReadbackTransLine"));

    AddReadbackBufferPass(GraphBuilder, RDG_EVENT_NAME("ReadbackExtLinePass"), OutExtinctionLineBuf,
        [Readback = OutReadbacks.Extinction, Buf = OutExtinctionLineBuf](FRHICommandList& RHICmdList)
        {
            Readback->EnqueueCopy(RHICmdList, Buf->GetRHI(), Buf->GetSize());
        });

    AddReadbackBufferPass(GraphBuilder, RDG_EVENT_NAME("ReadbackTransLinePass"), OutTransmittanceLineBuf,
        [Readback = OutReadbacks.Transmittance, Buf = OutTransmittanceLineBuf](FRHICommandList& RHICmdList)
        {
            Readback->EnqueueCopy(RHICmdList, Buf->GetRHI(), Buf->GetSize());
        });

    return OutReadbacks;
}
