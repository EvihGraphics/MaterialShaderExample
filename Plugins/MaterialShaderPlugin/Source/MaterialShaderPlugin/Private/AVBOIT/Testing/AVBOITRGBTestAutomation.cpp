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
#endif

double FAVBOITRGBTestAutomation::NextTime = 0.0;
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

    // Update to match UE3-1 validation path
    FString EvidenceRoot = FAVBOITEvidencePath::ResolveAVBOITEvidenceRoot(TEXT("UE3-1-RGB-Validation"), TEXT(""));
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
    // Obsolete for UE-3.1 Validation, we rely on the Suite now
}

bool FAVBOITRGBTestAutomation::WriteLinearPng(const TArray<FFloat16Color>& FloatPixels, FIntPoint Size, const FString& Filename)
{
    if (FloatPixels.Num() != Size.X * Size.Y) return false;

    TArray<FColor> Bitmap;
    Bitmap.SetNumUninitialized(FloatPixels.Num());
    for (int32 i = 0; i < FloatPixels.Num(); ++i)
    {
        // Simple linear to sRGB/8-bit for preview
        Bitmap[i] = FLinearColor(FloatPixels[i]).ToFColor(true);
        Bitmap[i].A = 255;
    }
    const FImageView Image(Bitmap.GetData(), Size.X, Size.Y);
    return FImageUtils::SaveImageByExtension(*Filename, Image);
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

    struct FTestConfig { int32 Preset; int32 Order; FString Folder; FString File; FString ManifestName; };
    TArray<FTestConfig> Configs = {
        {1, 0, TEXT("SingleWhite"), TEXT("StandardAlpha.png"), TEXT("SingleWhite.Manifest.json")},
        {2, 0, TEXT("DoubleRedBlue"), TEXT("StandardAlpha_CorrectSort.png"), TEXT("DoubleRedBlue.Manifest.json")},
        {3, 0, TEXT("TripleRGB"), TEXT("CorrectSort.png"), TEXT("TripleRGB.Manifest.json")},
        {3, 0, TEXT("TripleRGB/SubmissionOrders"), TEXT("00_RGB.png"), TEXT("00_RGB.Manifest.json")},
        {3, 1, TEXT("TripleRGB/SubmissionOrders"), TEXT("01_RBG.png"), TEXT("01_RBG.Manifest.json")},
        {3, 2, TEXT("TripleRGB/SubmissionOrders"), TEXT("02_GRB.png"), TEXT("02_GRB.Manifest.json")},
        {3, 3, TEXT("TripleRGB/SubmissionOrders"), TEXT("03_GBR.png"), TEXT("03_GBR.Manifest.json")},
        {3, 4, TEXT("TripleRGB/SubmissionOrders"), TEXT("04_BRG.png"), TEXT("04_BRG.Manifest.json")},
        {3, 5, TEXT("TripleRGB/SubmissionOrders"), TEXT("05_BGR.png"), TEXT("05_BGR.Manifest.json")}
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
        GEngine->Exec(nullptr, TEXT("r.UsePreExposure 0"));
        GEngine->Exec(nullptr, TEXT("r.EyeAdaptationQuality 0"));
        GEngine->Exec(nullptr, TEXT("r.AutoExposure.Method 0"));
        GEngine->Exec(nullptr, TEXT("r.AutoExposure.Bias 0"));
        GEngine->Exec(nullptr, TEXT("r.Tonemapper.Quality 0"));
        
        CVarAVBOITTestPreset.AsVariable()->Set(Cfg.Preset, ECVF_SetByConsole);
        CVarAVBOITTestSubmissionOrder.AsVariable()->Set(Cfg.Order, ECVF_SetByConsole);
        BuildSceneCmd({});
        
        // Ensure scene executes a frame before capture
        NextTime = FPlatformTime::Seconds() + 0.5;
    }
    else
    {
        UAVBOITTestSceneSubsystem* Subsystem = GEngine->GetEngineSubsystem<UAVBOITTestSceneSubsystem>();
        AAVBOITTestSceneActor* Actor = Subsystem ? Subsystem->GetActiveSceneActor() : nullptr;
        
        if (Actor)
        {
            Actor->CaptureScene();
            
            TArray<FFloat16Color> LinearPixels;
            FIntPoint Size;
            if (Actor->ReadbackLinear(LinearPixels, Size) && Size.X > 0 && Size.Y > 0)
            {
                FString EvidenceRoot = FAVBOITEvidencePath::ResolveAVBOITEvidenceRoot(TEXT("UE3-1-RGB-Validation"), TEXT(""));
                
                FString CapturePath = EvidenceRoot / Cfg.Folder / Cfg.File;
                WriteLinearPng(LinearPixels, Size, CapturePath);
                
                // Extract 64x64 ROI center
                int32 StartX = FMath::Max(0, (Size.X / 2) - 32);
                int32 StartY = FMath::Max(0, (Size.Y / 2) - 32);
                
                FVector3f SumColor = FVector3f::Zero();
                int32 Count = 0;
                for (int32 Y = StartY; Y < StartY + 64 && Y < Size.Y; ++Y)
                {
                    for (int32 X = StartX; X < StartX + 64 && X < Size.X; ++X)
                    {
                        FLinearColor LC(LinearPixels[Y * Size.X + X]);
                        SumColor += FVector3f(LC.R, LC.G, LC.B);
                        Count++;
                    }
                }
                
                FVector3f MeanColor = Count > 0 ? SumColor / Count : FVector3f::Zero();
                
                FVector3f ExpectedColor;
                float ExpectedTransmittance;
                Actor->GetExpectedAnalyticalResult(ExpectedColor, ExpectedTransmittance);
                
                FVector3f AbsErr = FVector3f(FMath::Abs(MeanColor.X - ExpectedColor.X), FMath::Abs(MeanColor.Y - ExpectedColor.Y), FMath::Abs(MeanColor.Z - ExpectedColor.Z));
                float MaxAbs = FMath::Max3(AbsErr.X, AbsErr.Y, AbsErr.Z);
                float MAE = (AbsErr.X + AbsErr.Y + AbsErr.Z) / 3.0f;
                
                FString ExpectedPath = EvidenceRoot / Cfg.Folder / FString::Printf(TEXT("%s_Expected.txt"), *FPaths::GetBaseFilename(Cfg.ManifestName));
                FString Report = FString::Printf(TEXT("Measured Mean: (%f, %f, %f)\nExpected: (%f, %f, %f)\nMaxAbs: %f\nMAE: %f\n"), 
                    MeanColor.X, MeanColor.Y, MeanColor.Z,
                    ExpectedColor.X, ExpectedColor.Y, ExpectedColor.Z,
                    MaxAbs, MAE);
                
                FFileHelper::SaveStringToFile(Report, *ExpectedPath);

                // Dump manifest
                FString ManifestRelative = Cfg.Folder / Cfg.ManifestName;
                DumpManifestCmd({ManifestRelative});
            }
        }

        NextTime = FPlatformTime::Seconds() + 0.1;
    }

    CurrentSuiteStep++;
    return true;
}
