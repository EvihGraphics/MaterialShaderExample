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
