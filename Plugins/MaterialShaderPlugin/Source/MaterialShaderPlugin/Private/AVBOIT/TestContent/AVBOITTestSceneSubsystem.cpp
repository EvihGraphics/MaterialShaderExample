#include "AVBOIT/TestContent/AVBOITTestSceneSubsystem.h"
#include "AVBOIT/TestContent/AVBOITTestSceneActor.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "UObject/ConstructorHelpers.h"

void UAVBOITTestSceneSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UAVBOITTestSceneSubsystem::Deinitialize()
{
    Super::Deinitialize();
}

void UAVBOITTestSceneSubsystem::SetBaseMaterial(UMaterialInterface* Material)
{
    BaseTestMaterial = Material;
}

void UAVBOITTestSceneSubsystem::SetPlaneMesh(UStaticMesh* Mesh)
{
    PlaneMesh = Mesh;
}

void UAVBOITTestSceneSubsystem::ClearScene(UWorld* World)
{
    if (!World) return;

    for (TActorIterator<AAVBOITTestSceneActor> It(World); It; ++It)
    {
        It->Destroy();
    }
    ActiveActor = nullptr;
}

void UAVBOITTestSceneSubsystem::BuildScene(UWorld* World, EAVBOITTestPreset Preset, EAVBOITOrderPermutation Order, EAVBOITTestReferenceMode RefMode)
{
    if (!World) return;

    ClearScene(World);

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
    ActiveActor = World->SpawnActor<AAVBOITTestSceneActor>(FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
    if (ActiveActor)
    {
        if (BaseTestMaterial)
        {
            ActiveActor->BaseTestMaterial = BaseTestMaterial;
        }
        if (PlaneMesh)
        {
            ActiveActor->PlaneMesh = PlaneMesh;
        }
        ActiveActor->SetupScene(Preset, Order, RefMode);
    }
}
