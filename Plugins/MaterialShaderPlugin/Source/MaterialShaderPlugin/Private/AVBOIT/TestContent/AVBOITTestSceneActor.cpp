#include "AVBOIT/TestContent/AVBOITTestSceneActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

AAVBOITTestSceneActor::AAVBOITTestSceneActor()
{
    PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;

    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
    SceneCapture->SetupAttachment(Root);
    SceneCapture->ProjectionType = ECameraProjectionMode::Orthographic;
    SceneCapture->OrthoWidth = 512.0f;
    SceneCapture->bCaptureEveryFrame = false;
    SceneCapture->bCaptureOnMovement = false;
    // Capture standard scene color (HDR linear) to get accurate pure linear composites without tonemapping
    SceneCapture->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR;
    SceneCapture->ShowFlags.SetLighting(false);
    SceneCapture->ShowFlags.SetPostProcessing(false);
    SceneCapture->ShowFlags.SetFog(false);
    SceneCapture->ShowFlags.SetAntiAliasing(false);
    // Transform: Looking down +X
    SceneCapture->SetRelativeTransform(FTransform(FRotator(0.f, 0.f, 0.f), FVector::ZeroVector));

    RenderTarget = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("RenderTarget"));
    if (RenderTarget)
    {
        RenderTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA16f;
        RenderTarget->InitAutoFormat(512, 512);
        RenderTarget->SRGB = false;
        RenderTarget->ClearColor = FLinearColor::Black;
        SceneCapture->TextureTarget = RenderTarget;
    }

    static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMeshFinder(TEXT("/Engine/BasicShapes/Plane.Plane"));
    if (PlaneMeshFinder.Succeeded())
    {
        PlaneMesh = PlaneMeshFinder.Object;
    }
}

void AAVBOITTestSceneActor::BeginPlay()
{
    Super::BeginPlay();
    RebuildScene();
}

void AAVBOITTestSceneActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    RebuildScene();
}

void AAVBOITTestSceneActor::SetupScene(EAVBOITTestPreset Preset, EAVBOITOrderPermutation Order, EAVBOITTestReferenceMode ReferenceMode)
{
    CurrentPreset = Preset;
    CurrentOrder = Order;
    CurrentReferenceMode = ReferenceMode;
    RebuildScene();
}

void AAVBOITTestSceneActor::RebuildScene()
{
    ClearScene();
    if (CurrentPreset == EAVBOITTestPreset::Disabled)
    {
        return;
    }

    BuildPrimitives();
    SpawnComponents();
}

void AAVBOITTestSceneActor::ClearScene()
{
    for (UStaticMeshComponent* Comp : SpawnedComponents)
    {
        if (Comp)
        {
            Comp->DestroyComponent();
        }
    }
    SpawnedComponents.Empty();
    SpawnedMIDs.Empty();
    CurrentPrimitives.Empty();

    if (BackgroundComponent)
    {
        BackgroundComponent->DestroyComponent();
        BackgroundComponent = nullptr;
    }
}

void AAVBOITTestSceneActor::BuildPrimitives()
{
    CurrentPrimitives.Empty();

    // Define physical positions
    // Plane is XY plane facing +Z, we need to rotate it to face -X camera?
    // The instructions say "Camera Forward: +X", so the camera is looking down +X.
    // So the planes should be at X=300, X=400, X=500, facing the camera (-X).
    // A standard UE Plane is Z-up. To face -X, we rotate Pitch by -90 or +90.
    FQuat FaceCameraRot = FRotator(90.f, 0.f, 0.f).Quaternion(); 
    // Wait, Pitch 90 makes Z point +X. Pitch -90 makes Z point -X.
    FaceCameraRot = FRotator(90.f, 0.f, 0.f).Quaternion(); // Let's assume +90 for +X normal, but we want it facing the camera so -X normal.
    FaceCameraRot = FRotator(270.f, 0.f, 0.f).Quaternion();

    FTransform FrontT(FaceCameraRot, FVector(300.f, 0.f, 0.f), FVector(5.f));
    FTransform MidT(FaceCameraRot, FVector(400.f, 0.f, 0.f), FVector(5.f));
    FTransform BackT(FaceCameraRot, FVector(500.f, 0.f, 0.f), FVector(5.f));

    FAVBOITTestPrimitiveDesc Front, Mid, Back;

    if (CurrentPreset == EAVBOITTestPreset::SingleWhite)
    {
        Front.StableId = TEXT("White_Front");
        Front.Transform = FrontT;
        Front.LinearColor = FVector3f(1.f, 1.f, 1.f);
        Front.Alpha = 0.5f;
        Front.PhysicalDepthIndex = 0;
        CurrentPrimitives.Add(Front);
    }
    else if (CurrentPreset == EAVBOITTestPreset::DoubleRedBlue)
    {
        Front.StableId = TEXT("Red_Front");
        Front.Transform = FrontT;
        Front.LinearColor = FVector3f(1.f, 0.f, 0.f);
        Front.Alpha = 0.5f;
        Front.PhysicalDepthIndex = 0;

        Back.StableId = TEXT("Blue_Back");
        Back.Transform = MidT; // Mid is 400
        Back.LinearColor = FVector3f(0.f, 0.f, 1.f);
        Back.Alpha = 0.5f;
        Back.PhysicalDepthIndex = 1;

        CurrentPrimitives.Add(Front);
        CurrentPrimitives.Add(Back);
    }
    else if (CurrentPreset == EAVBOITTestPreset::TripleRGB)
    {
        Front.StableId = TEXT("Red_Front");
        Front.Transform = FrontT;
        Front.LinearColor = FVector3f(1.f, 0.f, 0.f);
        Front.Alpha = 0.5f;
        Front.PhysicalDepthIndex = 0;

        Mid.StableId = TEXT("Green_Mid");
        Mid.Transform = MidT;
        Mid.LinearColor = FVector3f(0.f, 1.f, 0.f);
        Mid.Alpha = 0.5f;
        Mid.PhysicalDepthIndex = 1;

        Back.StableId = TEXT("Blue_Back");
        Back.Transform = BackT;
        Back.LinearColor = FVector3f(0.f, 0.f, 1.f);
        Back.Alpha = 0.5f;
        Back.PhysicalDepthIndex = 2;

        CurrentPrimitives.Add(Front);
        CurrentPrimitives.Add(Mid);
        CurrentPrimitives.Add(Back);
    }
    else if (CurrentPreset == EAVBOITTestPreset::ForgeRGBReference)
    {
        // ... (Optional, skipping for now as per instructions to only do it later)
    }

    // Apply Submission Orders
    // Priority: Smaller is drawn first. (Assuming standard UE sorting)
    // We want to control Submission Order exactly.
    // Let's assign priority based on permutation.
    // If we have 3 primitives, indices are 0, 1, 2.
    // Order Permutation: RGB, RBG, GRB, GBR, BRG, BGR.
    // E.g. RGB -> R draws first (0), G second (1), B third (2).
    // TranslucencySortPriority: larger is drawn later in UE? 
    // Wait, UE documentation says: Objects with a lower priority sort farther away, higher priority sorts closer.
    // Actually, UE draws lower priority first, higher priority later.
    // Let's use Priority values 0, 1, 2. Drawn order: 0, then 1, then 2.
    
    TArray<int32> DrawOrders;
    if (CurrentPreset == EAVBOITTestPreset::SingleWhite)
    {
        DrawOrders = {0};
    }
    else if (CurrentPreset == EAVBOITTestPreset::DoubleRedBlue)
    {
        // We can just use the natural order
        DrawOrders = {0, 1}; 
    }
    else if (CurrentPreset == EAVBOITTestPreset::TripleRGB)
    {
        // 0=R, 1=G, 2=B physical
        switch (CurrentOrder)
        {
        case EAVBOITOrderPermutation::RGB: DrawOrders = {0, 1, 2}; break;
        case EAVBOITOrderPermutation::RBG: DrawOrders = {0, 2, 1}; break;
        case EAVBOITOrderPermutation::GRB: DrawOrders = {1, 0, 2}; break;
        case EAVBOITOrderPermutation::GBR: DrawOrders = {2, 0, 1}; break;
        case EAVBOITOrderPermutation::BRG: DrawOrders = {1, 2, 0}; break;
        case EAVBOITOrderPermutation::BGR: DrawOrders = {2, 1, 0}; break;
        }
    }

    for (int32 i = 0; i < CurrentPrimitives.Num() && i < DrawOrders.Num(); ++i)
    {
        CurrentPrimitives[i].TranslucencySortPriority = DrawOrders[i];
        CurrentPrimitives[i].SubmissionOrderIndex = DrawOrders[i];
    }
}

void AAVBOITTestSceneActor::SpawnComponents()
{
    if (!PlaneMesh) return;

    for (const FAVBOITTestPrimitiveDesc& Desc : CurrentPrimitives)
    {
        UStaticMeshComponent* Comp = NewObject<UStaticMeshComponent>(this, Desc.StableId);
        Comp->SetStaticMesh(PlaneMesh);
        Comp->SetWorldTransform(Desc.Transform);
        Comp->SetTranslucentSortPriority(Desc.TranslucencySortPriority);
        Comp->SetCastShadow(false);
        Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        Comp->SetupAttachment(GetRootComponent());
        Comp->RegisterComponent();

        if (BaseTestMaterial)
        {
            UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(BaseTestMaterial, this);
            MID->SetVectorParameterValue(TEXT("TestColor"), FLinearColor(Desc.LinearColor.X, Desc.LinearColor.Y, Desc.LinearColor.Z, 1.f));
            MID->SetScalarParameterValue(TEXT("TestAlpha"), Desc.Alpha);
            Comp->SetMaterial(0, MID);
            SpawnedMIDs.Add(MID);
        }

        SpawnedComponents.Add(Comp);
    }

    // Add Black Background to block any SkySphere in the map
    BackgroundComponent = NewObject<UStaticMeshComponent>(this, TEXT("BlackBackground"));
    BackgroundComponent->SetStaticMesh(PlaneMesh);
    FQuat FaceCameraRot = FRotator(270.f, 0.f, 0.f).Quaternion();
    BackgroundComponent->SetWorldTransform(FTransform(FaceCameraRot, FVector(700.f, 0.f, 0.f), FVector(10.f)));
    BackgroundComponent->SetCastShadow(false);
    BackgroundComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    BackgroundComponent->SetupAttachment(GetRootComponent());
    BackgroundComponent->RegisterComponent();

    if (BaseTestMaterial)
    {
        UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(BaseTestMaterial, this);
        MID->SetVectorParameterValue(TEXT("TestColor"), FLinearColor::Black);
        MID->SetScalarParameterValue(TEXT("TestAlpha"), 1.0f); // Opaque black
        BackgroundComponent->SetMaterial(0, MID);
    }
}

FVector4f AAVBOITTestSceneActor::ComputeAnalyticalFrontToBack(TConstArrayView<FAVBOITTestPrimitiveDesc> SortedFrontToBack) const
{
    FVector3f C_out = FVector3f::Zero();
    float T_out = 1.0f;

    for (const FAVBOITTestPrimitiveDesc& Desc : SortedFrontToBack)
    {
        C_out += Desc.LinearColor * Desc.Alpha * T_out;
        T_out *= (1.0f - Desc.Alpha);
    }

    return FVector4f(C_out.X, C_out.Y, C_out.Z, T_out);
}

void AAVBOITTestSceneActor::GetExpectedAnalyticalResult(FVector3f& OutColor, float& OutTransmittance) const
{
    // Standard Alpha draws primitives in order of their TranslucencySortPriority (lower priority drawn first, higher priority drawn later).
    // To match what UE standard alpha rendering produces, we sort by SortPriority and apply standard Source-Over blending (Back-to-Front mathematically).
    // Our analytical ComputeAnalyticalFrontToBack assumes the array is ordered Front-to-Back. 
    // Wait, Standard Alpha is drawn Back-to-Front. So the primitive drawn *first* is the "Back" most in the blend equation.
    // So if we draw A, then B, then C, the composite is:
    // C_over( B_over( A_over( Black ) ) ).
    // Let's just implement a Back-to-Front standard blend here to be extremely precise!

    TArray<FAVBOITTestPrimitiveDesc> SortedByDraw = CurrentPrimitives;
    SortedByDraw.Sort([](const FAVBOITTestPrimitiveDesc& A, const FAVBOITTestPrimitiveDesc& B) {
        return A.TranslucencySortPriority < B.TranslucencySortPriority;
    });

    FVector3f C_out = FVector3f::Zero();
    
    // Draw order: index 0 is drawn first (placed on black background).
    for (const FAVBOITTestPrimitiveDesc& Desc : SortedByDraw)
    {
        // Standard Alpha: C_dst = C_src * A_src + C_dst * (1 - A_src)
        C_out = Desc.LinearColor * Desc.Alpha + C_out * (1.0f - Desc.Alpha);
    }
    
    // Transmittance is multiplicative and order-independent.
    float T_out = 1.0f;
    for (const FAVBOITTestPrimitiveDesc& Desc : SortedByDraw)
    {
        T_out *= (1.0f - Desc.Alpha);
    }

    OutColor = C_out;
    OutTransmittance = T_out;
}

void AAVBOITTestSceneActor::CaptureScene()
{
    if (SceneCapture && RenderTarget)
    {
        SceneCapture->CaptureScene();
    }
}

bool AAVBOITTestSceneActor::ReadbackLinear(TArray<FFloat16Color>& OutLinearPixels, FIntPoint& OutSize) const
{
    if (!RenderTarget || !RenderTarget->GameThread_GetRenderTargetResource())
    {
        return false;
    }
    
    FTextureRenderTargetResource* RTResource = RenderTarget->GameThread_GetRenderTargetResource();
    OutSize = RTResource->GetSizeXY();
    return RTResource->ReadFloat16Pixels(OutLinearPixels);
}
