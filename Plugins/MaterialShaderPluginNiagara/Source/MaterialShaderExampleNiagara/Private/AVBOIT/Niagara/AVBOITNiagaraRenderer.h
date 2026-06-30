#pragma once

#include "CoreMinimal.h"
#include "NiagaraRendererSprites.h"

class FNiagaraEmitterInstance;
class UNiagaraAVBOITSpriteRendererProperties;

class FNiagaraRendererAVBOITSprites final : public FNiagaraRendererSprites
{
public:
	FNiagaraRendererAVBOITSprites(
		ERHIFeatureLevel::Type FeatureLevel,
		const UNiagaraAVBOITSpriteRendererProperties* InProps,
		const FNiagaraEmitterInstance* Emitter);

	virtual void GetDynamicMeshElements(
		const TArray<const FSceneView*>& Views,
		const FSceneViewFamily& ViewFamily,
		uint32 VisibilityMap,
		FMeshElementCollector& Collector,
		const FNiagaraSceneProxy* SceneProxy) const override;

private:
	FString MaterialPath;
	ENiagaraSpriteFacingMode FacingMode = ENiagaraSpriteFacingMode::Automatic;
	ENiagaraSpriteAlignment Alignment = ENiagaraSpriteAlignment::Automatic;
	ENiagaraSortMode SortMode = ENiagaraSortMode::None;
	FVector2f SubImageSize = FVector2f(1.0f, 1.0f);
	bool bSubImageBlend = false;
};
