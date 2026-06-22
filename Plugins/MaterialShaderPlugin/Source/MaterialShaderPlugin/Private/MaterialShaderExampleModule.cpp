// Copyright Voxel Plugin SAS. All Rights Reserved.

#include "CoreMinimal.h"
#include "Containers/Ticker.h"
#include "CoreGlobals.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "HAL/FileManager.h"
#include "HAL/IConsoleManager.h"
#include "ImageUtils.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "Modules/ModuleManager.h"
#include "UnrealClient.h"

#if WITH_EDITOR
#include "Editor.h"
#include "Editor/UnrealEdEngine.h"
#include "IAssetViewport.h"
#include "LevelEditor.h"
#include "PlayInEditorDataTypes.h"
#include "UnrealEdGlobals.h"
#endif

DEFINE_LOG_CATEGORY_STATIC(LogAVBOITEvidence, Log, All);

class FMaterialShaderExampleModule : public IModuleInterface
{
public:
	//~ Begin IModuleInterface Implementation
	virtual void StartupModule() override
	{
		const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin("MaterialShaderExample");
		check(Plugin);

		const FString Path = FPaths::ConvertRelativePathToFull(Plugin->GetBaseDir() / "Shaders");

		AddShaderSourceDirectoryMapping(TEXT("/Plugin/MaterialShaderExample"), Path);

		CaptureScreenshotAndExitCommand = IConsoleManager::Get().RegisterConsoleCommand(
			TEXT("AVBOIT.CaptureScreenshotAndExit"),
			TEXT("Requests a screenshot and exits after FScreenshotRequest reports it was processed. Usage: AVBOIT.CaptureScreenshotAndExit filename=<AbsoluteOrProjectRelativePng>"),
			FConsoleCommandWithArgsDelegate::CreateRaw(this, &FMaterialShaderExampleModule::CaptureScreenshotAndExit),
			ECVF_Default);

		CaptureSmokeScreenshotAndExitCommand = IConsoleManager::Get().RegisterConsoleCommand(
			TEXT("AVBOIT.CaptureSmokeScreenshotAndExit"),
			TEXT("Enables the RDG smoke test with capture-friendly CVars, requests a screenshot, and exits. Optional: mode=<1|2|3> opacity=<float> filename=<png>"),
			FConsoleCommandWithArgsDelegate::CreateRaw(this, &FMaterialShaderExampleModule::CaptureSmokeScreenshotAndExit),
			ECVF_Default);

#if WITH_EDITOR
		EditorPIECloseoutCommand = IConsoleManager::Get().RegisterConsoleCommand(
			TEXT("AVBOIT.RunEditorPIECloseout"),
			TEXT("Runs the UE2 editor and PIE closeout sequence. Usage: AVBOIT.RunEditorPIECloseout root=<RelativeOrAbsoluteEvidenceRoot>"),
			FConsoleCommandWithArgsDelegate::CreateRaw(this, &FMaterialShaderExampleModule::RunEditorPIECloseout),
			ECVF_Default);
#endif
	}

	virtual void ShutdownModule() override
	{
#if WITH_EDITOR
		FinishEditorPIECloseout(/*bRequestExit*/ false);
#endif

		FScreenshotRequest::OnScreenshotRequestProcessed().RemoveAll(this);

		if (CaptureScreenshotAndExitCommand)
		{
			IConsoleManager::Get().UnregisterConsoleObject(CaptureScreenshotAndExitCommand);
			CaptureScreenshotAndExitCommand = nullptr;
		}

		if (CaptureSmokeScreenshotAndExitCommand)
		{
			IConsoleManager::Get().UnregisterConsoleObject(CaptureSmokeScreenshotAndExitCommand);
			CaptureSmokeScreenshotAndExitCommand = nullptr;
		}

#if WITH_EDITOR
		if (EditorPIECloseoutCommand)
		{
			IConsoleManager::Get().UnregisterConsoleObject(EditorPIECloseoutCommand);
			EditorPIECloseoutCommand = nullptr;
		}
#endif
	}
	//~ End IModuleInterface Implementation

private:
	static FString FindArgValue(const TArray<FString>& Args, const TCHAR* Prefix)
	{
		for (const FString& Arg : Args)
		{
			if (Arg.StartsWith(Prefix))
			{
				return Arg.RightChop(FCString::Strlen(Prefix));
			}
		}

		return FString();
	}

	static void SetIntCVar(const TCHAR* Name, int32 Value)
	{
		if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(Name))
		{
			CVar->Set(Value, ECVF_SetByConsole);
		}
	}

	static void SetFloatCVar(const TCHAR* Name, float Value)
	{
		if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(Name))
		{
			CVar->Set(Value, ECVF_SetByConsole);
		}
	}

	void CaptureScreenshotAndExit(const TArray<FString>& Args)
	{
		FString Filename;
		for (const FString& Arg : Args)
		{
			if (Arg.StartsWith(TEXT("filename=")))
			{
				Filename = Arg.RightChop(9);
				break;
			}

			if (Filename.IsEmpty())
			{
				Filename = Arg;
			}
		}

		if (Filename.IsEmpty())
		{
			Filename = FPaths::ProjectSavedDir() / TEXT("Screenshots/AVBOIT_CaptureScreenshotAndExit.png");
		}

		FScreenshotRequest::OnScreenshotRequestProcessed().RemoveAll(this);
		FScreenshotRequest::OnScreenshotRequestProcessed().AddRaw(this, &FMaterialShaderExampleModule::OnScreenshotProcessed);
		FScreenshotRequest::RequestScreenshot(Filename, /*bInShowUI*/ false, /*bAddUniqueSuffix*/ false, /*bHdrScreenshot*/ false);

		UE_LOG(LogAVBOITEvidence, Display, TEXT("Requested screenshot before exit: %s"), *Filename);
	}

	void CaptureSmokeScreenshotAndExit(const TArray<FString>& Args)
	{
		int32 DebugMode = 2;
		if (const FString ModeArg = FindArgValue(Args, TEXT("mode=")); !ModeArg.IsEmpty())
		{
			DebugMode = FCString::Atoi(*ModeArg);
		}

		float Opacity = 0.65f;
		if (const FString OpacityArg = FindArgValue(Args, TEXT("opacity=")); !OpacityArg.IsEmpty())
		{
			Opacity = FCString::Atof(*OpacityArg);
		}

		SetIntCVar(TEXT("r.ShaderDevelopmentMode"), 1);
		SetIntCVar(TEXT("r.RDG.Events"), 1);
		SetIntCVar(TEXT("r.RDG.Debug"), 1);
		SetIntCVar(TEXT("r.AVBOIT.Smoke.Enable"), 1);
		SetIntCVar(TEXT("r.AVBOIT.Smoke.DebugMode"), DebugMode);
		SetFloatCVar(TEXT("r.AVBOIT.Smoke.Opacity"), Opacity);
		SetEmitDrawEvents(true);

		TArray<FString> ScreenshotArgs;
		if (const FString Filename = FindArgValue(Args, TEXT("filename=")); !Filename.IsEmpty())
		{
			ScreenshotArgs.Add(TEXT("filename=") + Filename);
		}
		else
		{
			ScreenshotArgs.Add(TEXT("filename=") + FPaths::ProjectSavedDir() / TEXT("Screenshots/AVBOIT_CaptureSmokeScreenshotAndExit.png"));
		}

		UE_LOG(LogAVBOITEvidence, Display, TEXT("Configured smoke capture: Mode=%d Opacity=%f DrawEvents=true"), DebugMode, Opacity);
		CaptureScreenshotAndExit(ScreenshotArgs);
	}

	void OnScreenshotProcessed()
	{
		FScreenshotRequest::OnScreenshotRequestProcessed().RemoveAll(this);
		UE_LOG(LogAVBOITEvidence, Display, TEXT("Screenshot processed; requesting exit."));
		FPlatformMisc::RequestExit(false);
	}

	IConsoleObject* CaptureScreenshotAndExitCommand = nullptr;
	IConsoleObject* CaptureSmokeScreenshotAndExitCommand = nullptr;

#if WITH_EDITOR
	void ApplySmokeState(const int32 Enable, const int32 DebugMode)
	{
		SetIntCVar(TEXT("r.AVBOIT.Smoke.Enable"), Enable);
		SetIntCVar(TEXT("r.AVBOIT.Smoke.DebugMode"), DebugMode);
		SetFloatCVar(TEXT("r.AVBOIT.Smoke.Opacity"), 0.65f);
		UE_LOG(LogAVBOITEvidence, Display, TEXT("Closeout state: Enable=%d DebugMode=%d"), Enable, DebugMode);
	}

	bool IsPIEActive() const
	{
		return GEditor && GEditor->PlayWorld != nullptr;
	}

	void StartPIE()
	{
		if (!GUnrealEd || IsPIEActive())
		{
			return;
		}

		FRequestPlaySessionParams Params;
		FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
		Params.DestinationSlateViewport = LevelEditorModule.GetFirstActiveViewport();
		GUnrealEd->RequestPlaySession(Params);
		UE_LOG(LogAVBOITEvidence, Display, TEXT("Closeout requested PIE start."));
	}

	void EndPIE()
	{
		if (GUnrealEd && IsPIEActive())
		{
			GUnrealEd->RequestEndPlayMap();
			UE_LOG(LogAVBOITEvidence, Display, TEXT("Closeout requested PIE end."));
		}
	}

	void RunEditorPIECloseout(const TArray<FString>& Args)
	{
		if (bEditorPIECloseoutRunning)
		{
			UE_LOG(LogAVBOITEvidence, Warning, TEXT("Editor/PIE closeout is already running."));
			return;
		}

		FString Root = FindArgValue(Args, TEXT("root="));
		if (Root.IsEmpty())
		{
			Root = TEXT("LocalVisualResults/UE57/HIVE-4090X2/UE2-Closeout");
		}

		EditorPIECloseoutRoot = FPaths::IsRelative(Root)
			? FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / Root)
			: Root;
		FPaths::NormalizeFilename(EditorPIECloseoutRoot);
		bEditorPIECloseoutCaptureScreenshots = FindArgValue(Args, TEXT("screenshots=")) != TEXT("0");

		IFileManager::Get().MakeDirectory(*(EditorPIECloseoutRoot / TEXT("Editor")), /*Tree*/ true);
		IFileManager::Get().MakeDirectory(*(EditorPIECloseoutRoot / TEXT("PIE")), /*Tree*/ true);

		bEditorPIECloseoutRunning = true;
		EditorPIECloseoutStep = 0;
		EditorPIECloseoutNextTime = FPlatformTime::Seconds() + 2.0;

		EditorPIECloseoutBeginPIEHandle = FEditorDelegates::BeginPIE.AddRaw(this, &FMaterialShaderExampleModule::OnEditorPIECloseoutBeginPIE);
		EditorPIECloseoutEndPIEHandle = FEditorDelegates::EndPIE.AddRaw(this, &FMaterialShaderExampleModule::OnEditorPIECloseoutEndPIE);
		EditorPIECloseoutTickerHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateRaw(this, &FMaterialShaderExampleModule::TickEditorPIECloseout), 0.25f);

		UE_LOG(LogAVBOITEvidence, Display, TEXT("Started editor/PIE closeout sequence at %s screenshots=%s"), *EditorPIECloseoutRoot, bEditorPIECloseoutCaptureScreenshots ? TEXT("true") : TEXT("false"));
	}

	void RequestCloseoutScreenshot(const TCHAR* RelativePath)
	{
		if (!bEditorPIECloseoutCaptureScreenshots)
		{
			UE_LOG(LogAVBOITEvidence, Display, TEXT("Closeout skipped screenshot by request: %s"), RelativePath);
			AdvanceCloseoutAfterDelay(0.5);
			return;
		}

		const FString Filename = EditorPIECloseoutRoot / RelativePath;
		IFileManager::Get().MakeDirectory(*FPaths::GetPath(Filename), /*Tree*/ true);

		if (WriteCloseoutViewportPng(Filename))
		{
			UE_LOG(LogAVBOITEvidence, Display, TEXT("Closeout wrote viewport screenshot: %s"), *Filename);
		}
		else
		{
			UE_LOG(LogAVBOITEvidence, Warning, TEXT("Closeout failed to write viewport screenshot: %s"), *Filename);
		}

		AdvanceCloseoutAfterDelay(1.0);
	}

	FViewport* FindCloseoutViewport() const
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

	bool WriteCloseoutViewportPng(const FString& Filename) const
	{
		FViewport* Viewport = FindCloseoutViewport();
		if (!Viewport)
		{
			UE_LOG(LogAVBOITEvidence, Warning, TEXT("Closeout screenshot failed because no active viewport was found."));
			return false;
		}

		const FIntPoint Size = Viewport->GetSizeXY();
		if (Size.X <= 0 || Size.Y <= 0)
		{
			UE_LOG(LogAVBOITEvidence, Warning, TEXT("Closeout screenshot failed because viewport size is invalid: %dx%d"), Size.X, Size.Y);
			return false;
		}

		Viewport->Draw();

		TArray<FColor> Bitmap;
		if (!Viewport->ReadPixels(Bitmap) || Bitmap.Num() != Size.X * Size.Y)
		{
			UE_LOG(LogAVBOITEvidence, Warning, TEXT("Closeout screenshot failed while reading %dx%d viewport pixels; read %d pixels."), Size.X, Size.Y, Bitmap.Num());
			return false;
		}

		for (FColor& Color : Bitmap)
		{
			Color.A = 255;
		}

		const FImageView Image(Bitmap.GetData(), Size.X, Size.Y);
		return FImageUtils::SaveImageByExtension(*Filename, Image);
	}

	void AdvanceCloseoutAfterDelay(const double DelaySeconds)
	{
		++EditorPIECloseoutStep;
		EditorPIECloseoutNextTime = FPlatformTime::Seconds() + DelaySeconds;
	}

	bool TickEditorPIECloseout(float)
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

	void OnEditorPIECloseoutBeginPIE(const bool bIsSimulating)
	{
		UE_LOG(LogAVBOITEvidence, Display, TEXT("Closeout observed BeginPIE. Simulating=%s"), bIsSimulating ? TEXT("true") : TEXT("false"));
	}

	void OnEditorPIECloseoutEndPIE(const bool bIsSimulating)
	{
		UE_LOG(LogAVBOITEvidence, Display, TEXT("Closeout observed EndPIE. Simulating=%s"), bIsSimulating ? TEXT("true") : TEXT("false"));
	}

	void FinishEditorPIECloseout(const bool bRequestExit)
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
		UE_LOG(LogAVBOITEvidence, Display, TEXT("Finished editor/PIE closeout sequence."));

		if (bRequestExit)
		{
			FPlatformMisc::RequestExit(false);
		}
	}

	IConsoleObject* EditorPIECloseoutCommand = nullptr;
	FTSTicker::FDelegateHandle EditorPIECloseoutTickerHandle;
	FDelegateHandle EditorPIECloseoutBeginPIEHandle;
	FDelegateHandle EditorPIECloseoutEndPIEHandle;
	FString EditorPIECloseoutRoot;
	double EditorPIECloseoutNextTime = 0.0;
	int32 EditorPIECloseoutStep = 0;
	bool bEditorPIECloseoutRunning = false;
	bool bEditorPIECloseoutCaptureScreenshots = true;
#endif
};
IMPLEMENT_MODULE(FMaterialShaderExampleModule, MaterialShaderExample)
