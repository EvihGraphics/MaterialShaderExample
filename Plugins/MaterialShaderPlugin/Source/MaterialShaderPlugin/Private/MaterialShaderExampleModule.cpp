// Copyright Voxel Plugin SAS. All Rights Reserved.

#include "CoreMinimal.h"
#include "HAL/IConsoleManager.h"
#include "Interfaces/IPluginManager.h"
#include "UnrealClient.h"

DEFINE_LOG_CATEGORY_STATIC(LogAVBOITEvidence, Log, All);

class FMaterialShaderExampleModule : public IModuleInterface
{
public:
	//~ Begin IModuleInterface Implementation
	virtual void StartupModule() override
	{
		const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin("MaterialShaderExample");
		check(Plugin);

		const FString Path = FPaths::ConvertRelativePathToFull(Plugin->GetBaseDir() / "Shaders");

		AddShaderSourceDirectoryMapping(TEXT("/Plugin/MaterialShaderExample"), Path);

		CaptureScreenshotAndExitCommand = IConsoleManager::Get().RegisterConsoleCommand(
			TEXT("AVBOIT.CaptureScreenshotAndExit"),
			TEXT("Requests a screenshot and exits after FScreenshotRequest reports it was processed. Usage: AVBOIT.CaptureScreenshotAndExit filename=<AbsoluteOrProjectRelativePng>"),
			FConsoleCommandWithArgsDelegate::CreateRaw(this, &FMaterialShaderExampleModule::CaptureScreenshotAndExit),
			ECVF_Default);
	}

	virtual void ShutdownModule() override
	{
		FScreenshotRequest::OnScreenshotRequestProcessed().RemoveAll(this);

		if (CaptureScreenshotAndExitCommand)
		{
			IConsoleManager::Get().UnregisterConsoleObject(CaptureScreenshotAndExitCommand);
			CaptureScreenshotAndExitCommand = nullptr;
		}
	}
	//~ End IModuleInterface Implementation

private:
	void CaptureScreenshotAndExit(const TArray<FString>& Args)
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

		if (Filename.IsEmpty())
		{
			Filename = FPaths::ProjectSavedDir() / TEXT("Screenshots/AVBOIT_CaptureScreenshotAndExit.png");
		}

		FScreenshotRequest::OnScreenshotRequestProcessed().RemoveAll(this);
		FScreenshotRequest::OnScreenshotRequestProcessed().AddRaw(this, &FMaterialShaderExampleModule::OnScreenshotProcessed);
		FScreenshotRequest::RequestScreenshot(Filename, /*bInShowUI*/ false, /*bAddUniqueSuffix*/ false, /*bHdrScreenshot*/ false);

		UE_LOG(LogAVBOITEvidence, Display, TEXT("Requested screenshot before exit: %s"), *Filename);
	}

	void OnScreenshotProcessed()
	{
		FScreenshotRequest::OnScreenshotRequestProcessed().RemoveAll(this);
		UE_LOG(LogAVBOITEvidence, Display, TEXT("Screenshot processed; requesting exit."));
		FPlatformMisc::RequestExit(false);
	}

	IConsoleObject* CaptureScreenshotAndExitCommand = nullptr;
};
IMPLEMENT_MODULE(FMaterialShaderExampleModule, MaterialShaderExample)
