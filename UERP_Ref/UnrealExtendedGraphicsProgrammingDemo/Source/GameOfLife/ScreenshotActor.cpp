#include "ScreenshotActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "TimerManager.h"

AScreenshotActor::AScreenshotActor() {}

void AScreenshotActor::BeginPlay()
{
    Super::BeginPlay();
    FTimerHandle Handle;
    GetWorld()->GetTimerManager().SetTimer(Handle, [](){
        UKismetSystemLibrary::ExecuteConsoleCommand(nullptr, TEXT("HighResShot 1920x1080"));
        UKismetSystemLibrary::ExecuteConsoleCommand(nullptr, TEXT("Quit"));
    }, 5.0f, false);
}