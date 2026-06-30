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
	LastCompletedStats = ActiveStats;
	LastCompletedDraws = ActiveDraws;
}

void FAVBOITNiagaraSceneData::MarkPassesScheduled_RenderThread()
{
	check(IsInRenderingThread() || IsInParallelRenderingThread());

	FScopeLock Lock(&CriticalSection);
	ActiveStats.bClearPassScheduled = true;
	ActiveStats.bSpriteSplatPassScheduled = true;
	ActiveStats.bIntegratePassScheduled = true;
	ActiveStats.bForwardUnlitPassScheduled = true;
	ActiveStats.bCompositePassScheduled = true;
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
