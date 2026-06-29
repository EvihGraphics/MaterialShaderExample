#include "AVBOITEditorPIEAutomation.h"

#if WITH_EDITOR
#include "HAL/IConsoleManager.h"
#include "HAL/FileManager.h"
#include "Misc/Paths.h"
#include "ImageUtils.h"
#include "Editor.h"
#include "Editor/UnrealEdEngine.h"
#include "IAssetViewport.h"
#include "LevelEditor.h"
#include "PlayInEditorDataTypes.h"
#include "UnrealEdGlobals.h"
#include "AVBOIT/Raster/AVBOITRasterRenderer.h"
#include "Serialization/JsonSerializer.h"
#include "AVBOITEditorSceneBuilder.h"
#include "AVBOIT/Testing/AVBOITBackendTestAutomation.h"
#include "Engine/StaticMeshActor.h"
#include "Camera/CameraActor.h"
#include "AVBOIT/Raster/AVBOITTestMeshComponent.h"
#include "EngineUtils.h"
#include "AVBOIT/Raster/AVBOITRasterShaders.h"

FTSTicker::FDelegateHandle FAVBOITEditorPIEAutomation::EditorPIECloseoutTickerHandle;
FDelegateHandle FAVBOITEditorPIEAutomation::EditorPIECloseoutBeginPIEHandle;
FDelegateHandle FAVBOITEditorPIEAutomation::EditorPIECloseoutEndPIEHandle;
FString FAVBOITEditorPIEAutomation::EditorPIECloseoutRoot;
double FAVBOITEditorPIEAutomation::EditorPIECloseoutNextTime = 0.0;
int32 FAVBOITEditorPIEAutomation::EditorPIECloseoutStep = 0;
bool FAVBOITEditorPIEAutomation::bEditorPIECloseoutRunning = false;
bool FAVBOITEditorPIEAutomation::bEditorPIECloseoutCaptureScreenshots = true;

static const FName AVBOIT_TEST_TAG = TEXT("AVBOIT_AutomatedSceneIntegration");
static FAVBOITEditorSceneState GSceneState;
static FAVBOITRasterExecutionProbe GProbeInstance;

static void WriteProbeJson(const FString& Filename, const FAVBOITRasterExecutionProbe& Probe)
{
	TSharedPtr<FJsonObject> JsonObj = MakeShareable(new FJsonObject);
	JsonObj->SetNumberField(TEXT("FrameNumber"), Probe.FrameNumber);
	JsonObj->SetStringField(TEXT("WorldName"), Probe.WorldName);
	JsonObj->SetStringField(TEXT("Context"), Probe.Context);
	JsonObj->SetBoolField(TEXT("RasterEnabled"), Probe.bRasterEnabled);
	JsonObj->SetNumberField(TEXT("RegistryProxyCount"), Probe.RegistryProxyCount);
	JsonObj->SetNumberField(TEXT("AcceptedProxyCount"), Probe.AcceptedProxyCount);
	JsonObj->SetBoolField(TEXT("ClearPassScheduled"), Probe.bClearPassScheduled);
	JsonObj->SetBoolField(TEXT("SplatPassScheduled"), Probe.bSplatPassScheduled);
	JsonObj->SetBoolField(TEXT("IntegratePassScheduled"), Probe.bIntegratePassScheduled);
	JsonObj->SetBoolField(TEXT("ForwardPassScheduled"), Probe.bForwardPassScheduled);
	JsonObj->SetBoolField(TEXT("CompositePassScheduled"), Probe.bCompositePassScheduled);
	JsonObj->SetNumberField(TEXT("SplatDrawCount"), Probe.SplatDrawCount);
	JsonObj->SetNumberField(TEXT("ForwardDrawCount"), Probe.ForwardDrawCount);
	JsonObj->SetNumberField(TEXT("CompositeDrawCount"), Probe.CompositeDrawCount);
	JsonObj->SetBoolField(TEXT("DebugReadbackScheduled"), Probe.bDebugReadbackScheduled);
	JsonObj->SetNumberField(TEXT("SkipReason"), (int32)Probe.SkipReason);
	
	TSharedPtr<FJsonObject> RectObj = MakeShareable(new FJsonObject);
	RectObj->SetNumberField(TEXT("MinX"), Probe.ViewRect.Min.X);
	RectObj->SetNumberField(TEXT("MinY"), Probe.ViewRect.Min.Y);
	RectObj->SetNumberField(TEXT("MaxX"), Probe.ViewRect.Max.X);
	RectObj->SetNumberField(TEXT("MaxY"), Probe.ViewRect.Max.Y);
	JsonObj->SetObjectField(TEXT("ViewRect"), RectObj);
	
	TSharedPtr<FJsonObject> ExtentObj = MakeShareable(new FJsonObject);
	ExtentObj->SetNumberField(TEXT("X"), Probe.TextureExtent.X);
	ExtentObj->SetNumberField(TEXT("Y"), Probe.TextureExtent.Y);
	JsonObj->SetObjectField(TEXT("TextureExtent"), ExtentObj);

	if (Probe.ReadbackPayload && Probe.bReadbackReady)
	{
		TSharedPtr<FJsonObject> PayloadObj = MakeShareable(new FJsonObject);
		PayloadObj->SetNumberField(TEXT("CoverageCount"), Probe.ReadbackPayload->Header.FragmentCoverageCount);
		PayloadObj->SetNumberField(TEXT("SceneColorBeforeX"), Probe.ReadbackPayload->Header.SceneColorBefore.X);
		PayloadObj->SetNumberField(TEXT("SceneColorAfterX"), Probe.ReadbackPayload->Header.SceneColorAfter.X);
		PayloadObj->SetNumberField(TEXT("ColorAccumulationX"), Probe.ReadbackPayload->Header.ColorAccumulation.X);
		JsonObj->SetObjectField(TEXT("ReadbackPayload"), PayloadObj);
	}

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObj.ToSharedRef(), Writer);
	FFileHelper::SaveStringToFile(OutputString, *Filename);
}

static void SaveSlicesCSV(const FString& Filename, const FAVBOITRasterExecutionProbe& Probe)
{
	if (!Probe.bReadbackReady || !Probe.ReadbackPayload) return;
	FString CSV = TEXT("Slice,PackedExtinction,Transmittance\n");
	for (int32 i = 0; i < 64; ++i)
	{
		CSV += FString::Printf(TEXT("%d,%u,%f\n"), (int32)i, (uint32)(Probe.ReadbackPayload->PackedExtinction[i]), (float)(Probe.ReadbackPayload->Transmittance[i]));
	}
	FFileHelper::SaveStringToFile(CSV, *Filename);
}

static void EnableRaster(bool bEnable)
{
	if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.AVBOIT.Raster.Enable")))
	{
		CVar->Set(bEnable ? 1 : 0, ECVF_SetByConsole);
	}
}

bool FAVBOITEditorPIEAutomation::IsPIEActive() { return GEditor && GEditor->PlayWorld != nullptr; }

void FAVBOITEditorPIEAutomation::StartPIE()
{
	if (!GUnrealEd || IsPIEActive()) return;
	FRequestPlaySessionParams Params;
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
	Params.DestinationSlateViewport = LevelEditorModule.GetFirstActiveViewport();
	GUnrealEd->RequestPlaySession(Params);
}

void FAVBOITEditorPIEAutomation::EndPIE()
{
	if (GUnrealEd && IsPIEActive()) GUnrealEd->RequestEndPlayMap();
}

void FAVBOITEditorPIEAutomation::RunEditorPIECloseout(const FString& Root, bool bCaptureScreenshots)
{
	if (bEditorPIECloseoutRunning) return;
	EditorPIECloseoutRoot = Root;
	bEditorPIECloseoutCaptureScreenshots = bCaptureScreenshots;
	IFileManager::Get().MakeDirectory(*(EditorPIECloseoutRoot / TEXT("Editor")), true);
	IFileManager::Get().MakeDirectory(*(EditorPIECloseoutRoot / TEXT("PIE")), true);
	bEditorPIECloseoutRunning = true;
	EditorPIECloseoutStep = 0;
	EditorPIECloseoutNextTime = FPlatformTime::Seconds() + 1.0;
	
	FAVBOITEditorSceneBuilder::ApplyDeterministicSettings();
	
	EditorPIECloseoutBeginPIEHandle = FEditorDelegates::BeginPIE.AddStatic(&FAVBOITEditorPIEAutomation::OnEditorPIECloseoutBeginPIE);
	EditorPIECloseoutEndPIEHandle = FEditorDelegates::EndPIE.AddStatic(&FAVBOITEditorPIEAutomation::OnEditorPIECloseoutEndPIE);
	EditorPIECloseoutTickerHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateStatic(&FAVBOITEditorPIEAutomation::TickEditorPIECloseout), 0.25f);
}

void FAVBOITEditorPIEAutomation::RequestCloseoutScreenshot(const TCHAR* RelativePath)
{
	if (!bEditorPIECloseoutCaptureScreenshots) return;
	const FString Filename = EditorPIECloseoutRoot / RelativePath;
	IFileManager::Get().MakeDirectory(*FPaths::GetPath(Filename), true);
	WriteCloseoutViewportPng(Filename);
}

FViewport* FAVBOITEditorPIEAutomation::FindCloseoutViewport()
{
	if (GEngine && GEngine->GameViewport && GEngine->GameViewport->Viewport) return GEngine->GameViewport->Viewport;
	if (GEditor) if (FViewport* ActiveViewport = GEditor->GetActiveViewport()) return ActiveViewport;
	if (FModuleManager::Get().IsModuleLoaded(TEXT("LevelEditor")))
	{
		FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
		const TSharedPtr<IAssetViewport> ActiveLevelViewport = LevelEditorModule.GetFirstActiveViewport();
		if (ActiveLevelViewport.IsValid()) return ActiveLevelViewport->GetActiveViewport();
	}
	return nullptr;
}

bool FAVBOITEditorPIEAutomation::WriteCloseoutViewportPng(const FString& Filename)
{
	FViewport* Viewport = FindCloseoutViewport();
	if (!Viewport) return false;
	const FIntPoint Size = Viewport->GetSizeXY();
	if (Size.X <= 0 || Size.Y <= 0) return false;
	Viewport->Draw();
	TArray<FColor> Bitmap;
	if (!Viewport->ReadPixels(Bitmap) || Bitmap.Num() != Size.X * Size.Y) return false;
	for (FColor& Color : Bitmap) Color.A = 255;
	return FImageUtils::SaveImageByExtension(*Filename, FImageView(Bitmap.GetData(), Size.X, Size.Y));
}

void FAVBOITEditorPIEAutomation::AdvanceCloseoutAfterDelay(const double DelaySeconds)
{
	++EditorPIECloseoutStep;
	EditorPIECloseoutNextTime = FPlatformTime::Seconds() + DelaySeconds;
}

bool FAVBOITEditorPIEAutomation::TickEditorPIECloseout(float)
{
	if (!bEditorPIECloseoutRunning) return false;
	if (FPlatformTime::Seconds() < EditorPIECloseoutNextTime) return true;

	static FAVBOITRasterDebugPayload PayloadStorage;

	switch (EditorPIECloseoutStep)
	{
	case 0:
		// 1. Initialize
		GSceneState = FAVBOITEditorSceneBuilder::BuildTestScene(GEditor->GetEditorWorldContext().World());
		GAVBOITRasterProbe = &GProbeInstance;
		GProbeInstance = FAVBOITRasterExecutionProbe();
		GProbeInstance.ReadbackPayload = &PayloadStorage;
		AdvanceCloseoutAfterDelay(1.0);
		break;
	case 1:
		// EditorDisabled
		EnableRaster(false);
		FAVBOITEditorSceneBuilder::SetTestMeshVisible(GSceneState, true);
		FAVBOITEditorSceneBuilder::SetOccluderVisible(GSceneState, false);
		AdvanceCloseoutAfterDelay(0.5);
		break;
	case 2:
		RequestCloseoutScreenshot(TEXT("Editor/00_Disabled.png"));
		WriteProbeJson(EditorPIECloseoutRoot / TEXT("EditorDisabled.json"), GProbeInstance);
		// Prepare next
		EnableRaster(true);
		GProbeInstance.RequestedDebugPixel = FIntPoint(GProbeInstance.ViewRect.Width()/2, GProbeInstance.ViewRect.Height()/2); // Center pixel
		GProbeInstance.bReadbackReady = false;
		AdvanceCloseoutAfterDelay(0.5);
		break;
	case 3:
		// EditorVisible
		RequestCloseoutScreenshot(TEXT("Editor/01_Visible.png"));
		WriteProbeJson(EditorPIECloseoutRoot / TEXT("EditorVisible.json"), GProbeInstance);
		SaveSlicesCSV(EditorPIECloseoutRoot / TEXT("EditorVisible_Slices.csv"), GProbeInstance);
		// Prepare next
		GProbeInstance = FAVBOITRasterExecutionProbe();
		GProbeInstance.ReadbackPayload = &PayloadStorage;
		GProbeInstance.RequestedDebugPixel = FIntPoint(GProbeInstance.ViewRect.Width()/2, GProbeInstance.ViewRect.Height()/2);
		FAVBOITEditorSceneBuilder::SetOccluderVisible(GSceneState, true);
		AdvanceCloseoutAfterDelay(0.5);
		break;
	case 4:
		// EditorOccluded
		RequestCloseoutScreenshot(TEXT("Editor/02_Occluded.png"));
		WriteProbeJson(EditorPIECloseoutRoot / TEXT("EditorOccluded.json"), GProbeInstance);
		SaveSlicesCSV(EditorPIECloseoutRoot / TEXT("EditorOccluded_Slices.csv"), GProbeInstance);
		// Prepare next
		EnableRaster(false);
		StartPIE();
		AdvanceCloseoutAfterDelay(3.0);
		break;
	case 5:
		if (!IsPIEActive()) { EditorPIECloseoutNextTime = FPlatformTime::Seconds() + 1.0; return true; }
		// PIEDisabled
		GProbeInstance = FAVBOITRasterExecutionProbe();
		GProbeInstance.ReadbackPayload = &PayloadStorage;
		AdvanceCloseoutAfterDelay(0.5);
		break;
	case 6:
		RequestCloseoutScreenshot(TEXT("PIE/00_Disabled.png"));
		WriteProbeJson(EditorPIECloseoutRoot / TEXT("PIEDisabled.json"), GProbeInstance);
		// Prepare next
		EnableRaster(true);
		FAVBOITEditorSceneBuilder::SetOccluderVisible(GSceneState, false); // Applies to Editor objects, PIE duplicates need care
		// To fix PIE occluder, we should find the PIE actor or just rely on camera.
		// For simplicity, we just toggle all actors with the tag in the PIE world.
		if (UWorld* PIEWorld = GEditor->PlayWorld)
		{
			for (TActorIterator<AActor> It(PIEWorld); It; ++It)
			{
				if (It->ActorHasTag(AVBOIT_TEST_TAG))
				{
					if (It->IsA(AStaticMeshActor::StaticClass()) && !It->GetActorScale3D().Equals(FVector(50.f,50.f,1.f)))
					{
						It->SetActorHiddenInGame(true); // Hide occluder
					}
				}
			}
		}
		GProbeInstance.RequestedDebugPixel = FIntPoint(GProbeInstance.ViewRect.Width()/2, GProbeInstance.ViewRect.Height()/2);
		AdvanceCloseoutAfterDelay(0.5);
		break;
	case 7:
		// PIEVisible
		RequestCloseoutScreenshot(TEXT("PIE/01_Visible.png"));
		WriteProbeJson(EditorPIECloseoutRoot / TEXT("PIEVisible.json"), GProbeInstance);
		SaveSlicesCSV(EditorPIECloseoutRoot / TEXT("PIEVisible_Slices.csv"), GProbeInstance);
		// Prepare next
		GProbeInstance = FAVBOITRasterExecutionProbe();
		GProbeInstance.ReadbackPayload = &PayloadStorage;
		GProbeInstance.RequestedDebugPixel = FIntPoint(GProbeInstance.ViewRect.Width()/2, GProbeInstance.ViewRect.Height()/2);
		if (UWorld* PIEWorld = GEditor->PlayWorld)
		{
			for (TActorIterator<AActor> It(PIEWorld); It; ++It)
			{
				if (It->ActorHasTag(AVBOIT_TEST_TAG))
				{
					if (It->IsA(AStaticMeshActor::StaticClass()) && !It->GetActorScale3D().Equals(FVector(50.f,50.f,1.f)))
					{
						It->SetActorHiddenInGame(false); // Show occluder
					}
				}
			}
		}
		AdvanceCloseoutAfterDelay(0.5);
		break;
	case 8:
		// PIEOccluded
		RequestCloseoutScreenshot(TEXT("PIE/02_Occluded.png"));
		WriteProbeJson(EditorPIECloseoutRoot / TEXT("PIEOccluded.json"), GProbeInstance);
		SaveSlicesCSV(EditorPIECloseoutRoot / TEXT("PIEOccluded_Slices.csv"), GProbeInstance);
		
		// Setup LifecycleDestroy
		if (UWorld* PIEWorld = GEditor->PlayWorld)
		{
			for (TActorIterator<AActor> It(PIEWorld); It; ++It)
			{
				if (It->ActorHasTag(AVBOIT_TEST_TAG) && !It->IsA(AStaticMeshActor::StaticClass()) && !It->IsA(ACameraActor::StaticClass()))
				{
					It->Destroy(); // Destroy TestActor
				}
			}
		}
		GProbeInstance = FAVBOITRasterExecutionProbe();
		GProbeInstance.ReadbackPayload = &PayloadStorage;
		AdvanceCloseoutAfterDelay(1.0);
		break;
	case 9:
		WriteProbeJson(EditorPIECloseoutRoot / TEXT("LifecycleDestroy.json"), GProbeInstance);
		
		// Setup LifecycleRespawn
		if (UWorld* PIEWorld = GEditor->PlayWorld)
		{
			FActorSpawnParameters SpawnParams;
			AActor* ReActor = PIEWorld->SpawnActor<AActor>(FVector(100, 0, 100), FRotator::ZeroRotator, SpawnParams);
			ReActor->Tags.Add(AVBOIT_TEST_TAG);
			USceneComponent* RootComp = NewObject<USceneComponent>(ReActor);
			ReActor->SetRootComponent(RootComp);
			RootComp->RegisterComponent();
			UAVBOITTestMeshComponent* ReMesh = NewObject<UAVBOITTestMeshComponent>(ReActor);
			ReMesh->SetupAttachment(RootComp);
			ReMesh->MaterialParams.Color = FLinearColor::White;
			ReMesh->MaterialParams.Alpha = 0.5f;
			ReMesh->bIsTransparent = true;
			ReMesh->RegisterComponent();
		}
		GProbeInstance = FAVBOITRasterExecutionProbe();
		GProbeInstance.ReadbackPayload = &PayloadStorage;
		AdvanceCloseoutAfterDelay(1.0);
		break;
	case 10:
		WriteProbeJson(EditorPIECloseoutRoot / TEXT("LifecycleRespawn.json"), GProbeInstance);
		FAVBOITEditorSceneBuilder::ForceResizeViewport(640, 360);
		AdvanceCloseoutAfterDelay(1.0);
		break;
	case 11:
		WriteProbeJson(EditorPIECloseoutRoot / TEXT("Resize640x360.json"), GProbeInstance);
		FAVBOITEditorSceneBuilder::ForceResizeViewport(960, 540);
		AdvanceCloseoutAfterDelay(1.0);
		break;
	case 12:
		WriteProbeJson(EditorPIECloseoutRoot / TEXT("Resize960x540.json"), GProbeInstance);
		EndPIE();
		AdvanceCloseoutAfterDelay(2.0);
		break;
	case 13:
		if (IsPIEActive()) { EditorPIECloseoutNextTime = FPlatformTime::Seconds() + 1.0; return true; }
		// Original Regression
		EnableRaster(false);
		GProbeInstance = FAVBOITRasterExecutionProbe();
		GProbeInstance.ReadbackPayload = &PayloadStorage;
		AdvanceCloseoutAfterDelay(0.5);
		break;
	case 14:
		RequestCloseoutScreenshot(TEXT("Editor/10_OriginalMaterialShaderRegression.png"));
		FinishEditorPIECloseout(true);
		return false;
	default:
		FinishEditorPIECloseout(true);
		return false;
	}

	return true;
}

void FAVBOITEditorPIEAutomation::OnEditorPIECloseoutBeginPIE(const bool bIsSimulating) {}
void FAVBOITEditorPIEAutomation::OnEditorPIECloseoutEndPIE(const bool bIsSimulating) {}

void FAVBOITEditorPIEAutomation::FinishEditorPIECloseout(const bool bRequestExit)
{
	if (!bEditorPIECloseoutRunning) return;
	EnableRaster(false);
	GAVBOITRasterProbe = nullptr;
	FAVBOITEditorSceneBuilder::CleanupExistingTestActors(GSceneState.World);
	FAVBOITEditorSceneBuilder::RestoreSettings();

	if (EditorPIECloseoutTickerHandle.IsValid()) FTSTicker::GetCoreTicker().RemoveTicker(EditorPIECloseoutTickerHandle);
	if (EditorPIECloseoutBeginPIEHandle.IsValid()) FEditorDelegates::BeginPIE.Remove(EditorPIECloseoutBeginPIEHandle);
	if (EditorPIECloseoutEndPIEHandle.IsValid()) FEditorDelegates::EndPIE.Remove(EditorPIECloseoutEndPIEHandle);
	
	bEditorPIECloseoutRunning = false;
	if (bRequestExit) FPlatformMisc::RequestExit(false);
}

#endif
