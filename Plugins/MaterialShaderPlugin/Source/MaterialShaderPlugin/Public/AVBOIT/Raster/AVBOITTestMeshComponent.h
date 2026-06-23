// Copyright

#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h"
#include "AVBOITTestMeshComponent.generated.h"

USTRUCT(BlueprintType)
struct FAVBOITRasterMaterialParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "AVBOIT")
	FLinearColor Color = FLinearColor::White;

	UPROPERTY(EditAnywhere, Category = "AVBOIT", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float Alpha = 0.5f;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MATERIALSHADERPLUGIN_API UAVBOITTestMeshComponent : public UPrimitiveComponent
{
	GENERATED_BODY()

public:
	UAVBOITTestMeshComponent();

	UPROPERTY(EditAnywhere, Category = "AVBOIT")
	FAVBOITRasterMaterialParameters MaterialParams;

	UPROPERTY(EditAnywhere, Category = "AVBOIT")
	bool bIsTransparent = true;

	UPROPERTY(EditAnywhere, Category = "AVBOIT")
	int32 SubmissionOrder = 0; // For testing explicit sorting

	//~ Begin UPrimitiveComponent Interface
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
	//~ End UPrimitiveComponent Interface
};
