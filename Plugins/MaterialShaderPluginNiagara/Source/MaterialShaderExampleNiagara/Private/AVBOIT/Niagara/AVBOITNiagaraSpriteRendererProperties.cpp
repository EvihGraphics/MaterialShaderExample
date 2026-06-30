#include "AVBOIT/Niagara/AVBOITNiagaraSpriteRendererProperties.h"

#include "AVBOIT/Niagara/AVBOITNiagaraRenderer.h"
#include "NiagaraEmitterInstance.h"
#include "NiagaraSystemInstanceController.h"

FNiagaraRenderer* UNiagaraAVBOITSpriteRendererProperties::CreateEmitterRenderer(
	ERHIFeatureLevel::Type FeatureLevel,
	const FNiagaraEmitterInstance* Emitter,
	const FNiagaraSystemInstanceController& InController)
{
	FNiagaraRenderer* Renderer = new FNiagaraRendererAVBOITSprites(FeatureLevel, this, Emitter);
	Renderer->Initialize(this, Emitter, InController);
	return Renderer;
}
