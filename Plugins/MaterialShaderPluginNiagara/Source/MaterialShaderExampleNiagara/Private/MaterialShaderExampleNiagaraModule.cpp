#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/CoreDelegates.h"
#include "Modules/ModuleManager.h"
#include "SceneViewExtension.h"
#include "ShaderCore.h"

#include "AVBOIT/Niagara/AVBOITNiagaraSceneViewExtension.h"
#include "AVBOIT/Niagara/AVBOITNiagaraValidationCommands.h"

class FMaterialShaderExampleNiagaraModule final : public IModuleInterface
{
public:
	virtual void StartupModule() override
	{
		const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("MaterialShaderExampleNiagara"));
		check(Plugin);

		const FString ShaderPath = FPaths::ConvertRelativePathToFull(Plugin->GetBaseDir() / TEXT("Shaders"));
		AddShaderSourceDirectoryMapping(TEXT("/Plugin/MaterialShaderExampleNiagara"), ShaderPath);

		if (GEngine)
		{
			RegisterSceneViewExtension();
		}
		else
		{
			PostEngineInitHandle = FCoreDelegates::OnPostEngineInit.AddRaw(this, &FMaterialShaderExampleNiagaraModule::RegisterSceneViewExtension);
		}

		FAVBOITNiagaraValidationCommands::RegisterCommands();
	}

	virtual void ShutdownModule() override
	{
		if (PostEngineInitHandle.IsValid())
		{
			FCoreDelegates::OnPostEngineInit.Remove(PostEngineInitHandle);
			PostEngineInitHandle.Reset();
		}

		FAVBOITNiagaraValidationCommands::UnregisterCommands();
		SceneViewExtension.Reset();
	}

private:
	void RegisterSceneViewExtension()
	{
		if (!SceneViewExtension.IsValid() && GEngine)
		{
			SceneViewExtension = FSceneViewExtensions::NewExtension<FAVBOITNiagaraSceneViewExtension>();
		}
	}

	TSharedPtr<FAVBOITNiagaraSceneViewExtension, ESPMode::ThreadSafe> SceneViewExtension;
	FDelegateHandle PostEngineInitHandle;
};

IMPLEMENT_MODULE(FMaterialShaderExampleNiagaraModule, MaterialShaderExampleNiagara)
