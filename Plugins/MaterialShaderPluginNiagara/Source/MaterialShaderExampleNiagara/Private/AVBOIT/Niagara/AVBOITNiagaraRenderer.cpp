#include "AVBOIT/Niagara/AVBOITNiagaraRenderer.h"

#include "AVBOIT/Niagara/AVBOITNiagaraCVars.h"
#include "AVBOIT/Niagara/AVBOITNiagaraSceneData.h"
#include "AVBOIT/Niagara/AVBOITNiagaraSpriteRendererProperties.h"
#include "NiagaraDataSet.h"
#include "NiagaraRenderer.h"
#include "NiagaraSceneProxy.h"
#include "SceneView.h"

namespace
{
	const TCHAR* NiagaraSpritePrivateApiBlocker =
		TEXT("UE-4.2E plugin-first adapter is blocked by Niagara private sprite draw internals: ")
		TEXT("FNiagaraDynamicDataSprites, PrepareParticleSpriteRenderData, PrepareParticleRenderBuffers, ")
		TEXT("SetupVertexFactory, CreateViewUniformBuffer, and FNiagaraRendererSprites::CreateMeshBatchForView ")
		TEXT("are implemented in NiagaraRendererSprites.cpp without a public hook for external real VF/MaterialRenderProxy/FMeshBatch reuse.");

	uint32 HashDrawPacketField(uint32 Seed, const FString& Value)
	{
		return HashCombine(Seed, GetTypeHash(Value));
	}

	uint32 HashDrawPacketField(uint32 Seed, uint32 Value)
	{
		return HashCombine(Seed, Value);
	}

	uint32 HashDrawPacketField(uint32 Seed, int32 Value)
	{
		return HashCombine(Seed, static_cast<uint32>(Value));
	}

	uint32 HashDrawPacketFloat(uint32 Seed, float Value)
	{
		return HashCombine(Seed, static_cast<uint32>(FMath::RoundToInt(Value * 1000.0f)));
	}
}

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
	const bool bUseTintVisibleFallbackDraw = DrawData.bTintEnabled;
	DrawData.bTintVisibleFallbackDrawUsed = bUseTintVisibleFallbackDraw;
	DrawData.bDefaultDrawSuppressed = !bUseTintVisibleFallbackDraw;
	DrawData.bDefaultNiagaraFallbackUsed = bUseTintVisibleFallbackDraw;
	DrawData.bHasParticleBuffer = ParticleData != nullptr;
	DrawData.bHasMaterialContract = !MaterialPath.IsEmpty();
	DrawData.bHasVertexFactoryContract = false;
	DrawData.bHasMaterialRenderProxy = false;
	DrawData.bHasMeshBatch = false;
	DrawData.bCanRenderIdentity = false;
	DrawData.bCanRenderAVBOIT = false;
	DrawData.bPrivateApiBlocked = true;
	DrawData.bRealAVBOITDrawPacket = false;
	DrawData.KnownBlockingApi = NiagaraSpritePrivateApiBlocker;
	DrawData.BlockingReasons.Add(DrawData.KnownBlockingApi);
	DrawData.BlockingReasons.Add(TEXT("Patches/UE57/NiagaraAVBOITMinimalHook.patch is required before PluginIdentity or PluginAVBOIT can prove real SceneColor draws."));
	if (bUseTintVisibleFallbackDraw)
	{
		DrawData.BlockingReasons.Add(TEXT("UE-4.2F ROI tint visual gate uses the default Niagara sprite draw only as a visible red-tint preview while the real plugin-only sprite draw hook remains blocked."));
	}

	uint32 MetadataHash = 0;
	MetadataHash = HashDrawPacketField(MetadataHash, DrawData.SystemName);
	MetadataHash = HashDrawPacketField(MetadataHash, DrawData.MaterialPath);
	MetadataHash = HashDrawPacketField(MetadataHash, DrawData.ParticleCount);
	MetadataHash = HashDrawPacketField(MetadataHash, DrawData.FacingMode);
	MetadataHash = HashDrawPacketField(MetadataHash, DrawData.Alignment);
	MetadataHash = HashDrawPacketField(MetadataHash, DrawData.SortMode);
	MetadataHash = HashDrawPacketFloat(MetadataHash, DrawData.SubImageSize.X);
	MetadataHash = HashDrawPacketFloat(MetadataHash, DrawData.SubImageSize.Y);
	DrawData.RendererMetadataHash = MetadataHash;
	DrawData.RendererMetadataHashString = FString::Printf(TEXT("0x%08x"), MetadataHash);
	DrawData.ParticleStateHash = MetadataHash;
	DrawData.ParticleStateHashString = DrawData.RendererMetadataHashString;
	DrawData.ParticleAttributeHash = 0;
	DrawData.ParticleAttributeHashString = TEXT("unavailable-plugin-only-private-api-blocked");
	DrawData.bParticleAttributeHashComplete = false;

	FAVBOITNiagaraSceneData::Get().RegisterDraw_RenderThread(DrawData);

	if (bUseTintVisibleFallbackDraw)
	{
		FNiagaraRendererSprites::GetDynamicMeshElements(Views, ViewFamily, VisibilityMap, Collector, SceneProxy);
	}
}
