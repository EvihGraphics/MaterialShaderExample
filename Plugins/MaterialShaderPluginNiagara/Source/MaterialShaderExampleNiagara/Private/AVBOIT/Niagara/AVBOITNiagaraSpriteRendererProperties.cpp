#include "AVBOIT/Niagara/AVBOITNiagaraSpriteRendererProperties.h"

#include "AVBOIT/Niagara/AVBOITNiagaraRenderer.h"
#include "Materials/MaterialInstanceConstant.h"
#include "NiagaraEmitterInstance.h"
#include "NiagaraSystemInstanceController.h"

namespace
{
	const FName TintParameterNames[] = {
		TEXT("Color"),
		TEXT("TintColor"),
		TEXT("BaseColor"),
		TEXT("ParticleColor")
	};

	void SetVectorOverride(UMaterialInstanceConstant* MaterialInstance, const FName& ParameterName, const FLinearColor& Value)
	{
		if (!MaterialInstance)
		{
			return;
		}

		const FMaterialParameterInfo ParameterInfo(ParameterName);
		if (FVectorParameterValue* ExistingValue = MaterialInstance->VectorParameterValues.FindByPredicate(
			[&ParameterInfo](const FVectorParameterValue& Candidate)
			{
				return Candidate.ParameterInfo == ParameterInfo;
			}))
		{
			ExistingValue->ParameterValue = Value;
			return;
		}

		MaterialInstance->VectorParameterValues.Add(FVectorParameterValue(ParameterInfo, Value));
	}
}

FNiagaraRenderer* UNiagaraAVBOITSpriteRendererProperties::CreateEmitterRenderer(
	ERHIFeatureLevel::Type FeatureLevel,
	const FNiagaraEmitterInstance* Emitter,
	const FNiagaraSystemInstanceController& InController)
{
	CaptureRuntimeSourceMaterial();
	FNiagaraRenderer* Renderer = new FNiagaraRendererAVBOITSprites(FeatureLevel, this, Emitter);
	Renderer->Initialize(this, Emitter, InController);
	return Renderer;
}

void UNiagaraAVBOITSpriteRendererProperties::CaptureRuntimeSourceMaterial()
{
	if (!AVBOITRuntimeSourceMaterial && Material != AVBOITRuntimeTintMaterial)
	{
		AVBOITRuntimeSourceMaterial = Material;
	}
}

bool UNiagaraAVBOITSpriteRendererProperties::ApplyRuntimeTintMaterial(const FLinearColor& TintColor)
{
	CaptureRuntimeSourceMaterial();
	const bool bChanged = bAVBOITRuntimeTintMaterialActive || AVBOITRuntimeTintColor != TintColor;
	if (AVBOITRuntimeSourceMaterial && Material == AVBOITRuntimeTintMaterial)
	{
		Material = AVBOITRuntimeSourceMaterial;
	}
	AVBOITRuntimeTintColor = TintColor;
	bAVBOITRuntimeTintMaterialActive = false;
	return bChanged;
}

bool UNiagaraAVBOITSpriteRendererProperties::ClearRuntimeTintMaterial()
{
	const bool bChanged = bAVBOITRuntimeTintMaterialActive || Material == AVBOITRuntimeTintMaterial;
	if (AVBOITRuntimeSourceMaterial)
	{
		Material = AVBOITRuntimeSourceMaterial;
	}

	bAVBOITRuntimeTintMaterialActive = false;
	return bChanged;
}

bool UNiagaraAVBOITSpriteRendererProperties::IsRuntimeTintMaterialActive() const
{
	return bAVBOITRuntimeTintMaterialActive && Material == AVBOITRuntimeTintMaterial;
}
