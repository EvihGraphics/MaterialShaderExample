with open('Plugins/MaterialShaderPlugin/Source/MaterialShaderPlugin/Private/AVBOIT/Testing/AVBOITBackendTestAutomation.cpp', 'w', encoding='utf-8') as cout:
    cout.write('''#include "AVBOIT/Testing/AVBOITBackendTestAutomation.h"
#include "AVBOIT/Backend/AVBOITBackendRenderer.h"
#include "AVBOIT/Backend/AVBOITBackendDebugReadback.h"
#include "RenderGraphBuilder.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "RenderingThread.h"
#include "HAL/FileManager.h"

float AVBOITMapLinearDepthToNormalized(float LinearDepth, float ZNear, float ZFar)
{
    float DepthRatio = FMath::Clamp(LinearDepth / ZNear, 1.0f, ZFar / ZNear);
    return FMath::Log2(DepthRatio) / FMath::Log2(ZFar / ZNear);
}

uint32 AVBOITMapLinearDepthToSlice(float LinearDepth, float ZNear, float ZFar)
{
    float NormDepth = AVBOITMapLinearDepthToNormalized(LinearDepth, ZNear, ZFar);
    return FMath::Clamp((uint32)(NormDepth * 64.0f), 0u, 63u);
}

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

void FAVBOITBackendTestAutomation::RunMappingTests()
{
    UE_LOG(LogTemp, Log, TEXT("Running CPU/GPU Depth Mapping Tests..."));
    
    float ZNear = 10.0f;
    float ZFar = 1000.0f;
    TArray<float> TestDepths = { 10.0f, 100.0f, 500.0f, 900.0f, 1000.0f };
    
    FAVBOITMappingTestResult GpuResult = FAVBOITBackendDebugReadback::RunMappingTestSync(TestDepths, ZNear, ZFar);
    
    if (GpuResult.NormalizedDepths.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Mapping Test GPU Readback failed!"));
        FGenericPlatformMisc::RequestExitWithStatus(false, 1);
        return;
    }

    bool bPassed = true;
    for (int32 i = 0; i < TestDepths.Num(); ++i)
    {
        float CpuNorm = AVBOITMapLinearDepthToNormalized(TestDepths[i], ZNear, ZFar);
        uint32 CpuSlice = AVBOITMapLinearDepthToSlice(TestDepths[i], ZNear, ZFar);
        
        float GpuNorm = GpuResult.NormalizedDepths[i];
        uint32 GpuSlice = GpuResult.SliceIndices[i];
        
        if (FMath::Abs(CpuNorm - GpuNorm) > 0.001f || CpuSlice != GpuSlice)
        {
            UE_LOG(LogTemp, Error, TEXT("Mapping Test %f failed! CPU: (%.3f, %d), GPU: (%.3f, %d)"), TestDepths[i], CpuNorm, CpuSlice, GpuNorm, GpuSlice);
            bPassed = false;
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("Mapping Test %f PASS: (%.3f, %d)"), TestDepths[i], CpuNorm, CpuSlice);
        }
    }
    
    if (!bPassed)
    {
        FGenericPlatformMisc::RequestExitWithStatus(false, 1);
    }
}

void FAVBOITBackendTestAutomation::BuildTestCases()
{
    TestCases.Empty();

    FAVBOITInjectedFragment WhiteFrag = { FVector4f(1.f, 1.f, 1.f, 0.5f), 100.f, 0, FIntPoint(224, 224), FIntPoint(288, 288), FIntPoint(0, 0) };
    
    FAVBOITInjectedFragment RedFrag   = { FVector4f(1.f, 0.f, 0.f, 0.5f), 100.f, 0, FIntPoint(224, 224), FIntPoint(288, 288), FIntPoint(0, 0) };
    FAVBOITInjectedFragment GreenFrag = { FVector4f(0.f, 1.f, 0.f, 0.5f), 500.f, 1, FIntPoint(224, 224), FIntPoint(288, 288), FIntPoint(0, 0) };
    FAVBOITInjectedFragment BlueFrag  = { FVector4f(0.f, 0.f, 1.f, 0.5f), 900.f, 2, FIntPoint(224, 224), FIntPoint(288, 288), FIntPoint(0, 0) };

    // Case 1: Single White
    {
        FAVBOITBackendTestCase Case;
        Case.Name = TEXT("SingleWhite");
        Case.Fragments.Add(WhiteFrag);
        Case.ExpectedColor = FVector3f(0.5f, 0.5f, 0.5f);
        Case.ExpectedTransmittance = 0.5f;
        Case.ExpectedOccupiedSlices.Add(32);
        TestCases.Add(Case);
    }

    // Physical Reverse: NearRedFarBlue
    {
        FAVBOITBackendTestCase Case;
        Case.Name = TEXT("NearRedFarBlue");
        Case.Fragments.Add(RedFrag);
        Case.Fragments.Add(BlueFrag);
        Case.ExpectedColor = FVector3f(0.5f, 0.0f, 0.25f);
        Case.ExpectedTransmittance = 0.25f;
        Case.ExpectedOccupiedSlices = {32, 62};
        TestCases.Add(Case);
    }
    
    {
        FAVBOITBackendTestCase Case;
        Case.Name = TEXT("NearRedFarBlue_Reverse");
        Case.Fragments.Add(BlueFrag);
        Case.Fragments.Add(RedFrag);
        Case.ExpectedColor = FVector3f(0.5f, 0.0f, 0.25f);
        Case.ExpectedTransmittance = 0.25f;
        Case.ExpectedOccupiedSlices = {32, 62};
        TestCases.Add(Case);
    }

    // Physical Reverse: NearBlueFarRed
    FAVBOITInjectedFragment BlueNearFrag = { FVector4f(0.f, 0.f, 1.f, 0.5f), 100.f, 0, FIntPoint(224, 224), FIntPoint(288, 288), FIntPoint(0, 0) };
    FAVBOITInjectedFragment RedFarFrag   = { FVector4f(1.f, 0.f, 0.f, 0.5f), 900.f, 1, FIntPoint(224, 224), FIntPoint(288, 288), FIntPoint(0, 0) };

    {
        FAVBOITBackendTestCase Case;
        Case.Name = TEXT("NearBlueFarRed");
        Case.Fragments.Add(BlueNearFrag);
        Case.Fragments.Add(RedFarFrag);
        Case.ExpectedColor = FVector3f(0.25f, 0.0f, 0.5f);
        Case.ExpectedTransmittance = 0.25f;
        Case.ExpectedOccupiedSlices = {32, 62};
        TestCases.Add(Case);
    }
    
    {
        FAVBOITBackendTestCase Case;
        Case.Name = TEXT("NearBlueFarRed_Reverse");
        Case.Fragments.Add(RedFarFrag);
        Case.Fragments.Add(BlueNearFrag);
        Case.ExpectedColor = FVector3f(0.25f, 0.0f, 0.5f);
        Case.ExpectedTransmittance = 0.25f;
        Case.ExpectedOccupiedSlices = {32, 62};
        TestCases.Add(Case);
    }

    // Same-Slice
    FAVBOITInjectedFragment RedSame = { FVector4f(1.f, 0.f, 0.f, 0.5f), 500.f, 0, FIntPoint(224, 224), FIntPoint(288, 288), FIntPoint(0, 0) };
    FAVBOITInjectedFragment BlueSame= { FVector4f(0.f, 0.f, 1.f, 0.5f), 500.f, 1, FIntPoint(224, 224), FIntPoint(288, 288), FIntPoint(0, 0) };
    
    {
        FAVBOITBackendTestCase Case;
        Case.Name = TEXT("SameSlice_RedBlue");
        Case.Fragments.Add(RedSame);
        Case.Fragments.Add(BlueSame);
        // T = 0.25. (1-T)/(Ext) = (1-0.25)/(0.693 + 0.693) = 0.75 / 1.386 = 0.541.
        // C_red = 0.693 * 0.541 * (1,0,0) = 0.375
        // C_blue = 0.693 * 0.541 * (0,0,1) = 0.375
        Case.ExpectedColor = FVector3f(0.375f, 0.0f, 0.375f);
        Case.ExpectedTransmittance = 0.25f;
        Case.ExpectedOccupiedSlices = {54};
        Case.ColorMaxAbsTolerance = 0.01f;
        TestCases.Add(Case);
    }

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
        TestCases.Add(Case);
    }
}

void FAVBOITBackendTestAutomation::Tick(float DeltaTime)
{
    if (bShouldStartSuite)
    {
        bShouldStartSuite = false;
        bIsRunning = true;
        RunMappingTests();
        BuildTestCases();
        CurrentTestCaseIndex = 0;
        PassedCaseCount = 0;
        FailedCaseCount = 0;
        
        // Ensure evidence directory exists
        FString EvDir = FPaths::ProjectSavedDir() / TEXT("AVBOIT") / TEXT("LocalMachine") / TEXT("UE4-1-2-Acceptance-Closeout");
        IFileManager::Get().MakeDirectory(*EvDir, true);

        RunNextTest();
        return;
    }

    if (!bIsRunning) return;

    if (PendingReadbackResult || PendingReadbackExtinctionLine)
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
            this->PendingReadbackExtinctionLine.Reset(Readbacks.ExtinctionLine);
            this->PendingReadbackTransmittanceLine.Reset(Readbacks.TransmittanceLine);
            this->WaitFrameCount = 0;
        });

    FlushRenderingCommands();
}

void FAVBOITBackendTestAutomation::HandleReadback()
{
    bool bAllReady = (!PendingReadbackResult || PendingReadbackResult->IsReady()) &&
                     (!PendingReadbackExtinctionLine || PendingReadbackExtinctionLine->IsReady()) &&
                     (!PendingReadbackTransmittanceLine || PendingReadbackTransmittanceLine->IsReady());

    if (!bAllReady)
    {
        WaitFrameCount++;
        if (WaitFrameCount > MaximumWaitFrames)
        {
            UE_LOG(LogTemp, Error, TEXT("Test Case %s: GPU Readback Timeout!"), *TestCases[CurrentTestCaseIndex].Name);
            TestCases[CurrentTestCaseIndex].Status = TEXT("FAIL");
            TestCases[CurrentTestCaseIndex].FailureReasons.Add(TEXT("GPU Readback Timeout"));
            FailedCaseCount++;
            CurrentTestCaseIndex++;
            PendingReadbackResult.Reset();
            PendingReadbackTransmittanceLine.Reset();
            PendingReadbackExtinctionLine.Reset();
            RunNextTest();
        }
        return;
    }

    FAVBOITBackendTestCase& TestCase = TestCases[CurrentTestCaseIndex];
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
            TestCase.FailureReasons.Add(FString::Printf(TEXT("Color mismatch MaxDiff %f"), MaxDiff));
            bCasePassed = false;
        }

        if (DiffT > TestCase.TransmittanceTolerance)
        {
            UE_LOG(LogTemp, Error, TEXT("Test Case %s: Final Transmittance mismatch. Expected: %f, Got: %f (Diff: %f)"), 
                *TestCase.Name, TestCase.ExpectedTransmittance, MeanColor.W, DiffT);
            TestCase.FailureReasons.Add(FString::Printf(TEXT("Final Transmittance mismatch Diff %f"), DiffT));
            bCasePassed = false;
        }
    }

    FString EvDir = FPaths::ProjectSavedDir() / TEXT("AVBOIT") / TEXT("LocalMachine") / TEXT("UE4-1-2-Acceptance-Closeout");
    FString CSVContent = TEXT("Slice,ExpectedExtinction,MeasuredPacked,MeasuredExtinction,ExpectedT,MeasuredT\n");

    // Extinction and Transmittance Lines Check
    if (PendingReadbackExtinctionLine && PendingReadbackTransmittanceLine)
    {
        const uint32* ExtData = (const uint32*)PendingReadbackExtinctionLine->Lock(64 * sizeof(uint32));
        const float* TransData = (const float*)PendingReadbackTransmittanceLine->Lock(64 * sizeof(float));

        TArray<int32> MeasuredOccupiedSlices;
        float ExpectedT = 1.0f;
        
        for (int32 i = 0; i < 64; ++i)
        {
            uint32 MeasuredPacked = ExtData[i];
            float MeasuredExt = MeasuredPacked / 10000.0f;
            float MeasuredT = TransData[i];

            float ExpectedExt = 0.0f;
            if (TestCase.ExpectedOccupiedSlices.Contains(i))
            {
                MeasuredOccupiedSlices.Add(i);
                ExpectedExt = 0.693147f; // alpha=0.5
            }
            
            ExpectedT = ExpectedT * FMath::Exp(-ExpectedExt);
            
            CSVContent += FString::Printf(TEXT("%d,%f,%u,%f,%f,%f\n"), i, ExpectedExt, MeasuredPacked, MeasuredExt, ExpectedT, MeasuredT);

            if (MeasuredExt > 0 && !TestCase.ExpectedOccupiedSlices.Contains(i))
            {
                TestCase.FailureReasons.Add(FString::Printf(TEXT("Unexpected Extinction %f at Slice %d"), MeasuredExt, i));
                bCasePassed = false;
            }
            
            if (FMath::Abs(MeasuredT - ExpectedT) > TestCase.TransmittanceTolerance)
            {
                TestCase.FailureReasons.Add(FString::Printf(TEXT("Transmittance mismatch at Slice %d: Exp %f, Got %f"), i, ExpectedT, MeasuredT));
                bCasePassed = false;
            }
        }

        PendingReadbackExtinctionLine->Unlock();
        PendingReadbackExtinctionLine.Reset();
        PendingReadbackTransmittanceLine->Unlock();
        PendingReadbackTransmittanceLine.Reset();
    }

    FFileHelper::SaveStringToFile(CSVContent, *(EvDir / (TestCase.Name + TEXT(".csv"))));

    if (bCasePassed)
    {
        TestCase.Status = TEXT("PASS");
        PassedCaseCount++;
    }
    else
    {
        TestCase.Status = TEXT("FAIL");
        FailedCaseCount++;
    }

    CurrentTestCaseIndex++;
    RunNextTest();
}

void FAVBOITBackendTestAutomation::FinalizeSuite()
{
    bIsRunning = false;
    
    FString EvDir = FPaths::ProjectSavedDir() / TEXT("AVBOIT") / TEXT("LocalMachine") / TEXT("UE4-1-2-Acceptance-Closeout");
    FString Json = TEXT("{\n");
    Json += FString::Printf(TEXT("\"TotalCases\": %d,\n"), TestCases.Num());
    Json += FString::Printf(TEXT("\"PassedCases\": %d,\n"), PassedCaseCount);
    Json += FString::Printf(TEXT("\"FailedCases\": %d,\n"), FailedCaseCount);
    Json += TEXT("\"Cases\": [\n");
    for (int i = 0; i < TestCases.Num(); ++i)
    {
        Json += TEXT("  {\n");
        Json += FString::Printf(TEXT("    \"Name\": \"%s\",\n"), *TestCases[i].Name);
        Json += FString::Printf(TEXT("    \"Status\": \"%s\",\n"), *TestCases[i].Status);
        Json += TEXT("    \"FailureReasons\": [");
        for (int j=0; j<TestCases[i].FailureReasons.Num(); ++j)
        {
            Json += FString::Printf(TEXT("\"%s\""), *TestCases[i].FailureReasons[j]);
            if (j < TestCases[i].FailureReasons.Num() - 1) Json += TEXT(",");
        }
        Json += TEXT("]\n  }");
        if (i < TestCases.Num() - 1) Json += TEXT(",");
        Json += TEXT("\n");
    }
    Json += TEXT("]\n}\n");
    
    FFileHelper::SaveStringToFile(Json, *(EvDir / TEXT("SuiteSummary.json")));

    UE_LOG(LogTemp, Log, TEXT("======================================"));
    UE_LOG(LogTemp, Log, TEXT("AVBOIT Backend Suite Complete."));
    UE_LOG(LogTemp, Log, TEXT("Passed: %d / %d"), PassedCaseCount, TestCases.Num());
    UE_LOG(LogTemp, Log, TEXT("Failed: %d"), FailedCaseCount);
    UE_LOG(LogTemp, Log, TEXT("======================================"));
    
    FGenericPlatformMisc::RequestExitWithStatus(false, FailedCaseCount > 0 ? 1 : 0);
}
''')
