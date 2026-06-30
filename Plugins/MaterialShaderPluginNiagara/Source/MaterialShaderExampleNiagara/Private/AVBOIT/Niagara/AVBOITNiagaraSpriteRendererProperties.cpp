#include "AVBOIT/Niagara/AVBOITNiagaraSpriteRendererProperties.h"

#include "AVBOIT/Niagara/AVBOITNiagaraRenderer.h"
#include "Materials/MaterialInstanceDynamic.h"
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

	UMaterialInterface* SourceMaterial = AVBOITRuntimeSourceMaterial.Get();
	if (!SourceMaterial)
	{
		SourceMaterial = Material;
		AVBOITRuntimeSourceMaterial = SourceMaterial;
	}

	if (!SourceMaterial)
	{
		bAVBOITRuntimeTintMaterialActive = false;
		return false;
	}

	const bool bNeedsNewMaterial = !AVBOITRuntimeTintMaterial;
	if (bNeedsNewMaterial)
	{
		AVBOITRuntimeTintMaterial = UMaterialInstanceDynamic::Create(SourceMaterial, this);
	}

	if (!AVBOITRuntimeTintMaterial)
	{
		bAVBOITRuntimeTintMaterialActive = false;
		return false;
	}

	for (const FName& ParameterName : TintParameterNames)
	{
		AVBOITRuntimeTintMaterial->SetVectorParameterValue(ParameterName, TintColor);
	}

	const bool bChanged =
		Material != AVBOITRuntimeTintMaterial ||
		!bAVBOITRuntimeTintMaterialActive ||
		AVBOITRuntimeTintColor != TintColor ||
		bNeedsNewMaterial;

	Material = AVBOITRuntimeTintMaterial;
	AVBOITRuntimeTintColor = TintColor;
	bAVBOITRuntimeTintMaterialActive = true;
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
