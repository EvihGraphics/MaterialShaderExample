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

FTSTicker::FDelegateHandle FAVBOITEditorPIEAutomation::EditorPIECloseoutTickerHandle;
FDelegateHandle FAVBOITEditorPIEAutomation::EditorPIECloseoutBeginPIEHandle;
FDelegateHandle FAVBOITEditorPIEAutomation::EditorPIECloseoutEndPIEHandle;
FString FAVBOITEditorPIEAutomation::EditorPIECloseoutRoot;
double FAVBOITEditorPIEAutomation::EditorPIECloseoutNextTime = 0.0;
int32 FAVBOITEditorPIEAutomation::EditorPIECloseoutStep = 0;
bool FAVBOITEditorPIEAutomation::bEditorPIECloseoutRunning = false;
bool FAVBOITEditorPIEAutomation::bEditorPIECloseoutCaptureScreenshots = true;

void FAVBOITEditorPIEAutomation::ApplySmokeState(const int32 Enable, const int32 DebugMode)
{
	if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.AVBOIT.Smoke.Enable"))) CVar->Set(Enable, ECVF_SetByConsole);
	if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.AVBOIT.Smoke.DebugMode"))) CVar->Set(DebugMode, ECVF_SetByConsole);
	if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.AVBOIT.Smoke.Opacity"))) CVar->Set(0.65f, ECVF_SetByConsole);
	UE_LOG(LogTemp, Display, TEXT("Closeout state: Enable=%d DebugMode=%d"), Enable, DebugMode);
}

bool FAVBOITEditorPIEAutomation::IsPIEActive()
{
	return GEditor && GEditor->PlayWorld != nullptr;
}

void FAVBOITEditorPIEAutomation::StartPIE()
{
	if (!GUnrealEd || IsPIEActive())
	{
		return;
	}

	FRequestPlaySessionParams Params;
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
	Params.DestinationSlateViewport = LevelEditorModule.GetFirstActiveViewport();
	GUnrealEd->RequestPlaySession(Params);
	UE_LOG(LogTemp, Display, TEXT("Closeout requested PIE start."));
}

void FAVBOITEditorPIEAutomation::EndPIE()
{
	if (GUnrealEd && IsPIEActive())
	{
		GUnrealEd->RequestEndPlayMap();
		UE_LOG(LogTemp, Display, TEXT("Closeout requested PIE end."));
	}
}

void FAVBOITEditorPIEAutomation::RunEditorPIECloseout(const FString& Root, bool bCaptureScreenshots)
{
	if (bEditorPIECloseoutRunning)
	{
		UE_LOG(LogTemp, Warning, TEXT("Editor/PIE closeout is already running."));
		return;
	}

	EditorPIECloseoutRoot = Root;
	bEditorPIECloseoutCaptureScreenshots = bCaptureScreenshots;

	IFileManager::Get().MakeDirectory(*(EditorPIECloseoutRoot / TEXT("Editor")), /*Tree*/ true);
	IFileManager::Get().MakeDirectory(*(EditorPIECloseoutRoot / TEXT("PIE")), /*Tree*/ true);

	bEditorPIECloseoutRunning = true;
	EditorPIECloseoutStep = 0;
	EditorPIECloseoutNextTime = FPlatformTime::Seconds() + 2.0;

	EditorPIECloseoutBeginPIEHandle = FEditorDelegates::BeginPIE.AddStatic(&FAVBOITEditorPIEAutomation::OnEditorPIECloseoutBeginPIE);
	EditorPIECloseoutEndPIEHandle = FEditorDelegates::EndPIE.AddStatic(&FAVBOITEditorPIEAutomation::OnEditorPIECloseoutEndPIE);
	EditorPIECloseoutTickerHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateStatic(&FAVBOITEditorPIEAutomation::TickEditorPIECloseout), 0.25f);

	UE_LOG(LogTemp, Display, TEXT("Started editor/PIE closeout sequence at %s screenshots=%s"), *EditorPIECloseoutRoot, bEditorPIECloseoutCaptureScreenshots ? TEXT("true") : TEXT("false"));
}

void FAVBOITEditorPIEAutomation::RequestCloseoutScreenshot(const TCHAR* RelativePath)
{
	if (!bEditorPIECloseoutCaptureScreenshots)
	{
		UE_LOG(LogTemp, Display, TEXT("Closeout skipped screenshot by request: %s"), RelativePath);
		AdvanceCloseoutAfterDelay(0.5);
		return;
	}

	const FString Filename = EditorPIECloseoutRoot / RelativePath;
	IFileManager::Get().MakeDirectory(*FPaths::GetPath(Filename), /*Tree*/ true);

	if (WriteCloseoutViewportPng(Filename))
	{
		UE_LOG(LogTemp, Display, TEXT("Closeout wrote viewport screenshot: %s"), *Filename);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Closeout failed to write viewport screenshot: %s"), *Filename);
	}

	AdvanceCloseoutAfterDelay(1.0);
}

FViewport* FAVBOITEditorPIEAutomation::FindCloseoutViewport()
{
	if (GEngine && GEngine->GameViewport && GEngine->GameViewport->Viewport)
	{
		return GEngine->GameViewport->Viewport;
	}

	if (GEditor)
	{
		if (FViewport* ActiveViewport = GEditor->GetActiveViewport())
		{
			return ActiveViewport;
		}
	}

	if (FModuleManager::Get().IsModuleLoaded(TEXT("LevelEditor")))
	{
		FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
		const TSharedPtr<IAssetViewport> ActiveLevelViewport = LevelEditorModule.GetFirstActiveViewport();
		if (ActiveLevelViewport.IsValid())
		{
			return ActiveLevelViewport->GetActiveViewport();
		}
	}

	return nullptr;
}

bool FAVBOITEditorPIEAutomation::WriteCloseoutViewportPng(const FString& Filename)
{
	FViewport* Viewport = FindCloseoutViewport();
	if (!Viewport)
	{
		UE_LOG(LogTemp, Warning, TEXT("Closeout screenshot failed because no active viewport was found."));
		return false;
	}

	const FIntPoint Size = Viewport->GetSizeXY();
	if (Size.X <= 0 || Size.Y <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Closeout screenshot failed because viewport size is invalid: %dx%d"), Size.X, Size.Y);
		return false;
	}

	Viewport->Draw();

	TArray<FColor> Bitmap;
	if (!Viewport->ReadPixels(Bitmap) || Bitmap.Num() != Size.X * Size.Y)
	{
		UE_LOG(LogTemp, Warning, TEXT("Closeout screenshot failed while reading %dx%d viewport pixels; read %d pixels."), Size.X, Size.Y, Bitmap.Num());
		return false;
	}

	for (FColor& Color : Bitmap)
	{
		Color.A = 255;
	}

	const FImageView Image(Bitmap.GetData(), Size.X, Size.Y);
	return FImageUtils::SaveImageByExtension(*Filename, Image);
}

void FAVBOITEditorPIEAutomation::AdvanceCloseoutAfterDelay(const double DelaySeconds)
{
	++EditorPIECloseoutStep;
	EditorPIECloseoutNextTime = FPlatformTime::Seconds() + DelaySeconds;
}

bool FAVBOITEditorPIEAutomation::TickEditorPIECloseout(float)
{
	if (!bEditorPIECloseoutRunning)
	{
		return false;
	}

	if (FPlatformTime::Seconds() < EditorPIECloseoutNextTime)
	{
		return true;
	}

	switch (EditorPIECloseoutStep)
	{
	case 0:
		ApplySmokeState(0, 1);
		RequestCloseoutScreenshot(TEXT("Editor/00_Disabled.png"));
		break;
	case 1:
		ApplySmokeState(1, 1);
		RequestCloseoutScreenshot(TEXT("Editor/01_Mode1_Deterministic.png"));
		break;
	case 2:
		ApplySmokeState(1, 2);
		RequestCloseoutScreenshot(TEXT("Editor/02_Mode2_SceneDepth.png"));
		break;
	case 3:
		ApplySmokeState(1, 3);
		RequestCloseoutScreenshot(TEXT("Editor/03_Mode3_SceneColorOverlay.png"));
		break;
	case 4:
		ApplySmokeState(0, 1);
		RequestCloseoutScreenshot(TEXT("Editor/10_OriginalMaterialShaderRegression.png"));
		break;
	case 5:
		StartPIE();
		AdvanceCloseoutAfterDelay(4.0);
		break;
	case 6:
		if (!IsPIEActive())
		{
			EditorPIECloseoutNextTime = FPlatformTime::Seconds() + 1.0;
			return true;
		}
		ApplySmokeState(0, 1);
		RequestCloseoutScreenshot(TEXT("PIE/00_Disabled.png"));
		break;
	case 7:
		ApplySmokeState(1, 1);
		RequestCloseoutScreenshot(TEXT("PIE/01_Mode1.png"));
		break;
	case 8:
		ApplySmokeState(1, 2);
		RequestCloseoutScreenshot(TEXT("PIE/02_Mode2_SceneDepth.png"));
		break;
	case 9:
		ApplySmokeState(1, 3);
		RequestCloseoutScreenshot(TEXT("PIE/03_Mode3_SceneColorOverlay.png"));
		break;
	case 10:
		EndPIE();
		AdvanceCloseoutAfterDelay(3.0);
		break;
	case 11:
		if (IsPIEActive())
		{
			EditorPIECloseoutNextTime = FPlatformTime::Seconds() + 1.0;
			return true;
		}
		StartPIE();
		AdvanceCloseoutAfterDelay(4.0);
		break;
	case 12:
		if (!IsPIEActive())
		{
			EditorPIECloseoutNextTime = FPlatformTime::Seconds() + 1.0;
			return true;
		}
		ApplySmokeState(1, 1);
		AdvanceCloseoutAfterDelay(1.0);
		break;
	case 13:
		ApplySmokeState(0, 1);
		AdvanceCloseoutAfterDelay(1.0);
		break;
	case 14:
		ApplySmokeState(1, 2);
		AdvanceCloseoutAfterDelay(1.0);
		break;
	case 15:
		ApplySmokeState(0, 2);
		AdvanceCloseoutAfterDelay(1.0);
		break;
	case 16:
		ApplySmokeState(1, 3);
		RequestCloseoutScreenshot(TEXT("PIE/04_Lifecycle_FinalEnable.png"));
		break;
	case 17:
		EndPIE();
		AdvanceCloseoutAfterDelay(3.0);
		break;
	case 18:
		if (IsPIEActive())
		{
			EditorPIECloseoutNextTime = FPlatformTime::Seconds() + 1.0;
			return true;
		}
		FinishEditorPIECloseout(/*bRequestExit*/ true);
		return false;
	default:
		FinishEditorPIECloseout(/*bRequestExit*/ true);
		return false;
	}

	return true;
}

void FAVBOITEditorPIEAutomation::OnEditorPIECloseoutBeginPIE(const bool bIsSimulating)
{
	UE_LOG(LogTemp, Display, TEXT("Closeout observed BeginPIE. Simulating=%s"), bIsSimulating ? TEXT("true") : TEXT("false"));
}

void FAVBOITEditorPIEAutomation::OnEditorPIECloseoutEndPIE(const bool bIsSimulating)
{
	UE_LOG(LogTemp, Display, TEXT("Closeout observed EndPIE. Simulating=%s"), bIsSimulating ? TEXT("true") : TEXT("false"));
}

void FAVBOITEditorPIEAutomation::FinishEditorPIECloseout(const bool bRequestExit)
{
	if (!bEditorPIECloseoutRunning)
	{
		return;
	}

	ApplySmokeState(0, 1);

	if (EditorPIECloseoutTickerHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(EditorPIECloseoutTickerHandle);
		EditorPIECloseoutTickerHandle.Reset();
	}

	if (EditorPIECloseoutBeginPIEHandle.IsValid())
	{
		FEditorDelegates::BeginPIE.Remove(EditorPIECloseoutBeginPIEHandle);
		EditorPIECloseoutBeginPIEHandle.Reset();
	}

	if (EditorPIECloseoutEndPIEHandle.IsValid())
	{
		FEditorDelegates::EndPIE.Remove(EditorPIECloseoutEndPIEHandle);
		EditorPIECloseoutEndPIEHandle.Reset();
	}

	bEditorPIECloseoutRunning = false;
	bEditorPIECloseoutCaptureScreenshots = true;
	UE_LOG(LogTemp, Display, TEXT("Finished editor/PIE closeout sequence."));

	if (bRequestExit)
	{
		FPlatformMisc::RequestExit(false);
	}
}

#endif
