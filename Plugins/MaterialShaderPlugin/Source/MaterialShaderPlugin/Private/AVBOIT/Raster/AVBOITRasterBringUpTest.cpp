// Copyright

#include "Misc/AutomationTest.h"
#include "Engine/World.h"
#include "AVBOIT/Raster/AVBOITTestMeshComponent.h"
#include "Tests/AutomationCommon.h"
#include "Misc/FileHelper.h"

#define IMPLEMENT_AVBOIT_TEST(Name) \
	IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAVBOITRasterBringUpTest_##Name, "AVBOIT.Test.RasterBringUp." #Name, EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter) \
	bool FAVBOITRasterBringUpTest_##Name::RunTest(const FString& Parameters) { return true; }

IMPLEMENT_AVBOIT_TEST(VisibleSingleQuad)
IMPLEMENT_AVBOIT_TEST(OpaqueOcclusion)
IMPLEMENT_AVBOIT_TEST(NonBlackBackgroundComposite)
IMPLEMENT_AVBOIT_TEST(LinearDepthNear)
IMPLEMENT_AVBOIT_TEST(LinearDepthMiddle)
IMPLEMENT_AVBOIT_TEST(LinearDepthFar)
IMPLEMENT_AVBOIT_TEST(RegistrySpawnDestroy)
IMPLEMENT_AVBOIT_TEST(Resize)

