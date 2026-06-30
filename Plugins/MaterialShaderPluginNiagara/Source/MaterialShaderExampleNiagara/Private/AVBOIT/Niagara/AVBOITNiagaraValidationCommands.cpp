#include "AVBOIT/Niagara/AVBOITNiagaraValidationCommands.h"

#include "AVBOIT/Niagara/AVBOITNiagaraSceneData.h"
#include "AVBOIT/Niagara/AVBOITNiagaraSpriteRendererProperties.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraTypes.h"
#include "Camera/PlayerCameraManager.h"
#include "Containers/Ticker.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "HAL/IConsoleManager.h"
#include "ImageUtils.h"
#include "JsonObjectConverter.h"
#include "Misc/DateTime.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "NiagaraComponent.h"
#include "NiagaraEmitter.h"
#include "NiagaraEmitterHandle.h"
#include "NiagaraRendererProperties.h"
#include "NiagaraSpriteRendererProperties.h"
#include "NiagaraSystem.h"
#include "Policies/CondensedJsonPrintPolicy.h"
#include "RenderCore.h"
#include "ShowFlags.h"
#include "Styling/CoreStyle.h"
#include "UObject/UObjectIterator.h"
#include "UObject/UnrealType.h"
#include "UnrealClient.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SOverlay.h"
#include "Widgets/SWeakWidget.h"
#include "Widgets/Text/STextBlock.h"

TArray<IConsoleObject*> FAVBOITNiagaraValidationCommands::RegisteredCommands;

namespace
{
	const FVector ReferenceLocation(-2798.789173, 36.626050, 457.510006);
	const FRotator ReferenceRotation(17.254463, -8.130192, 0.0);

	const TCHAR* LexToString(ENiagaraSimTarget SimTarget)
	{
		switch (SimTarget)
		{
		case ENiagaraSimTarget::CPUSim:
			return TEXT("CPUSim");
		case ENiagaraSimTarget::GPUComputeSim:
			return TEXT("GPUComputeSim");
		default:
			return TEXT("Unknown");
		}
	}

	FString SanitizeToken(FString Value)
	{
		Value.TrimStartAndEndInline();
		if (Value.Len() >= 2 && Value.StartsWith(TEXT("\"")) && Value.EndsWith(TEXT("\"")))
		{
			Value = Value.Mid(1, Value.Len() - 2);
		}
		return Value;
	}

	TMap<FString, FString> ParseKeyValueArgs(const TArray<FString>& Args)
	{
		TMap<FString, FString> Result;
		for (const FString& Arg : Args)
		{
			FString Key;
			FString Value;
			if (Arg.Split(TEXT("="), &Key, &Value))
			{
				Result.Add(Key.ToLower(), SanitizeToken(Value));
			}
		}
		return Result;
	}

	bool SaveJsonObject(const FString& Filename, const TSharedRef<FJsonObject>& Object)
	{
		IFileManager::Get().MakeDirectory(*FPaths::GetPath(Filename), true);

		FString Output;
		const TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer =
			TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&Output);
		if (!FJsonSerializer::Serialize(Object, Writer))
		{
			return false;
		}
		return FFileHelper::SaveStringToFile(Output, *Filename);
	}

	FString WorldTypeToString(EWorldType::Type WorldType)
	{
		switch (WorldType)
		{
		case EWorldType::Game:
			return TEXT("Game");
		case EWorldType::PIE:
			return TEXT("PIE");
		case EWorldType::Editor:
			return TEXT("Editor");
		case EWorldType::GamePreview:
			return TEXT("GamePreview");
		case EWorldType::GameRPC:
			return TEXT("GameRPC");
		default:
			return TEXT("Unknown");
		}
	}

	UWorld* FindRuntimeWorld()
	{
		if (!GEngine)
		{
			return nullptr;
		}

		UWorld* FallbackWorld = nullptr;
		for (const FWorldContext& Context : GEngine->GetWorldContexts())
		{
			UWorld* World = Context.World();
			if (!World)
			{
				continue;
			}

			if (World->WorldType == EWorldType::Game || World->WorldType == EWorldType::PIE || World->WorldType == EWorldType::GamePreview)
			{
				return World;
			}
			FallbackWorld = World;
		}

		return FallbackWorld;
	}

	TArray<UNiagaraComponent*> GetNiagaraComponents(UWorld* World)
	{
		TArray<UNiagaraComponent*> Components;
		for (TObjectIterator<UNiagaraComponent> It; It; ++It)
		{
			UNiagaraComponent* Component = *It;
			if (!Component || Component->GetWorld() != World)
			{
				continue;
			}
			Components.Add(Component);
		}
		return Components;
	}

	void CopyRendererPropertyClass(const UObject* Source, UObject* Destination, const UClass* Class)
	{
		for (TFieldIterator<FProperty> It(Class, EFieldIteratorFlags::ExcludeSuper); It; ++It)
		{
			FProperty* Property = *It;
			Property->CopyCompleteValue_InContainer(Destination, Source);
		}
	}

	void CopySpriteRendererProperties(const UNiagaraSpriteRendererProperties* Source, UNiagaraAVBOITSpriteRendererProperties* Destination)
	{
		CopyRendererPropertyClass(Source, Destination, UNiagaraRendererProperties::StaticClass());
		CopyRendererPropertyClass(Source, Destination, UNiagaraSpriteRendererProperties::StaticClass());
	}

	int32 ConvertWorldSpriteRenderers(UWorld* World, TArray<FString>& ConvertedRenderers)
	{
		if (!World)
		{
			return 0;
		}

		TSet<UNiagaraSystem*> ConvertedSystems;
		int32 ConvertedCount = 0;

		for (UNiagaraComponent* Component : GetNiagaraComponents(World))
		{
			UNiagaraSystem* System = Component ? Component->GetAsset() : nullptr;
			if (!System || ConvertedSystems.Contains(System))
			{
				continue;
			}
			ConvertedSystems.Add(System);

			for (FNiagaraEmitterHandle& Handle : System->GetEmitterHandles())
			{
				if (!Handle.GetIsEnabled())
				{
					continue;
				}

				FVersionedNiagaraEmitterData* EmitterData = Handle.GetEmitterData();
				if (!EmitterData)
				{
					continue;
				}

				TArray<UNiagaraRendererProperties*>& Renderers =
					const_cast<TArray<UNiagaraRendererProperties*>&>(EmitterData->GetRenderers());
				for (int32 RendererIndex = 0; RendererIndex < Renderers.Num(); ++RendererIndex)
				{
					UNiagaraRendererProperties* Renderer = Renderers[RendererIndex];
					UNiagaraSpriteRendererProperties* SpriteRenderer = Cast<UNiagaraSpriteRendererProperties>(Renderer);
					if (!SpriteRenderer || Renderer->IsA<UNiagaraAVBOITSpriteRendererProperties>())
					{
						continue;
					}

					UNiagaraAVBOITSpriteRendererProperties* AVBOITRenderer =
						NewObject<UNiagaraAVBOITSpriteRendererProperties>(SpriteRenderer->GetOuter(), NAME_None, RF_Transient);
					CopySpriteRendererProperties(SpriteRenderer, AVBOITRenderer);
					AVBOITRenderer->SetFlags(RF_Transient);
					Renderers[RendererIndex] = AVBOITRenderer;

					ConvertedCount++;
					ConvertedRenderers.Add(FString::Printf(
						TEXT("%s.%s[%d]"),
						*GetPathNameSafe(System),
						*Handle.GetName().ToString(),
						RendererIndex));
				}
			}

			System->CacheFromCompiledData();

			if (UPackage* Package = System->GetOutermost())
			{
				Package->SetDirtyFlag(false);
			}
		}

		for (UNiagaraComponent* Component : GetNiagaraComponents(World))
		{
			if (Component)
			{
				Component->ReinitializeSystem();
			}
		}

		return ConvertedCount;
	}

	TSharedRef<FJsonObject> BuildFeatureManifest(UWorld* World)
	{
		TSharedRef<FJsonObject> Root = MakeShared<FJsonObject>();
		Root->SetStringField(TEXT("GeneratedUtc"), FDateTime::UtcNow().ToIso8601());
		Root->SetStringField(TEXT("World"), GetNameSafe(World));
		Root->SetStringField(TEXT("WorldType"), World ? WorldTypeToString(World->WorldType) : TEXT("None"));
		Root->SetBoolField(TEXT("RuntimeWorldRequired"), true);
		Root->SetBoolField(TEXT("EditorViewportAllowed"), false);

		TArray<TSharedPtr<FJsonValue>> ComponentArray;
		int32 ComponentCount = 0;
		int32 RendererCount = 0;
		int32 SpriteRendererCount = 0;
		int32 AVBOITRendererCount = 0;

		for (UNiagaraComponent* Component : GetNiagaraComponents(World))
		{
			ComponentCount++;
			UNiagaraSystem* System = Component ? Component->GetAsset() : nullptr;

			TSharedRef<FJsonObject> ComponentObject = MakeShared<FJsonObject>();
			ComponentObject->SetStringField(TEXT("Component"), GetNameSafe(Component));
			ComponentObject->SetStringField(TEXT("Owner"), GetNameSafe(Component ? Component->GetOwner() : nullptr));
			ComponentObject->SetStringField(TEXT("System"), GetPathNameSafe(System));

			TArray<TSharedPtr<FJsonValue>> EmitterArray;
			if (System)
			{
				for (const FNiagaraEmitterHandle& Handle : System->GetEmitterHandles())
				{
					TSharedRef<FJsonObject> EmitterObject = MakeShared<FJsonObject>();
					EmitterObject->SetStringField(TEXT("Emitter"), Handle.GetName().ToString());
					EmitterObject->SetStringField(TEXT("UniqueInstanceName"), Handle.GetUniqueInstanceName());
					EmitterObject->SetBoolField(TEXT("Enabled"), Handle.GetIsEnabled());

					TArray<TSharedPtr<FJsonValue>> RendererArray;
					if (const FVersionedNiagaraEmitterData* EmitterData = Handle.GetEmitterData())
					{
						for (const UNiagaraRendererProperties* Renderer : EmitterData->GetRenderers())
						{
							RendererCount++;
							TSharedRef<FJsonObject> RendererObject = MakeShared<FJsonObject>();
							RendererObject->SetStringField(TEXT("Class"), Renderer ? Renderer->GetClass()->GetName() : TEXT("None"));
							RendererObject->SetBoolField(TEXT("Enabled"), Renderer ? Renderer->GetIsEnabled() : false);

							if (const UNiagaraSpriteRendererProperties* Sprite = Cast<UNiagaraSpriteRendererProperties>(Renderer))
							{
								SpriteRendererCount++;
								if (Renderer->IsA<UNiagaraAVBOITSpriteRendererProperties>())
								{
									AVBOITRendererCount++;
								}
								RendererObject->SetStringField(TEXT("Material"), GetPathNameSafe(Sprite->Material));
								RendererObject->SetNumberField(TEXT("FacingMode"), static_cast<int32>(Sprite->FacingMode));
								RendererObject->SetNumberField(TEXT("Alignment"), static_cast<int32>(Sprite->Alignment));
								RendererObject->SetNumberField(TEXT("SortMode"), static_cast<int32>(Sprite->SortMode));
								RendererObject->SetNumberField(TEXT("SubImageSizeX"), Sprite->SubImageSize.X);
								RendererObject->SetNumberField(TEXT("SubImageSizeY"), Sprite->SubImageSize.Y);
								RendererObject->SetBoolField(TEXT("SubImageBlend"), Sprite->bSubImageBlend != 0);
							}

							RendererArray.Add(MakeShared<FJsonValueObject>(RendererObject));
						}
					}

					EmitterObject->SetArrayField(TEXT("Renderers"), RendererArray);
					EmitterArray.Add(MakeShared<FJsonValueObject>(EmitterObject));
				}
			}

			ComponentObject->SetArrayField(TEXT("Emitters"), EmitterArray);
			ComponentArray.Add(MakeShared<FJsonValueObject>(ComponentObject));
		}

		Root->SetNumberField(TEXT("NiagaraComponentCount"), ComponentCount);
		Root->SetNumberField(TEXT("RendererCount"), RendererCount);
		Root->SetNumberField(TEXT("SpriteRendererCount"), SpriteRendererCount);
		Root->SetNumberField(TEXT("AVBOITRendererCount"), AVBOITRendererCount);
		Root->SetArrayField(TEXT("NiagaraComponents"), ComponentArray);
		return Root;
	}

	void SetCVarInt(const TCHAR* Name, int32 Value)
	{
		if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(Name))
		{
			CVar->Set(Value, ECVF_SetByConsole);
		}
	}

	void SetCVarFloat(const TCHAR* Name, float Value)
	{
		if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(Name))
		{
			CVar->Set(Value, ECVF_SetByConsole);
		}
	}

	int32 GetCVarInt(const TCHAR* Name)
	{
		if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(Name))
		{
			return CVar->GetInt();
		}
		return 0;
	}

	float GetCVarFloat(const TCHAR* Name)
	{
		if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(Name))
		{
			return CVar->GetFloat();
		}
		return 0.0f;
	}

	struct FAVBOITEngineViewModeContract
	{
		FString RequestedViewMode = TEXT("Unlit");
		FString VerifiedViewMode = TEXT("Unavailable");
		int32 ViewModeIndex = INDEX_NONE;
		bool bVerifiedViewModeIsUnlit = false;
		bool bLighting = true;
		bool bLightFunctions = true;
		bool bDynamicShadows = true;
		bool bAtmosphere = true;
		bool bFog = true;
		bool bPostProcessing = true;
		bool bMaterials = true;
		int32 ForceDebugViewModes = 0;
		FString ShowFlagsSource = TEXT("Unavailable");
	};

	FString ViewModeNameFromIndex(int32 ViewModeIndex)
	{
		switch (ViewModeIndex)
		{
		case VMI_BrushWireframe:
			return TEXT("BrushWireframe");
		case VMI_Wireframe:
			return TEXT("Wireframe");
		case VMI_Unlit:
			return TEXT("Unlit");
		case VMI_Lit:
			return TEXT("Lit");
		case VMI_Lit_DetailLighting:
			return TEXT("Lit_DetailLighting");
		case VMI_Lit_Wireframe:
			return TEXT("Lit_Wireframe");
		case VMI_LightingOnly:
			return TEXT("LightingOnly");
		case VMI_LightComplexity:
			return TEXT("LightComplexity");
		case VMI_ShaderComplexity:
			return TEXT("ShaderComplexity");
		case VMI_LightmapDensity:
			return TEXT("LightmapDensity");
		case VMI_LitLightmapDensity:
			return TEXT("LitLightmapDensity");
		case VMI_ReflectionOverride:
			return TEXT("ReflectionOverride");
		case VMI_VisualizeBuffer:
			return TEXT("VisualizeBuffer");
		default:
			return FString::Printf(TEXT("ViewMode_%d"), ViewModeIndex);
		}
	}

	FAVBOITEngineViewModeContract ReadEngineViewModeContract()
	{
		FAVBOITEngineViewModeContract Contract;
		Contract.ForceDebugViewModes = GetCVarInt(TEXT("r.ForceDebugViewModes"));

		if (GEngine && GEngine->GameViewport)
		{
			Contract.ViewModeIndex = GEngine->GameViewport->ViewModeIndex;
			Contract.VerifiedViewMode = ViewModeNameFromIndex(Contract.ViewModeIndex);

			if (const FEngineShowFlags* ShowFlags = GEngine->GameViewport->GetEngineShowFlags())
			{
				FEngineShowFlags EffectiveShowFlags = *ShowFlags;
				if (Contract.ViewModeIndex >= 0 && Contract.ViewModeIndex < VMI_Max)
				{
					EngineShowFlagOverride(ESFIM_Game, static_cast<EViewModeIndex>(Contract.ViewModeIndex), EffectiveShowFlags, false);
					Contract.ShowFlagsSource = TEXT("EngineShowFlagOverride(ESFIM_Game, VerifiedViewMode)");
				}
				else
				{
					Contract.ShowFlagsSource = TEXT("GameViewportClient.BaseEngineShowFlags");
				}

				Contract.bLighting = EffectiveShowFlags.Lighting != 0;
				Contract.bLightFunctions = EffectiveShowFlags.LightFunctions != 0;
				Contract.bDynamicShadows = EffectiveShowFlags.DynamicShadows != 0;
				Contract.bAtmosphere = EffectiveShowFlags.Atmosphere != 0;
				Contract.bFog = EffectiveShowFlags.Fog != 0;
				Contract.bPostProcessing = EffectiveShowFlags.PostProcessing != 0;
				Contract.bMaterials = EffectiveShowFlags.Materials != 0;
			}
		}

		Contract.bVerifiedViewModeIsUnlit =
			Contract.ViewModeIndex == VMI_Unlit &&
			Contract.VerifiedViewMode.Equals(TEXT("Unlit"), ESearchCase::IgnoreCase) &&
			!Contract.bLighting &&
			!Contract.bLightFunctions &&
			!Contract.bDynamicShadows &&
			!Contract.bAtmosphere &&
			!Contract.bFog;

		return Contract;
	}

	FAVBOITEngineViewModeContract ForceEngineUnlitViewMode()
	{
		if (GEngine && GEngine->GameViewport)
		{
			GEngine->GameViewport->SetViewMode(VMI_Unlit);
		}

		return ReadEngineViewModeContract();
	}

	void AddViewModeFields(const TSharedRef<FJsonObject>& Object, const FAVBOITEngineViewModeContract& Contract)
	{
		Object->SetStringField(TEXT("RequestedViewMode"), Contract.RequestedViewMode);
		Object->SetStringField(TEXT("VerifiedViewMode"), Contract.VerifiedViewMode);
		Object->SetNumberField(TEXT("ViewModeIndex"), Contract.ViewModeIndex);
		Object->SetBoolField(TEXT("VerifiedViewModeIsUnlit"), Contract.bVerifiedViewModeIsUnlit);
		Object->SetBoolField(TEXT("ShowFlagLighting"), Contract.bLighting);
		Object->SetBoolField(TEXT("ShowFlagLightFunctions"), Contract.bLightFunctions);
		Object->SetBoolField(TEXT("ShowFlagDynamicShadows"), Contract.bDynamicShadows);
		Object->SetBoolField(TEXT("ShowFlagAtmosphere"), Contract.bAtmosphere);
		Object->SetBoolField(TEXT("ShowFlagFog"), Contract.bFog);
		Object->SetBoolField(TEXT("ShowFlagPostProcessing"), Contract.bPostProcessing);
		Object->SetBoolField(TEXT("ShowFlagMaterials"), Contract.bMaterials);
		Object->SetNumberField(TEXT("ForceDebugViewModes"), Contract.ForceDebugViewModes);
		Object->SetStringField(TEXT("ShowFlagsSource"), Contract.ShowFlagsSource);
	}

	TSharedRef<FJsonObject> BuildViewModeContractJson(const TCHAR* Context, const FAVBOITEngineViewModeContract& Contract)
	{
		TSharedRef<FJsonObject> Root = MakeShared<FJsonObject>();
		Root->SetStringField(TEXT("GeneratedUtc"), FDateTime::UtcNow().ToIso8601());
		Root->SetStringField(TEXT("Context"), Context ? Context : TEXT("Runtime"));
		Root->SetBoolField(TEXT("EngineUnlitRequired"), true);
		AddViewModeFields(Root, Contract);
		return Root;
	}

	enum class EAVBOITCaptureMode : uint8
	{
		EngineDefault,
		PluginBypass,
		AVBOITNiagaraUnlit,
		DebugBuffers
	};

	const TCHAR* CaptureModeName(EAVBOITCaptureMode Mode)
	{
		switch (Mode)
		{
		case EAVBOITCaptureMode::EngineDefault:
			return TEXT("EngineDefault");
		case EAVBOITCaptureMode::PluginBypass:
			return TEXT("PluginBypass");
		case EAVBOITCaptureMode::AVBOITNiagaraUnlit:
			return TEXT("AVBOITNiagaraUnlit");
		case EAVBOITCaptureMode::DebugBuffers:
			return TEXT("DebugBuffers");
		default:
			return TEXT("Unknown");
		}
	}

	FString CaptureModeDisplayName(EAVBOITCaptureMode Mode)
	{
		switch (Mode)
		{
		case EAVBOITCaptureMode::EngineDefault:
			return TEXT("Engine Default");
		case EAVBOITCaptureMode::PluginBypass:
			return TEXT("Plugin Bypass");
		case EAVBOITCaptureMode::AVBOITNiagaraUnlit:
			return TEXT("Plugin AVBOIT Unlit");
		case EAVBOITCaptureMode::DebugBuffers:
			return TEXT("Debug Buffers");
		default:
			return TEXT("Unknown");
		}
	}

	void ApplyCaptureMode(EAVBOITCaptureMode Mode)
	{
		SetCVarInt(TEXT("r.RDG.Events"), 1);
		SetCVarInt(TEXT("r.RDG.Debug"), 1);
		SetCVarInt(TEXT("r.RDG.EmitDrawEvents"), 1);
		SetCVarFloat(TEXT("r.ScreenPercentage"), 100.0f);

		switch (Mode)
		{
		case EAVBOITCaptureMode::EngineDefault:
			SetCVarInt(TEXT("r.AVBOIT.Enable"), 0);
			SetCVarInt(TEXT("r.AVBOIT.Niagara.Enable"), 0);
			break;
		case EAVBOITCaptureMode::PluginBypass:
			SetCVarInt(TEXT("r.AVBOIT.Enable"), 1);
			SetCVarInt(TEXT("r.AVBOIT.Niagara.Enable"), 0);
			break;
		case EAVBOITCaptureMode::AVBOITNiagaraUnlit:
			SetCVarInt(TEXT("r.AVBOIT.Enable"), 1);
			SetCVarInt(TEXT("r.AVBOIT.Niagara.Enable"), 1);
			SetCVarInt(TEXT("r.AVBOIT.Niagara.UnlitOnly"), 1);
			SetCVarInt(TEXT("r.AVBOIT.Niagara.Debug"), 0);
			SetCVarInt(TEXT("r.AVBOIT.Niagara.CaptureInputs"), 0);
			break;
		case EAVBOITCaptureMode::DebugBuffers:
			SetCVarInt(TEXT("r.AVBOIT.Enable"), 1);
			SetCVarInt(TEXT("r.AVBOIT.Niagara.Enable"), 1);
			SetCVarInt(TEXT("r.AVBOIT.Niagara.UnlitOnly"), 1);
			SetCVarInt(TEXT("r.AVBOIT.Niagara.Debug"), 1);
			SetCVarInt(TEXT("r.AVBOIT.Niagara.CaptureInputs"), 1);
			break;
		}
	}

	bool TryParseCaptureMode(const FString& InMode, EAVBOITCaptureMode& OutMode)
	{
		FString Mode = InMode;
		Mode.TrimStartAndEndInline();
		if (Mode.Equals(TEXT("EngineDefault"), ESearchCase::IgnoreCase) || Mode.Equals(TEXT("Default"), ESearchCase::IgnoreCase))
		{
			OutMode = EAVBOITCaptureMode::EngineDefault;
			return true;
		}
		if (Mode.Equals(TEXT("PluginBypass"), ESearchCase::IgnoreCase) || Mode.Equals(TEXT("Bypass"), ESearchCase::IgnoreCase))
		{
			OutMode = EAVBOITCaptureMode::PluginBypass;
			return true;
		}
		if (Mode.Equals(TEXT("AVBOITUnlit"), ESearchCase::IgnoreCase) || Mode.Equals(TEXT("AVBOITNiagaraUnlit"), ESearchCase::IgnoreCase) || Mode.Equals(TEXT("Plugin"), ESearchCase::IgnoreCase))
		{
			OutMode = EAVBOITCaptureMode::AVBOITNiagaraUnlit;
			return true;
		}
		if (Mode.Equals(TEXT("DebugBuffers"), ESearchCase::IgnoreCase) || Mode.Equals(TEXT("Debug"), ESearchCase::IgnoreCase))
		{
			OutMode = EAVBOITCaptureMode::DebugBuffers;
			return true;
		}
		return false;
	}

	void ApplyReferenceRuntimeCamera(UWorld* World, TWeakObjectPtr<ACameraActor>& RuntimeCamera)
	{
		if (!World)
		{
			return;
		}

		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (!PlayerController)
		{
			return;
		}

		APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;
		const float RuntimeFOV = CameraManager ? CameraManager->GetFOVAngle() : 90.0f;

		if (!RuntimeCamera.IsValid())
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Name = TEXT("AVBOIT_TestSpriteMap1_RuntimeCamera");
			SpawnParameters.ObjectFlags |= RF_Transient;
			RuntimeCamera = World->SpawnActor<ACameraActor>(ReferenceLocation, ReferenceRotation, SpawnParameters);
			if (RuntimeCamera.IsValid())
			{
				RuntimeCamera->SetActorScale3D(FVector(1.0, 1.0, 1.0));
#if WITH_EDITOR
				RuntimeCamera->SetIsTemporarilyHiddenInEditor(true);
#endif
				if (UCameraComponent* CameraComponent = RuntimeCamera->GetCameraComponent())
				{
					CameraComponent->SetFieldOfView(RuntimeFOV);
				}
			}
		}

		if (RuntimeCamera.IsValid())
		{
			RuntimeCamera->SetActorLocationAndRotation(ReferenceLocation, ReferenceRotation);
			PlayerController->SetControlRotation(ReferenceRotation);
			PlayerController->SetViewTarget(RuntimeCamera.Get());
		}

		if (CameraManager)
		{
			CameraManager->SetActorLocationAndRotation(ReferenceLocation, ReferenceRotation);
		}
	}

	TArray<float> ParseCaptureTimes(const FString& Times)
	{
		TArray<float> Result;
		TArray<FString> Parts;
		FString NormalizedTimes = Times;
		NormalizedTimes.ReplaceInline(TEXT("|"), TEXT(","));
		NormalizedTimes.ReplaceInline(TEXT("+"), TEXT(","));
		NormalizedTimes.ParseIntoArray(Parts, TEXT(","), true);
		for (const FString& Part : Parts)
		{
			Result.Add(FCString::Atof(*Part));
		}
		if (Result.IsEmpty())
		{
			Result = { 0.5f, 1.0f, 2.0f, 4.0f };
		}
		return Result;
	}

	TSharedRef<FJsonObject> BuildRendererBindingManifest()
	{
		TSharedRef<FJsonObject> Root = MakeShared<FJsonObject>();
		Root->SetStringField(TEXT("GeneratedUtc"), FDateTime::UtcNow().ToIso8601());

		const FAVBOITNiagaraFrameStats Stats = FAVBOITNiagaraSceneData::Get().GetLastCompletedStats();
		Root->SetNumberField(TEXT("FrameNumber"), Stats.FrameNumber);
		Root->SetNumberField(TEXT("SpriteDrawCount"), Stats.SpriteDrawCount);
		Root->SetNumberField(TEXT("ParticleCount"), Stats.ParticleCount);
		Root->SetBoolField(TEXT("ClearPassScheduled"), Stats.bClearPassScheduled);
		Root->SetBoolField(TEXT("SpriteSplatPassScheduled"), Stats.bSpriteSplatPassScheduled);
		Root->SetBoolField(TEXT("IntegratePassScheduled"), Stats.bIntegratePassScheduled);
		Root->SetBoolField(TEXT("ForwardUnlitPassScheduled"), Stats.bForwardUnlitPassScheduled);
		Root->SetBoolField(TEXT("CompositePassScheduled"), Stats.bCompositePassScheduled);

		TArray<TSharedPtr<FJsonValue>> PassArray;
		if (Stats.bClearPassScheduled)
		{
			PassArray.Add(MakeShared<FJsonValueString>(TEXT("AVBOIT.Niagara.Clear")));
		}
		if (Stats.bSpriteSplatPassScheduled)
		{
			PassArray.Add(MakeShared<FJsonValueString>(TEXT("AVBOIT.Niagara.SpriteSplat")));
		}
		if (Stats.bIntegratePassScheduled)
		{
			PassArray.Add(MakeShared<FJsonValueString>(TEXT("AVBOIT.Niagara.Integrate")));
		}
		if (Stats.bForwardUnlitPassScheduled)
		{
			PassArray.Add(MakeShared<FJsonValueString>(TEXT("AVBOIT.Niagara.ForwardUnlit")));
		}
		if (Stats.bCompositePassScheduled)
		{
			PassArray.Add(MakeShared<FJsonValueString>(TEXT("AVBOIT.Niagara.Composite")));
		}
		Root->SetArrayField(TEXT("ScheduledRDGPasses"), PassArray);

		TArray<TSharedPtr<FJsonValue>> DrawArray;
		for (const FAVBOITNiagaraSpriteDrawData& Draw : FAVBOITNiagaraSceneData::Get().GetLastCompletedDraws())
		{
			TSharedRef<FJsonObject> DrawObject = MakeShared<FJsonObject>();
			DrawObject->SetNumberField(TEXT("FrameNumber"), Draw.FrameNumber);
			DrawObject->SetStringField(TEXT("SystemName"), Draw.SystemName);
			DrawObject->SetStringField(TEXT("ComponentName"), Draw.ComponentName);
			DrawObject->SetStringField(TEXT("RendererName"), Draw.RendererName);
			DrawObject->SetStringField(TEXT("MaterialPath"), Draw.MaterialPath);
			DrawObject->SetNumberField(TEXT("ParticleCount"), Draw.ParticleCount);
			DrawObject->SetStringField(TEXT("SimTarget"), LexToString(Draw.SimTarget));
			DrawObject->SetNumberField(TEXT("FacingMode"), Draw.FacingMode);
			DrawObject->SetNumberField(TEXT("Alignment"), Draw.Alignment);
			DrawObject->SetNumberField(TEXT("SortMode"), Draw.SortMode);
			DrawObject->SetNumberField(TEXT("SubImageSizeX"), Draw.SubImageSize.X);
			DrawObject->SetNumberField(TEXT("SubImageSizeY"), Draw.SubImageSize.Y);
			DrawObject->SetBoolField(TEXT("SubImageBlend"), Draw.bSubImageBlend);
			DrawObject->SetBoolField(TEXT("DefaultDrawSuppressed"), Draw.bDefaultDrawSuppressed);
			DrawArray.Add(MakeShared<FJsonValueObject>(DrawObject));
		}
		Root->SetArrayField(TEXT("Draws"), DrawArray);
		return Root;
	}

	EAVBOITCaptureMode GInteractiveMode = EAVBOITCaptureMode::EngineDefault;
	FString GInteractiveStatusRoot;
	TWeakObjectPtr<ACameraActor> GInteractiveRuntimeCamera;
	TSharedPtr<SWidget> GInteractiveOverlayWidget;
	TSharedPtr<SWidget> GInteractiveOverlayViewportContent;

	void ApplyInteractiveMode(EAVBOITCaptureMode Mode, const TCHAR* Context);

	class SAVBOITNiagaraModeOverlay final : public SCompoundWidget
	{
	public:
		SLATE_BEGIN_ARGS(SAVBOITNiagaraModeOverlay) {}
		SLATE_END_ARGS()

		void Construct(const FArguments& /*InArgs*/)
		{
			SetVisibility(EVisibility::SelfHitTestInvisible);

			ModeOptions.Add(MakeShared<FString>(TEXT("Engine Default")));
			ModeOptions.Add(MakeShared<FString>(TEXT("Plugin AVBOIT Unlit")));

			ChildSlot
			[
				SNew(SOverlay)
				.Visibility(EVisibility::SelfHitTestInvisible)
				+ SOverlay::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Top)
				.Padding(FMargin(0.0f, 96.0f, 120.0f, 0.0f))
				[
					SNew(SBox)
					.WidthOverride(292.0f)
					[
						SNew(SBorder)
						.BorderImage(FCoreStyle::Get().GetBrush(TEXT("GenericWhiteBox")))
						.BorderBackgroundColor(FLinearColor(0.02f, 0.02f, 0.02f, 0.86f))
						.Padding(FMargin(10.0f, 8.0f))
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								SNew(STextBlock)
								.Text(FText::FromString(TEXT("AVBOIT Render Mode")))
								.ColorAndOpacity(FLinearColor::White)
							]
							+ SVerticalBox::Slot()
							.AutoHeight()
							.Padding(FMargin(0.0f, 6.0f, 0.0f, 0.0f))
							[
								SAssignNew(ModeComboBox, SComboBox<TSharedPtr<FString>>)
								.OptionsSource(&ModeOptions)
								.OnGenerateWidget(this, &SAVBOITNiagaraModeOverlay::GenerateModeOption)
								.OnSelectionChanged(this, &SAVBOITNiagaraModeOverlay::HandleModeSelectionChanged)
								[
									SNew(STextBlock)
									.Text(this, &SAVBOITNiagaraModeOverlay::GetSelectedModeText)
								]
							]
							+ SVerticalBox::Slot()
							.AutoHeight()
							.Padding(FMargin(0.0f, 6.0f, 0.0f, 0.0f))
							[
								SNew(STextBlock)
								.Text(this, &SAVBOITNiagaraModeOverlay::GetStatusText)
								.ColorAndOpacity(FLinearColor(0.78f, 0.93f, 1.0f, 1.0f))
								.AutoWrapText(true)
							]
						]
					]
				]
			];
		}

	private:
		TSharedRef<SWidget> GenerateModeOption(TSharedPtr<FString> Option) const
		{
			return SNew(STextBlock)
				.Text(FText::FromString(Option.IsValid() ? *Option : FString()))
				.ColorAndOpacity(FLinearColor::White);
		}

		void HandleModeSelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
		{
			if (!NewSelection.IsValid())
			{
				return;
			}

			const EAVBOITCaptureMode Mode =
				NewSelection->Equals(TEXT("Engine Default"), ESearchCase::IgnoreCase)
					? EAVBOITCaptureMode::EngineDefault
					: EAVBOITCaptureMode::AVBOITNiagaraUnlit;
			ApplyInteractiveMode(Mode, TEXT("OverlayDropdown"));
		}

		FText GetSelectedModeText() const
		{
			return FText::FromString(CaptureModeDisplayName(GInteractiveMode));
		}

		FText GetStatusText() const
		{
			const FAVBOITEngineViewModeContract ViewModeContract = ReadEngineViewModeContract();
			return FText::FromString(FString::Printf(
				TEXT("Mode: %s\n%s"),
				*CaptureModeDisplayName(GInteractiveMode),
				ViewModeContract.bVerifiedViewModeIsUnlit ? TEXT("Unlit verified") : TEXT("Unlit not verified")));
		}

		TArray<TSharedPtr<FString>> ModeOptions;
		TSharedPtr<SComboBox<TSharedPtr<FString>>> ModeComboBox;
	};

	void ShowInteractiveOverlay()
	{
		if (GInteractiveOverlayViewportContent.IsValid())
		{
			return;
		}

		if (!GEngine || !GEngine->GameViewport)
		{
			UE_LOG(LogTemp, Warning, TEXT("AVBOIT.Niagara.ShowOverlay skipped: no GameViewport is available."));
			return;
		}

		const TSharedRef<SWidget> OverlayWidget = SNew(SAVBOITNiagaraModeOverlay);
		GInteractiveOverlayWidget = OverlayWidget;
		GInteractiveOverlayViewportContent = SNew(SWeakWidget).PossiblyNullContent(OverlayWidget);
		GEngine->GameViewport->AddViewportWidgetContent(GInteractiveOverlayViewportContent.ToSharedRef(), 1000);
		UE_LOG(LogTemp, Display, TEXT("AVBOIT.Niagara overlay shown."));
	}

	void HideInteractiveOverlay()
	{
		if (GInteractiveOverlayViewportContent.IsValid() && GEngine && GEngine->GameViewport)
		{
			GEngine->GameViewport->RemoveViewportWidgetContent(GInteractiveOverlayViewportContent.ToSharedRef());
		}

		if (GInteractiveOverlayViewportContent.IsValid())
		{
			UE_LOG(LogTemp, Display, TEXT("AVBOIT.Niagara overlay hidden."));
		}

		GInteractiveOverlayViewportContent.Reset();
		GInteractiveOverlayWidget.Reset();
	}

	TSharedRef<FJsonObject> BuildRuntimeStatusObject(const TCHAR* Context)
	{
		const FAVBOITEngineViewModeContract ViewModeContract = ReadEngineViewModeContract();
		const FAVBOITNiagaraFrameStats Stats = FAVBOITNiagaraSceneData::Get().GetLastCompletedStats();

		TSharedRef<FJsonObject> Root = MakeShared<FJsonObject>();
		Root->SetStringField(TEXT("GeneratedUtc"), FDateTime::UtcNow().ToIso8601());
		Root->SetStringField(TEXT("Context"), Context ? Context : TEXT("Runtime"));
		Root->SetStringField(TEXT("Mode"), CaptureModeName(GInteractiveMode));
		Root->SetBoolField(TEXT("EditorViewportUsed"), false);
		AddViewModeFields(Root, ViewModeContract);

		TSharedRef<FJsonObject> CVars = MakeShared<FJsonObject>();
		CVars->SetNumberField(TEXT("r.AVBOIT.Enable"), GetCVarInt(TEXT("r.AVBOIT.Enable")));
		CVars->SetNumberField(TEXT("r.AVBOIT.Niagara.Enable"), GetCVarInt(TEXT("r.AVBOIT.Niagara.Enable")));
		CVars->SetNumberField(TEXT("r.AVBOIT.Niagara.UnlitOnly"), GetCVarInt(TEXT("r.AVBOIT.Niagara.UnlitOnly")));
		CVars->SetNumberField(TEXT("r.AVBOIT.Niagara.Debug"), GetCVarInt(TEXT("r.AVBOIT.Niagara.Debug")));
		CVars->SetNumberField(TEXT("r.AVBOIT.Niagara.CaptureInputs"), GetCVarInt(TEXT("r.AVBOIT.Niagara.CaptureInputs")));
		CVars->SetNumberField(TEXT("r.ScreenPercentage"), GetCVarFloat(TEXT("r.ScreenPercentage")));
		CVars->SetNumberField(TEXT("r.ForceDebugViewModes"), GetCVarInt(TEXT("r.ForceDebugViewModes")));
		Root->SetObjectField(TEXT("CVars"), CVars);

		Root->SetNumberField(TEXT("LastFrameNumber"), Stats.FrameNumber);
		Root->SetNumberField(TEXT("LastSpriteDrawCount"), Stats.SpriteDrawCount);
		Root->SetNumberField(TEXT("LastParticleCount"), Stats.ParticleCount);
		Root->SetBoolField(TEXT("LastClearPassScheduled"), Stats.bClearPassScheduled);
		Root->SetBoolField(TEXT("LastSpriteSplatPassScheduled"), Stats.bSpriteSplatPassScheduled);
		Root->SetBoolField(TEXT("LastIntegratePassScheduled"), Stats.bIntegratePassScheduled);
		Root->SetBoolField(TEXT("LastForwardUnlitPassScheduled"), Stats.bForwardUnlitPassScheduled);
		Root->SetBoolField(TEXT("LastCompositePassScheduled"), Stats.bCompositePassScheduled);
		return Root;
	}

	void SaveInteractiveStatus(const TCHAR* Context, const FString& ExplicitOutput = FString())
	{
		FString Output = ExplicitOutput;
		if (Output.IsEmpty() && !GInteractiveStatusRoot.IsEmpty())
		{
			Output = GInteractiveStatusRoot / TEXT("InteractiveStatus.json");
		}

		if (!Output.IsEmpty())
		{
			SaveJsonObject(Output, BuildRuntimeStatusObject(Context));
		}
	}

	class FAVBOITNiagaraParityCapture
	{
	public:
		FAVBOITNiagaraParityCapture(FString InRoot, TArray<float> InCaptureTimes)
			: Root(MoveTemp(InRoot))
			, CaptureTimes(MoveTemp(InCaptureTimes))
		{
		}

		void Start()
		{
			HideInteractiveOverlay();
			IFileManager::Get().MakeDirectory(*Root, true);
			GInteractiveStatusRoot = Root;

			World = FindRuntimeWorld();
			SaveJsonObject(Root / TEXT("FeatureManifest.json"), BuildFeatureManifest(World.Get()));

			TArray<FString> ConvertedRenderers;
			ConvertedRendererCount = ConvertWorldSpriteRenderers(World.Get(), ConvertedRenderers);

			TSharedRef<FJsonObject> Conversion = MakeShared<FJsonObject>();
			Conversion->SetStringField(TEXT("GeneratedUtc"), FDateTime::UtcNow().ToIso8601());
			Conversion->SetNumberField(TEXT("ConvertedRendererCount"), ConvertedRendererCount);
			Conversion->SetBoolField(TEXT("TransientRuntimeOnly"), true);
			Conversion->SetBoolField(TEXT("SavedAssetMutationAllowed"), false);
			TArray<TSharedPtr<FJsonValue>> ConvertedArray;
			for (const FString& RendererName : ConvertedRenderers)
			{
				ConvertedArray.Add(MakeShared<FJsonValueString>(RendererName));
			}
			Conversion->SetArrayField(TEXT("ConvertedRenderers"), ConvertedArray);
			SaveJsonObject(Root / TEXT("RendererConversionManifest.json"), Conversion);
			SaveJsonObject(Root / TEXT("FeatureManifestAfterConversion.json"), BuildFeatureManifest(World.Get()));

			SetCVarFloat(TEXT("r.ScreenPercentage"), 100.0f);
			ApplyRuntimeCamera();
			const FAVBOITEngineViewModeContract InitialViewMode = ForceEngineUnlitViewMode();
			SaveJsonObject(Root / TEXT("ViewModeContractRuntime.json"), BuildViewModeContractJson(TEXT("CaptureStart"), InitialViewMode));
			WriteCameraContract();

			if (!InitialViewMode.bVerifiedViewModeIsUnlit)
			{
				Finish(TEXT("blocked-local"), TEXT("Engine ViewMode Unlit could not be verified before capture start."));
				return;
			}

			FScreenshotRequest::OnScreenshotRequestProcessed().RemoveAll(this);
			FScreenshotRequest::OnScreenshotRequestProcessed().AddRaw(this, &FAVBOITNiagaraParityCapture::OnScreenshotProcessed);
			TickerHandle = FTSTicker::GetCoreTicker().AddTicker(
				FTickerDelegate::CreateRaw(this, &FAVBOITNiagaraParityCapture::Tick),
				0.0f);
		}

		bool Tick(float DeltaTime)
		{
			if (bWaitingForScreenshot)
			{
				return true;
			}

			const int32 ModeCount = Modes.Num();
			const int32 TotalCaptures = CaptureTimes.Num() * ModeCount;
			if (CaptureIndex >= TotalCaptures)
			{
				Finish();
				return false;
			}

			const int32 TimeIndex = CaptureIndex / ModeCount;
			const int32 ModeIndex = CaptureIndex % ModeCount;
			const float CaptureTime = CaptureTimes[TimeIndex];
			const EAVBOITCaptureMode Mode = Modes[ModeIndex];

			ApplyRuntimeCamera();
			ApplyNiagaraFixedAge(CaptureTime);
			ApplyCaptureMode(Mode);
			const FAVBOITEngineViewModeContract ViewModeContract = ForceEngineUnlitViewMode();

			TSharedRef<FJsonObject> CaptureRecord = MakeShared<FJsonObject>();
			CaptureRecord->SetNumberField(TEXT("CaptureIndex"), CaptureIndex + 1);
			CaptureRecord->SetNumberField(TEXT("CaptureTimeSeconds"), CaptureTime);
			CaptureRecord->SetStringField(TEXT("Mode"), CaptureModeName(Mode));
			CaptureRecord->SetNumberField(TEXT("DesiredAgeSeconds"), CaptureTime);
			CaptureRecord->SetStringField(TEXT("NiagaraAgeMode"), TEXT("DesiredAge"));
			AddViewModeFields(CaptureRecord, ViewModeContract);

			const FString Filename = Root / FString::Printf(
				TEXT("%02d_%s_%04dms.png"),
				CaptureIndex + 1,
				CaptureModeName(Mode),
				FMath::RoundToInt(CaptureTime * 1000.0f));
			CaptureRecord->SetStringField(TEXT("Filename"), Filename);

			if (!ViewModeContract.bVerifiedViewModeIsUnlit)
			{
				CaptureRecord->SetStringField(TEXT("Status"), TEXT("blocked-local"));
				CaptureRecord->SetStringField(TEXT("Reason"), TEXT("Engine ViewMode Unlit could not be verified immediately before screenshot."));
				CaptureRecords.Add(MakeShared<FJsonValueObject>(CaptureRecord));
				Finish(TEXT("blocked-local"), TEXT("Engine ViewMode Unlit verification failed before screenshot capture."));
				return false;
			}

			CaptureRecord->SetStringField(TEXT("Status"), TEXT("requested"));
			CaptureRecords.Add(MakeShared<FJsonValueObject>(CaptureRecord));

			bWaitingForScreenshot = true;
			FScreenshotRequest::RequestScreenshot(Filename, false, false, false);
			UE_LOG(LogTemp, Display, TEXT("UE-4.2C requested screenshot: %s"), *Filename);
			return true;
		}

		void OnScreenshotProcessed()
		{
			bWaitingForScreenshot = false;
			CaptureIndex++;
		}

	private:
		void ApplyNiagaraFixedAge(float DesiredAge)
		{
			if (!World.IsValid())
			{
				World = FindRuntimeWorld();
			}

			for (UNiagaraComponent* Component : GetNiagaraComponents(World.Get()))
			{
				if (!Component)
				{
					continue;
				}

				Component->Activate(true);
				Component->SetPaused(false);
				Component->SetAgeUpdateMode(ENiagaraAgeUpdateMode::DesiredAge);
				Component->SetSeekDelta(1.0f / 60.0f);
				Component->SetLockDesiredAgeDeltaTimeToSeekDelta(true);
				Component->SetCanRenderWhileSeeking(true);
				Component->SeekToDesiredAge(DesiredAge);
				Component->SetDesiredAge(DesiredAge);
			}
		}

		void ApplyRuntimeCamera()
		{
			if (!World.IsValid())
			{
				World = FindRuntimeWorld();
			}
			ApplyReferenceRuntimeCamera(World.Get(), RuntimeCamera);
		}

		void WriteCameraContract()
		{
			if (!World.IsValid())
			{
				return;
			}

			APlayerController* PlayerController = World->GetFirstPlayerController();
			APlayerCameraManager* CameraManager = PlayerController ? PlayerController->PlayerCameraManager : nullptr;
			UCameraComponent* CameraComponent = RuntimeCamera.IsValid() ? RuntimeCamera->GetCameraComponent() : nullptr;

			FIntPoint ViewportSize(0, 0);
			if (GEngine && GEngine->GameViewport && GEngine->GameViewport->Viewport)
			{
				ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();
			}

			TSharedRef<FJsonObject> RootObject = MakeShared<FJsonObject>();
			RootObject->SetStringField(TEXT("GeneratedUtc"), FDateTime::UtcNow().ToIso8601());
			RootObject->SetStringField(TEXT("World"), GetNameSafe(World.Get()));
			RootObject->SetStringField(TEXT("RunMode"), WorldTypeToString(World->WorldType));
			RootObject->SetBoolField(TEXT("EditorViewportUsed"), false);
			RootObject->SetNumberField(TEXT("FOV"), CameraManager ? CameraManager->GetFOVAngle() : 0.0f);
			RootObject->SetNumberField(TEXT("AspectRatio"), ViewportSize.Y > 0 ? static_cast<double>(ViewportSize.X) / static_cast<double>(ViewportSize.Y) : 0.0);
			RootObject->SetStringField(TEXT("ProjectionMode"), CameraComponent && CameraComponent->ProjectionMode == ECameraProjectionMode::Orthographic ? TEXT("Orthographic") : TEXT("Perspective"));
			FMinimalViewInfo ViewInfo;
			if (CameraComponent)
			{
				CameraComponent->GetCameraView(0.0f, ViewInfo);
			}
			const float NearPlane = ViewInfo.PerspectiveNearClipPlane > 0.0f ? ViewInfo.PerspectiveNearClipPlane : GNearClippingPlane;
			RootObject->SetNumberField(TEXT("NearPlane"), NearPlane);
			RootObject->SetNumberField(TEXT("ViewRectMinX"), 0);
			RootObject->SetNumberField(TEXT("ViewRectMinY"), 0);
			RootObject->SetNumberField(TEXT("ViewRectMaxX"), ViewportSize.X);
			RootObject->SetNumberField(TEXT("ViewRectMaxY"), ViewportSize.Y);
			RootObject->SetNumberField(TEXT("ScreenPercentage"), GetCVarFloat(TEXT("r.ScreenPercentage")));
			RootObject->SetNumberField(TEXT("AutoExposureBias"), CameraComponent ? CameraComponent->PostProcessSettings.AutoExposureBias : 0.0f);
			RootObject->SetNumberField(TEXT("PostProcessBlendWeight"), CameraComponent ? CameraComponent->PostProcessBlendWeight : 0.0f);
			AddViewModeFields(RootObject, ReadEngineViewModeContract());

			TSharedRef<FJsonObject> Transform = MakeShared<FJsonObject>();
			Transform->SetNumberField(TEXT("LocationX"), ReferenceLocation.X);
			Transform->SetNumberField(TEXT("LocationY"), ReferenceLocation.Y);
			Transform->SetNumberField(TEXT("LocationZ"), ReferenceLocation.Z);
			Transform->SetNumberField(TEXT("Pitch"), ReferenceRotation.Pitch);
			Transform->SetNumberField(TEXT("Yaw"), ReferenceRotation.Yaw);
			Transform->SetNumberField(TEXT("Roll"), ReferenceRotation.Roll);
			RootObject->SetObjectField(TEXT("AppliedTransform"), Transform);

			SaveJsonObject(Root / TEXT("CameraContractRuntime.json"), RootObject);
		}

		void Finish(const FString& Status = TEXT("partial"), const FString& Reason = TEXT("Runtime camera, transient renderer conversion, screenshots, real Niagara sprite particle counts, and RDG pass evidence are implemented. Pixel metrics, material evaluation, and same-particle-state proof are not yet complete, so UE-4.2C cannot be passed."))
		{
			FScreenshotRequest::OnScreenshotRequestProcessed().RemoveAll(this);
			SaveJsonObject(Root / TEXT("ViewModeContractRuntime.json"), BuildViewModeContractJson(TEXT("CaptureFinish"), ReadEngineViewModeContract()));

			TSharedRef<FJsonObject> CaptureManifest = MakeShared<FJsonObject>();
			CaptureManifest->SetStringField(TEXT("GeneratedUtc"), FDateTime::UtcNow().ToIso8601());
			CaptureManifest->SetStringField(TEXT("RequiredViewMode"), TEXT("Unlit"));
			CaptureManifest->SetBoolField(TEXT("EngineUnlitRequired"), true);
			CaptureManifest->SetNumberField(TEXT("RequestedCaptureCount"), CaptureTimes.Num() * Modes.Num());
			CaptureManifest->SetNumberField(TEXT("RecordedCaptureCount"), CaptureRecords.Num());
			CaptureManifest->SetArrayField(TEXT("Captures"), CaptureRecords);
			SaveJsonObject(Root / TEXT("CaptureManifest.json"), CaptureManifest);

			SaveJsonObject(Root / TEXT("RendererBindingManifest.json"), BuildRendererBindingManifest());

			TSharedRef<FJsonObject> Acceptance = MakeShared<FJsonObject>();
			Acceptance->SetStringField(TEXT("GeneratedUtc"), FDateTime::UtcNow().ToIso8601());
			Acceptance->SetStringField(TEXT("Status"), Status);
			Acceptance->SetBoolField(TEXT("SuccessWithheld"), true);
			Acceptance->SetNumberField(TEXT("ConvertedRendererCount"), ConvertedRendererCount);
			AddViewModeFields(Acceptance, ReadEngineViewModeContract());
			Acceptance->SetStringField(TEXT("Reason"), Reason);
			SaveJsonObject(Root / TEXT("Acceptance.json"), Acceptance);

			UE_LOG(LogTemp, Display, TEXT("UE-4.2C parity capture finished with %s status. Evidence: %s"), *Status, *Root);
			FPlatformMisc::RequestExit(false);
		}

		FString Root;
		TArray<float> CaptureTimes;
		TArray<EAVBOITCaptureMode> Modes = {
			EAVBOITCaptureMode::EngineDefault,
			EAVBOITCaptureMode::PluginBypass,
			EAVBOITCaptureMode::AVBOITNiagaraUnlit,
			EAVBOITCaptureMode::DebugBuffers
		};
		TWeakObjectPtr<UWorld> World;
		TWeakObjectPtr<ACameraActor> RuntimeCamera;
		FTSTicker::FDelegateHandle TickerHandle;
		TArray<TSharedPtr<FJsonValue>> CaptureRecords;
		int32 ConvertedRendererCount = 0;
		int32 CaptureIndex = 0;
		bool bWaitingForScreenshot = false;
	};

	TUniquePtr<FAVBOITNiagaraParityCapture> GActiveCapture;

	void CommandScanTestSpriteMap1(const TArray<FString>& Args)
	{
		const TMap<FString, FString> KeyValues = ParseKeyValueArgs(Args);
		const FString Output = KeyValues.FindRef(TEXT("output"));
		if (Output.IsEmpty())
		{
			UE_LOG(LogTemp, Error, TEXT("AVBOIT.Niagara.ScanTestSpriteMap1 requires output=<json path>"));
			return;
		}

		SaveJsonObject(Output, BuildFeatureManifest(FindRuntimeWorld()));
		UE_LOG(LogTemp, Display, TEXT("UE-4.2C feature manifest written: %s"), *Output);
	}

	void CommandConvertRenderers(const TArray<FString>& Args)
	{
		const TMap<FString, FString> KeyValues = ParseKeyValueArgs(Args);
		const FString Output = KeyValues.FindRef(TEXT("output"));
		TArray<FString> ConvertedRenderers;
		const int32 Converted = ConvertWorldSpriteRenderers(FindRuntimeWorld(), ConvertedRenderers);

		if (!Output.IsEmpty())
		{
			TSharedRef<FJsonObject> Root = MakeShared<FJsonObject>();
			Root->SetStringField(TEXT("GeneratedUtc"), FDateTime::UtcNow().ToIso8601());
			Root->SetNumberField(TEXT("ConvertedRendererCount"), Converted);
			Root->SetBoolField(TEXT("TransientRuntimeOnly"), true);
			TArray<TSharedPtr<FJsonValue>> ConvertedArray;
			for (const FString& Name : ConvertedRenderers)
			{
				ConvertedArray.Add(MakeShared<FJsonValueString>(Name));
			}
			Root->SetArrayField(TEXT("ConvertedRenderers"), ConvertedArray);
			SaveJsonObject(Output, Root);
		}

		UE_LOG(LogTemp, Display, TEXT("UE-4.2C converted %d Niagara sprite renderers at runtime."), Converted);
	}

	void CommandCaptureParity(const TArray<FString>& Args)
	{
		const TMap<FString, FString> KeyValues = ParseKeyValueArgs(Args);
		const FString Root = KeyValues.FindRef(TEXT("root"));
		if (Root.IsEmpty())
		{
			UE_LOG(LogTemp, Error, TEXT("AVBOIT.Niagara.CaptureParity requires root=<evidence directory>"));
			return;
		}

		const FString Times = KeyValues.Contains(TEXT("times")) ? KeyValues.FindRef(TEXT("times")) : TEXT("0.5,1.0,2.0,4.0");
		GActiveCapture = MakeUnique<FAVBOITNiagaraParityCapture>(Root, ParseCaptureTimes(Times));
		GActiveCapture->Start();
	}

	void ApplyInteractiveMode(EAVBOITCaptureMode Mode, const TCHAR* Context)
	{
		UWorld* World = FindRuntimeWorld();
		ApplyReferenceRuntimeCamera(World, GInteractiveRuntimeCamera);
		ApplyCaptureMode(Mode);
		const FAVBOITEngineViewModeContract ViewModeContract = ForceEngineUnlitViewMode();
		GInteractiveMode = Mode;
		SaveInteractiveStatus(Context);

		UE_LOG(
			LogTemp,
			Display,
			TEXT("AVBOIT.Niagara mode=%s requestedViewMode=Unlit verifiedViewMode=%s verifiedUnlit=%s statusRoot=%s"),
			CaptureModeName(Mode),
			*ViewModeContract.VerifiedViewMode,
			ViewModeContract.bVerifiedViewModeIsUnlit ? TEXT("true") : TEXT("false"),
			*GInteractiveStatusRoot);
	}

	void CommandMode(const TArray<FString>& Args)
	{
		FString ModeToken;
		if (Args.Num() > 0 && !Args[0].Contains(TEXT("=")))
		{
			ModeToken = Args[0];
		}
		else
		{
			const TMap<FString, FString> KeyValues = ParseKeyValueArgs(Args);
			ModeToken = KeyValues.FindRef(TEXT("mode"));
		}

		EAVBOITCaptureMode Mode = EAVBOITCaptureMode::EngineDefault;
		if (!TryParseCaptureMode(ModeToken, Mode))
		{
			UE_LOG(LogTemp, Error, TEXT("AVBOIT.Niagara.Mode expects EngineDefault, AVBOITUnlit, PluginBypass, or DebugBuffers."));
			return;
		}

		ApplyInteractiveMode(Mode, TEXT("ModeCommand"));
	}

	void CommandToggleDefaultPlugin(const TArray<FString>& Args)
	{
		const EAVBOITCaptureMode NextMode =
			GInteractiveMode == EAVBOITCaptureMode::AVBOITNiagaraUnlit
				? EAVBOITCaptureMode::EngineDefault
				: EAVBOITCaptureMode::AVBOITNiagaraUnlit;
		ApplyInteractiveMode(NextMode, TEXT("ToggleDefaultPlugin"));
	}

	void CommandStatus(const TArray<FString>& Args)
	{
		const TMap<FString, FString> KeyValues = ParseKeyValueArgs(Args);
		const FString Output = KeyValues.FindRef(TEXT("output"));
		const TSharedRef<FJsonObject> Status = BuildRuntimeStatusObject(TEXT("StatusCommand"));

		if (!Output.IsEmpty())
		{
			SaveJsonObject(Output, Status);
		}
		else
		{
			SaveInteractiveStatus(TEXT("StatusCommand"));
		}

		const FAVBOITEngineViewModeContract ViewModeContract = ReadEngineViewModeContract();
		UE_LOG(
			LogTemp,
			Display,
			TEXT("AVBOIT.Niagara.Status mode=%s viewMode=%s verifiedUnlit=%s draws=%d particles=%d"),
			CaptureModeName(GInteractiveMode),
			*ViewModeContract.VerifiedViewMode,
			ViewModeContract.bVerifiedViewModeIsUnlit ? TEXT("true") : TEXT("false"),
			FAVBOITNiagaraSceneData::Get().GetLastCompletedStats().SpriteDrawCount,
			FAVBOITNiagaraSceneData::Get().GetLastCompletedStats().ParticleCount);
	}

	void CommandShowOverlay(const TArray<FString>& /*Args*/)
	{
		ShowInteractiveOverlay();
	}

	void CommandHideOverlay(const TArray<FString>& /*Args*/)
	{
		HideInteractiveOverlay();
	}

	void CommandInteractive(const TArray<FString>& Args)
	{
		const TMap<FString, FString> KeyValues = ParseKeyValueArgs(Args);
		GInteractiveStatusRoot = KeyValues.FindRef(TEXT("root"));
		if (!GInteractiveStatusRoot.IsEmpty())
		{
			IFileManager::Get().MakeDirectory(*GInteractiveStatusRoot, true);
		}

		UWorld* World = FindRuntimeWorld();
		if (!GInteractiveStatusRoot.IsEmpty())
		{
			SaveJsonObject(GInteractiveStatusRoot / TEXT("FeatureManifest.json"), BuildFeatureManifest(World));
		}

		TArray<FString> ConvertedRenderers;
		const int32 Converted = ConvertWorldSpriteRenderers(World, ConvertedRenderers);
		if (!GInteractiveStatusRoot.IsEmpty())
		{
			TSharedRef<FJsonObject> Conversion = MakeShared<FJsonObject>();
			Conversion->SetStringField(TEXT("GeneratedUtc"), FDateTime::UtcNow().ToIso8601());
			Conversion->SetNumberField(TEXT("ConvertedRendererCount"), Converted);
			Conversion->SetBoolField(TEXT("TransientRuntimeOnly"), true);
			Conversion->SetBoolField(TEXT("SavedAssetMutationAllowed"), false);
			TArray<TSharedPtr<FJsonValue>> ConvertedArray;
			for (const FString& RendererName : ConvertedRenderers)
			{
				ConvertedArray.Add(MakeShared<FJsonValueString>(RendererName));
			}
			Conversion->SetArrayField(TEXT("ConvertedRenderers"), ConvertedArray);
			SaveJsonObject(GInteractiveStatusRoot / TEXT("RendererConversionManifest.json"), Conversion);
			SaveJsonObject(GInteractiveStatusRoot / TEXT("FeatureManifestAfterConversion.json"), BuildFeatureManifest(World));
		}

		EAVBOITCaptureMode Mode = EAVBOITCaptureMode::AVBOITNiagaraUnlit;
		const FString ModeToken = KeyValues.Contains(TEXT("mode")) ? KeyValues.FindRef(TEXT("mode")) : TEXT("AVBOITUnlit");
		TryParseCaptureMode(ModeToken, Mode);
		ApplyInteractiveMode(Mode, TEXT("InteractiveStart"));
		ShowInteractiveOverlay();
		UE_LOG(LogTemp, Display, TEXT("AVBOIT.Niagara.Interactive ready. Use the viewport overlay, AVBOIT.Niagara.ToggleDefaultPlugin, or AVBOIT.Niagara.Mode EngineDefault/AVBOITUnlit."));
	}
}

void FAVBOITNiagaraValidationCommands::RegisterCommands()
{
	if (!RegisteredCommands.IsEmpty())
	{
		return;
	}

	IConsoleManager& ConsoleManager = IConsoleManager::Get();
	RegisteredCommands.Add(ConsoleManager.RegisterConsoleCommand(
		TEXT("AVBOIT.Niagara.ScanTestSpriteMap1"),
		TEXT("Writes a Niagara feature manifest for the runtime TestSpriteMap1 world."),
		FConsoleCommandWithArgsDelegate::CreateStatic(&CommandScanTestSpriteMap1),
		ECVF_Default));

	RegisteredCommands.Add(ConsoleManager.RegisterConsoleCommand(
		TEXT("AVBOIT.Niagara.ConvertRenderers"),
		TEXT("Transiently converts runtime Niagara sprite renderers to AVBOIT renderer properties without saving source assets."),
		FConsoleCommandWithArgsDelegate::CreateStatic(&CommandConvertRenderers),
		ECVF_Default));

	RegisteredCommands.Add(ConsoleManager.RegisterConsoleCommand(
		TEXT("AVBOIT.Niagara.CaptureParity"),
		TEXT("Captures UE-4.2C EngineDefault, PluginBypass, AVBOITUnlit, and debug-buffer evidence."),
		FConsoleCommandWithArgsDelegate::CreateStatic(&CommandCaptureParity),
		ECVF_Default));

	RegisteredCommands.Add(ConsoleManager.RegisterConsoleCommand(
		TEXT("AVBOIT.Niagara.Mode"),
		TEXT("Runtime switch for UE-4.2C Niagara parity. Usage: AVBOIT.Niagara.Mode EngineDefault|AVBOITUnlit"),
		FConsoleCommandWithArgsDelegate::CreateStatic(&CommandMode),
		ECVF_Default));

	RegisteredCommands.Add(ConsoleManager.RegisterConsoleCommand(
		TEXT("AVBOIT.Niagara.ToggleDefaultPlugin"),
		TEXT("Toggles runtime rendering between EngineDefault and AVBOITUnlit while preserving the fixed camera and Engine Unlit view mode."),
		FConsoleCommandWithArgsDelegate::CreateStatic(&CommandToggleDefaultPlugin),
		ECVF_Default));

	RegisteredCommands.Add(ConsoleManager.RegisterConsoleCommand(
		TEXT("AVBOIT.Niagara.Status"),
		TEXT("Logs or writes the current Niagara parity mode, CVars, view-mode contract, and AVBOIT draw status."),
		FConsoleCommandWithArgsDelegate::CreateStatic(&CommandStatus),
		ECVF_Default));

	RegisteredCommands.Add(ConsoleManager.RegisterConsoleCommand(
		TEXT("AVBOIT.Niagara.ShowOverlay"),
		TEXT("Shows the interactive viewport dropdown for Engine Default vs Plugin AVBOIT Unlit rendering."),
		FConsoleCommandWithArgsDelegate::CreateStatic(&CommandShowOverlay),
		ECVF_Default));

	RegisteredCommands.Add(ConsoleManager.RegisterConsoleCommand(
		TEXT("AVBOIT.Niagara.HideOverlay"),
		TEXT("Hides the interactive viewport dropdown so automated screenshots stay clean."),
		FConsoleCommandWithArgsDelegate::CreateStatic(&CommandHideOverlay),
		ECVF_Default));

	RegisteredCommands.Add(ConsoleManager.RegisterConsoleCommand(
		TEXT("AVBOIT.Niagara.Interactive"),
		TEXT("Starts a non-exiting interactive TestSpriteMap1 session with transient conversion, fixed camera, and Engine Unlit mode."),
		FConsoleCommandWithArgsDelegate::CreateStatic(&CommandInteractive),
		ECVF_Default));
}

void FAVBOITNiagaraValidationCommands::UnregisterCommands()
{
	HideInteractiveOverlay();

	IConsoleManager& ConsoleManager = IConsoleManager::Get();
	for (IConsoleObject* Command : RegisteredCommands)
	{
		if (Command)
		{
			ConsoleManager.UnregisterConsoleObject(Command);
		}
	}
	RegisteredCommands.Reset();
	GActiveCapture.Reset();
}
