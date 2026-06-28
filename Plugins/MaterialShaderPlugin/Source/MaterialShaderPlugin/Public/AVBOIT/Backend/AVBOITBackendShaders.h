#pragma once

#include "CoreMinimal.h"
#include "GlobalShader.h"
#include "ShaderParameterStruct.h"

// Forward declaration
struct FAVBOITInjectedFragment;

class FAVBOITClearCS : public FGlobalShader
{
    DECLARE_GLOBAL_SHADER(FAVBOITClearCS);
    SHADER_USE_PARAMETER_STRUCT(FAVBOITClearCS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(FVector2f, ViewResolution)
        SHADER_PARAMETER_RDG_BUFFER_UAV(RWStructuredBuffer<uint>, OutExtinctionVolume)
        SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2DArray<float>, OutTransmittanceVolume)
        SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<float4>, OutResultTexture)
    END_SHADER_PARAMETER_STRUCT()
};

class FAVBOITBuildCS : public FGlobalShader
{
    DECLARE_GLOBAL_SHADER(FAVBOITBuildCS);
    SHADER_USE_PARAMETER_STRUCT(FAVBOITBuildCS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(FVector2f, ViewResolution)
        SHADER_PARAMETER(float, ZNear)
        SHADER_PARAMETER(float, ZFar)
        SHADER_PARAMETER(uint32, FragmentCount)
        SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<FInjectedFragment>, InjectedFragments)
        SHADER_PARAMETER_RDG_BUFFER_UAV(RWStructuredBuffer<uint>, OutExtinctionVolume)
    END_SHADER_PARAMETER_STRUCT()
};

class FAVBOITIntegrateCS : public FGlobalShader
{
    DECLARE_GLOBAL_SHADER(FAVBOITIntegrateCS);
    SHADER_USE_PARAMETER_STRUCT(FAVBOITIntegrateCS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(FVector2f, ViewResolution)
        SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<uint>, InExtinctionVolume)
        SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2DArray<float>, OutTransmittanceVolume)
    END_SHADER_PARAMETER_STRUCT()
};

class FAVBOITCompositeCS : public FGlobalShader
{
    DECLARE_GLOBAL_SHADER(FAVBOITCompositeCS);
    SHADER_USE_PARAMETER_STRUCT(FAVBOITCompositeCS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(FVector2f, ViewResolution)
        SHADER_PARAMETER_RDG_TEXTURE_SRV(Texture2DArray<float>, InTransmittanceVolume)
        SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<float4>, OutResultTexture)
    END_SHADER_PARAMETER_STRUCT()
};

class FAVBOITShadeCS : public FGlobalShader
{
    DECLARE_GLOBAL_SHADER(FAVBOITShadeCS);
    SHADER_USE_PARAMETER_STRUCT(FAVBOITShadeCS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(FVector2f, ViewResolution)
        SHADER_PARAMETER(float, ZNear)
        SHADER_PARAMETER(float, ZFar)
        SHADER_PARAMETER(uint32, FragmentCount)
        SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<FInjectedFragment>, InjectedFragments)
        SHADER_PARAMETER_RDG_TEXTURE_SRV(Texture2DArray<float>, InTransmittanceVolume)
        SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<float4>, OutResultTexture)
    END_SHADER_PARAMETER_STRUCT()
};
