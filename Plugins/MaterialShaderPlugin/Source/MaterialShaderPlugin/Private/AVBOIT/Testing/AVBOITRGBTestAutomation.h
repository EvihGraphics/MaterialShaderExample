#pragma once

#include "CoreMinimal.h"
#include "Containers/Ticker.h"
#include "AVBOIT/TestContent/AVBOITTestSceneTypes.h"

class UAVBOITTestSceneSubsystem;

class FAVBOITRGBTestAutomation
{
public:
    static void Initialize();
    static void Shutdown();

    static void BuildSceneCmd(const TArray<FString>& Args);
    static void CaptureCurrentCmd(const TArray<FString>& Args);
    static void CaptureRGBSuiteCmd(const TArray<FString>& Args);
    static void DumpManifestCmd(const TArray<FString>& Args);

    static class UMaterial* CreateOrLoadTestMaterial();

private:
    static void CaptureSuiteStep();
    static void FinishCaptureSuite();
    static bool Tick(float DeltaTime);
    static bool WriteLinearPng(const TArray<FFloat16Color>& FloatPixels, FIntPoint Size, const FString& Filename);

    static int32 CurrentSuiteStep;
    static double NextTime;
    static TArray<FString> SuiteSteps;
    static FString CurrentSuiteName;
    static FTSTicker::FDelegateHandle TickerHandle;
};
