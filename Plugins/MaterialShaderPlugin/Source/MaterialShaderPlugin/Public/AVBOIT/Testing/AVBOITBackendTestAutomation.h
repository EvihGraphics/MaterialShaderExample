#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
#include "RHIGPUReadback.h"
#include "Stats/Stats.h"

class FAVBOITBackendTestAutomation : public FTickableGameObject
{
public:
    static void Initialize();
    static void Shutdown();

    // FTickableGameObject interface
    virtual void Tick(float DeltaTime) override;
    virtual TStatId GetStatId() const override { return TStatId(); }
    virtual bool IsTickable() const override { return true; } // Always tickable to catch start signal


    bool bIsRunning = false;
    bool bShouldStartSuite = false;
    int32 CurrentTestPhase = 0; // 0=Single, 1=Double, 2=Triple
    int32 CurrentPermutation = 0;
    
    void RunNextTest();

public:
    static FAVBOITBackendTestAutomation* Instance;
private:
    FRHIGPUTextureReadback* PendingReadback = nullptr;
    void HandleReadback();
    void FinalizeSuite();
};
