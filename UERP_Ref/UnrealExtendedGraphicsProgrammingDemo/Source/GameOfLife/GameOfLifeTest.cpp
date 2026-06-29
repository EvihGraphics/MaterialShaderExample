#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameOfLifeTest, "GameOfLife.SmokeTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FGameOfLifeTest::RunTest(const FString& Parameters)
{
    if (GEngine)
    {
        GEngine->Exec(nullptr, TEXT("HighResShot 1"));
    }
    return true;
}