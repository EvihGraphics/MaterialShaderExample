#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AVBOIT/TestContent/AVBOITTestSceneTypes.h"
#include "AVBOITTestSceneActor.generated.h"

class UStaticMeshComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;

UCLASS()
class AAVBOITTestSceneActor : public AActor
{
    GENERATED_BODY()

public:
    AAVBOITTestSceneActor();

    virtual bool ShouldTickIfViewportsOnly() const override { return false; }

    void SetupScene(EAVBOITTestPreset Preset, EAVBOITOrderPermutation Order, EAVBOITTestReferenceMode ReferenceMode);

    UFUNCTION(BlueprintCallable, Category = "AVBOIT Test Content")
    void RebuildScene();

    void CaptureScene();
    bool ReadbackLinear(TArray<FFloat16Color>& OutLinearPixels, FIntPoint& OutSize) const;

    void GetExpectedAnalyticalResult(FVector3f& OutColor, float& OutTransmittance) const;
    const TArray<FAVBOITTestPrimitiveDesc>& GetCurrentPrimitives() const { return CurrentPrimitives; }

protected:
    virtual void BeginPlay() override;
    virtual void OnConstruction(const FTransform& Transform) override;

private:
    void ClearScene();
    void BuildPrimitives();
    void SpawnComponents();
    
    // CPU Analytical Reference
    FVector4f ComputeAnalyticalFrontToBack(TConstArrayView<FAVBOITTestPrimitiveDesc> SortedFrontToBack) const;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AVBOIT Test Config")
    EAVBOITTestPreset CurrentPreset = EAVBOITTestPreset::Disabled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AVBOIT Test Config")
    EAVBOITOrderPermutation CurrentOrder = EAVBOITOrderPermutation::RGB;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AVBOIT Test Config")
    EAVBOITTestReferenceMode CurrentReferenceMode = EAVBOITTestReferenceMode::StandardAlpha;

    UPROPERTY(EditAnywhere, Category = "AVBOIT Test Config")
    UMaterialInterface* BaseTestMaterial;

    UPROPERTY(EditAnywhere, Category = "AVBOIT Test Config")
    class UStaticMesh* PlaneMesh;

private:
    UPROPERTY(Transient)
    TArray<FAVBOITTestPrimitiveDesc> CurrentPrimitives;

    UPROPERTY(Transient)
    TArray<UStaticMeshComponent*> SpawnedComponents;

    UPROPERTY(Transient)
    TArray<UMaterialInstanceDynamic*> SpawnedMIDs;

    // BackgroundComponent was removed in favor of PRM_UseShowOnlyList

    UPROPERTY(Transient)
    class USceneCaptureComponent2D* SceneCapture;

    UPROPERTY(Transient)
    class UTextureRenderTarget2D* RenderTarget;
};
