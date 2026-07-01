#include "AVBOIT/Backend/AVBOITBackendRenderer.h"
#include "AVBOIT/Backend/AVBOITBackendShaders.h"
#include "AVBOIT/Backend/AVBOITBackendReadback.h"
#include "AVBOIT/Backend/AVBOITBackendDebugReadback.h"
#include "RenderGraphUtils.h"
#include "RHIStaticStates.h"

FAVBOITBackendReadbacks FAVBOITBackendRenderer::Execute(FRDGBuilder& GraphBuilder, const FAVBOITBackendSettings& Settings, const TArray<FAVBOITInjectedFragment>& InjectedFragments)
{
    FAVBOITBackendReadbacks OutReadbacks;
    if (!Settings.bEnabled || Settings.Mode == 0)
    {
        return OutReadbacks;
    }

    uint32 FragCount = InjectedFragments.Num();
    if (FragCount == 0)
    {
        return OutReadbacks;
    }

    FIntPoint Res = Settings.Resolution;
    FIntVector GroupCount = FIntVector(FMath::DivideAndRoundUp(Res.X, 8), FMath::DivideAndRoundUp(Res.Y, 8), 1);

    // 1. Create Resources
    FRDGBufferDesc ExtinctionDesc = FRDGBufferDesc::CreateStructuredDesc(4, Res.X * Res.Y * 64);
    ExtinctionDesc.Usage |= EBufferUsageFlags::UnorderedAccess | EBufferUsageFlags::ShaderResource;
    FRDGBufferRef ExtinctionVolume = GraphBuilder.CreateBuffer(ExtinctionDesc, TEXT("AVBOIT.ExtinctionVolume"));

    FRDGTextureDesc TransmittanceDesc = FRDGTextureDesc::Create2DArray(Res, PF_R32_FLOAT, FClearValueBinding::None, TexCreate_UAV | TexCreate_ShaderResource, 64);
    FRDGTextureRef TransmittanceVolume = GraphBuilder.CreateTexture(TransmittanceDesc, TEXT("AVBOIT.TransmittanceVolume"));

    FRDGTextureDesc ResultDesc = FRDGTextureDesc::Create2D(Res, PF_FloatRGBA, FClearValueBinding::None, TexCreate_UAV | TexCreate_ShaderResource | TexCreate_RenderTargetable);
    FRDGTextureRef ResultTexture = GraphBuilder.CreateTexture(ResultDesc, TEXT("AVBOIT.ResultTexture"));

    // Fragment Buffer
    FRDGBufferRef FragmentBuffer = CreateStructuredBuffer(GraphBuilder, TEXT("AVBOIT.FragmentBuffer"), sizeof(FAVBOITInjectedFragment), FragCount, InjectedFragments.GetData(), sizeof(FAVBOITInjectedFragment) * FragCount);

    // 2. Clear Pass
    {
        FAVBOITClearCS::FParameters* PassParams = GraphBuilder.AllocParameters<FAVBOITClearCS::FParameters>();
        PassParams->ViewResolution = FVector2f((float)Res.X, (float)Res.Y);
        PassParams->VolumeResolution = FVector2f((float)Res.X, (float)Res.Y);
          PassParams->ZNear = Settings.ZNear;
          PassParams->ZFar = Settings.ZFar;
          PassParams->NumSlices = 64;
          PassParams->FragmentCount = FragCount;
        PassParams->OutExtinctionVolume = GraphBuilder.CreateUAV(ExtinctionVolume);
        PassParams->OutTransmittanceVolume = GraphBuilder.CreateUAV(TransmittanceVolume);
        PassParams->OutResultTexture = GraphBuilder.CreateUAV(ResultTexture);

        TShaderMapRef<FAVBOITClearCS> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
        FComputeShaderUtils::AddPass(GraphBuilder, RDG_EVENT_NAME("AVBOIT.Clear"), ComputeShader, PassParams, GroupCount);
    }

    // 3. Build Pass
    {
        FAVBOITBuildCS::FParameters* PassParams = GraphBuilder.AllocParameters<FAVBOITBuildCS::FParameters>();
        PassParams->ViewResolution = FVector2f((float)Res.X, (float)Res.Y);
        PassParams->ZNear = Settings.ZNear;
        PassParams->ZFar = Settings.ZFar;
        PassParams->NumSlices = 64;
        PassParams->FragmentCount = FragCount;
        PassParams->InjectedFragments = GraphBuilder.CreateSRV(FragmentBuffer);
        PassParams->OutExtinctionVolume = GraphBuilder.CreateUAV(ExtinctionVolume);

        TShaderMapRef<FAVBOITBuildCS> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
        FComputeShaderUtils::AddPass(GraphBuilder, RDG_EVENT_NAME("AVBOIT.Build"), ComputeShader, PassParams, GroupCount);
    }

    // 4. Integrate Pass
    {
        FAVBOITIntegrateCS::FParameters* PassParams = GraphBuilder.AllocParameters<FAVBOITIntegrateCS::FParameters>();
        PassParams->ViewResolution = FVector2f((float)Res.X, (float)Res.Y);
        PassParams->VolumeResolution = FVector2f((float)Res.X, (float)Res.Y);
          PassParams->ZNear = Settings.ZNear;
          PassParams->ZFar = Settings.ZFar;
          PassParams->NumSlices = 64;
          PassParams->FragmentCount = FragCount;
        PassParams->InExtinctionVolume = GraphBuilder.CreateSRV(ExtinctionVolume);
        PassParams->OutTransmittanceVolume = GraphBuilder.CreateUAV(TransmittanceVolume);

        TShaderMapRef<FAVBOITIntegrateCS> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
        FComputeShaderUtils::AddPass(GraphBuilder, RDG_EVENT_NAME("AVBOIT.Integrate"), ComputeShader, PassParams, GroupCount);
    }

    // 5. Composite Pass
    {
        FAVBOITCompositeCS::FParameters* PassParams = GraphBuilder.AllocParameters<FAVBOITCompositeCS::FParameters>();
        PassParams->ViewResolution = FVector2f((float)Res.X, (float)Res.Y);
          PassParams->ZNear = Settings.ZNear;
          PassParams->ZFar = Settings.ZFar;
          PassParams->NumSlices = 64;
          PassParams->FragmentCount = FragCount;
        PassParams->InTransmittanceVolume = GraphBuilder.CreateSRV(TransmittanceVolume);
        PassParams->OutResultTexture = GraphBuilder.CreateUAV(ResultTexture);

        TShaderMapRef<FAVBOITCompositeCS> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
        FComputeShaderUtils::AddPass(GraphBuilder, RDG_EVENT_NAME("AVBOIT.Composite"), ComputeShader, PassParams, GroupCount);
    }

    // 6. Shade Pass
    {
        FAVBOITShadeCS::FParameters* PassParams = GraphBuilder.AllocParameters<FAVBOITShadeCS::FParameters>();
        PassParams->ViewResolution = FVector2f((float)Res.X, (float)Res.Y);
        PassParams->ZNear = Settings.ZNear;
        PassParams->ZFar = Settings.ZFar;
        PassParams->NumSlices = 64;
        PassParams->FragmentCount = FragCount;
        PassParams->InjectedFragments = GraphBuilder.CreateSRV(FragmentBuffer);
        PassParams->InTransmittanceVolume = GraphBuilder.CreateSRV(TransmittanceVolume);
        PassParams->OutResultTexture = GraphBuilder.CreateUAV(ResultTexture);

        TShaderMapRef<FAVBOITShadeCS> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
        FComputeShaderUtils::AddPass(GraphBuilder, RDG_EVENT_NAME("AVBOIT.Shade"), ComputeShader, PassParams, GroupCount);
    }

    // 7. Enqueue Readback
    OutReadbacks.Result = FAVBOITBackendReadback::EnqueueReadback(GraphBuilder, ResultTexture);
    
    FAVBOITSliceLineReadbacks DebugLines = FAVBOITBackendDebugReadback::EnqueueExtractSliceLine(GraphBuilder, ExtinctionVolume, TransmittanceVolume, FIntPoint(256, 256), Res);
    OutReadbacks.ExtinctionLine = DebugLines.Extinction;
    OutReadbacks.TransmittanceLine = DebugLines.Transmittance;

    return OutReadbacks;
}
