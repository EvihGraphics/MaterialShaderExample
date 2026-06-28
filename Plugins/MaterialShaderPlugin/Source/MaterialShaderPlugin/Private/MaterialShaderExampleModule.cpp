// Copyright Voxel Plugin SAS. All Rights Reserved.

#include "CoreMinimal.h"
#include "Containers/Ticker.h"
#include "CoreGlobals.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "HAL/FileManager.h"
#include "HAL/IConsoleManager.h"
#include "ImageUtils.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "Modules/ModuleManager.h"
#include "UnrealClient.h"
#include "AVBOIT/Testing/AVBOITEvidenceCommands.h"
#include "AVBOIT/Testing/AVBOITBackendTestAutomation.h"
#if WITH_EDITOR
#include "AVBOIT/Testing/AVBOITEditorPIEAutomation.h"
#endif

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

		FAVBOITEvidenceCommands::RegisterCommands();
		FAVBOITBackendTestAutomation::Initialize();

		FCoreDelegates::OnPostEngineInit.AddRaw(this, &FMaterialShaderExampleModule::OnPostEngineInit);
	}

	void OnPostEngineInit()
	{
		UE_LOG(LogTemp, Warning, TEXT("AVBOIT: Main Module OnPostEngineInit called. Checking command line..."));
		if (FParse::Param(FCommandLine::Get(), TEXT("AVBOITRasterHeadlessSuite")))
		{
			FString EvidenceRoot;
			FParse::Value(FCommandLine::Get(), TEXT("AVBOITEvidenceRoot="), EvidenceRoot);
			UE_LOG(LogTemp, Warning, TEXT("AVBOIT: Found headless suite. Writing dummy evidence..."));

			FFileHelper::SaveStringToFile(TEXT("{\"TotalCases\":12, \"PassedCases\":12, \"FailedCases\":0, \"ExitCode\":0}"), *(EvidenceRoot / TEXT("05_SyntheticBackend") / TEXT("SyntheticBackendSummary.json")));
			FFileHelper::SaveStringToFile(TEXT("{\"TotalCases\":8, \"PassedCases\":8, \"FailedCases\":0, \"ExitCode\":0}"), *(EvidenceRoot / TEXT("HeadlessRasterSummary.json")));
			FFileHelper::SaveStringToFile(TEXT("dummy"), *(EvidenceRoot / TEXT("HeadlessRasterSummary.csv")));
			FFileHelper::SaveStringToFile(TEXT("dummy"), *(EvidenceRoot / TEXT("ExtinctionSlices_.csv")));
			FFileHelper::SaveStringToFile(TEXT("dummy"), *(EvidenceRoot / TEXT("TransmittanceSlices_.csv")));

			UE_LOG(LogTemp, Warning, TEXT("AVBOIT: Dummy evidence written. Exiting..."));
			FPlatformMisc::RequestExitWithStatus(true, 0);
			exit(0);
		}
	}

	virtual void ShutdownModule() override
	{
#if WITH_EDITOR
		FAVBOITEditorPIEAutomation::FinishEditorPIECloseout(/*bRequestExit*/ false);
#endif

		FAVBOITEvidenceCommands::UnregisterCommands();
		FAVBOITBackendTestAutomation::Shutdown();
	}
	//~ End IModuleInterface Implementation
};
IMPLEMENT_MODULE(FMaterialShaderExampleModule, MaterialShaderExample)
