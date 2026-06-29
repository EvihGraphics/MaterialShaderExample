#include "AVBOITEditorSceneBuilder.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "AVBOIT/Raster/AVBOITTestMeshComponent.h"
#include "HAL/IConsoleManager.h"

#if WITH_EDITOR
#include "Editor.h"
#include "Editor/EditorEngine.h"
#include "LevelEditorViewport.h"
#endif

static const FName AVBOIT_TEST_TAG = TEXT("AVBOIT_AutomatedSceneIntegration");

static int32 OldDynamicRes = 0;
static int32 OldMotionBlur = 0;
static int32 OldMotionBlurFeature = 0;
static int32 OldTAAUpsampling = 0;
static int32 OldEyeAdaptation = 0;
static int32 OldBloom = 0;
static int32 OldSceneColorFringe = 0;
static int32 OldTonemapperGrain = 0;
static int32 OldVSync = 0;

void FAVBOITEditorSceneBuilder::CleanupExistingTestActors(UWorld* World)
{
	if (!World) return;
	
	TArray<AActor*> ActorsToDestroy;
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		if (It->ActorHasTag(AVBOIT_TEST_TAG))
		{
			ActorsToDestroy.Add(*It);
		}
	}
	for (AActor* Actor : ActorsToDestroy)
	{
		Actor->Destroy();
	}
}

FAVBOITEditorSceneState FAVBOITEditorSceneBuilder::BuildTestScene(UWorld* World)
{
	FAVBOITEditorSceneState State;
	State.World = World;
	if (!World) return State;

	CleanupExistingTestActors(World);

	// 1. Camera
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	State.CameraActor = World->SpawnActor<ACameraActor>(FVector(0, 0, 100), FRotator(0, 0, 0), SpawnParams);
	State.CameraActor->Tags.Add(AVBOIT_TEST_TAG);
	State.CameraActor->GetCameraComponent()->SetAspectRatio(16.0f / 9.0f);
	State.CameraActor->GetCameraComponent()->SetFieldOfView(90.0f);
	State.CameraActor->GetCameraComponent()->PostProcessSettings.bOverride_AutoExposureMethod = true;
	State.CameraActor->GetCameraComponent()->PostProcessSettings.AutoExposureMethod = AEM_Manual;
	State.CameraActor->GetCameraComponent()->PostProcessSettings.bOverride_CameraISO = true;
	State.CameraActor->GetCameraComponent()->PostProcessSettings.CameraISO = 100.0f;

	// 2. AVBOIT Transparent Mesh
	State.TestActor = World->SpawnActor<AActor>(FVector(100, 0, 100), FRotator::ZeroRotator, SpawnParams);
	State.TestActor->Tags.Add(AVBOIT_TEST_TAG);
	
	USceneComponent* RootComp = NewObject<USceneComponent>(State.TestActor);
	State.TestActor->SetRootComponent(RootComp);
	RootComp->RegisterComponent();

	State.TestMesh = NewObject<UAVBOITTestMeshComponent>(State.TestActor);
	State.TestMesh->SetupAttachment(RootComp);
	State.TestMesh->MaterialParams.Color = FLinearColor::White;
	State.TestMesh->MaterialParams.Alpha = 0.5f;
	State.TestMesh->bIsTransparent = true;
	State.TestMesh->RegisterComponent();

	// 3. Occluder (Cube)
	State.OccluderActor = World->SpawnActor<AStaticMeshActor>(FVector(50, 0, 100), FRotator::ZeroRotator, SpawnParams);
	State.OccluderActor->Tags.Add(AVBOIT_TEST_TAG);
	if (UStaticMesh* CubeMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"))))
	{
		Cast<AStaticMeshActor>(State.OccluderActor)->GetStaticMeshComponent()->SetStaticMesh(CubeMesh);
		State.OccluderActor->SetActorScale3D(FVector(0.5f)); // Make it fit in front
	}
	State.OccluderActor->SetActorHiddenInGame(true);
#if WITH_EDITOR
	State.OccluderActor->SetIsTemporarilyHiddenInEditor(true);
#endif

	// 4. Background (Cube)
	State.BackgroundActor = World->SpawnActor<AStaticMeshActor>(FVector(200, 0, 100), FRotator::ZeroRotator, SpawnParams);
	State.BackgroundActor->Tags.Add(AVBOIT_TEST_TAG);
	if (UStaticMesh* CubeMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"))))
	{
		Cast<AStaticMeshActor>(State.BackgroundActor)->GetStaticMeshComponent()->SetStaticMesh(CubeMesh);
		State.BackgroundActor->SetActorScale3D(FVector(50.0f, 50.0f, 1.0f));
	}

#if WITH_EDITOR
	if (GCurrentLevelEditingViewportClient)
	{
		GCurrentLevelEditingViewportClient->SetViewLocation(State.CameraActor->GetActorLocation());
		GCurrentLevelEditingViewportClient->SetViewRotation(State.CameraActor->GetActorRotation());
	}
#endif

	return State;
}

void FAVBOITEditorSceneBuilder::SetTestMeshVisible(FAVBOITEditorSceneState& State, bool bVisible)
{
	if (State.TestMesh)
	{
		State.TestMesh->SetVisibility(bVisible);
		State.TestMesh->SetHiddenInGame(!bVisible);
	}
}

void FAVBOITEditorSceneBuilder::SetOccluderVisible(FAVBOITEditorSceneState& State, bool bVisible)
{
	if (State.OccluderActor)
	{
		State.OccluderActor->SetActorHiddenInGame(!bVisible);
#if WITH_EDITOR
		State.OccluderActor->SetIsTemporarilyHiddenInEditor(!bVisible);
#endif
	}
}

static int32 SafeGetCVar(const TCHAR* Name)
{
	if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(Name))
		return CVar->GetInt();
	return 0;
}

static void SafeSetCVar(const TCHAR* Name, int32 Value)
{
	if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(Name))
		CVar->Set(Value, ECVF_SetByConsole);
}

void FAVBOITEditorSceneBuilder::ApplyDeterministicSettings()
{
	OldDynamicRes = SafeGetCVar(TEXT("r.DynamicRes.OperationMode"));
	OldMotionBlur = SafeGetCVar(TEXT("r.MotionBlurQuality"));
	OldMotionBlurFeature = SafeGetCVar(TEXT("r.DefaultFeature.MotionBlur"));
	OldTAAUpsampling = SafeGetCVar(TEXT("r.TemporalAA.Upsampling"));
	OldEyeAdaptation = SafeGetCVar(TEXT("r.EyeAdaptationQuality"));
	OldBloom = SafeGetCVar(TEXT("r.BloomQuality"));
	OldSceneColorFringe = SafeGetCVar(TEXT("r.SceneColorFringeQuality"));
	OldTonemapperGrain = SafeGetCVar(TEXT("r.Tonemapper.GrainQuantization"));
	OldVSync = SafeGetCVar(TEXT("r.VSync"));

	SafeSetCVar(TEXT("r.DynamicRes.OperationMode"), 0);
	SafeSetCVar(TEXT("r.MotionBlurQuality"), 0);
	SafeSetCVar(TEXT("r.DefaultFeature.MotionBlur"), 0);
	SafeSetCVar(TEXT("r.TemporalAA.Upsampling"), 0);
	SafeSetCVar(TEXT("r.EyeAdaptationQuality"), 0);
	SafeSetCVar(TEXT("r.BloomQuality"), 0);
	SafeSetCVar(TEXT("r.SceneColorFringeQuality"), 0);
	SafeSetCVar(TEXT("r.Tonemapper.GrainQuantization"), 0);
	SafeSetCVar(TEXT("r.VSync"), 0);
}

void FAVBOITEditorSceneBuilder::RestoreSettings()
{
	SafeSetCVar(TEXT("r.DynamicRes.OperationMode"), OldDynamicRes);
	SafeSetCVar(TEXT("r.MotionBlurQuality"), OldMotionBlur);
	SafeSetCVar(TEXT("r.DefaultFeature.MotionBlur"), OldMotionBlurFeature);
	SafeSetCVar(TEXT("r.TemporalAA.Upsampling"), OldTAAUpsampling);
	SafeSetCVar(TEXT("r.EyeAdaptationQuality"), OldEyeAdaptation);
	SafeSetCVar(TEXT("r.BloomQuality"), OldBloom);
	SafeSetCVar(TEXT("r.SceneColorFringeQuality"), OldSceneColorFringe);
	SafeSetCVar(TEXT("r.Tonemapper.GrainQuantization"), OldTonemapperGrain);
	SafeSetCVar(TEXT("r.VSync"), OldVSync);
}

void FAVBOITEditorSceneBuilder::ForceResizeViewport(int32 Width, int32 Height)
{
	// PIE resize can be done via console
	FString Cmd = FString::Printf(TEXT("r.SetRes %dx%dw"), Width, Height);
	if (GEngine)
	{
		GEngine->Exec(nullptr, *Cmd);
	}
}
