// Copyright Voxel Plugin SAS. All Rights Reserved.

#include "AVBOITSmokeSubsystem.h"

#include "AVBOITSceneViewExtension.h"

void UAVBOITSmokeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	SceneViewExtension = FSceneViewExtensions::NewExtension<FAVBOITSceneViewExtension>();
}

void UAVBOITSmokeSubsystem::Deinitialize()
{
	SceneViewExtension.Reset();

	Super::Deinitialize();
}
