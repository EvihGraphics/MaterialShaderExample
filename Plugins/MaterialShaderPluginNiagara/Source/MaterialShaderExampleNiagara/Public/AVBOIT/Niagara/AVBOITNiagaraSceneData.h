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
};

class MATERIALSHADEREXAMPLENIAGARA_API FAVBOITNiagaraSceneData
{
public:
	static FAVBOITNiagaraSceneData& Get();

	void Reset_RenderThread();
	void BeginFrame_RenderThread(uint32 InFrameNumber);
	void RegisterDraw_RenderThread(const FAVBOITNiagaraSpriteDrawData& DrawData);
	void MarkPassesScheduled_RenderThread();

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
