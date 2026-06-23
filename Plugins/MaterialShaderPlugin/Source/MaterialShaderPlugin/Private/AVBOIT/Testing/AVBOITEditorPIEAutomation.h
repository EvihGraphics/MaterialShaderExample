#pragma once

#include "CoreMinimal.h"
#include "Containers/Ticker.h"

#if WITH_EDITOR

class FAVBOITEditorPIEAutomation
{
public:
	static void RunEditorPIECloseout(const FString& Root, bool bCaptureScreenshots);
	static void FinishEditorPIECloseout(bool bRequestExit);

private:
	static void ApplySmokeState(int32 Enable, int32 DebugMode);
	static bool IsPIEActive();
	static void StartPIE();
	static void EndPIE();

	static void RequestCloseoutScreenshot(const TCHAR* RelativePath);
	static class FViewport* FindCloseoutViewport();
	static bool WriteCloseoutViewportPng(const FString& Filename);

	static void AdvanceCloseoutAfterDelay(double DelaySeconds);
	static bool TickEditorPIECloseout(float DeltaTime);

	static void OnEditorPIECloseoutBeginPIE(const bool bIsSimulating);
	static void OnEditorPIECloseoutEndPIE(const bool bIsSimulating);

	static FTSTicker::FDelegateHandle EditorPIECloseoutTickerHandle;
	static FDelegateHandle EditorPIECloseoutBeginPIEHandle;
	static FDelegateHandle EditorPIECloseoutEndPIEHandle;
	static FString EditorPIECloseoutRoot;
	static double EditorPIECloseoutNextTime;
	static int32 EditorPIECloseoutStep;
	static bool bEditorPIECloseoutRunning;
	static bool bEditorPIECloseoutCaptureScreenshots;
};

#endif
