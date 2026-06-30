#include "AVBOIT/Niagara/AVBOITNiagaraCVars.h"

#include "HAL/IConsoleManager.h"

namespace
{
	const FLinearColor DefaultTintColor(0.0f, 0.85f, 1.0f, 1.0f);

	TAutoConsoleVariable<int32> CVarAVBOITEnable(
		TEXT("r.AVBOIT.Enable"),
		0,
		TEXT("Global AVBOIT integration gate.\n")
		TEXT(" 0: Disabled\n")
		TEXT(" 1: Enabled\n"),
		ECVF_RenderThreadSafe);

	TAutoConsoleVariable<int32> CVarAVBOITNiagaraEnable(
		TEXT("r.AVBOIT.Niagara.Enable"),
		0,
		TEXT("Enable AVBOIT Niagara sprite renderer takeover.\n")
		TEXT(" 0: Plugin installed but bypassed\n")
		TEXT(" 1: Capture Niagara sprite data and suppress the default sprite draw\n"),
		ECVF_RenderThreadSafe);

	TAutoConsoleVariable<int32> CVarAVBOITNiagaraUnlitOnly(
		TEXT("r.AVBOIT.Niagara.UnlitOnly"),
		1,
		TEXT("Restrict UE-4.2C Niagara parity path to unlit sprite materials."),
		ECVF_RenderThreadSafe);

	TAutoConsoleVariable<int32> CVarAVBOITNiagaraDebug(
		TEXT("r.AVBOIT.Niagara.Debug"),
		0,
		TEXT("Enable AVBOIT Niagara debug buffer evidence."),
		ECVF_RenderThreadSafe);

	TAutoConsoleVariable<int32> CVarAVBOITNiagaraValidateBindings(
		TEXT("r.AVBOIT.Niagara.ValidateBindings"),
		1,
		TEXT("Validate required Niagara sprite renderer bindings before capture."),
		ECVF_Default);

	TAutoConsoleVariable<int32> CVarAVBOITNiagaraCaptureInputs(
		TEXT("r.AVBOIT.Niagara.CaptureInputs"),
		0,
		TEXT("Persist captured Niagara input summaries with the validation evidence."),
		ECVF_Default);

	TAutoConsoleVariable<int32> CVarAVBOITNiagaraTintEnable(
		TEXT("r.AVBOIT.Niagara.Tint.Enable"),
		0,
		TEXT("Enable runtime debug tint for AVBOIT Niagara plugin rendering.\n")
		TEXT(" 0: Preserve source color\n")
		TEXT(" 1: Replace RGB with r.AVBOIT.Niagara.Tint.R/G/B while preserving alpha\n"),
		ECVF_RenderThreadSafe);

	TAutoConsoleVariable<float> CVarAVBOITNiagaraTintR(
		TEXT("r.AVBOIT.Niagara.Tint.R"),
		DefaultTintColor.R,
		TEXT("Runtime AVBOIT Niagara debug tint linear red channel."),
		ECVF_RenderThreadSafe);

	TAutoConsoleVariable<float> CVarAVBOITNiagaraTintG(
		TEXT("r.AVBOIT.Niagara.Tint.G"),
		DefaultTintColor.G,
		TEXT("Runtime AVBOIT Niagara debug tint linear green channel."),
		ECVF_RenderThreadSafe);

	TAutoConsoleVariable<float> CVarAVBOITNiagaraTintB(
		TEXT("r.AVBOIT.Niagara.Tint.B"),
		DefaultTintColor.B,
		TEXT("Runtime AVBOIT Niagara debug tint linear blue channel."),
		ECVF_RenderThreadSafe);

	TAutoConsoleVariable<float> CVarAVBOITNiagaraTintA(
		TEXT("r.AVBOIT.Niagara.Tint.A"),
		DefaultTintColor.A,
		TEXT("Runtime AVBOIT Niagara debug tint alpha multiplier/control channel."),
		ECVF_RenderThreadSafe);

	FLinearColor ClampTintColor(const FLinearColor& Color)
	{
		return FLinearColor(
			FMath::Clamp(Color.R, 0.0f, 1.0f),
			FMath::Clamp(Color.G, 0.0f, 1.0f),
			FMath::Clamp(Color.B, 0.0f, 1.0f),
			FMath::Clamp(Color.A, 0.0f, 1.0f));
	}
}

namespace AVBOITNiagara
{
	bool IsAVBOITEnabled()
	{
		return CVarAVBOITEnable.GetValueOnAnyThread() > 0;
	}

	bool IsNiagaraEnabled()
	{
		return CVarAVBOITNiagaraEnable.GetValueOnAnyThread() > 0;
	}

	bool IsUnlitOnly()
	{
		return CVarAVBOITNiagaraUnlitOnly.GetValueOnAnyThread() > 0;
	}

	bool IsDebugEnabled()
	{
		return CVarAVBOITNiagaraDebug.GetValueOnAnyThread() > 0;
	}

	bool ShouldValidateBindings()
	{
		return CVarAVBOITNiagaraValidateBindings.GetValueOnAnyThread() > 0;
	}

	bool ShouldCaptureInputs()
	{
		return CVarAVBOITNiagaraCaptureInputs.GetValueOnAnyThread() > 0;
	}

	bool IsTintEnabled()
	{
		return CVarAVBOITNiagaraTintEnable.GetValueOnAnyThread() > 0;
	}

	FLinearColor GetTintColor()
	{
		return ClampTintColor(FLinearColor(
			CVarAVBOITNiagaraTintR.GetValueOnAnyThread(),
			CVarAVBOITNiagaraTintG.GetValueOnAnyThread(),
			CVarAVBOITNiagaraTintB.GetValueOnAnyThread(),
			CVarAVBOITNiagaraTintA.GetValueOnAnyThread()));
	}

	void SetTintEnabled(bool bEnabled)
	{
		CVarAVBOITNiagaraTintEnable->Set(bEnabled ? 1 : 0, ECVF_SetByConsole);
	}

	void SetTintColor(const FLinearColor& Color)
	{
		const FLinearColor ClampedColor = ClampTintColor(Color);
		CVarAVBOITNiagaraTintR->Set(ClampedColor.R, ECVF_SetByConsole);
		CVarAVBOITNiagaraTintG->Set(ClampedColor.G, ECVF_SetByConsole);
		CVarAVBOITNiagaraTintB->Set(ClampedColor.B, ECVF_SetByConsole);
		CVarAVBOITNiagaraTintA->Set(ClampedColor.A, ECVF_SetByConsole);
	}

	void ResetTint()
	{
		SetTintColor(DefaultTintColor);
		SetTintEnabled(false);
	}
}
