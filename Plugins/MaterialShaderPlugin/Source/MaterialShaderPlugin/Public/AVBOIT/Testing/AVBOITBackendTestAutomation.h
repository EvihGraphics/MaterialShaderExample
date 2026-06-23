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

class FAVBOITBackendTestAutomation : public FTickableGameObject
{
public:
    static void Initialize();
    static void Shutdown();

    virtual void Tick(float DeltaTime) override;
    virtual TStatId GetStatId() const override { return TStatId(); }
    virtual bool IsTickable() const override { return true; }

    bool bIsRunning = false;
    bool bShouldStartSuite = false;
    
    int32 CurrentTestCaseIndex = 0;
    TArray<FAVBOITBackendTestCase> TestCases;
    
    int32 PassedCaseCount = 0;
    int32 FailedCaseCount = 0;

    void RunMappingTests();
    void BuildTestCases();
    void RunNextTest();

public:
    static FAVBOITBackendTestAutomation* Instance;
private:
    TUniquePtr<FRHIGPUTextureReadback> PendingReadbackResult;
    TUniquePtr<FRHIGPUBufferReadback> PendingReadbackExtinctionLine;
    TUniquePtr<FRHIGPUBufferReadback> PendingReadbackTransmittanceLine;
    
    int32 WaitFrameCount = 0;
    const int32 MaximumWaitFrames = 500;

    void HandleReadback();
    void FinalizeSuite();
};
