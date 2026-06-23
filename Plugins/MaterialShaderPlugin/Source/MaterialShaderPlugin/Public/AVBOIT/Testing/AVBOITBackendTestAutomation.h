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
    float ColorMAETolerance;
    float ColorMaxAbsTolerance;
    float TransmittanceTolerance;
    TArray<int32> ExpectedOccupiedSlices;
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

    void BuildTestCases();
    void RunNextTest();

public:
    static FAVBOITBackendTestAutomation* Instance;
private:
    TUniquePtr<FRHIGPUTextureReadback> PendingReadbackResult;
    TUniquePtr<FRHIGPUTextureReadback> PendingReadbackExtinction;
    TUniquePtr<FRHIGPUTextureReadback> PendingReadbackTransmittance;
    
    int32 WaitFrameCount = 0;
    const int32 MaximumWaitFrames = 500;

    void HandleReadback();
    void FinalizeSuite();
};
