#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
#include "RHIGPUReadback.h"
#include "AVBOIT/Backend/AVBOITBackendTypes.h"
#include "Stats/Stats.h"

struct FAVBOITBackendTestCase
{
    FString Name;
    TArray<FAVBOITInjectedFragment> Fragments;
    FVector3f ExpectedColor;
    float ExpectedTransmittance;
    TArray<int32> ExpectedOccupiedSlices;
    bool bRequireOrderIndependence = false;
    bool bRequireExactSourceOver = false;
    float ColorMAETolerance = 0.002f;
    float ColorMaxAbsTolerance = 0.01f;
    float TransmittanceTolerance = 0.002f;
    float ExtinctionTolerance = 0.00015f;
    
    // Output
    FString Status;
    TArray<FString> FailureReasons;
};

class MATERIALSHADEREXAMPLE_API FAVBOITBackendTestAutomation
{
public:
    static void Initialize();
    static void Shutdown();

    // Returns the executed test cases synchronously
    static TArray<FAVBOITBackendTestCase> RunSuite();
};

