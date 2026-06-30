#include "AVBOIT/Niagara/AVBOITNiagaraRenderer.h"

#include "AVBOIT/Niagara/AVBOITNiagaraCVars.h"
#include "AVBOIT/Niagara/AVBOITNiagaraSceneData.h"
#include "AVBOIT/Niagara/AVBOITNiagaraSpriteRendererProperties.h"
#include "NiagaraDataSet.h"
#include "NiagaraRenderer.h"
#include "NiagaraSceneProxy.h"
#include "SceneView.h"

FNiagaraRendererAVBOITSprites::FNiagaraRendererAVBOITSprites(
	ERHIFeatureLevel::Type FeatureLevel,
	const UNiagaraAVBOITSpriteRendererProperties* InProps,
	const FNiagaraEmitterInstance* Emitter)
	: FNiagaraRendererSprites(FeatureLevel, InProps, Emitter)
{
	if (InProps)
	{
		MaterialPath = GetPathNameSafe(InProps->Material);
		FacingMode = InProps->FacingMode;
		Alignment = InProps->Alignment;
		SortMode = InProps->SortMode;
		SubImageSize = FVector2f(static_cast<float>(InProps->SubImageSize.X), static_cast<float>(InProps->SubImageSize.Y));
		bSubImageBlend = InProps->bSubImageBlend != 0;
	}
}

void FNiagaraRendererAVBOITSprites::GetDynamicMeshElements(
	const TArray<const FSceneView*>& Views,
	const FSceneViewFamily& ViewFamily,
	uint32 VisibilityMap,
	FMeshElementCollector& Collector,
	const FNiagaraSceneProxy* SceneProxy) const
{
	if (!AVBOITNiagara::IsAVBOITEnabled() || !AVBOITNiagara::IsNiagaraEnabled())
	{
		FNiagaraRendererSprites::GetDynamicMeshElements(Views, ViewFamily, VisibilityMap, Collector, SceneProxy);
		return;
	}

	FNiagaraDynamicDataBase* DynamicData = GetDynamicData();
	if (!DynamicData)
	{
		return;
	}

	FNiagaraDataBuffer* ParticleData = DynamicData->GetParticleDataToRender(Collector.GetRHICommandList());
	const uint32 ParticleCount = ParticleData ? ParticleData->GetNumInstances() : 0;
	if (ParticleCount == 0)
	{
		return;
	}

	FAVBOITNiagaraSpriteDrawData DrawData;
	DrawData.FrameNumber = ViewFamily.FrameNumber;
	DrawData.SystemName = SceneProxy ? SceneProxy->GetResourceName().ToString() : FString();
	DrawData.ComponentName = DrawData.SystemName;
	DrawData.RendererName = TEXT("UNiagaraAVBOITSpriteRendererProperties");
	DrawData.MaterialPath = MaterialPath;
	DrawData.ParticleCount = ParticleCount;
	DrawData.SimTarget = GetSimTarget();
	DrawData.FacingMode = static_cast<int32>(FacingMode);
	DrawData.Alignment = static_cast<int32>(Alignment);
	DrawData.SortMode = static_cast<int32>(SortMode);
	DrawData.SubImageSize = SubImageSize;
	DrawData.bSubImageBlend = bSubImageBlend;
	DrawData.bTintEnabled = AVBOITNiagara::IsTintEnabled();
	DrawData.TintColor = AVBOITNiagara::GetTintColor();
	DrawData.bTintVisibleFallbackDrawUsed = DrawData.bTintEnabled;
	DrawData.bDefaultDrawSuppressed = !DrawData.bTintVisibleFallbackDrawUsed;

	FAVBOITNiagaraSceneData::Get().RegisterDraw_RenderThread(DrawData);

	if (DrawData.bTintVisibleFallbackDrawUsed)
	{
		FNiagaraRendererSprites::GetDynamicMeshElements(Views, ViewFamily, VisibilityMap, Collector, SceneProxy);
	}
}
