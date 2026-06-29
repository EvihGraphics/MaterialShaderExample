#include "Misc/AutomationTest.h"
#include "Engine/World.h"
#include "Editor.h"
#include "Tests/AutomationCommon.h"
#include "AVBOIT/Testing/AVBOITTheForgeSceneBuilder.h"
#include "AVBOIT/Testing/AVBOITScreenshotAutomation.h"
#include "HAL/IConsoleManager.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAVBOITTheForgeParityTest, "AVBOIT.Parity", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

DEFINE_LATENT_AUTOMATION_COMMAND(FBuildTheForgeSceneCommand);
bool FBuildTheForgeSceneCommand::Update()
{
	if (!GEditor) return true;
	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (World)
	{
		FAVBOITTheForgeSceneBuilder::BuildSceneFromContract(World);
	}
	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND(FCaptureTheForgeScreenshotCommand);
bool FCaptureTheForgeScreenshotCommand::Update()
{
	FString EvidenceDir = FPaths::ProjectDir() / TEXT("LocalVisualResults/UE57/HIVE_4090x2/UE4-2B-TheForgeParity");
	FString Filename = EvidenceDir / TEXT("TheForgeParity.png");
	FAVBOITScreenshotAutomation::CaptureScreenshotAndExit(Filename);
	return true; 
}

bool FAVBOITTheForgeParityTest::RunTest(const FString& Parameters)
{
	if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.AVBOIT.Raster.Enable")))
	{
		CVar->Set(1, ECVF_SetByConsole);
	}
	ADD_LATENT_AUTOMATION_COMMAND(FBuildTheForgeSceneCommand());
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(2.0f));
	ADD_LATENT_AUTOMATION_COMMAND(FCaptureTheForgeScreenshotCommand());
	return true;
}
