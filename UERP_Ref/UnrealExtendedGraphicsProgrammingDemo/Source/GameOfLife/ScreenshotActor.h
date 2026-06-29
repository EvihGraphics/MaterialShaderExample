#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScreenshotActor.generated.h"

UCLASS()
class GAMEOFLIFE_API AScreenshotActor : public AActor
{
    GENERATED_BODY()
public:
    AScreenshotActor();
    virtual void BeginPlay() override;
};