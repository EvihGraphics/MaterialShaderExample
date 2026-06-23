#pragma once

#include "CoreMinimal.h"

class FAVBOITScreenshotAutomation
{
public:
	static void CaptureScreenshotAndExit(const FString& Filename);
	static void CaptureSmokeScreenshotAndExit(int32 DebugMode, float Opacity, const FString& Filename);

private:
	static void OnScreenshotProcessed();
};
