// Copyright Voxel Plugin SAS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "AVBOITSmokeSubsystem.generated.h"

class FAVBOITSceneViewExtension;

UCLASS()
class UAVBOITSmokeSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	//~ Begin UEngineSubsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	//~ End UEngineSubsystem Interface

private:
	TSharedPtr<FAVBOITSceneViewExtension> SceneViewExtension;
};
