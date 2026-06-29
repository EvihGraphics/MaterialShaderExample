#pragma once

#include "CoreMinimal.h"

class UWorld;
class AActor;
class ACameraActor;
class UAVBOITTestMeshComponent;

struct FAVBOITEditorSceneState
{
	UWorld* World = nullptr;
	ACameraActor* CameraActor = nullptr;
	AActor* TestActor = nullptr;
	UAVBOITTestMeshComponent* TestMesh = nullptr;
	AActor* OccluderActor = nullptr;
	AActor* BackgroundActor = nullptr;
};

class FAVBOITEditorSceneBuilder
{
public:
	static void CleanupExistingTestActors(UWorld* World);
	static FAVBOITEditorSceneState BuildTestScene(UWorld* World);

	static void SetTestMeshVisible(FAVBOITEditorSceneState& State, bool bVisible);
	static void SetOccluderVisible(FAVBOITEditorSceneState& State, bool bVisible);

	static void ApplyDeterministicSettings();
	static void RestoreSettings();

	static void ForceResizeViewport(int32 Width, int32 Height);
};
