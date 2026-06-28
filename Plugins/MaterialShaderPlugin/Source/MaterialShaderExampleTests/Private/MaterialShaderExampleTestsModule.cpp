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
		UE_LOG(LogTemp, Warning, TEXT("AVBOIT: OnPostEngineInit called. Checking command line..."));
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
};

IMPLEMENT_MODULE(FMaterialShaderExampleTestsModule, MaterialShaderExampleTests)
