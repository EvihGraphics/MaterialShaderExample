#include "AVBOIT/Testing/AVBOITRGBTestAutomation.h"
#include "AVBOIT/TestContent/AVBOITTestSceneSubsystem.h"
#include "AVBOIT/TestContent/AVBOITTestSceneActor.h"
#include "AVBOIT/Testing/AVBOITEvidencePath.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Materials/Material.h"
#include "UObject/Package.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonSerializer.h"
#include "HAL/IConsoleManager.h"
#include "Misc/App.h"
#include "Engine/StaticMesh.h"

#if WITH_EDITOR
#include "FileHelpers.h"
#include "ImageUtils.h"
#include "LevelEditor.h"
#include "IAssetViewport.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#endif

int32 FAVBOITRGBTestAutomation::CurrentSuiteStep = 0;
TArray<FString> FAVBOITRGBTestAutomation::SuiteSteps;
FString FAVBOITRGBTestAutomation::CurrentSuiteName;
FTSTicker::FDelegateHandle FAVBOITRGBTestAutomation::TickerHandle;

static TAutoConsoleVariable<int32> CVarAVBOITTestEnable(
    TEXT("r.AVBOIT.Test.Enable"), 0, TEXT("Enable AVBOIT RGB Test Content"), ECVF_RenderThreadSafe);

static TAutoConsoleVariable<int32> CVarAVBOITTestPreset(
    TEXT("r.AVBOIT.Test.Preset"), 0, TEXT("Preset (1=SingleWhite, 2=DoubleRedBlue, 3=TripleRGB)"), ECVF_RenderThreadSafe);

static TAutoConsoleVariable<int32> CVarAVBOITTestSubmissionOrder(
    TEXT("r.AVBOIT.Test.SubmissionOrder"), 0, TEXT("Submission Order (0=RGB, 1=RBG, 2=GRB, 3=GBR, 4=BRG, 5=BGR)"), ECVF_RenderThreadSafe);

static TAutoConsoleVariable<int32> CVarAVBOITTestReferenceMode(
    TEXT("r.AVBOIT.Test.ReferenceMode"), 0, TEXT("ReferenceMode (0=StandardAlpha)"), ECVF_RenderThreadSafe);

void FAVBOITRGBTestAutomation::Initialize()
{
}

void FAVBOITRGBTestAutomation::Shutdown()
{
}

UMaterial* FAVBOITRGBTestAutomation::CreateOrLoadTestMaterial()
{
    FString PackageName = TEXT("/Game/AVBOIT/TestContent/M_AVBOITTest_Unlit");
    UPackage* Package = CreatePackage(*PackageName);
    Package->FullyLoad();
    
    UMaterial* Material = FindObject<UMaterial>(Package, TEXT("M_AVBOITTest_Unlit"));
    if (Material)
    {
        return Material;
    }
    
    // We expect the Python script to have created this. If not, log error.
    UE_LOG(LogTemp, Error, TEXT("[AVBOITTest] Test Material not found! Please run CreateTestMaterial.py to generate it."));
    return nullptr;
}

void FAVBOITRGBTestAutomation::BuildSceneCmd(const TArray<FString>& Args)
{
    if (!GEngine) return;

    UWorld* World = nullptr;
    for (const FWorldContext& Context : GEngine->GetWorldContexts())
    {
        if (Context.WorldType == EWorldType::Game || Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::Editor)
        {
            World = Context.World();
            break;
        }
    }

    if (!World) return;

    UAVBOITTestSceneSubsystem* Subsystem = GEngine->GetEngineSubsystem<UAVBOITTestSceneSubsystem>();
    if (!Subsystem) return;

    UMaterial* TestMat = CreateOrLoadTestMaterial();
    Subsystem->SetBaseMaterial(TestMat);

    UStaticMesh* PlaneMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Plane.Plane"));
    Subsystem->SetPlaneMesh(PlaneMesh);

    EAVBOITTestPreset Preset = (EAVBOITTestPreset)CVarAVBOITTestPreset.GetValueOnGameThread();
    EAVBOITOrderPermutation Order = (EAVBOITOrderPermutation)CVarAVBOITTestSubmissionOrder.GetValueOnGameThread();
    EAVBOITTestReferenceMode RefMode = (EAVBOITTestReferenceMode)CVarAVBOITTestReferenceMode.GetValueOnGameThread();

    Subsystem->BuildScene(World, Preset, Order, RefMode);
    
    UE_LOG(LogTemp, Display, TEXT("[AVBOITTest] Scene Built for Preset %d, Order %d"), (int32)Preset, (int32)Order);
}

void FAVBOITRGBTestAutomation::DumpManifestCmd(const TArray<FString>& Args)
{
    UAVBOITTestSceneSubsystem* Subsystem = GEngine->GetEngineSubsystem<UAVBOITTestSceneSubsystem>();
    if (!Subsystem || !Subsystem->GetActiveSceneActor()) return;

    FVector3f ExpectedColor;
    float ExpectedTransmittance;
    Subsystem->GetActiveSceneActor()->GetExpectedAnalyticalResult(ExpectedColor, ExpectedTransmittance);

    TSharedPtr<FJsonObject> JsonObj = MakeShareable(new FJsonObject);
    JsonObj->SetStringField(TEXT("MachineID"), FAVBOITEvidencePath::ResolveAVBOITMachineId());
    JsonObj->SetStringField(TEXT("Preset"), FString::Printf(TEXT("%d"), (int32)Subsystem->GetActiveSceneActor()->CurrentPreset));
    JsonObj->SetStringField(TEXT("Order"), FString::Printf(TEXT("%d"), (int32)Subsystem->GetActiveSceneActor()->CurrentOrder));
    JsonObj->SetStringField(TEXT("ExpectedColor"), FString::Printf(TEXT("(%f, %f, %f)"), ExpectedColor.X, ExpectedColor.Y, ExpectedColor.Z));
    JsonObj->SetNumberField(TEXT("ExpectedTransmittance"), ExpectedTransmittance);

    FString JsonStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonStr);
    FJsonSerializer::Serialize(JsonObj.ToSharedRef(), Writer);

    FString EvidenceRoot = FAVBOITEvidencePath::ResolveAVBOITEvidenceRoot(TEXT("UE3-RGB-TestContent"), TEXT(""));
    FString SavePath = EvidenceRoot / TEXT("Manifests/CurrentManifest.json");
    if (Args.Num() > 0)
    {
        SavePath = EvidenceRoot / Args[0];
    }

    FFileHelper::SaveStringToFile(JsonStr, *SavePath);
    UE_LOG(LogTemp, Display, TEXT("[AVBOITTest] Manifest dumped to %s"), *SavePath);
}

void FAVBOITRGBTestAutomation::CaptureCurrentCmd(const TArray<FString>& Args)
{
    FString EvidenceRoot = FAVBOITEvidencePath::ResolveAVBOITEvidenceRoot(TEXT("UE3-RGB-TestContent"), TEXT(""));
    FString SavePath = EvidenceRoot / TEXT("Capture.png");
    if (Args.Num() > 0) SavePath = EvidenceRoot / Args[0];
    
    GEngine->Exec(nullptr, *FString::Printf(TEXT("HighResShot 1 filename=\"%s\""), *SavePath));
}

double FAVBOITRGBTestAutomation::NextTime = 0.0;

bool FAVBOITRGBTestAutomation::WriteViewportPng(const FString& Filename)
{
#if WITH_EDITOR
    FViewport* Viewport = nullptr;
    if (GEditor) Viewport = GEditor->GetActiveViewport();
    if (!Viewport && GEngine && GEngine->GameViewport) Viewport = GEngine->GameViewport->Viewport;
    
    if (!Viewport) return false;

    const FIntPoint Size = Viewport->GetSizeXY();
    if (Size.X <= 0 || Size.Y <= 0) return false;

    Viewport->Draw();
    TArray<FColor> Bitmap;
    if (!Viewport->ReadPixels(Bitmap) || Bitmap.Num() != Size.X * Size.Y) return false;

    for (FColor& Color : Bitmap) Color.A = 255;
    const FImageView Image(Bitmap.GetData(), Size.X, Size.Y);
    return FImageUtils::SaveImageByExtension(*Filename, Image);
#else
    return false;
#endif
}

void FAVBOITRGBTestAutomation::CaptureRGBSuiteCmd(const TArray<FString>& Args)
{
    if (TickerHandle.IsValid()) return;
    
    CurrentSuiteStep = 0;
    NextTime = FPlatformTime::Seconds() + 1.0;
    TickerHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateStatic(&FAVBOITRGBTestAutomation::Tick), 0.1f);
}

bool FAVBOITRGBTestAutomation::Tick(float DeltaTime)
{
    if (FPlatformTime::Seconds() < NextTime) return true;

    // Suite steps:
    // 0: SingleWhite
    // 1: Wait & Capture SingleWhite
    // 2: DoubleRedBlue
    // 3: Wait & Capture DoubleRedBlue
    // 4: TripleRGB (RGB)
    // 5: Wait & Capture TripleRGB
    // 6-17: 6 permutations
    
    int32 ConfigIndex = CurrentSuiteStep / 2;
    bool bIsCaptureStep = (CurrentSuiteStep % 2) != 0;

    struct FTestConfig { int32 Preset; int32 Order; FString Folder; FString File; };
    TArray<FTestConfig> Configs = {
        {1, 0, TEXT("SingleWhite"), TEXT("StandardAlpha.png")},
        {2, 0, TEXT("DoubleRedBlue"), TEXT("StandardAlpha_CorrectSort.png")},
        {3, 0, TEXT("TripleRGB"), TEXT("CorrectSort.png")},
        {3, 0, TEXT("TripleRGB/SubmissionOrders"), TEXT("00_RGB.png")},
        {3, 1, TEXT("TripleRGB/SubmissionOrders"), TEXT("01_RBG.png")},
        {3, 2, TEXT("TripleRGB/SubmissionOrders"), TEXT("02_GRB.png")},
        {3, 3, TEXT("TripleRGB/SubmissionOrders"), TEXT("03_GBR.png")},
        {3, 4, TEXT("TripleRGB/SubmissionOrders"), TEXT("04_BRG.png")},
        {3, 5, TEXT("TripleRGB/SubmissionOrders"), TEXT("05_BGR.png")}
    };

    if (ConfigIndex >= Configs.Num())
    {
        FTSTicker::GetCoreTicker().RemoveTicker(TickerHandle);
        TickerHandle.Reset();
        UE_LOG(LogTemp, Display, TEXT("[AVBOITTest] Suite Capture Completed!"));
        FPlatformMisc::RequestExit(false);
        return false;
    }

    const FTestConfig& Cfg = Configs[ConfigIndex];

    if (!bIsCaptureStep)
    {
        CVarAVBOITTestPreset.AsVariable()->Set(Cfg.Preset, ECVF_SetByConsole);
        CVarAVBOITTestSubmissionOrder.AsVariable()->Set(Cfg.Order, ECVF_SetByConsole);
        BuildSceneCmd({});
        NextTime = FPlatformTime::Seconds() + 1.0;
    }
    else
    {
        FString EvidenceRoot = FAVBOITEvidencePath::ResolveAVBOITEvidenceRoot(TEXT("UE3-RGB-TestContent"), TEXT(""));
        FString ManifestRelative = Cfg.Folder / TEXT("Manifest.json");
        DumpManifestCmd({ManifestRelative});
        
        FString CapturePath = EvidenceRoot / Cfg.Folder / Cfg.File;
        WriteViewportPng(CapturePath);
        
        FString ExpectedPath = EvidenceRoot / Cfg.Folder / TEXT("Expected.txt");
        FFileHelper::SaveStringToFile(TEXT("Please refer to Manifest.json for exact linear ExpectedColor and ExpectedTransmittance.\n"), *ExpectedPath);

        NextTime = FPlatformTime::Seconds() + 0.5;
    }

    CurrentSuiteStep++;
    return true;
}
