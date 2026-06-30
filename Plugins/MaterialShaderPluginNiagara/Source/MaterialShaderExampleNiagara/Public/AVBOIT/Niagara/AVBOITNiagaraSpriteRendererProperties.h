#pragma once

#include "CoreMinimal.h"
#include "NiagaraSpriteRendererProperties.h"
#include "AVBOITNiagaraSpriteRendererProperties.generated.h"

class FNiagaraEmitterInstance;
class FNiagaraRenderer;
class FNiagaraSystemInstanceController;

UCLASS(EditInlineNew, meta = (DisplayName = "AVBOIT Sprite Renderer", SupportsStateless), MinimalAPI)
class UNiagaraAVBOITSpriteRendererProperties : public UNiagaraSpriteRendererProperties
{
	GENERATED_BODY()

public:
	MATERIALSHADEREXAMPLENIAGARA_API virtual FNiagaraRenderer* CreateEmitterRenderer(
		ERHIFeatureLevel::Type FeatureLevel,
		const FNiagaraEmitterInstance* Emitter,
		const FNiagaraSystemInstanceController& InController) override;
};
