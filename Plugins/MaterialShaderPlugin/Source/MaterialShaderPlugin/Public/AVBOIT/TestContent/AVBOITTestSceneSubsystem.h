#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "AVBOIT/TestContent/AVBOITTestSceneTypes.h"
#include "AVBOITTestSceneSubsystem.generated.h"

class AAVBOITTestSceneActor;

UCLASS()
class UAVBOITTestSceneSubsystem : public UEngineSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    void BuildScene(UWorld* World, EAVBOITTestPreset Preset, EAVBOITOrderPermutation Order, EAVBOITTestReferenceMode RefMode);
    void ClearScene(UWorld* World);

    void SetBaseMaterial(class UMaterialInterface* Material);
    void SetPlaneMesh(class UStaticMesh* Mesh);

    AAVBOITTestSceneActor* GetActiveSceneActor() const { return ActiveActor; }

private:
    UPROPERTY()
    AAVBOITTestSceneActor* ActiveActor;

    UPROPERTY()
    class UMaterialInterface* BaseTestMaterial;

    UPROPERTY()
    class UStaticMesh* PlaneMesh;
};
