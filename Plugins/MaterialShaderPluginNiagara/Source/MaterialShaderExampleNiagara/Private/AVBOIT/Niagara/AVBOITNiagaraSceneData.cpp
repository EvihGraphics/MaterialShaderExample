#include "AVBOIT/Niagara/AVBOITNiagaraSceneData.h"

FAVBOITNiagaraSceneData& FAVBOITNiagaraSceneData::Get()
{
	static FAVBOITNiagaraSceneData Instance;
	return Instance;
}

void FAVBOITNiagaraSceneData::Reset_RenderThread()
{
	check(IsInRenderingThread() || IsInParallelRenderingThread());

	FScopeLock Lock(&CriticalSection);
	ActiveFrameNumber = 0;
	ActiveStats = FAVBOITNiagaraFrameStats();
	ActiveDraws.Reset();
	LastCompletedStats = FAVBOITNiagaraFrameStats();
	LastCompletedDraws.Reset();
}

void FAVBOITNiagaraSceneData::BeginFrame_RenderThread(uint32 InFrameNumber)
{
	check(IsInRenderingThread() || IsInParallelRenderingThread());

	FScopeLock Lock(&CriticalSection);
	if (ActiveFrameNumber == InFrameNumber)
	{
		return;
	}

	if (ActiveFrameNumber != 0)
	{
		LastCompletedStats = ActiveStats;
		LastCompletedDraws = ActiveDraws;
	}

	ActiveFrameNumber = InFrameNumber;
	ActiveStats = FAVBOITNiagaraFrameStats();
	ActiveStats.FrameNumber = InFrameNumber;
	ActiveDraws.Reset();
}

void FAVBOITNiagaraSceneData::RegisterDraw_RenderThread(const FAVBOITNiagaraSpriteDrawData& DrawData)
{
	check(IsInRenderingThread() || IsInParallelRenderingThread());

	FScopeLock Lock(&CriticalSection);
	if (ActiveFrameNumber == 0 || ActiveFrameNumber != DrawData.FrameNumber)
	{
		ActiveFrameNumber = DrawData.FrameNumber;
		ActiveStats = FAVBOITNiagaraFrameStats();
		ActiveStats.FrameNumber = DrawData.FrameNumber;
		ActiveDraws.Reset();
	}

	ActiveDraws.Add(DrawData);
	ActiveStats.SpriteDrawCount++;
	ActiveStats.ParticleCount += static_cast<int32>(DrawData.ParticleCount);
	ActiveStats.bRealAVBOITDraw |= DrawData.bRealAVBOITDrawPacket;
	ActiveStats.bDefaultNiagaraFallbackUsed |= DrawData.bDefaultNiagaraFallbackUsed || DrawData.bTintVisibleFallbackDrawUsed;
	ActiveStats.ParticleStateHash.Hash = HashCombine(ActiveStats.ParticleStateHash.Hash, DrawData.ParticleStateHash);
	ActiveStats.ParticleStateHash.DrawCount = ActiveStats.SpriteDrawCount;
	ActiveStats.ParticleStateHash.ParticleCount = ActiveStats.ParticleCount;
	ActiveStats.ParticleStateHash.HashString = FString::Printf(TEXT("0x%08x"), ActiveStats.ParticleStateHash.Hash);
	ActiveStats.RendererMetadataHash.Hash = HashCombine(ActiveStats.RendererMetadataHash.Hash, DrawData.RendererMetadataHash);
	ActiveStats.RendererMetadataHash.DrawCount = ActiveStats.SpriteDrawCount;
	ActiveStats.RendererMetadataHash.ParticleCount = ActiveStats.ParticleCount;
	ActiveStats.RendererMetadataHash.HashString = FString::Printf(TEXT("0x%08x"), ActiveStats.RendererMetadataHash.Hash);
	if (DrawData.bParticleAttributeHashComplete)
	{
		ActiveStats.ParticleAttributeHash.Hash = HashCombine(ActiveStats.ParticleAttributeHash.Hash, DrawData.ParticleAttributeHash);
		ActiveStats.ParticleAttributeHash.DrawCount = ActiveStats.SpriteDrawCount;
		ActiveStats.ParticleAttributeHash.ParticleCount = ActiveStats.ParticleCount;
		ActiveStats.ParticleAttributeHash.HashString = FString::Printf(TEXT("0x%08x"), ActiveStats.ParticleAttributeHash.Hash);
		ActiveStats.ParticleAttributeHash.bDeterministicStateVerified = true;
	}
	LastCompletedStats = ActiveStats;
	LastCompletedDraws = ActiveDraws;
}

void FAVBOITNiagaraSceneData::MarkPassesScheduled_RenderThread(
	bool bCompositeWritesSceneColor,
	bool bBufferOverviewScheduled,
	bool bTintConsumedInForwardShader)
{
	check(IsInRenderingThread() || IsInParallelRenderingThread());

	FScopeLock Lock(&CriticalSection);
	ActiveStats.bClearPassScheduled = true;
	ActiveStats.bSpriteSplatPassScheduled = true;
	ActiveStats.bIntegratePassScheduled = true;
	ActiveStats.bForwardUnlitPassScheduled = true;
	ActiveStats.bCompositePassScheduled = true;
	ActiveStats.bBufferOverviewPassScheduled = bBufferOverviewScheduled;
	ActiveStats.bCompositeWritesSceneColor = bCompositeWritesSceneColor;
	ActiveStats.bTintConsumedInForwardShader = bTintConsumedInForwardShader;
	LastCompletedStats = ActiveStats;
	LastCompletedDraws = ActiveDraws;
}

void FAVBOITNiagaraSceneData::MarkFrameGraphResources_RenderThread(const FAVBOITBufferReadbackStats& Stats)
{
	check(IsInRenderingThread() || IsInParallelRenderingThread());

	FScopeLock Lock(&CriticalSection);
	ActiveStats.BufferReadbackStats = Stats;
	LastCompletedStats = ActiveStats;
	LastCompletedDraws = ActiveDraws;
}

FAVBOITNiagaraFrameStats FAVBOITNiagaraSceneData::GetLastCompletedStats() const
{
	FScopeLock Lock(&CriticalSection);
	return LastCompletedStats;
}

TArray<FAVBOITNiagaraSpriteDrawData> FAVBOITNiagaraSceneData::GetLastCompletedDraws() const
{
	FScopeLock Lock(&CriticalSection);
	return LastCompletedDraws;
}
