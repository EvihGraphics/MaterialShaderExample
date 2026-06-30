#pragma once

#include "CoreMinimal.h"
#include "NiagaraSpriteRendererProperties.h"
#include "AVBOITNiagaraSpriteRendererProperties.generated.h"

class FNiagaraEmitterInstance;
class FNiagaraRenderer;
class FNiagaraSystemInstanceController;
class UMaterialInstanceDynamic;
class UMaterialInterface;

UCLASS(EditInlineNew, meta = (DisplayName = "AVBOIT Sprite Renderer", SupportsStateless), MinimalAPI)
class UNiagaraAVBOITSpriteRendererProperties : public UNiagaraSpriteRendererProperties
{
	GENERATED_BODY()

public:
	MATERIALSHADEREXAMPLENIAGARA_API virtual FNiagaraRenderer* CreateEmitterRenderer(
		ERHIFeatureLevel::Type FeatureLevel,
		const FNiagaraEmitterInstance* Emitter,
		const FNiagaraSystemInstanceController& InController) override;

	MATERIALSHADEREXAMPLENIAGARA_API void CaptureRuntimeSourceMaterial();
	MATERIALSHADEREXAMPLENIAGARA_API bool ApplyRuntimeTintMaterial(const FLinearColor& TintColor);
	MATERIALSHADEREXAMPLENIAGARA_API bool ClearRuntimeTintMaterial();
	MATERIALSHADEREXAMPLENIAGARA_API bool IsRuntimeTintMaterialActive() const;

private:
	UPROPERTY(Transient)
	TObjectPtr<UMaterialInterface> AVBOITRuntimeSourceMaterial;

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> AVBOITRuntimeTintMaterial;

	UPROPERTY(Transient)
	FLinearColor AVBOITRuntimeTintColor = FLinearColor::Transparent;

	UPROPERTY(Transient)
	bool bAVBOITRuntimeTintMaterialActive = false;
};
