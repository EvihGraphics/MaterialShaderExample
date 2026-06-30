#include "AVBOIT/Niagara/AVBOITNiagaraCVars.h"

#include "HAL/IConsoleManager.h"

namespace
{
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
}
