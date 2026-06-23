#include "AVBOIT/Testing/AVBOITBackendTestAutomation.h"
#include "AVBOIT/Backend/AVBOITBackendRenderer.h"
#include "RenderGraphBuilder.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "RenderingThread.h"

FAVBOITBackendTestAutomation* FAVBOITBackendTestAutomation::Instance = nullptr;

static FAutoConsoleCommand GCmdRunBackendSuite(
    TEXT("AVBOIT.Test.BackendSuite"),
    TEXT("Runs the AVBOIT Backend Vertical Slice validation suite"),
    FConsoleCommandDelegate::CreateStatic([]()
    {
        if (!FAVBOITBackendTestAutomation::Instance)
        {
            FAVBOITBackendTestAutomation::Initialize();
        }
        
        if (FAVBOITBackendTestAutomation::Instance)
        {
            FAVBOITBackendTestAutomation::Instance->bShouldStartSuite = true;
        }
    })
);

void FAVBOITBackendTestAutomation::Initialize()
{
    if (!Instance)
    {
        Instance = new FAVBOITBackendTestAutomation();
    }
    
    if (FParse::Param(FCommandLine::Get(), TEXT("AVBOITBackendSuite")))
    {
        Instance->bShouldStartSuite = true;
    }
}

void FAVBOITBackendTestAutomation::Shutdown()
{
    if (Instance)
    {
        delete Instance;
        Instance = nullptr;
    }
}

void FAVBOITBackendTestAutomation::Tick(float DeltaTime)
{
    if (bShouldStartSuite)
    {
        bShouldStartSuite = false;
        bIsRunning = true;
        CurrentTestPhase = 0;
        CurrentPermutation = 0;
        RunNextTest();
        return;
    }

    if (!bIsRunning) return;

    if (PendingReadback)
    {
        HandleReadback();
    }
}

void FAVBOITBackendTestAutomation::RunNextTest()
{
    TArray<FAVBOITInjectedFragment> Fragments;
    
    // Create base fragments
    FAVBOITInjectedFragment RedFrag   = { FVector4f(1.f, 0.f, 0.f, 0.5f), 100.f, 0, FIntPoint(224, 224), FIntPoint(288, 288) };
    FAVBOITInjectedFragment GreenFrag = { FVector4f(0.f, 1.f, 0.f, 0.5f), 500.f, 1, FIntPoint(224, 224), FIntPoint(288, 288) };
    FAVBOITInjectedFragment BlueFrag  = { FVector4f(0.f, 0.f, 1.f, 0.5f), 900.f, 2, FIntPoint(224, 224), FIntPoint(288, 288) };
    FAVBOITInjectedFragment WhiteFrag = { FVector4f(1.f, 1.f, 1.f, 0.5f), 100.f, 0, FIntPoint(224, 224), FIntPoint(288, 288) };

    if (CurrentTestPhase == 0)
    {
        // Single White
        Fragments.Add(WhiteFrag);
    }
    else if (CurrentTestPhase == 1)
    {
        // Double Red Blue
        if (CurrentPermutation == 0) { Fragments.Add(RedFrag); Fragments.Add(BlueFrag); }
        else                         { Fragments.Add(BlueFrag); Fragments.Add(RedFrag); }
    }
    else if (CurrentTestPhase == 2)
    {
        // Triple RGB
        if (CurrentPermutation == 0)      { Fragments.Append({RedFrag, GreenFrag, BlueFrag}); }
        else if (CurrentPermutation == 1) { Fragments.Append({RedFrag, BlueFrag, GreenFrag}); }
        else if (CurrentPermutation == 2) { Fragments.Append({GreenFrag, RedFrag, BlueFrag}); }
        else if (CurrentPermutation == 3) { Fragments.Append({GreenFrag, BlueFrag, RedFrag}); }
        else if (CurrentPermutation == 4) { Fragments.Append({BlueFrag, RedFrag, GreenFrag}); }
        else if (CurrentPermutation == 5) { Fragments.Append({BlueFrag, GreenFrag, RedFrag}); }
    }

    FAVBOITBackendSettings Settings;
    Settings.bEnabled = true;
    Settings.Mode = 1;
    Settings.Resolution = FIntPoint(512, 512);

    UE_LOG(LogTemp, Log, TEXT("RunNextTest: Phase %d Permutation %d"), CurrentTestPhase, CurrentPermutation);

    ENQUEUE_RENDER_COMMAND(RunAVBOITBackendTest)(
        [this, Settings, Fragments](FRHICommandListImmediate& RHICmdList)
        {
            FRDGBuilder GraphBuilder(RHICmdList);
            FRHIGPUTextureReadback* Readback = FAVBOITBackendRenderer::Execute(GraphBuilder, Settings, Fragments);
            GraphBuilder.Execute();
            
            RHICmdList.ImmediateFlush(EImmediateFlushType::FlushRHIThread);
            this->PendingReadback = Readback;
        });

    // Wait for render thread and GPU
    FlushRenderingCommands();
}

void FAVBOITBackendTestAutomation::HandleReadback()
{
    if (!PendingReadback->IsReady())
    {
        static int32 WaitCount = 0;
        if (WaitCount++ % 100 == 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("Waiting for PendingReadback->IsReady()..."));
        }
        return; // wait for next tick
    }

    int32 Width = 512;
    int32 Height = 512;
    int32 OutRowPitchInPixels = 0;
    const void* RawData = PendingReadback->Lock(OutRowPitchInPixels);
    const FFloat16Color* ColorData = static_cast<const FFloat16Color*>(RawData);

    // Calculate ROI metrics (224-288)
    FVector4f SumColor = FVector4f(0, 0, 0, 0);
    int32 Count = 0;
    
    for (int32 y = 224; y < 288; ++y)
    {
        for (int32 x = 224; x < 288; ++x)
        {
            int32 Index = y * Width + x;
            FFloat16Color C = ColorData[Index];
            SumColor += FVector4f(C.R.GetFloat(), C.G.GetFloat(), C.B.GetFloat(), C.A.GetFloat());
            Count++;
        }
    }

    PendingReadback->Unlock();
    delete PendingReadback;
    PendingReadback = nullptr;

    FVector4f MeanColor = SumColor / (float)Count;
    UE_LOG(LogTemp, Log, TEXT("BackendTest Phase: %d Permutation: %d Mean Color: (%.6f, %.6f, %.6f)"), CurrentTestPhase, CurrentPermutation, MeanColor.X, MeanColor.Y, MeanColor.Z);

    // Advance
    if (CurrentTestPhase == 0)
    {
        CurrentTestPhase++; CurrentPermutation = 0;
        RunNextTest();
    }
    else if (CurrentTestPhase == 1)
    {
        CurrentPermutation++;
        if (CurrentPermutation >= 2) { CurrentTestPhase++; CurrentPermutation = 0; RunNextTest(); }
        else { RunNextTest(); }
    }
    else if (CurrentTestPhase == 2)
    {
        CurrentPermutation++;
        if (CurrentPermutation >= 6) { FinalizeSuite(); }
        else { RunNextTest(); }
    }
}

void FAVBOITBackendTestAutomation::FinalizeSuite()
{
    bIsRunning = false;
    UE_LOG(LogTemp, Log, TEXT("Backend Suite Complete."));
    FGenericPlatformMisc::RequestExit(false);
}
