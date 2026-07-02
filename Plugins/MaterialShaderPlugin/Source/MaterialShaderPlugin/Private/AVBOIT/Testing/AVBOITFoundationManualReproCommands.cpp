#include "AVBOITFoundationManualReproCommands.h"

#include "AVBOIT/Raster/AVBOITRasterRenderer.h"
#include "AVBOIT/Raster/AVBOITRasterShaders.h"
#include "AVBOIT/Raster/AVBOITTestMeshComponent.h"
#include "AVBOITScreenshotAutomation.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Containers/Ticker.h"
#include "Components/SceneComponent.h"
#include "DynamicRHI.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "HAL/FileManager.h"
#include "HAL/IConsoleManager.h"
#include "HAL/PlatformProcess.h"
#include "Misc/App.h"
#include "Misc/DateTime.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

#if WITH_EDITOR
#include "Editor.h"
#include "Editor/EditorEngine.h"
#include "LevelEditorViewport.h"
#endif

DEFINE_LOG_CATEGORY_STATIC(LogAVBOITFoundationManualRepro, Log, All);

IConsoleObject* FAVBOITFoundationManualReproCommands::SpawnTransparentSortingSceneCommand = nullptr;
IConsoleObject* FAVBOITFoundationManualReproCommands::SetOrderCommand = nullptr;
IConsoleObject* FAVBOITFoundationManualReproCommands::SetModeCommand = nullptr;
IConsoleObject* FAVBOITFoundationManualReproCommands::StatusCommand = nullptr;
IConsoleObject* FAVBOITFoundationManualReproCommands::CleanupTransparentSortingSceneCommand = nullptr;
IConsoleObject* FAVBOITFoundationManualReproCommands::ValidateTransparentSortingSceneAndExitCommand = nullptr;

namespace
{
	static const FName AVBOIT_FOUNDATION_MANUAL_TAG(TEXT("AVBOIT_FoundationManualTransparentSorting"));
	static FString GLastManualReproManifestPath;
	static FTSTicker::FDelegateHandle GManualValidationTickerHandle;
	static FString GManualValidationRoot;
	static FString GManualValidationScreenshotPath;
	static double GManualValidationStartTime = 0.0;
	static bool bManualValidationExitAfter = true;
	static bool bManualValidationRequireDebugReadback = false;
#if WITH_EDITOR || WITH_DEV_AUTOMATION_TESTS
	static FAVBOITRasterDebugPayload GManualDebugPayload;
#endif

#if WITH_EDITOR || WITH_DEV_AUTOMATION_TESTS
	static FAVBOITRasterExecutionProbe GManualRasterProbe;
	static bool bManualRasterProbeInstalled = false;
#endif

	static FString EscapeJson(const FString& Value)
	{
		return Value.ReplaceCharWithEscapedChar();
	}

	static FString FindArgValue(const TArray<FString>& Args, const TCHAR* Prefix)
	{
		for (const FString& Arg : Args)
		{
			if (Arg.StartsWith(Prefix, ESearchCase::IgnoreCase))
			{
				return Arg.RightChop(FCString::Strlen(Prefix));
			}
		}
		return FString();
	}

	static FString FirstPositionalArg(const TArray<FString>& Args)
	{
		for (const FString& Arg : Args)
		{
			if (!Arg.Contains(TEXT("=")))
			{
				return Arg;
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
		else
		{
			UE_LOG(LogAVBOITFoundationManualRepro, Warning, TEXT("Missing CVar: %s"), Name);
		}
	}

	static int32 GetIntCVar(const TCHAR* Name, int32 DefaultValue = 0)
	{
		if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(Name))
		{
			return CVar->GetInt();
		}
		return DefaultValue;
	}

	static int32 ParseOrderValue(const FString& RawOrder)
	{
		const FString Order = RawOrder.IsEmpty() ? TEXT("AB") : RawOrder;
		if (Order.Equals(TEXT("BA"), ESearchCase::IgnoreCase)) return 1;
		if (Order.Equals(TEXT("ABC"), ESearchCase::IgnoreCase)) return 2;
		if (Order.Equals(TEXT("CBA"), ESearchCase::IgnoreCase)) return 3;
		if (Order.Equals(TEXT("RandomSeed1"), ESearchCase::IgnoreCase)) return 4;
		if (Order.Equals(TEXT("RandomSeed2"), ESearchCase::IgnoreCase)) return 5;
		if (Order.Equals(TEXT("RandomSeed3"), ESearchCase::IgnoreCase)) return 6;
		if (Order.IsNumeric()) return FCString::Atoi(*Order);
		return 0;
	}

	static FString OrderToString(int32 Order)
	{
		switch (Order)
		{
		case 1: return TEXT("BA");
		case 2: return TEXT("ABC");
		case 3: return TEXT("CBA");
		case 4: return TEXT("RandomSeed1");
		case 5: return TEXT("RandomSeed2");
		case 6: return TEXT("RandomSeed3");
		case 0:
		default: return TEXT("AB");
		}
	}

	static int32 ParseModeValue(const FString& RawMode)
	{
		const FString Mode = RawMode.IsEmpty() ? TEXT("PluginAVBOIT") : RawMode;
		if (Mode.Equals(TEXT("EngineDefault"), ESearchCase::IgnoreCase)) return 0;
		if (Mode.Equals(TEXT("UESortedPixelsOIT"), ESearchCase::IgnoreCase)) return 1;
		if (Mode.Equals(TEXT("PluginIdentity"), ESearchCase::IgnoreCase)) return 2;
		if (Mode.Equals(TEXT("PluginAVBOIT"), ESearchCase::IgnoreCase)) return 3;
		if (Mode.Equals(TEXT("ExactReference"), ESearchCase::IgnoreCase)) return 4;
		if (Mode.Equals(TEXT("BufferOverview"), ESearchCase::IgnoreCase)) return 5;
		if (Mode.IsNumeric()) return FCString::Atoi(*Mode);
		return 3;
	}

	static FString ModeToString(int32 Mode)
	{
		switch (Mode)
		{
		case 0: return TEXT("EngineDefault");
		case 1: return TEXT("UESortedPixelsOIT");
		case 2: return TEXT("PluginIdentity");
		case 4: return TEXT("ExactReference");
		case 5: return TEXT("BufferOverview");
		case 3:
		default: return TEXT("PluginAVBOIT");
		}
	}

	static FString WorldTypeToString(EWorldType::Type WorldType)
	{
		switch (WorldType)
		{
		case EWorldType::Editor: return TEXT("Editor");
		case EWorldType::PIE: return TEXT("PIE");
		case EWorldType::Game: return TEXT("Game");
		case EWorldType::EditorPreview: return TEXT("EditorPreview");
		case EWorldType::GamePreview: return TEXT("GamePreview");
		case EWorldType::GameRPC: return TEXT("GameRPC");
		case EWorldType::Inactive: return TEXT("Inactive");
		case EWorldType::None:
		default: return TEXT("None");
		}
	}

	static uint32 SubmissionOrderForPhysicalIndex(int32 RequestedOrder, int32 PhysicalIndex)
	{
		switch (RequestedOrder)
		{
		case 1:
		case 4:
		case 6:
			return PhysicalIndex == 0 ? 1u : 0u;
		case 0:
		case 5:
		default:
			return static_cast<uint32>(PhysicalIndex);
		}
	}

	static UWorld* ResolveActiveWorld()
	{
		if (GEngine)
		{
			for (const FWorldContext& Context : GEngine->GetWorldContexts())
			{
				UWorld* World = Context.World();
				if (World && (World->WorldType == EWorldType::PIE || World->WorldType == EWorldType::Game))
				{
					return World;
				}
			}
			for (const FWorldContext& Context : GEngine->GetWorldContexts())
			{
				UWorld* World = Context.World();
				if (World && World->WorldType == EWorldType::Editor)
				{
					return World;
				}
			}
		}
#if WITH_EDITOR
		if (GEditor)
		{
			return GEditor->GetEditorWorldContext().World();
		}
#endif
		return GWorld;
	}

	static FString ResolveGitHead()
	{
		int32 ReturnCode = -1;
		FString StdOut;
		FString StdErr;
		const FString Params = FString::Printf(TEXT("-C \"%s\" rev-parse HEAD"), *FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()));
		if (FPlatformProcess::ExecProcess(TEXT("git"), *Params, &ReturnCode, &StdOut, &StdErr) && ReturnCode == 0)
		{
			StdOut.TrimStartAndEndInline();
			return StdOut;
		}
		return TEXT("unknown-runtime");
	}

	static int32 CleanupManualActors(UWorld* World)
	{
		if (!World)
		{
			return 0;
		}

		TArray<AActor*> ActorsToDestroy;
		for (TActorIterator<AActor> It(World); It; ++It)
		{
			if (It->ActorHasTag(AVBOIT_FOUNDATION_MANUAL_TAG))
			{
				ActorsToDestroy.Add(*It);
			}
		}

		for (AActor* Actor : ActorsToDestroy)
		{
			World->DestroyActor(Actor);
		}
		return ActorsToDestroy.Num();
	}

	static void InstallManualRasterProbe()
	{
#if WITH_EDITOR || WITH_DEV_AUTOMATION_TESTS
		if (!GAVBOITRasterProbe || bManualRasterProbeInstalled)
		{
			GManualRasterProbe = FAVBOITRasterExecutionProbe();
			GAVBOITRasterProbe = &GManualRasterProbe;
			bManualRasterProbeInstalled = true;
		}
#endif
	}

	static void ApplyFoundationCVars(int32 Mode, int32 Order, int32 DownsampleFactor, int32 NumSlices)
	{
		SetIntCVar(TEXT("r.AVBOIT.Smoke.Enable"), 0);
		SetIntCVar(TEXT("r.AVBOIT.Test.Enable"), 0);
		SetIntCVar(TEXT("r.AVBOIT.Raster.Enable"), 0);
		SetIntCVar(TEXT("r.AVBOIT.Foundation.Enable"), 1);
		SetIntCVar(TEXT("r.AVBOIT.Foundation.Scene"), 1);
		SetIntCVar(TEXT("r.AVBOIT.Foundation.Mode"), Mode);
		SetIntCVar(TEXT("r.AVBOIT.Foundation.SubmissionOrder"), Order);
		SetIntCVar(TEXT("r.AVBOIT.Foundation.DownsampleFactor"), FMath::Max(DownsampleFactor, 1));
		SetIntCVar(TEXT("r.AVBOIT.Foundation.NumSlices"), FMath::Clamp(NumSlices, 1, 64));
	}

	static UAVBOITTestMeshComponent* CreateManualQuad(
		AActor* Owner,
		USceneComponent* Root,
		const FName ComponentName,
		const FLinearColor& Color,
		float Alpha,
		int32 SubmissionOrder,
		float LeftDepthCm,
		float RightDepthCm)
	{
		UAVBOITTestMeshComponent* Component = NewObject<UAVBOITTestMeshComponent>(Owner, ComponentName, RF_Transient);
		Component->SetupAttachment(Root);
		Component->SetMobility(EComponentMobility::Movable);
		Component->SetVisibility(true);
		Component->SetHiddenInGame(false);
		Component->MaterialParams.Color = Color;
		Component->MaterialParams.Alpha = Alpha;
		Component->bIsTransparent = true;
		Component->SubmissionOrder = SubmissionOrder;
		Component->bUseCustomLocalVertices = true;
		Component->CustomLocalVertices[0] = FVector3f(LeftDepthCm, -600.0f, 100.0f);
		Component->CustomLocalVertices[1] = FVector3f(RightDepthCm, 600.0f, 100.0f);
		Component->CustomLocalVertices[2] = FVector3f(RightDepthCm, 600.0f, 1200.0f);
		Component->CustomLocalVertices[3] = FVector3f(LeftDepthCm, -600.0f, 1200.0f);
		Component->SetWorldTransform(FTransform::Identity);
		Owner->AddInstanceComponent(Component);
		Component->RegisterComponent();
		return Component;
	}

	static void UpdateExistingManualOrder(UWorld* World, int32 Order)
	{
		if (!World)
		{
			return;
		}

		for (TActorIterator<AActor> It(World); It; ++It)
		{
			AActor* Actor = *It;
			if (!Actor->ActorHasTag(AVBOIT_FOUNDATION_MANUAL_TAG))
			{
				continue;
			}

			TArray<UAVBOITTestMeshComponent*> Components;
			Actor->GetComponents(Components);
			for (UAVBOITTestMeshComponent* Component : Components)
			{
				if (!Component)
				{
					continue;
				}
				const bool bIsA = Component->GetName().StartsWith(TEXT("A_Green"));
				const int32 PhysicalIndex = bIsA ? 0 : 1;
				Component->SubmissionOrder = static_cast<int32>(SubmissionOrderForPhysicalIndex(Order, PhysicalIndex));
				Component->MarkRenderStateDirty();
			}
		}
	}

	static int32 CountManualComponents(UWorld* World)
	{
		int32 Count = 0;
		if (!World)
		{
			return Count;
		}
		for (TActorIterator<AActor> It(World); It; ++It)
		{
			if (!It->ActorHasTag(AVBOIT_FOUNDATION_MANUAL_TAG))
			{
				continue;
			}
			TArray<UAVBOITTestMeshComponent*> Components;
			It->GetComponents(Components);
			Count += Components.Num();
		}
		return Count;
	}

	static int32 CountManualActors(UWorld* World)
	{
		int32 Count = 0;
		if (!World)
		{
			return Count;
		}
		for (TActorIterator<AActor> It(World); It; ++It)
		{
			if (It->ActorHasTag(AVBOIT_FOUNDATION_MANUAL_TAG))
			{
				++Count;
			}
		}
		return Count;
	}

	static FString SkipReasonToString()
	{
#if WITH_EDITOR || WITH_DEV_AUTOMATION_TESTS
		if (!GAVBOITRasterProbe)
		{
			return TEXT("NoProbe");
		}
		switch (GAVBOITRasterProbe->SkipReason)
		{
		case EAVBOITRasterSkipReason::NoProxies: return TEXT("NoProxies");
		case EAVBOITRasterSkipReason::InvalidSceneTextures: return TEXT("InvalidSceneTextures");
		case EAVBOITRasterSkipReason::ViewRejected: return TEXT("ViewRejected");
		case EAVBOITRasterSkipReason::InvalidViewRect: return TEXT("InvalidViewRect");
		case EAVBOITRasterSkipReason::Executed: return TEXT("Executed");
		case EAVBOITRasterSkipReason::Disabled:
		default: return TEXT("Disabled");
		}
#else
		return TEXT("ProbeUnavailable");
#endif
	}

	static void SetViewportToManualCamera(UWorld* World, ACameraActor* CameraActor)
	{
		if (!World || !CameraActor)
		{
			return;
		}

		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			PlayerController->SetViewTarget(CameraActor);
		}

#if WITH_EDITOR
		if (GEditor)
		{
			for (FLevelEditorViewportClient* ViewportClient : GEditor->GetLevelViewportClients())
			{
				if (!ViewportClient || !ViewportClient->IsPerspective())
				{
					continue;
				}
				ViewportClient->SetViewLocation(CameraActor->GetActorLocation());
				ViewportClient->SetViewRotation(CameraActor->GetActorRotation());
				ViewportClient->Invalidate();
			}
			GEditor->RedrawLevelEditingViewports(true);
		}
#endif
	}

	static void WriteManualReproManifest(
		UWorld* World,
		const FString& CommandText,
		const FString& EvidenceRoot,
		int32 Mode,
		int32 Order,
		int32 DownsampleFactor,
		int32 NumSlices,
		int32 SpawnedActorCount,
		int32 SpawnedComponentCount)
	{
		const FString Timestamp = FDateTime::UtcNow().ToString(TEXT("%Y%m%dT%H%M%SZ"));
		const FString StageRoot = EvidenceRoot.IsEmpty()
			? FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / TEXT("LocalVisualResults/TempResults/UE57/HIVE_4090x2/UE4-2G3-ManualTransparentSorting") / Timestamp / TEXT("ManualRepro"))
			: FPaths::ConvertRelativePathToFull(EvidenceRoot / TEXT("ManualRepro"));
		IFileManager::Get().MakeDirectory(*StageRoot, true);
		GLastManualReproManifestPath = StageRoot / TEXT("ManualReproManifest.json");

		const FString MapName = World ? World->GetMapName() : TEXT("unknown");
		const FString WorldType = World ? WorldTypeToString(World->WorldType) : TEXT("unknown");
		const FString Json = FString::Printf(
			TEXT("{\n")
			TEXT("  \"SchemaVersion\": 1,\n")
			TEXT("  \"Stage\": \"UE-4.2G.3 Manual Transparent Sorting Viewport Repro\",\n")
			TEXT("  \"GeneratedUtc\": \"%s\",\n")
			TEXT("  \"ProjectDir\": \"%s\",\n")
			TEXT("  \"ProjectName\": \"%s\",\n")
			TEXT("  \"MapName\": \"%s\",\n")
			TEXT("  \"WorldType\": \"%s\",\n")
			TEXT("  \"Command\": \"%s\",\n")
			TEXT("  \"Scene\": \"TwoIntersectingQuads\",\n")
			TEXT("  \"Mode\": \"%s\",\n")
			TEXT("  \"ModeValue\": %d,\n")
			TEXT("  \"SubmissionOrder\": \"%s\",\n")
			TEXT("  \"SubmissionOrderValue\": %d,\n")
			TEXT("  \"SpawnedActorCount\": %d,\n")
			TEXT("  \"SpawnedComponentCount\": %d,\n")
			TEXT("  \"ExpectedVisual\": \"Left side A/Green front, right side B/Cyan front, center crossing line visible.\",\n")
			TEXT("  \"CVars\": [\n")
			TEXT("    \"r.AVBOIT.Foundation.Enable 1\",\n")
			TEXT("    \"r.AVBOIT.Foundation.Scene 1\",\n")
			TEXT("    \"r.AVBOIT.Foundation.Mode %d\",\n")
			TEXT("    \"r.AVBOIT.Foundation.SubmissionOrder %d\",\n")
			TEXT("    \"r.AVBOIT.Foundation.DownsampleFactor %d\",\n")
			TEXT("    \"r.AVBOIT.Foundation.NumSlices %d\"\n")
			TEXT("  ],\n")
			TEXT("  \"Camera\": { \"Location\": [0.0, 0.0, 650.0], \"Rotation\": [0.0, 0.0, 0.0], \"FOV\": 65.0 },\n")
		TEXT("  \"Quads\": [\n")
			TEXT("    { \"Name\": \"A_Green\", \"Color\": [0.0, 1.0, 0.0, 1.0], \"Alpha\": 0.55, \"LeftDepthCm\": 700.0, \"RightDepthCm\": 1300.0 },\n")
			TEXT("    { \"Name\": \"B_Cyan\", \"Color\": [0.0, 0.75, 1.0, 1.0], \"Alpha\": 0.55, \"LeftDepthCm\": 1300.0, \"RightDepthCm\": 700.0 }\n")
			TEXT("  ],\n")
			TEXT("  \"DesktopScreenshotUsed\": false,\n")
			TEXT("  \"ScreenshotSource\": \"none-spawn-manifest-only\",\n")
			TEXT("  \"RHI\": \"%s\",\n")
			TEXT("  \"GPU\": \"%s\",\n")
			TEXT("  \"Commit\": \"%s\",\n")
			TEXT("  \"Status\": \"manual-spawned-not-promoted\"\n")
			TEXT("}\n"),
			*EscapeJson(FDateTime::UtcNow().ToIso8601()),
			*EscapeJson(FPaths::ConvertRelativePathToFull(FPaths::ProjectDir())),
			FApp::GetProjectName(),
			*EscapeJson(MapName),
			*EscapeJson(WorldType),
			*EscapeJson(CommandText),
			*EscapeJson(ModeToString(Mode)),
			Mode,
			*EscapeJson(OrderToString(Order)),
			Order,
			SpawnedActorCount,
			SpawnedComponentCount,
			Mode,
			Order,
			DownsampleFactor,
			NumSlices,
			*EscapeJson(GDynamicRHI ? GDynamicRHI->GetName() : TEXT("unknown")),
			TEXT("unknown-runtime"),
			*EscapeJson(ResolveGitHead()));

		FFileHelper::SaveStringToFile(Json, *GLastManualReproManifestPath);
		FFileHelper::SaveStringToFile(GLastManualReproManifestPath, *(FPaths::GetPath(StageRoot) / TEXT("LATEST_MANUAL_REPRO.txt")));
	}

	static void LogStatus(UWorld* World)
	{
		const int32 Mode = GetIntCVar(TEXT("r.AVBOIT.Foundation.Mode"), 3);
		const int32 Order = GetIntCVar(TEXT("r.AVBOIT.Foundation.SubmissionOrder"), 0);
		const int32 ManualActors = CountManualActors(World);
		const int32 ManualComponents = CountManualComponents(World);

#if WITH_EDITOR || WITH_DEV_AUTOMATION_TESTS
		const FString DrawOrder = GAVBOITRasterProbe ? GAVBOITRasterProbe->ActualDrawOrder : FString();
		const int32 RegistryProxyCount = GAVBOITRasterProbe ? GAVBOITRasterProbe->RegistryProxyCount : -1;
		const int32 AcceptedProxyCount = GAVBOITRasterProbe ? GAVBOITRasterProbe->AcceptedProxyCount : -1;
		const FIntRect ViewRect = GAVBOITRasterProbe ? GAVBOITRasterProbe->AcceptedViewRect : FIntRect();
#else
		const FString DrawOrder;
		const int32 RegistryProxyCount = -1;
		const int32 AcceptedProxyCount = -1;
		const FIntRect ViewRect;
#endif

		UE_LOG(LogAVBOITFoundationManualRepro, Display,
			TEXT("AVBOIT.Foundation.Status Enable=%d Mode=%s(%d) Scene=%d SubmissionOrder=%s(%d) ManualActors=%d ManualComponents=%d RegistryProxyCount=%d AcceptedProxyCount=%d SkipReason=%s ActualDrawOrder=%s ViewRect=(%d,%d)-(%d,%d) Manifest=%s"),
			GetIntCVar(TEXT("r.AVBOIT.Foundation.Enable"), 0),
			*ModeToString(Mode),
			Mode,
			GetIntCVar(TEXT("r.AVBOIT.Foundation.Scene"), 1),
			*OrderToString(Order),
			Order,
			ManualActors,
			ManualComponents,
			RegistryProxyCount,
			AcceptedProxyCount,
			*SkipReasonToString(),
			*DrawOrder,
			ViewRect.Min.X,
			ViewRect.Min.Y,
			ViewRect.Max.X,
			ViewRect.Max.Y,
			*GLastManualReproManifestPath);
	}

	static bool WriteManualValidationResult(const FString& Root, const FString& ScreenshotPath, bool bPassed, const FString& Reason)
	{
		const FString MetricsDir = FPaths::ConvertRelativePathToFull(Root / TEXT("Metrics"));
		IFileManager::Get().MakeDirectory(*MetricsDir, true);
		const FString Path = MetricsDir / TEXT("ManualViewportValidation.json");

		UWorld* World = ResolveActiveWorld();
		const int32 ManualActors = CountManualActors(World);
		const int32 ManualComponents = CountManualComponents(World);
#if WITH_EDITOR || WITH_DEV_AUTOMATION_TESTS
		const int32 RegistryProxyCount = GAVBOITRasterProbe ? GAVBOITRasterProbe->RegistryProxyCount : -1;
		const int32 AcceptedProxyCount = GAVBOITRasterProbe ? GAVBOITRasterProbe->AcceptedProxyCount : -1;
		const int32 AcceptedViewCount = GAVBOITRasterProbe ? GAVBOITRasterProbe->AcceptedViewCount : -1;
		const int32 ForwardDrawCount = GAVBOITRasterProbe ? GAVBOITRasterProbe->ForwardDrawCount : -1;
		const FString ActualDrawOrder = GAVBOITRasterProbe ? GAVBOITRasterProbe->ActualDrawOrder : FString();
		const bool bClear = GAVBOITRasterProbe ? GAVBOITRasterProbe->bClearPassScheduled : false;
		const bool bSplat = GAVBOITRasterProbe ? GAVBOITRasterProbe->bSplatPassScheduled : false;
		const bool bIntegrate = GAVBOITRasterProbe ? GAVBOITRasterProbe->bIntegratePassScheduled : false;
		const bool bForward = GAVBOITRasterProbe ? GAVBOITRasterProbe->bForwardPassScheduled : false;
		const bool bComposite = GAVBOITRasterProbe ? GAVBOITRasterProbe->bCompositePassScheduled : false;
#else
		const int32 RegistryProxyCount = -1;
		const int32 AcceptedProxyCount = -1;
		const int32 AcceptedViewCount = -1;
		const int32 ForwardDrawCount = -1;
		const FString ActualDrawOrder;
		const bool bClear = false;
		const bool bSplat = false;
		const bool bIntegrate = false;
		const bool bForward = false;
		const bool bComposite = false;
#endif

		const FString Json = FString::Printf(
			TEXT("{\n")
			TEXT("  \"SchemaVersion\": 1,\n")
			TEXT("  \"GeneratedUtc\": \"%s\",\n")
			TEXT("  \"Passed\": %s,\n")
			TEXT("  \"Reason\": \"%s\",\n")
			TEXT("  \"ManualActors\": %d,\n")
			TEXT("  \"ManualComponents\": %d,\n")
			TEXT("  \"RegistryProxyCount\": %d,\n")
			TEXT("  \"AcceptedProxyCount\": %d,\n")
			TEXT("  \"AcceptedViewCount\": %d,\n")
			TEXT("  \"SkipReason\": \"%s\",\n")
			TEXT("  \"ForwardDrawCount\": %d,\n")
			TEXT("  \"ActualDrawOrder\": \"%s\",\n")
			TEXT("  \"Passes\": { \"Clear\": %s, \"Splat\": %s, \"Integrate\": %s, \"ForwardUnlit\": %s, \"Composite\": %s },\n")
			TEXT("  \"Manifest\": \"%s\",\n")
			TEXT("  \"Screenshot\": \"%s\"\n")
			TEXT("}\n"),
			*EscapeJson(FDateTime::UtcNow().ToIso8601()),
			bPassed ? TEXT("true") : TEXT("false"),
			*EscapeJson(Reason),
			ManualActors,
			ManualComponents,
			RegistryProxyCount,
			AcceptedProxyCount,
			AcceptedViewCount,
			*EscapeJson(SkipReasonToString()),
			ForwardDrawCount,
			*EscapeJson(ActualDrawOrder),
			bClear ? TEXT("true") : TEXT("false"),
			bSplat ? TEXT("true") : TEXT("false"),
			bIntegrate ? TEXT("true") : TEXT("false"),
			bForward ? TEXT("true") : TEXT("false"),
			bComposite ? TEXT("true") : TEXT("false"),
			*EscapeJson(GLastManualReproManifestPath),
			*EscapeJson(ScreenshotPath));

		return FFileHelper::SaveStringToFile(Json, *Path);
	}

	static void WriteManualDebugPayload(const FString& Root)
	{
#if WITH_EDITOR || WITH_DEV_AUTOMATION_TESTS
		if (!GAVBOITRasterProbe || !GAVBOITRasterProbe->bReadbackReady || !GAVBOITRasterProbe->ReadbackPayload)
		{
			return;
		}

		const FAVBOITRasterDebugPayload& Payload = *GAVBOITRasterProbe->ReadbackPayload;
		FString ExtinctionJson;
		FString TransmittanceJson;
		const uint32 ActiveSlices = FMath::Clamp(Payload.Header.NumSlices, 1u, 64u);
		for (uint32 Index = 0; Index < ActiveSlices; ++Index)
		{
			if (Index > 0)
			{
				ExtinctionJson += TEXT(",");
				TransmittanceJson += TEXT(",");
			}
			ExtinctionJson += FString::Printf(TEXT("%u"), Payload.PackedExtinction[Index]);
			TransmittanceJson += FString::Printf(TEXT("%.9g"), Payload.Transmittance[Index]);
		}

		const FString MetricsDir = FPaths::ConvertRelativePathToFull(Root / TEXT("Metrics"));
		IFileManager::Get().MakeDirectory(*MetricsDir, true);
		const FString Path = MetricsDir / TEXT("ManualDebugPayload.json");
		const FString Json = FString::Printf(
			TEXT("{\n")
			TEXT("  \"SchemaVersion\": 1,\n")
			TEXT("  \"GeneratedUtc\": \"%s\",\n")
			TEXT("  \"DebugPixel\": [%d, %d],\n")
			TEXT("  \"LinearViewDepth\": %.9g,\n")
			TEXT("  \"NormalizedDepth\": %.9g,\n")
			TEXT("  \"Slice\": %u,\n")
			TEXT("  \"DebugPixelHitCount\": %u,\n")
			TEXT("  \"LastLinearViewDepth\": %.9g,\n")
			TEXT("  \"LastNormalizedDepth\": %.9g,\n")
			TEXT("  \"LastSlice\": %u,\n")
			TEXT("  \"FragmentCoverageCount\": %u,\n")
			TEXT("  \"NumSlices\": %u,\n")
			TEXT("  \"OverflowCount\": %u,\n")
			TEXT("  \"OutOfBoundsCount\": %u,\n")
			TEXT("  \"TextureExtent\": [%d, %d],\n")
			TEXT("  \"VolumeExtent\": [%d, %d],\n")
			TEXT("  \"ViewRectMin\": [%d, %d],\n")
			TEXT("  \"ViewRectMax\": [%d, %d],\n")
			TEXT("  \"ColorAccumulation\": [%.9g, %.9g, %.9g, %.9g],\n")
			TEXT("  \"SceneColorBefore\": [%.9g, %.9g, %.9g, %.9g],\n")
			TEXT("  \"SceneColorAfter\": [%.9g, %.9g, %.9g, %.9g],\n")
			TEXT("  \"PackedExtinction\": [%s],\n")
			TEXT("  \"Transmittance\": [%s]\n")
			TEXT("}\n"),
			*EscapeJson(FDateTime::UtcNow().ToIso8601()),
			GAVBOITRasterProbe->RequestedDebugPixel.X,
			GAVBOITRasterProbe->RequestedDebugPixel.Y,
			Payload.Header.LinearViewDepth,
			Payload.Header.NormalizedDepth,
			Payload.Header.Slice,
			Payload.Header.DebugPixelHitCount,
			Payload.Header.LastLinearViewDepth,
			Payload.Header.LastNormalizedDepth,
			Payload.Header.LastSlice,
			Payload.Header.FragmentCoverageCount,
			Payload.Header.NumSlices,
			Payload.Header.OverflowCount,
			Payload.Header.OutOfBoundsCount,
			Payload.Header.TextureExtent.X,
			Payload.Header.TextureExtent.Y,
			Payload.Header.VolumeExtent.X,
			Payload.Header.VolumeExtent.Y,
			Payload.Header.ViewRectMin.X,
			Payload.Header.ViewRectMin.Y,
			Payload.Header.ViewRectMax.X,
			Payload.Header.ViewRectMax.Y,
			Payload.Header.ColorAccumulation.X,
			Payload.Header.ColorAccumulation.Y,
			Payload.Header.ColorAccumulation.Z,
			Payload.Header.ColorAccumulation.W,
			Payload.Header.SceneColorBefore.X,
			Payload.Header.SceneColorBefore.Y,
			Payload.Header.SceneColorBefore.Z,
			Payload.Header.SceneColorBefore.W,
			Payload.Header.SceneColorAfter.X,
			Payload.Header.SceneColorAfter.Y,
			Payload.Header.SceneColorAfter.Z,
			Payload.Header.SceneColorAfter.W,
			*ExtinctionJson,
			*TransmittanceJson);

		FFileHelper::SaveStringToFile(Json, *Path);
#endif
	}

	static bool IsManualViewportExecuted()
	{
#if WITH_EDITOR || WITH_DEV_AUTOMATION_TESTS
		return GAVBOITRasterProbe
			&& GAVBOITRasterProbe->SkipReason == EAVBOITRasterSkipReason::Executed
			&& GAVBOITRasterProbe->RegistryProxyCount >= 2
			&& GAVBOITRasterProbe->AcceptedProxyCount >= 2
			&& GAVBOITRasterProbe->ForwardDrawCount >= 2
			&& !GAVBOITRasterProbe->ActualDrawOrder.IsEmpty();
#else
		return false;
#endif
	}

	static bool TickManualValidation(float DeltaTime)
	{
		const double Elapsed = FPlatformTime::Seconds() - GManualValidationStartTime;
		const bool bExecuted = IsManualViewportExecuted();
		const bool bDebugReady =
#if WITH_EDITOR || WITH_DEV_AUTOMATION_TESTS
			!bManualValidationRequireDebugReadback || (GAVBOITRasterProbe && GAVBOITRasterProbe->bReadbackReady);
#else
			!bManualValidationRequireDebugReadback;
#endif
		if ((!bExecuted || !bDebugReady) && Elapsed < 30.0)
		{
			return true;
		}

		FTSTicker::GetCoreTicker().RemoveTicker(GManualValidationTickerHandle);
		GManualValidationTickerHandle.Reset();

		const bool bPassed = bExecuted;
		const FString Reason = bPassed
			? (bDebugReady
				? TEXT("Manual transparent sorting scene reached Foundation render execution and debug readback.")
				: TEXT("Manual transparent sorting scene reached Foundation render execution; debug readback timed out."))
			: FString::Printf(TEXT("Timed out waiting for Foundation render execution/readback. Last SkipReason=%s Executed=%s DebugReady=%s."),
				*SkipReasonToString(),
				bExecuted ? TEXT("true") : TEXT("false"),
				bDebugReady ? TEXT("true") : TEXT("false"));
		WriteManualValidationResult(GManualValidationRoot, GManualValidationScreenshotPath, bPassed, Reason);
		WriteManualDebugPayload(GManualValidationRoot);
		LogStatus(ResolveActiveWorld());

		UE_LOG(LogAVBOITFoundationManualRepro, Display, TEXT("Manual viewport validation %s: %s"), bPassed ? TEXT("PASSED") : TEXT("FAILED"), *Reason);
		if (bPassed && !GManualValidationScreenshotPath.IsEmpty())
		{
			FAVBOITScreenshotAutomation::CaptureScreenshotAndExit(GManualValidationScreenshotPath);
		}
		else if (bManualValidationExitAfter)
		{
			FPlatformMisc::RequestExitWithStatus(false, bPassed ? 0 : 7, TEXT("AVBOITManualValidation"));
		}

		return false;
	}
}

void FAVBOITFoundationManualReproCommands::RegisterCommands()
{
	SpawnTransparentSortingSceneCommand = IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("AVBOIT.Foundation.SpawnTransparentSortingScene"),
		TEXT("Spawns the transient Foundation TwoIntersectingQuads viewport scene. Usage: order=AB mode=PluginAVBOIT"),
		FConsoleCommandWithArgsDelegate::CreateStatic(&FAVBOITFoundationManualReproCommands::SpawnTransparentSortingScene),
		ECVF_Default);

	SetOrderCommand = IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("AVBOIT.Foundation.SetOrder"),
		TEXT("Sets Foundation manual scene order. Usage: AVBOIT.Foundation.SetOrder AB|BA|RandomSeed1|RandomSeed2|RandomSeed3"),
		FConsoleCommandWithArgsDelegate::CreateStatic(&FAVBOITFoundationManualReproCommands::SetOrder),
		ECVF_Default);

	SetModeCommand = IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("AVBOIT.Foundation.SetMode"),
		TEXT("Sets Foundation viewport mode. Usage: AVBOIT.Foundation.SetMode PluginIdentity|PluginAVBOIT|ExactReference|BufferOverview"),
		FConsoleCommandWithArgsDelegate::CreateStatic(&FAVBOITFoundationManualReproCommands::SetMode),
		ECVF_Default);

	StatusCommand = IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("AVBOIT.Foundation.Status"),
		TEXT("Prints Foundation manual repro CVar/proxy/render status."),
		FConsoleCommandWithArgsDelegate::CreateStatic(&FAVBOITFoundationManualReproCommands::Status),
		ECVF_Default);

	CleanupTransparentSortingSceneCommand = IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("AVBOIT.Foundation.CleanupTransparentSortingScene"),
		TEXT("Destroys transient Foundation manual transparent sorting actors."),
		FConsoleCommandWithArgsDelegate::CreateStatic(&FAVBOITFoundationManualReproCommands::CleanupTransparentSortingScene),
		ECVF_Default);

	ValidateTransparentSortingSceneAndExitCommand = IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("AVBOIT.Foundation.ValidateTransparentSortingSceneAndExit"),
		TEXT("Spawns the manual Foundation transparent sorting scene, waits for render execution, writes metrics, optionally screenshots, and exits. Usage: root=<path> screenshot=<png> order=AB mode=PluginAVBOIT"),
		FConsoleCommandWithArgsDelegate::CreateStatic(&FAVBOITFoundationManualReproCommands::ValidateTransparentSortingSceneAndExit),
		ECVF_Default);
}

void FAVBOITFoundationManualReproCommands::UnregisterCommands()
{
	if (SpawnTransparentSortingSceneCommand) IConsoleManager::Get().UnregisterConsoleObject(SpawnTransparentSortingSceneCommand);
	if (SetOrderCommand) IConsoleManager::Get().UnregisterConsoleObject(SetOrderCommand);
	if (SetModeCommand) IConsoleManager::Get().UnregisterConsoleObject(SetModeCommand);
	if (StatusCommand) IConsoleManager::Get().UnregisterConsoleObject(StatusCommand);
	if (CleanupTransparentSortingSceneCommand) IConsoleManager::Get().UnregisterConsoleObject(CleanupTransparentSortingSceneCommand);
	if (ValidateTransparentSortingSceneAndExitCommand) IConsoleManager::Get().UnregisterConsoleObject(ValidateTransparentSortingSceneAndExitCommand);

	SpawnTransparentSortingSceneCommand = nullptr;
	SetOrderCommand = nullptr;
	SetModeCommand = nullptr;
	StatusCommand = nullptr;
	CleanupTransparentSortingSceneCommand = nullptr;
	ValidateTransparentSortingSceneAndExitCommand = nullptr;
}

void FAVBOITFoundationManualReproCommands::SpawnTransparentSortingScene(const TArray<FString>& Args)
{
	UWorld* World = ResolveActiveWorld();
	if (!World)
	{
		UE_LOG(LogAVBOITFoundationManualRepro, Error, TEXT("Cannot spawn Foundation transparent sorting scene: no active World."));
		return;
	}

	const FString OrderArg = !FindArgValue(Args, TEXT("order=")).IsEmpty() ? FindArgValue(Args, TEXT("order=")) : TEXT("AB");
	const FString ModeArg = !FindArgValue(Args, TEXT("mode=")).IsEmpty() ? FindArgValue(Args, TEXT("mode=")) : TEXT("PluginAVBOIT");
	const FString EvidenceRoot = FindArgValue(Args, TEXT("root="));
	const FString DownsampleArg = FindArgValue(Args, TEXT("downsample="));
	const FString NumSlicesArg = FindArgValue(Args, TEXT("numslices="));
	const int32 Order = ParseOrderValue(OrderArg);
	const int32 Mode = ParseModeValue(ModeArg);
	const int32 DownsampleFactor = DownsampleArg.IsEmpty() ? 8 : FMath::Max(FCString::Atoi(*DownsampleArg), 1);
	const int32 NumSlices = NumSlicesArg.IsEmpty() ? 64 : FMath::Clamp(FCString::Atoi(*NumSlicesArg), 1, 64);

	InstallManualRasterProbe();
	ApplyFoundationCVars(Mode, Order, DownsampleFactor, NumSlices);
	const int32 DestroyedActors = CleanupManualActors(World);

	FActorSpawnParameters SpawnParams;
	SpawnParams.ObjectFlags |= RF_Transient;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* SceneActor = World->SpawnActor<AActor>(FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (!SceneActor)
	{
		UE_LOG(LogAVBOITFoundationManualRepro, Error, TEXT("Failed to spawn Foundation manual scene actor."));
		return;
	}
	SceneActor->Tags.Add(AVBOIT_FOUNDATION_MANUAL_TAG);
	SceneActor->SetFlags(RF_Transient);
#if WITH_EDITOR
	SceneActor->SetActorLabel(TEXT("AVBOIT_Foundation_TwoIntersectingQuads_Transient"));
#endif

	USceneComponent* Root = NewObject<USceneComponent>(SceneActor, TEXT("Root"), RF_Transient);
	Root->SetMobility(EComponentMobility::Movable);
	SceneActor->SetRootComponent(Root);
	SceneActor->AddInstanceComponent(Root);
	Root->RegisterComponent();

	CreateManualQuad(
		SceneActor,
		Root,
		TEXT("A_Green_FoundationManualQuad"),
		FLinearColor(0.0f, 1.0f, 0.0f, 1.0f),
		0.55f,
		static_cast<int32>(SubmissionOrderForPhysicalIndex(Order, 0)),
		700.0f,
		1300.0f);

	CreateManualQuad(
		SceneActor,
		Root,
		TEXT("B_Cyan_FoundationManualQuad"),
		FLinearColor(0.0f, 0.75f, 1.0f, 1.0f),
		0.55f,
		static_cast<int32>(SubmissionOrderForPhysicalIndex(Order, 1)),
		1300.0f,
		700.0f);

	ACameraActor* CameraActor = World->SpawnActor<ACameraActor>(FVector(0.0f, 0.0f, 650.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
	if (CameraActor)
	{
		CameraActor->Tags.Add(AVBOIT_FOUNDATION_MANUAL_TAG);
		CameraActor->SetFlags(RF_Transient);
		if (UCameraComponent* Camera = CameraActor->GetCameraComponent())
		{
			Camera->SetFieldOfView(65.0f);
			Camera->PostProcessSettings.bOverride_AutoExposureMethod = true;
			Camera->PostProcessSettings.AutoExposureMethod = AEM_Manual;
			Camera->PostProcessSettings.bOverride_CameraISO = true;
			Camera->PostProcessSettings.CameraISO = 100.0f;
			Camera->PostProcessSettings.bOverride_BloomIntensity = true;
			Camera->PostProcessSettings.BloomIntensity = 0.0f;
		}
#if WITH_EDITOR
		CameraActor->SetActorLabel(TEXT("AVBOIT_Foundation_ManualCamera_Transient"));
#endif
		SetViewportToManualCamera(World, CameraActor);
	}

	WriteManualReproManifest(
		World,
		FString::Printf(TEXT("AVBOIT.Foundation.SpawnTransparentSortingScene order=%s mode=%s downsample=%d numslices=%d root=%s"), *OrderArg, *ModeArg, DownsampleFactor, NumSlices, *EvidenceRoot),
		EvidenceRoot,
		Mode,
		Order,
		DownsampleFactor,
		NumSlices,
		CountManualActors(World),
		CountManualComponents(World));

	UE_LOG(LogAVBOITFoundationManualRepro, Display,
		TEXT("Spawned Foundation transparent sorting scene: mode=%s order=%s destroyed_previous=%d manual_components=%d manifest=%s"),
		*ModeToString(Mode),
		*OrderToString(Order),
		DestroyedActors,
		CountManualComponents(World),
		*GLastManualReproManifestPath);

	if (Mode == 4 || Mode == 5)
	{
		UE_LOG(LogAVBOITFoundationManualRepro, Warning,
			TEXT("Mode %s is recorded for manual workflow, but viewport rendering currently has PluginIdentity (2) and PluginAVBOIT (3) paths. Use the validation runner for ExactReference/BufferOverview evidence."),
			*ModeToString(Mode));
	}

	LogStatus(World);
}

void FAVBOITFoundationManualReproCommands::SetOrder(const TArray<FString>& Args)
{
	UWorld* World = ResolveActiveWorld();
	const FString RawOrder = !FindArgValue(Args, TEXT("order=")).IsEmpty() ? FindArgValue(Args, TEXT("order=")) : FirstPositionalArg(Args);
	const int32 Order = ParseOrderValue(RawOrder);
	SetIntCVar(TEXT("r.AVBOIT.Foundation.SubmissionOrder"), Order);
	UpdateExistingManualOrder(World, Order);
	UE_LOG(LogAVBOITFoundationManualRepro, Display, TEXT("Set Foundation manual submission order to %s(%d)."), *OrderToString(Order), Order);
#if WITH_EDITOR
	if (GEditor)
	{
		GEditor->RedrawLevelEditingViewports(true);
	}
#endif
	LogStatus(World);
}

void FAVBOITFoundationManualReproCommands::SetMode(const TArray<FString>& Args)
{
	UWorld* World = ResolveActiveWorld();
	const FString RawMode = !FindArgValue(Args, TEXT("mode=")).IsEmpty() ? FindArgValue(Args, TEXT("mode=")) : FirstPositionalArg(Args);
	const int32 Mode = ParseModeValue(RawMode);
	SetIntCVar(TEXT("r.AVBOIT.Foundation.Mode"), Mode);
	SetIntCVar(TEXT("r.AVBOIT.Foundation.Enable"), Mode == 0 ? 0 : 1);
	if (Mode == 4 || Mode == 5)
	{
		UE_LOG(LogAVBOITFoundationManualRepro, Warning,
			TEXT("Viewport mode %s(%d) is not a complete runtime replacement for the commandlet evidence path yet; it is recorded for diagnosis only."),
			*ModeToString(Mode),
			Mode);
	}
	UE_LOG(LogAVBOITFoundationManualRepro, Display, TEXT("Set Foundation manual mode to %s(%d)."), *ModeToString(Mode), Mode);
#if WITH_EDITOR
	if (GEditor)
	{
		GEditor->RedrawLevelEditingViewports(true);
	}
#endif
	LogStatus(World);
}

void FAVBOITFoundationManualReproCommands::Status(const TArray<FString>& Args)
{
	LogStatus(ResolveActiveWorld());
}

void FAVBOITFoundationManualReproCommands::CleanupTransparentSortingScene(const TArray<FString>& Args)
{
	UWorld* World = ResolveActiveWorld();
	const int32 DestroyedActors = CleanupManualActors(World);
	UE_LOG(LogAVBOITFoundationManualRepro, Display, TEXT("Cleaned up %d Foundation manual transparent sorting actors."), DestroyedActors);
#if WITH_EDITOR
	if (GEditor)
	{
		GEditor->RedrawLevelEditingViewports(true);
	}
#endif
	LogStatus(World);
}

void FAVBOITFoundationManualReproCommands::ValidateTransparentSortingSceneAndExit(const TArray<FString>& Args)
{
	FString Root = FindArgValue(Args, TEXT("root="));
	if (Root.IsEmpty())
	{
		const FString Timestamp = FDateTime::UtcNow().ToString(TEXT("%Y%m%dT%H%M%SZ"));
		Root = FPaths::ProjectDir() / TEXT("LocalVisualResults/TempResults/UE57/HIVE_4090x2/UE4-2G3-ManualTransparentSorting-Repro") / Timestamp;
	}

	const FString ScreenshotArg = FindArgValue(Args, TEXT("screenshot="));
	const FString ScreenshotPath = ScreenshotArg.IsEmpty()
		? FPaths::ConvertRelativePathToFull(Root / TEXT("Raw/Manual_PluginAVBOIT_AB.png"))
		: FPaths::ConvertRelativePathToFull(ScreenshotArg);
	IFileManager::Get().MakeDirectory(*FPaths::GetPath(ScreenshotPath), true);

	TArray<FString> SpawnArgs = Args;
	if (FindArgValue(SpawnArgs, TEXT("mode=")).IsEmpty())
	{
		const int32 CurrentMode = GetIntCVar(TEXT("r.AVBOIT.Foundation.Mode"), 3);
		SpawnArgs.Add(FString::Printf(TEXT("mode=%d"), CurrentMode == 0 ? 3 : CurrentMode));
	}
	if (FindArgValue(SpawnArgs, TEXT("order=")).IsEmpty())
	{
		SpawnArgs.Add(FString::Printf(TEXT("order=%d"), GetIntCVar(TEXT("r.AVBOIT.Foundation.SubmissionOrder"), 0)));
	}
	SpawnArgs.AddUnique(FString::Printf(TEXT("root=%s"), *Root));
	SpawnTransparentSortingScene(SpawnArgs);

	const FString DebugXArg = FindArgValue(Args, TEXT("debugx="));
	const FString DebugYArg = FindArgValue(Args, TEXT("debugy="));
	const int32 DebugX = DebugXArg.IsEmpty() ? GetIntCVar(TEXT("r.AVBOIT.Foundation.DebugPixelX"), 640) : FCString::Atoi(*DebugXArg);
	const int32 DebugY = DebugYArg.IsEmpty() ? GetIntCVar(TEXT("r.AVBOIT.Foundation.DebugPixelY"), 360) : FCString::Atoi(*DebugYArg);
#if WITH_EDITOR || WITH_DEV_AUTOMATION_TESTS
	if (GAVBOITRasterProbe)
	{
		GManualDebugPayload = FAVBOITRasterDebugPayload();
		GAVBOITRasterProbe->ReadbackPayload = &GManualDebugPayload;
		GAVBOITRasterProbe->RequestedDebugPixel = FIntPoint(DebugX, DebugY);
		GAVBOITRasterProbe->bReadbackReady = false;
	}
#endif

	GManualValidationRoot = FPaths::ConvertRelativePathToFull(Root);
	GManualValidationScreenshotPath = ScreenshotPath;
	GManualValidationStartTime = FPlatformTime::Seconds();
	bManualValidationExitAfter = true;
	bManualValidationRequireDebugReadback = GetIntCVar(TEXT("r.AVBOIT.Foundation.Mode"), 3) == 3;

	if (GManualValidationTickerHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(GManualValidationTickerHandle);
	}
	GManualValidationTickerHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateStatic(&TickManualValidation), 0.1f);
	UE_LOG(LogAVBOITFoundationManualRepro, Display, TEXT("Manual viewport validation started. Root=%s Screenshot=%s"), *GManualValidationRoot, *GManualValidationScreenshotPath);
}
