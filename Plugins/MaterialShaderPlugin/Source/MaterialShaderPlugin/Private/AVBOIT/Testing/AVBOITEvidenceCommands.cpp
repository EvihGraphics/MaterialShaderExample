#include "AVBOITEvidenceCommands.h"
#include "AVBOITScreenshotAutomation.h"
#include "AVBOITEvidencePath.h"
#if WITH_EDITOR
#include "AVBOITEditorPIEAutomation.h"
#endif
#include "AVBOITRGBTestAutomation.h"
#include "HAL/IConsoleManager.h"

IConsoleObject* FAVBOITEvidenceCommands::CaptureScreenshotAndExitCommand = nullptr;
IConsoleObject* FAVBOITEvidenceCommands::CaptureSmokeScreenshotAndExitCommand = nullptr;
#if WITH_EDITOR
IConsoleObject* FAVBOITEvidenceCommands::EditorPIECloseoutCommand = nullptr;
#endif
IConsoleObject* FAVBOITEvidenceCommands::TestBuildSceneCommand = nullptr;
IConsoleObject* FAVBOITEvidenceCommands::TestCaptureCurrentCommand = nullptr;
IConsoleObject* FAVBOITEvidenceCommands::TestCaptureRGBSuiteCommand = nullptr;
IConsoleObject* FAVBOITEvidenceCommands::TestDumpManifestCommand = nullptr;

FString FAVBOITEvidenceCommands::FindArgValue(const TArray<FString>& Args, const TCHAR* Prefix)
{
	for (const FString& Arg : Args)
	{
		if (Arg.StartsWith(Prefix))
		{
			return Arg.RightChop(FCString::Strlen(Prefix));
		}
	}
	return FString();
}

void FAVBOITEvidenceCommands::RegisterCommands()
{
	CaptureScreenshotAndExitCommand = IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("AVBOIT.CaptureScreenshotAndExit"),
		TEXT("Requests a screenshot and exits. Usage: AVBOIT.CaptureScreenshotAndExit filename=<path>"),
		FConsoleCommandWithArgsDelegate::CreateStatic(&FAVBOITEvidenceCommands::CaptureScreenshotAndExit),
		ECVF_Default);

	CaptureSmokeScreenshotAndExitCommand = IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("AVBOIT.CaptureSmokeScreenshotAndExit"),
		TEXT("Capture RDG smoke screenshot. Usage: mode=<1|2|3> opacity=<float> filename=<png>"),
		FConsoleCommandWithArgsDelegate::CreateStatic(&FAVBOITEvidenceCommands::CaptureSmokeScreenshotAndExit),
		ECVF_Default);

#if WITH_EDITOR
	EditorPIECloseoutCommand = IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("AVBOIT.RunEditorPIECloseout"),
		TEXT("Runs closeout. Usage: root=<path> screenshots=<0|1>"),
		FConsoleCommandWithArgsDelegate::CreateStatic(&FAVBOITEvidenceCommands::RunEditorPIECloseout),
		ECVF_Default);
#endif
	TestBuildSceneCommand = IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("AVBOIT.Test.BuildScene"),
		TEXT("Builds the RGB test scene based on current CVars"),
		FConsoleCommandWithArgsDelegate::CreateStatic(&FAVBOITRGBTestAutomation::BuildSceneCmd),
		ECVF_Default);

	TestCaptureCurrentCommand = IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("AVBOIT.Test.CaptureCurrent"),
		TEXT("Captures the current test scene"),
		FConsoleCommandWithArgsDelegate::CreateStatic(&FAVBOITRGBTestAutomation::CaptureCurrentCmd),
		ECVF_Default);

	TestCaptureRGBSuiteCommand = IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("AVBOIT.Test.CaptureRGBSuite"),
		TEXT("Captures the whole RGB suite"),
		FConsoleCommandWithArgsDelegate::CreateStatic(&FAVBOITRGBTestAutomation::CaptureRGBSuiteCmd),
		ECVF_Default);

	TestDumpManifestCommand = IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("AVBOIT.Test.DumpManifest"),
		TEXT("Dumps manifest of the current test scene"),
		FConsoleCommandWithArgsDelegate::CreateStatic(&FAVBOITRGBTestAutomation::DumpManifestCmd),
		ECVF_Default);
}

void FAVBOITEvidenceCommands::UnregisterCommands()
{
	if (CaptureScreenshotAndExitCommand)
	{
		IConsoleManager::Get().UnregisterConsoleObject(CaptureScreenshotAndExitCommand);
		CaptureScreenshotAndExitCommand = nullptr;
	}
	if (CaptureSmokeScreenshotAndExitCommand)
	{
		IConsoleManager::Get().UnregisterConsoleObject(CaptureSmokeScreenshotAndExitCommand);
		CaptureSmokeScreenshotAndExitCommand = nullptr;
	}
#if WITH_EDITOR
	if (EditorPIECloseoutCommand)
	{
		IConsoleManager::Get().UnregisterConsoleObject(EditorPIECloseoutCommand);
		EditorPIECloseoutCommand = nullptr;
	}
#endif
	if (TestBuildSceneCommand) IConsoleManager::Get().UnregisterConsoleObject(TestBuildSceneCommand);
	if (TestCaptureCurrentCommand) IConsoleManager::Get().UnregisterConsoleObject(TestCaptureCurrentCommand);
	if (TestCaptureRGBSuiteCommand) IConsoleManager::Get().UnregisterConsoleObject(TestCaptureRGBSuiteCommand);
	if (TestDumpManifestCommand) IConsoleManager::Get().UnregisterConsoleObject(TestDumpManifestCommand);
}

void FAVBOITEvidenceCommands::CaptureScreenshotAndExit(const TArray<FString>& Args)
{
	FString Filename;
	for (const FString& Arg : Args)
	{
		if (Arg.StartsWith(TEXT("filename=")))
		{
			Filename = Arg.RightChop(9);
			break;
		}
		if (Filename.IsEmpty())
		{
			Filename = Arg;
		}
	}
	FAVBOITScreenshotAutomation::CaptureScreenshotAndExit(Filename);
}

void FAVBOITEvidenceCommands::CaptureSmokeScreenshotAndExit(const TArray<FString>& Args)
{
	int32 DebugMode = 2;
	if (const FString ModeArg = FindArgValue(Args, TEXT("mode=")); !ModeArg.IsEmpty())
	{
		DebugMode = FCString::Atoi(*ModeArg);
	}

	float Opacity = 0.65f;
	if (const FString OpacityArg = FindArgValue(Args, TEXT("opacity=")); !OpacityArg.IsEmpty())
	{
		Opacity = FCString::Atof(*OpacityArg);
	}

	FString Filename = FindArgValue(Args, TEXT("filename="));
	FAVBOITScreenshotAutomation::CaptureSmokeScreenshotAndExit(DebugMode, Opacity, Filename);
}

#if WITH_EDITOR
void FAVBOITEvidenceCommands::RunEditorPIECloseout(const TArray<FString>& Args)
{
	FString Root = FindArgValue(Args, TEXT("root="));
	Root = FAVBOITEvidencePath::ResolveAVBOITEvidenceRoot(TEXT("UE2-Closeout"), Root);

	bool bCaptureScreenshots = FindArgValue(Args, TEXT("screenshots=")) != TEXT("0");
	FAVBOITEditorPIEAutomation::RunEditorPIECloseout(Root, bCaptureScreenshots);
}
#endif
