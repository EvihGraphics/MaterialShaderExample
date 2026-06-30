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
	DrawData.bTintVisibleFallbackDrawUsed = false;
	DrawData.bDefaultDrawSuppressed = true;
	DrawData.bDefaultNiagaraFallbackUsed = false;
	DrawData.bHasParticleBuffer = ParticleData != nullptr;
	DrawData.bHasMaterialContract = !MaterialPath.IsEmpty();
	DrawData.bHasVertexFactoryContract = false;
	DrawData.bRealAVBOITDrawPacket = false;
	DrawData.KnownBlockingApi = TEXT("Plugin-first UE-4.2D bridge has Niagara particle counts and renderer/material metadata, but no public Niagara sprite material/VF draw packet hook yet.");

	uint32 PacketHash = 0;
	PacketHash = HashDrawPacketField(PacketHash, DrawData.SystemName);
	PacketHash = HashDrawPacketField(PacketHash, DrawData.MaterialPath);
	PacketHash = HashDrawPacketField(PacketHash, DrawData.ParticleCount);
	PacketHash = HashDrawPacketField(PacketHash, DrawData.FacingMode);
	PacketHash = HashDrawPacketField(PacketHash, DrawData.Alignment);
	PacketHash = HashDrawPacketField(PacketHash, DrawData.SortMode);
	PacketHash = HashDrawPacketFloat(PacketHash, DrawData.SubImageSize.X);
	PacketHash = HashDrawPacketFloat(PacketHash, DrawData.SubImageSize.Y);
	DrawData.ParticleStateHash = PacketHash;
	DrawData.ParticleStateHashString = FString::Printf(TEXT("0x%08x"), PacketHash);

	FAVBOITNiagaraSceneData::Get().RegisterDraw_RenderThread(DrawData);
}
