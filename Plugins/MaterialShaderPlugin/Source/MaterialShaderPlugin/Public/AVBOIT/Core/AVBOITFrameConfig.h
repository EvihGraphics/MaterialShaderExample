#pragma once

#include "CoreMinimal.h"

struct FAVBOITFrameConfig
{
	float NearDepthCm = 100.0f;
	float FarDepthCm = 400000.0f;
	uint32 DownsampleFactor = 8;
	uint32 NumSlices = 64;
	FIntRect ViewRect = FIntRect(0, 0, 0, 0);
	FIntPoint TextureExtent = FIntPoint::ZeroValue;
	FIntPoint VolumeExtent = FIntPoint::ZeroValue;

	bool IsValid() const
	{
		return NearDepthCm > 0.0f
			&& FarDepthCm > NearDepthCm
			&& DownsampleFactor > 0
			&& NumSlices > 0
			&& ViewRect.Width() > 0
			&& ViewRect.Height() > 0
			&& TextureExtent.X > 0
			&& TextureExtent.Y > 0
			&& VolumeExtent.X > 0
			&& VolumeExtent.Y > 0;
	}

	static FAVBOITFrameConfig Build(
		const FIntRect& InViewRect,
		const FIntPoint& InTextureExtent,
		float InNearDepthCm = 100.0f,
		float InFarDepthCm = 400000.0f,
		uint32 InDownsampleFactor = 8,
		uint32 InNumSlices = 64)
	{
		FAVBOITFrameConfig Config;
		Config.NearDepthCm = FMath::Max(InNearDepthCm, 1.0f);
		Config.FarDepthCm = FMath::Max(InFarDepthCm, Config.NearDepthCm + 1.0f);
		Config.DownsampleFactor = FMath::Max(InDownsampleFactor, 1u);
		Config.NumSlices = FMath::Clamp(InNumSlices, 1u, 64u);
		Config.ViewRect = InViewRect;
		Config.TextureExtent = InTextureExtent;
		Config.VolumeExtent = FIntPoint(
			FMath::Max(1, FMath::DivideAndRoundUp(InViewRect.Width(), static_cast<int32>(Config.DownsampleFactor))),
			FMath::Max(1, FMath::DivideAndRoundUp(InViewRect.Height(), static_cast<int32>(Config.DownsampleFactor))));
		return Config;
	}
};
