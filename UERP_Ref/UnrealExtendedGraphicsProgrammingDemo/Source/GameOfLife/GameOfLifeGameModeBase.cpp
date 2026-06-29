// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOfLifeGameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "TimerManager.h"

void AGameOfLifeGameModeBase::StartPlay()
{
    Super::StartPlay();
    FTimerHandle Handle;
    GetWorld()->GetTimerManager().SetTimer(Handle, [](){
        UKismetSystemLibrary::ExecuteConsoleCommand(nullptr, TEXT("HighResShot 1280x720"));
        UKismetSystemLibrary::ExecuteConsoleCommand(nullptr, TEXT("Quit"));
    }, 5.0f, false);
}

