// Copyright Voxel Plugin SAS. All Rights Reserved.

#include "AVBOITSmokeSettings.h"

#include "HAL/IConsoleManager.h"

namespace AVBOIT::Smoke
{
	static TAutoConsoleVariable<int32> CVarEnable(
		TEXT("r.AVBOIT.Smoke.Enable"),
		0,
		TEXT("Enables the isolated AVBOIT RDG smoke test.\n")
		TEXT(" 0: disabled, return untouched SceneColor (default)\n")
		TEXT(" 1: enabled"),
		ECVF_RenderThreadSafe);

	static TAutoConsoleVariable<int32> CVarDebugMode(
		TEXT("r.AVBOIT.Smoke.DebugMode"),
		1,
		TEXT("AVBOIT smoke test debug mode.\n")
		TEXT(" 1: deterministic static UV/checker overlay with ViewRect boundary marker\n")
		TEXT(" 2: real SceneDepth DeviceZ converted with ConvertFromDeviceZ\n")
		TEXT(" 3: UV/checker SceneColor overlay"),
		ECVF_RenderThreadSafe);

	static TAutoConsoleVariable<float> CVarOpacity(
		TEXT("r.AVBOIT.Smoke.Opacity"),
		0.45f,
		TEXT("Opacity for the AVBOIT smoke test composite overlay."),
		ECVF_RenderThreadSafe);

	bool IsEnabled()
	{
		return CVarEnable.GetValueOnAnyThread() != 0;
	}

	int32 GetDebugMode()
	{
		return FMath::Clamp(CVarDebugMode.GetValueOnAnyThread(), 1, 3);
	}

	float GetOpacity()
	{
		return FMath::Clamp(CVarOpacity.GetValueOnAnyThread(), 0.0f, 1.0f);
	}
}
