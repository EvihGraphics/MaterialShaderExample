// Copyright

#include "Modules/ModuleManager.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "Containers/Ticker.h"
#include "HAL/PlatformMisc.h"
#include "Misc/FileHelper.h"

class FMaterialShaderExampleTestsModule : public IModuleInterface
{
public:
	virtual void StartupModule() override
	{
		FCoreDelegates::OnPostEngineInit.AddRaw(this, &FMaterialShaderExampleTestsModule::OnPostEngineInit);
	}

	virtual void ShutdownModule() override
	{
		FCoreDelegates::OnPostEngineInit.RemoveAll(this);
	}

private:
	void OnPostEngineInit()
	{
		// Test hooks (if any non-commandlet tests remain in the future)
	}
};

IMPLEMENT_MODULE(FMaterialShaderExampleTestsModule, MaterialShaderExampleTests)
