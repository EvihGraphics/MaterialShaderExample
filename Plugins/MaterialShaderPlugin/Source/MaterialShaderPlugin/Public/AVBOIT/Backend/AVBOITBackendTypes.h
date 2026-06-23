#pragma once

#include "CoreMinimal.h"

#pragma pack(push, 16)
struct FAVBOITInjectedFragment
{
    FVector4f LinearColorAndAlpha;
    float LinearDepth;
    uint32 SubmissionIndex;
    FIntPoint RectMin;
    FIntPoint RectMax;
};
#pragma pack(pop)

struct FAVBOITBackendSettings
{
    bool bEnabled = false;
    int32 Mode = 0; // 0=Disabled, 1=Mathematical Validation
    int32 TestPreset = 0; // single, double, triple
    int32 SubmissionOrder = 0;
    
    FIntPoint Resolution = FIntPoint(512, 512);
    float ZNear = 10.0f;
    float ZFar = 1000.0f;
};
