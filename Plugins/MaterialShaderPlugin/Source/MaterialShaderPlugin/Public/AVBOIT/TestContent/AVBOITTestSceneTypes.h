#pragma once

#include "CoreMinimal.h"
#include "AVBOITTestSceneTypes.generated.h"

UENUM(BlueprintType)
enum class EAVBOITTestPreset : uint8
{
    Disabled = 0,
    SingleWhite = 1,
    DoubleRedBlue = 2,
    TripleRGB = 3,
    ForgeRGBReference = 4
};

UENUM(BlueprintType)
enum class EAVBOITOrderPermutation : uint8
{
    RGB = 0,
    RBG = 1,
    GRB = 2,
    GBR = 3,
    BRG = 4,
    BGR = 5
};

UENUM(BlueprintType)
enum class EAVBOITTestReferenceMode : uint8
{
    StandardAlpha = 0,
    FutureAVBOIT = 1
};

USTRUCT(BlueprintType)
struct FAVBOITTestPrimitiveDesc
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AVBOIT Test Content")
    FName StableId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AVBOIT Test Content")
    FTransform Transform;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AVBOIT Test Content")
    FVector3f LinearColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AVBOIT Test Content")
    float Alpha;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AVBOIT Test Content")
    int32 PhysicalDepthIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AVBOIT Test Content")
    int32 SubmissionOrderIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AVBOIT Test Content")
    int32 TranslucencySortPriority;

    FAVBOITTestPrimitiveDesc()
        : StableId(NAME_None)
        , Transform(FTransform::Identity)
        , LinearColor(FVector3f::One())
        , Alpha(1.0f)
        , PhysicalDepthIndex(0)
        , SubmissionOrderIndex(0)
        , TranslucencySortPriority(0)
    {}
};
