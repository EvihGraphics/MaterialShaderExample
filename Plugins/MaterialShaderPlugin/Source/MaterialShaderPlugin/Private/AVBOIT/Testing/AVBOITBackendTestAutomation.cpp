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

void FAVBOITBackendTestAutomation::BuildTestCases()
{
    TestCases.Empty();

    FAVBOITInjectedFragment WhiteFrag = { FVector4f(1.f, 1.f, 1.f, 0.5f), 100.f, 0, FIntPoint(224, 224), FIntPoint(288, 288), FIntPoint(0, 0) };
    
    FAVBOITInjectedFragment RedFrag   = { FVector4f(1.f, 0.f, 0.f, 0.5f), 100.f, 0, FIntPoint(224, 224), FIntPoint(288, 288), FIntPoint(0, 0) };
    FAVBOITInjectedFragment GreenFrag = { FVector4f(0.f, 1.f, 0.f, 0.5f), 500.f, 1, FIntPoint(224, 224), FIntPoint(288, 288), FIntPoint(0, 0) };
    FAVBOITInjectedFragment BlueFrag  = { FVector4f(0.f, 0.f, 1.f, 0.5f), 900.f, 2, FIntPoint(224, 224), FIntPoint(288, 288), FIntPoint(0, 0) };

    // Slice 0 is ZNear (10.0), Slice 63 is ZFar (1000.0)
    // ZNear = 10, ZFar = 1000.
    // Map(100) = log2(100/10) / log2(1000/10) = log2(10) / log2(100) = 0.5 -> Slice 32
    // Map(500) = log2(50) / log2(100) = 5.6438 / 6.6438 = 0.849 -> Slice 54
    // Map(900) = log2(90) / log2(100) = 6.4918 / 6.6438 = 0.977 -> Slice 62

    // Transmittance calculation
    // Extinction = -ln(1 - 0.5) = 0.693147
    // T = exp(-0.693147) = 0.5
    
    // Case 1: Single White
    {
        FAVBOITBackendTestCase Case;
        Case.Name = TEXT("SingleWhite");
        Case.Fragments.Add(WhiteFrag);
        Case.ExpectedColor = FVector3f(0.5f, 0.5f, 0.5f); // Color * Alpha * T_front (1.0)
        Case.ExpectedTransmittance = 0.5f;
        Case.ExpectedOccupiedSlices.Add(32);
        Case.ColorMAETolerance = 0.01f;
        Case.ColorMaxAbsTolerance = 0.05f;
        Case.TransmittanceTolerance = 0.05f;
        TestCases.Add(Case);
    }

    // Case 2: Double Red Blue
    {
        FAVBOITBackendTestCase Case;
        Case.Name = TEXT("Double_RedBlue");
        Case.Fragments.Add(RedFrag);
        Case.Fragments.Add(BlueFrag);
        // Red is at Slice 32. T_front = 1.0. Contrib = (1,0,0) * 0.5 * 1.0 = (0.5, 0, 0)
        // Blue is at Slice 62. T_front = T_red = 0.5. Contrib = (0,0,1) * 0.5 * 0.5 = (0, 0, 0.25)
        // Total Color = (0.5, 0, 0.25)
        Case.ExpectedColor = FVector3f(0.5f, 0.0f, 0.25f);
        Case.ExpectedTransmittance = 0.25f; // 0.5 * 0.5
        Case.ExpectedOccupiedSlices.Add(32);
        Case.ExpectedOccupiedSlices.Add(62);
        Case.ColorMAETolerance = 0.01f;
        Case.ColorMaxAbsTolerance = 0.05f;
        Case.TransmittanceTolerance = 0.05f;
        TestCases.Add(Case);
    }

    // Case 3: Double Blue Red (Order Independent)
    {
        FAVBOITBackendTestCase Case;
        Case.Name = TEXT("Double_BlueRed");
        Case.Fragments.Add(BlueFrag);
        Case.Fragments.Add(RedFrag);
        Case.ExpectedColor = FVector3f(0.5f, 0.0f, 0.25f);
        Case.ExpectedTransmittance = 0.25f;
        Case.ExpectedOccupiedSlices.Add(32);
        Case.ExpectedOccupiedSlices.Add(62);
        Case.ColorMAETolerance = 0.01f;
        Case.ColorMaxAbsTolerance = 0.05f;
        Case.TransmittanceTolerance = 0.05f;
        TestCases.Add(Case);
    }

    // Case 4: Triple RGB
    // Red (32), Green (54), Blue (62)
    // Red Contrib = (1,0,0) * 0.5 * 1.0 = (0.5, 0, 0)
    // Green Contrib = (0,1,0) * 0.5 * 0.5 = (0, 0.25, 0)
    // Blue Contrib = (0,0,1) * 0.5 * 0.25 = (0, 0, 0.125)
    // Total Color = (0.5, 0.25, 0.125)
    // Total Transmittance = 0.125
    TArray<TArray<FAVBOITInjectedFragment>> TriplePerms = {
        {RedFrag, GreenFrag, BlueFrag},
        {RedFrag, BlueFrag, GreenFrag},
        {GreenFrag, RedFrag, BlueFrag},
        {GreenFrag, BlueFrag, RedFrag},
        {BlueFrag, RedFrag, GreenFrag},
        {BlueFrag, GreenFrag, RedFrag}
    };
    
    TArray<FString> TripleNames = { TEXT("RGB"), TEXT("RBG"), TEXT("GRB"), TEXT("GBR"), TEXT("BRG"), TEXT("BGR") };
    
    for (int32 i = 0; i < TriplePerms.Num(); ++i)
    {
        FAVBOITBackendTestCase Case;
        Case.Name = FString::Printf(TEXT("Triple_%s"), *TripleNames[i]);
        Case.Fragments = TriplePerms[i];
        Case.ExpectedColor = FVector3f(0.5f, 0.25f, 0.125f);
        Case.ExpectedTransmittance = 0.125f;
        Case.ExpectedOccupiedSlices = {32, 54, 62};
        Case.ColorMAETolerance = 0.01f;
        Case.ColorMaxAbsTolerance = 0.05f;
        Case.TransmittanceTolerance = 0.05f;
        TestCases.Add(Case);
    }
}

void FAVBOITBackendTestAutomation::Tick(float DeltaTime)
{
    if (bShouldStartSuite)
    {
        bShouldStartSuite = false;
        bIsRunning = true;
        BuildTestCases();
        CurrentTestCaseIndex = 0;
        PassedCaseCount = 0;
        FailedCaseCount = 0;
        RunNextTest();
        return;
    }

    if (!bIsRunning) return;

    if (PendingReadbackResult || PendingReadbackTransmittance)
    {
        HandleReadback();
    }
}

void FAVBOITBackendTestAutomation::RunNextTest()
{
    if (CurrentTestCaseIndex >= TestCases.Num())
    {
        FinalizeSuite();
        return;
    }

    const FAVBOITBackendTestCase& TestCase = TestCases[CurrentTestCaseIndex];
    UE_LOG(LogTemp, Log, TEXT("Running Test Case [%d/%d]: %s"), CurrentTestCaseIndex + 1, TestCases.Num(), *TestCase.Name);

    FAVBOITBackendSettings Settings;
    Settings.bEnabled = true;
    Settings.Mode = 1;
    Settings.Resolution = FIntPoint(512, 512);
    Settings.ZNear = 10.0f;
    Settings.ZFar = 1000.0f;

    ENQUEUE_RENDER_COMMAND(RunAVBOITBackendTest)(
        [this, Settings, Fragments = TestCase.Fragments](FRHICommandListImmediate& RHICmdList)
        {
            FRDGBuilder GraphBuilder(RHICmdList);
            FAVBOITBackendReadbacks Readbacks = FAVBOITBackendRenderer::Execute(GraphBuilder, Settings, Fragments);
            GraphBuilder.Execute();
            
            RHICmdList.ImmediateFlush(EImmediateFlushType::FlushRHIThread);
            this->PendingReadbackResult.Reset(Readbacks.Result);
            this->PendingReadbackExtinction.Reset(Readbacks.Extinction);
            this->PendingReadbackTransmittance.Reset(Readbacks.Transmittance);
            this->WaitFrameCount = 0;
        });

    FlushRenderingCommands();
}

void FAVBOITBackendTestAutomation::HandleReadback()
{
    bool bAllReady = (!PendingReadbackResult || PendingReadbackResult->IsReady());

    if (!bAllReady)
    {
        WaitFrameCount++;
        if (WaitFrameCount > MaximumWaitFrames)
        {
            UE_LOG(LogTemp, Error, TEXT("Test Case %s: GPU Readback Timeout!"), *TestCases[CurrentTestCaseIndex].Name);
            FailedCaseCount++;
            CurrentTestCaseIndex++;
            PendingReadbackResult.Reset();
            PendingReadbackTransmittance.Reset();
            PendingReadbackExtinction.Reset();
            RunNextTest();
        }
        return;
    }

    const FAVBOITBackendTestCase& TestCase = TestCases[CurrentTestCaseIndex];
    bool bCasePassed = true;

    // Result Check
    if (PendingReadbackResult)
    {
        int32 OutRowPitchInPixels = 0;
        const void* RawData = PendingReadbackResult->Lock(OutRowPitchInPixels);
        const FFloat16Color* ColorData = static_cast<const FFloat16Color*>(RawData);

        FVector4f SumColor = FVector4f(0, 0, 0, 0);
        int32 Count = 0;
        
        for (int32 y = 224; y < 288; ++y)
        {
            for (int32 x = 224; x < 288; ++x)
            {
                int32 Index = y * OutRowPitchInPixels + x;
                FFloat16Color C = ColorData[Index];
                SumColor += FVector4f(C.R.GetFloat(), C.G.GetFloat(), C.B.GetFloat(), C.A.GetFloat());
                Count++;
            }
        }

        PendingReadbackResult->Unlock();
        PendingReadbackResult.Reset();

        FVector4f MeanColor = SumColor / (float)Count;
        float DiffR = FMath::Abs(MeanColor.X - TestCase.ExpectedColor.X);
        float DiffG = FMath::Abs(MeanColor.Y - TestCase.ExpectedColor.Y);
        float DiffB = FMath::Abs(MeanColor.Z - TestCase.ExpectedColor.Z);
        float DiffT = FMath::Abs(MeanColor.W - TestCase.ExpectedTransmittance);
        
        float MaxDiff = FMath::Max3(DiffR, DiffG, DiffB);

        if (MaxDiff > TestCase.ColorMaxAbsTolerance)
        {
            UE_LOG(LogTemp, Error, TEXT("Test Case %s: Color mismatch. Expected: %s, Got: %s (MaxDiff: %f)"), 
                *TestCase.Name, *TestCase.ExpectedColor.ToString(), *FVector3f(MeanColor.X, MeanColor.Y, MeanColor.Z).ToString(), MaxDiff);
            bCasePassed = false;
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("Test Case %s: Color PASS. Got: %s"), *TestCase.Name, *FVector3f(MeanColor.X, MeanColor.Y, MeanColor.Z).ToString());
        }

        if (DiffT > TestCase.TransmittanceTolerance)
        {
            UE_LOG(LogTemp, Error, TEXT("Test Case %s: Transmittance mismatch. Expected: %f, Got: %f (Diff: %f)"), 
                *TestCase.Name, TestCase.ExpectedTransmittance, MeanColor.W, DiffT);
            bCasePassed = false;
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("Test Case %s: Transmittance PASS. Got: %f"), *TestCase.Name, MeanColor.W);
        }
    }


    if (bCasePassed)
    {
        PassedCaseCount++;
    }
    else
    {
        FailedCaseCount++;
    }

    CurrentTestCaseIndex++;
    RunNextTest();
}

void FAVBOITBackendTestAutomation::FinalizeSuite()
{
    bIsRunning = false;
    UE_LOG(LogTemp, Log, TEXT("======================================"));
    UE_LOG(LogTemp, Log, TEXT("AVBOIT Backend Suite Complete."));
    UE_LOG(LogTemp, Log, TEXT("Passed: %d / %d"), PassedCaseCount, TestCases.Num());
    UE_LOG(LogTemp, Log, TEXT("Failed: %d"), FailedCaseCount);
    UE_LOG(LogTemp, Log, TEXT("======================================"));
    
    // Explicit Exit Status
    FGenericPlatformMisc::RequestExitWithStatus(false, FailedCaseCount > 0 ? 1 : 0);
}
