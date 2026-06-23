#include "AVBOITScreenshotAutomation.h"
#include "HAL/IConsoleManager.h"
#include "Misc/Paths.h"
#include "UnrealClient.h"

void FAVBOITScreenshotAutomation::CaptureScreenshotAndExit(const FString& Filename)
{
	FString TargetFilename = Filename;
	if (TargetFilename.IsEmpty())
	{
		TargetFilename = FPaths::ProjectSavedDir() / TEXT("Screenshots/AVBOIT_CaptureScreenshotAndExit.png");
	}

	FScreenshotRequest::OnScreenshotRequestProcessed().RemoveAll(&FAVBOITScreenshotAutomation::OnScreenshotProcessed);
	FScreenshotRequest::OnScreenshotRequestProcessed().AddStatic(&FAVBOITScreenshotAutomation::OnScreenshotProcessed);
	FScreenshotRequest::RequestScreenshot(TargetFilename, /*bInShowUI*/ false, /*bAddUniqueSuffix*/ false, /*bHdrScreenshot*/ false);

	UE_LOG(LogTemp, Display, TEXT("Requested screenshot before exit: %s"), *TargetFilename);
}

void FAVBOITScreenshotAutomation::CaptureSmokeScreenshotAndExit(int32 DebugMode, float Opacity, const FString& Filename)
{
	if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ShaderDevelopmentMode"))) CVar->Set(1, ECVF_SetByConsole);
	if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.RDG.Events"))) CVar->Set(1, ECVF_SetByConsole);
	if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.RDG.Debug"))) CVar->Set(1, ECVF_SetByConsole);
	if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.AVBOIT.Smoke.Enable"))) CVar->Set(1, ECVF_SetByConsole);
	if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.AVBOIT.Smoke.DebugMode"))) CVar->Set(DebugMode, ECVF_SetByConsole);
	if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.AVBOIT.Smoke.Opacity"))) CVar->Set(Opacity, ECVF_SetByConsole);
	SetEmitDrawEvents(true);

	FString TargetFilename = Filename;
	if (TargetFilename.IsEmpty())
	{
		TargetFilename = FPaths::ProjectSavedDir() / TEXT("Screenshots/AVBOIT_CaptureSmokeScreenshotAndExit.png");
	}

	UE_LOG(LogTemp, Display, TEXT("Configured smoke capture: Mode=%d Opacity=%f DrawEvents=true"), DebugMode, Opacity);
	CaptureScreenshotAndExit(TargetFilename);
}

void FAVBOITScreenshotAutomation::OnScreenshotProcessed()
{
	FScreenshotRequest::OnScreenshotRequestProcessed().RemoveAll(&FAVBOITScreenshotAutomation::OnScreenshotProcessed);
	UE_LOG(LogTemp, Display, TEXT("Screenshot processed; requesting exit."));
	FPlatformMisc::RequestExit(false);
}
