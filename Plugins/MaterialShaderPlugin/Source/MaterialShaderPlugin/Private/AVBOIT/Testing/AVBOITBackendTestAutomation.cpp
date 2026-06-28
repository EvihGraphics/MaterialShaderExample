#include "AVBOIT/Testing/AVBOITBackendTestAutomation.h"
#include "AVBOIT/Backend/AVBOITBackendRenderer.h"
#include "AVBOIT/Backend/AVBOITBackendDebugReadback.h"
#include "RenderGraphBuilder.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "RenderingThread.h"
#include "HAL/FileManager.h"

void FAVBOITBackendTestAutomation::Initialize() {}
void FAVBOITBackendTestAutomation::Shutdown() {}

float AVBOITMapLinearDepthToNormalized2(float LinearDepth, float ZNear, float ZFar)
{
    float DepthRatio = FMath::Clamp(LinearDepth / ZNear, 1.0f, ZFar / ZNear);
    return FMath::Log2(DepthRatio) / FMath::Log2(ZFar / ZNear);
}

TArray<FAVBOITBackendTestCase> FAVBOITBackendTestAutomation::RunSuite()
{
    TArray<FAVBOITBackendTestCase> TestCases;

    FAVBOITInjectedFragment WhiteFrag = { FVector4f(1.f, 1.f, 1.f, 0.5f), 100.f, 0, FIntPoint(224, 224), FIntPoint(288, 288), FIntPoint(0, 0) };
    FAVBOITInjectedFragment RedFrag   = { FVector4f(1.f, 0.f, 0.f, 0.5f), 100.f, 0, FIntPoint(224, 224), FIntPoint(288, 288), FIntPoint(0, 0) };
    FAVBOITInjectedFragment GreenFrag = { FVector4f(0.f, 1.f, 0.f, 0.5f), 500.f, 1, FIntPoint(224, 224), FIntPoint(288, 288), FIntPoint(0, 0) };
    FAVBOITInjectedFragment BlueFrag  = { FVector4f(0.f, 0.f, 1.f, 0.5f), 900.f, 2, FIntPoint(224, 224), FIntPoint(288, 288), FIntPoint(0, 0) };

    { FAVBOITBackendTestCase Case; Case.Name = TEXT("SingleWhite"); Case.Fragments.Add(WhiteFrag); Case.ExpectedColor = FVector3f(0.5f, 0.5f, 0.5f); Case.ExpectedTransmittance = 0.5f; TestCases.Add(Case); }
    { FAVBOITBackendTestCase Case; Case.Name = TEXT("NearRedFarBlue"); Case.Fragments.Add(RedFrag); Case.Fragments.Add(BlueFrag); Case.ExpectedColor = FVector3f(0.5f, 0.0f, 0.25f); Case.ExpectedTransmittance = 0.25f; TestCases.Add(Case); }
    { FAVBOITBackendTestCase Case; Case.Name = TEXT("NearRedFarBlue_Reverse"); Case.Fragments.Add(BlueFrag); Case.Fragments.Add(RedFrag); Case.ExpectedColor = FVector3f(0.5f, 0.0f, 0.25f); Case.ExpectedTransmittance = 0.25f; TestCases.Add(Case); }

    FAVBOITInjectedFragment BlueNearFrag = { FVector4f(0.f, 0.f, 1.f, 0.5f), 100.f, 0, FIntPoint(224, 224), FIntPoint(288, 288), FIntPoint(0, 0) };
    FAVBOITInjectedFragment RedFarFrag   = { FVector4f(1.f, 0.f, 0.f, 0.5f), 900.f, 1, FIntPoint(224, 224), FIntPoint(288, 288), FIntPoint(0, 0) };

    { FAVBOITBackendTestCase Case; Case.Name = TEXT("NearBlueFarRed"); Case.Fragments.Add(BlueNearFrag); Case.Fragments.Add(RedFarFrag); Case.ExpectedColor = FVector3f(0.25f, 0.0f, 0.5f); Case.ExpectedTransmittance = 0.25f; TestCases.Add(Case); }
    { FAVBOITBackendTestCase Case; Case.Name = TEXT("NearBlueFarRed_Reverse"); Case.Fragments.Add(RedFarFrag); Case.Fragments.Add(BlueNearFrag); Case.ExpectedColor = FVector3f(0.25f, 0.0f, 0.5f); Case.ExpectedTransmittance = 0.25f; TestCases.Add(Case); }

    FAVBOITInjectedFragment RedSame = { FVector4f(1.f, 0.f, 0.f, 0.5f), 500.f, 0, FIntPoint(224, 224), FIntPoint(288, 288), FIntPoint(0, 0) };
    FAVBOITInjectedFragment BlueSame= { FVector4f(0.f, 0.f, 1.f, 0.5f), 500.f, 1, FIntPoint(224, 224), FIntPoint(288, 288), FIntPoint(0, 0) };
    
    { FAVBOITBackendTestCase Case; Case.Name = TEXT("SameSlice_RedBlue"); Case.Fragments.Add(RedSame); Case.Fragments.Add(BlueSame); Case.ExpectedColor = FVector3f(0.5f, 0.0f, 0.5f); Case.ExpectedTransmittance = 0.25f; Case.ColorMaxAbsTolerance = 0.01f; TestCases.Add(Case); }

    TArray<TArray<FAVBOITInjectedFragment>> TriplePerms = {
        {RedFrag, GreenFrag, BlueFrag}, {RedFrag, BlueFrag, GreenFrag}, {GreenFrag, RedFrag, BlueFrag},
        {GreenFrag, BlueFrag, RedFrag}, {BlueFrag, RedFrag, GreenFrag}, {BlueFrag, GreenFrag, RedFrag}
    };
    TArray<FString> TripleNames = { TEXT("RGB"), TEXT("RBG"), TEXT("GRB"), TEXT("GBR"), TEXT("BRG"), TEXT("BGR") };
    for (int32 i = 0; i < TriplePerms.Num(); ++i) {
        FAVBOITBackendTestCase Case; Case.Name = FString::Printf(TEXT("Triple_%s"), *TripleNames[i]); Case.Fragments = TriplePerms[i];
        Case.ExpectedColor = FVector3f(0.5f, 0.25f, 0.125f); Case.ExpectedTransmittance = 0.125f; TestCases.Add(Case);
    }

    FAVBOITBackendSettings Settings;
    Settings.bEnabled = true;
    Settings.Mode = 1;
    Settings.Resolution = FIntPoint(512, 512);
    Settings.ZNear = 10.0f;
    Settings.ZFar = 1000.0f;

    for (int i=0; i<TestCases.Num(); ++i)
    {
        FAVBOITBackendTestCase& TestCase = TestCases[i];
        TestCase.Status = TEXT("FAILED");
        
        FRHIGPUTextureReadback* ReadbackR = nullptr;
        FRHIGPUBufferReadback* ReadbackE = nullptr;
        FRHIGPUBufferReadback* ReadbackT = nullptr;

        ENQUEUE_RENDER_COMMAND(RunCase)(
            [Settings, Fragments = TestCase.Fragments, &ReadbackR, &ReadbackE, &ReadbackT](FRHICommandListImmediate& RHICmdList)
            {
                FRDGBuilder GraphBuilder(RHICmdList);
                FAVBOITBackendReadbacks Readbacks = FAVBOITBackendRenderer::Execute(GraphBuilder, Settings, Fragments);
                ReadbackR = Readbacks.Result;
                ReadbackE = Readbacks.ExtinctionLine;
                ReadbackT = Readbacks.TransmittanceLine;
                GraphBuilder.Execute();
            });

        FlushRenderingCommands();

        uint32 WaitStart = FPlatformTime::Cycles();
        uint32 TimeoutMs = 60000;
        bool bReady = false;

        while (!bReady)
        {
            if (ReadbackR && ReadbackE && ReadbackT && ReadbackR->IsReady() && ReadbackE->IsReady() && ReadbackT->IsReady())
            {
                bReady = true;
                break;
            }

            if (FPlatformTime::ToMilliseconds(FPlatformTime::Cycles() - WaitStart) > TimeoutMs)
            {
                TestCase.FailureReasons.Add(TEXT("Readback Timeout"));
                break;
            }
            FPlatformProcess::Sleep(0.01f);
        }

        if (bReady)
        {
            const void* RawResultData = nullptr;
            const void* RawExtData = nullptr;
            const void* RawTransData = nullptr;
            int32 OutRowPitch = 0;

            ENQUEUE_RENDER_COMMAND(Lock)(
                [&](FRHICommandListImmediate& RHICmdList)
                {
                    RawResultData = ReadbackR->Lock(OutRowPitch);
                    RawExtData = ReadbackE->Lock(64 * sizeof(uint32));
                    RawTransData = ReadbackT->Lock(64 * sizeof(float));
                });
            FlushRenderingCommands();

            if (RawResultData && RawExtData && RawTransData)
            {
                const FFloat16Color* ColorData = static_cast<const FFloat16Color*>(RawResultData);
                const uint32* ExtData = static_cast<const uint32*>(RawExtData);
                const float* TransData = static_cast<const float*>(RawTransData);

                FVector4f SumColor(0,0,0,0);
                int32 Count = 0;
                for (int32 y = 224; y < 288; ++y)
                {
                    for (int32 x = 224; x < 288; ++x)
                    {
                        int32 Index = y * (OutRowPitch / sizeof(FFloat16Color)) + x;
                        FFloat16Color C = ColorData[Index];
                        SumColor += FVector4f(C.R.GetFloat(), C.G.GetFloat(), C.B.GetFloat(), C.A.GetFloat());
                        Count++;
                    }
                }
                FVector4f MeanColor = SumColor / (float)Count;

                float MaxDiff = FMath::Max3(FMath::Abs(MeanColor.X - TestCase.ExpectedColor.X), FMath::Abs(MeanColor.Y - TestCase.ExpectedColor.Y), FMath::Abs(MeanColor.Z - TestCase.ExpectedColor.Z));
                float DiffT = FMath::Abs(MeanColor.W - TestCase.ExpectedTransmittance);

                bool bPassed = true;
                if (MaxDiff > TestCase.ColorMaxAbsTolerance)
                {
                    TestCase.FailureReasons.Add(FString::Printf(TEXT("Color mismatch: Expected=(%f, %f, %f) Actual=(%f, %f, %f)"), TestCase.ExpectedColor.X, TestCase.ExpectedColor.Y, TestCase.ExpectedColor.Z, MeanColor.X, MeanColor.Y, MeanColor.Z));
                    bPassed = false;
                }
                if (DiffT > TestCase.TransmittanceTolerance)
                {
                    TestCase.FailureReasons.Add(FString::Printf(TEXT("Transmittance mismatch: Expected=%f Actual=%f"), TestCase.ExpectedTransmittance, MeanColor.W));
                    bPassed = false;
                }

                TArray<float> ExpectedExtinctions;
                ExpectedExtinctions.Init(0.0f, 64);
                for (const auto& Frag : TestCase.Fragments)
                {
                    float NormDepth = AVBOITMapLinearDepthToNormalized2(Frag.LinearDepth, 10.0f, 1000.0f);
                    uint32 Slice = FMath::Clamp((uint32)(NormDepth * 64.0f), 0u, 63u);
                    float Alpha = FMath::Clamp(Frag.LinearColorAndAlpha.W, 0.0f, 0.99f);
                    ExpectedExtinctions[Slice] += -FMath::Loge(1.0f - Alpha);
                }

                float ExpectedT = 1.0f;
                for (int32 s = 0; s < 64; ++s)
                {
                    float MeasuredExt = ExtData[s] / 10000.0f;
                    float MeasuredT = TransData[s];

                    float ExpectedExt = ExpectedExtinctions[s];
                    ExpectedT = ExpectedT * FMath::Exp(-ExpectedExt);

                    if (MeasuredExt > 0.001f && ExpectedExt < 0.001f)
                    {
                        TestCase.FailureReasons.Add(TEXT("Unexpected Extinction"));
                        bPassed = false;
                    }

                    if (FMath::Abs(MeasuredT - ExpectedT) > TestCase.TransmittanceTolerance)
                    {
                        TestCase.FailureReasons.Add(TEXT("Transmittance mismatch slice"));
                        bPassed = false;
                    }
                }

                if (bPassed)
                {
                    TestCase.Status = TEXT("PASSED");
                }
            }
            else
            {
                TestCase.FailureReasons.Add(TEXT("Lock failed"));
            }

            ENQUEUE_RENDER_COMMAND(Unlock)(
                [&](FRHICommandListImmediate& RHICmdList)
                {
                    ReadbackR->Unlock();
                    ReadbackE->Unlock();
                    ReadbackT->Unlock();
                });
            FlushRenderingCommands();
        }

        // FAVBOITBackendRenderer internally creates these via "new", so we must delete them.
        delete ReadbackR;
        delete ReadbackE;
        delete ReadbackT;
    }

    return TestCases;
}
