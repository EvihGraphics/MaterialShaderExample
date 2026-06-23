#pragma once

#include "CoreMinimal.h"

class FAVBOITEvidenceCommands
{
public:
	static void RegisterCommands();
	static void UnregisterCommands();

private:
	static void CaptureScreenshotAndExit(const TArray<FString>& Args);
	static void CaptureSmokeScreenshotAndExit(const TArray<FString>& Args);
#if WITH_EDITOR
	static void RunEditorPIECloseout(const TArray<FString>& Args);
#endif

	static class IConsoleObject* CaptureScreenshotAndExitCommand;
	static class IConsoleObject* CaptureSmokeScreenshotAndExitCommand;
#if WITH_EDITOR
	static class IConsoleObject* EditorPIECloseoutCommand;
#endif

	static FString FindArgValue(const TArray<FString>& Args, const TCHAR* Prefix);
};
