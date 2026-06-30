#pragma once

#include "CoreMinimal.h"
#include "NiagaraCommon.h"

struct FAVBOITNiagaraSpriteDrawData
{
	uint32 FrameNumber = 0;
	FString SystemName;
	FString ComponentName;
	FString RendererName;
	FString MaterialPath;
	uint32 ParticleCount = 0;
	ENiagaraSimTarget SimTarget = ENiagaraSimTarget::CPUSim;
	int32 FacingMode = 0;
	int32 Alignment = 0;
	int32 SortMode = 0;
	FVector2f SubImageSize = FVector2f(1.0f, 1.0f);
	bool bSubImageBlend = false;
	bool bDefaultDrawSuppressed = false;
	bool bTintEnabled = false;
	bool bTintVisibleFallbackDrawUsed = false;
	bool bDefaultNiagaraFallbackUsed = false;
	bool bRealAVBOITDrawPacket = false;
	bool bHasParticleBuffer = false;
	bool bHasMaterialContract = false;
	bool bHasVertexFactoryContract = false;
	uint32 ParticleStateHash = 0;
	FString ParticleStateHashString;
	FString KnownBlockingApi;
	FLinearColor TintColor = FLinearColor::White;
};

struct FAVBOITBufferReadbackStats
{
	bool bFrameGraphResourcesAllocated = false;
	bool bCompositeWritesSceneColor = false;
	bool bExtinctionNonZero = false;
	bool bTransmittanceBelowOne = false;
	bool bAccumulationAlphaNonZero = false;
	int32 ExtinctionNonZeroVoxelCount = 0;
	float TransmittanceMinimum = 1.0f;
	float AccumulationAlphaSum = 0.0f;
	int32 CompositeChangedPixelCount = 0;
};

struct FAVBOITParticleStateHash
{
	uint32 Hash = 0;
	FString HashString;
	int32 DrawCount = 0;
	int32 ParticleCount = 0;
	bool bDeterministicStateVerified = false;
};

struct FAVBOITNiagaraFrameStats
{
	uint32 FrameNumber = 0;
	int32 SpriteDrawCount = 0;
	int32 ParticleCount = 0;
	bool bClearPassScheduled = false;
	bool bSpriteSplatPassScheduled = false;
	bool bIntegratePassScheduled = false;
	bool bForwardUnlitPassScheduled = false;
	bool bCompositePassScheduled = false;
	bool bBufferOverviewPassScheduled = false;
	bool bRealAVBOITDraw = false;
	bool bTintConsumedInForwardShader = false;
	bool bDefaultNiagaraFallbackUsed = false;
	bool bCompositeWritesSceneColor = false;
	FAVBOITBufferReadbackStats BufferReadbackStats;
	FAVBOITParticleStateHash ParticleStateHash;
};

class MATERIALSHADEREXAMPLENIAGARA_API FAVBOITNiagaraSceneData
{
public:
	static FAVBOITNiagaraSceneData& Get();

	void Reset_RenderThread();
	void BeginFrame_RenderThread(uint32 InFrameNumber);
	void RegisterDraw_RenderThread(const FAVBOITNiagaraSpriteDrawData& DrawData);
	void MarkPassesScheduled_RenderThread(bool bCompositeWritesSceneColor, bool bBufferOverviewScheduled, bool bTintConsumedInForwardShader);
	void MarkFrameGraphResources_RenderThread(const FAVBOITBufferReadbackStats& Stats);

	FAVBOITNiagaraFrameStats GetLastCompletedStats() const;
	TArray<FAVBOITNiagaraSpriteDrawData> GetLastCompletedDraws() const;

private:
	mutable FCriticalSection CriticalSection;
	uint32 ActiveFrameNumber = 0;
	FAVBOITNiagaraFrameStats ActiveStats;
	TArray<FAVBOITNiagaraSpriteDrawData> ActiveDraws;
	FAVBOITNiagaraFrameStats LastCompletedStats;
	TArray<FAVBOITNiagaraSpriteDrawData> LastCompletedDraws;
};
