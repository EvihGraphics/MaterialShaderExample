#pragma once

#include "CoreMinimal.h"

namespace AVBOIT::Core
{
	enum class ERuntimeMode : uint8
	{
		EngineDefault,
		UESortedPixelsOIT,
		PluginIdentity,
		PluginAVBOIT,
		ExactReference,
		BufferOverview
	};

	enum class EFoundationScene : uint8
	{
		SingleLayerIdentity,
		TwoIntersectingQuads,
		ThreeLayerStress
	};

	enum class EFoundationSubmissionOrder : uint8
	{
		AB,
		BA,
		ABC,
		CBA,
		RandomSeed1,
		RandomSeed2,
		RandomSeed3
	};

	enum class EStageStatus : uint8
	{
		Partial,
		BlockedLocal,
		Failed,
		PassedLocal
	};

	struct FDepthSliceContract
	{
		FString DepthMapping = TEXT("reverse_correct");
		FString TransmittanceDirection = TEXT("front");
		int32 DownsampleFactor = 8;
		int32 SliceCount = 64;
		float BrightnessMultiplier = 1.0f;
		bool bExposureOrGammaBoostAllowed = false;
	};

	struct FFrameGraphContract
	{
		bool bClear = true;
		bool bSpriteSplat = true;
		bool bIntegrate = true;
		bool bForwardUnlit = true;
		bool bComposite = true;
		bool bCompositeWritesSceneColor = false;
		bool bUsesScratchOnlyComposite = true;
	};

	struct FPrimitiveMaterialContract
	{
		FString MaterialPath;
		bool bHasMaterialRenderProxy = false;
		bool bUnlit = false;
		bool bUsesOpacity = false;
		bool bUsesParticleColor = false;
	};

	struct FPrimitiveDrawPacket
	{
		FString ProviderName;
		FString ComponentName;
		FString RendererName;
		uint32 ParticleCount = 0;
		bool bHasParticleBuffer = false;
		bool bHasVertexFactory = false;
		bool bHasMaterialRenderProxy = false;
		bool bHasMeshBatch = false;
		bool bCanRenderIdentity = false;
		bool bCanRenderAVBOIT = false;
		FString RendererMetadataHash;
		FString ParticleAttributeHash;
		TArray<FString> BlockingReasons;
	};

	struct FBufferReadbackStats
	{
		bool bFrameGraphResourcesAllocated = false;
		bool bExtinctionNonZero = false;
		bool bTransmittanceBelowOne = false;
		bool bAccumulationAlphaNonZero = false;
		bool bCompositeChangedSceneColor = false;
		int32 ExtinctionNonZeroVoxelCount = 0;
		float TransmittanceMinimum = 1.0f;
		float AccumulationAlphaSum = 0.0f;
		int32 CompositeChangedPixelCount = 0;
	};

	struct FPromotionGateState
	{
		EStageStatus Status = EStageStatus::Partial;
		bool bPromotionEligible = false;
		TArray<FString> BlockingReasons;
	};

	inline const TCHAR* LexToString(ERuntimeMode Mode)
	{
		switch (Mode)
		{
		case ERuntimeMode::EngineDefault:
			return TEXT("EngineDefault");
		case ERuntimeMode::UESortedPixelsOIT:
			return TEXT("UESortedPixelsOIT");
		case ERuntimeMode::PluginIdentity:
			return TEXT("PluginIdentity");
		case ERuntimeMode::PluginAVBOIT:
			return TEXT("PluginAVBOIT");
		case ERuntimeMode::ExactReference:
			return TEXT("ExactReference");
		case ERuntimeMode::BufferOverview:
			return TEXT("BufferOverview");
		default:
			return TEXT("Unknown");
		}
	}

	inline const TCHAR* LexToString(EStageStatus Status)
	{
		switch (Status)
		{
		case EStageStatus::Partial:
			return TEXT("partial");
		case EStageStatus::BlockedLocal:
			return TEXT("blocked-local");
		case EStageStatus::Failed:
			return TEXT("failed");
		case EStageStatus::PassedLocal:
			return TEXT("passed-local");
		default:
			return TEXT("partial");
		}
	}
}
