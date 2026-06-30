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

	TAutoConsoleVariable<int32> CVarAVBOITCoreIdentityEnable(
		TEXT("r.AVBOIT.Core.Identity.Enable"),
		0,
		TEXT("Enable the UE-4.2E plugin identity path contract.\n")
		TEXT(" 0: Disabled\n")
		TEXT(" 1: Require the Niagara adapter to provide a real identity draw packet\n"),
		ECVF_RenderThreadSafe);

	TAutoConsoleVariable<int32> CVarAVBOITCoreFixedSliceEnable(
		TEXT("r.AVBOIT.Core.FixedSlice.Enable"),
		0,
		TEXT("Enable the UE-4.2E fixed-slice AVBOIT frame graph contract.\n")
		TEXT(" 0: Disabled\n")
		TEXT(" 1: Clear -> SpriteSplat -> Integrate -> ForwardUnlit -> Composite\n"),
		ECVF_RenderThreadSafe);

	TAutoConsoleVariable<int32> CVarAVBOITNiagaraBufferOverview(
		TEXT("r.AVBOIT.Niagara.BufferOverview"),
		0,
		TEXT("Enable AVBOIT Niagara buffer overview mode.\n")
		TEXT(" 0: Normal AVBOIT Niagara mode\n")
		TEXT(" 1: Show/read AVBOIT Niagara intermediate resource overview evidence\n"),
		ECVF_RenderThreadSafe);

	TAutoConsoleVariable<int32> CVarAVBOITNiagaraDebugSlice(
		TEXT("r.AVBOIT.Niagara.DebugSlice"),
		0,
		TEXT("AVBOIT Niagara debug slice index used by buffer overview/readback paths."),
		ECVF_RenderThreadSafe);

	TAutoConsoleVariable<int32> CVarAVBOITBufferOverview(
		TEXT("r.AVBOIT.BufferOverview"),
		0,
		TEXT("Shared AVBOIT buffer overview mode used by adapters."),
		ECVF_RenderThreadSafe);

	TAutoConsoleVariable<int32> CVarAVBOITDebugSlice(
		TEXT("r.AVBOIT.DebugSlice"),
		0,
		TEXT("Shared AVBOIT debug slice index used by buffer overview/readback paths."),
		ECVF_RenderThreadSafe);

	TAutoConsoleVariable<int32> CVarAVBOITNiagaraRequireRealDraw(
		TEXT("r.AVBOIT.Niagara.RequireRealDraw"),
		0,
		TEXT("Require a real AVBOIT Niagara draw packet before scheduling UE-4.2D passes.\n")
		TEXT(" 0: Allow foundation/resource evidence\n")
		TEXT(" 1: Block fallback-only/foundation-only passes\n"),
		ECVF_RenderThreadSafe);

	TAutoConsoleVariable<int32> CVarAVBOITNiagaraRequireRealVertexFactory(
		TEXT("r.AVBOIT.Niagara.RequireRealVertexFactory"),
		0,
		TEXT("Require the Niagara adapter to prove a real Niagara sprite vertex factory contract."),
		ECVF_RenderThreadSafe);

	TAutoConsoleVariable<int32> CVarAVBOITNiagaraRequireRealMaterial(
		TEXT("r.AVBOIT.Niagara.RequireRealMaterial"),
		0,
		TEXT("Require the Niagara adapter to prove a real MaterialRenderProxy contract."),
		ECVF_RenderThreadSafe);

	TAutoConsoleVariable<int32> CVarAVBOITNiagaraRequireParticleAttributeHash(
		TEXT("r.AVBOIT.Niagara.RequireParticleAttributeHash"),
		0,
		TEXT("Require the Niagara adapter to hash Position/Size/Rotation/Facing/SubUV/Color/Alpha/Material/Sim state."),
		ECVF_RenderThreadSafe);

	TAutoConsoleVariable<int32> CVarAVBOITNiagaraRequireSceneColorComposite(
		TEXT("r.AVBOIT.Niagara.RequireSceneColorComposite"),
		0,
		TEXT("Require the plugin identity/AVBOIT path to composite into SceneColor."),
		ECVF_RenderThreadSafe);

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

	bool IsCoreIdentityEnabled()
	{
		return CVarAVBOITCoreIdentityEnable.GetValueOnAnyThread() > 0;
	}

	bool IsCoreFixedSliceEnabled()
	{
		return CVarAVBOITCoreFixedSliceEnable.GetValueOnAnyThread() > 0;
	}

	bool IsBufferOverviewEnabled()
	{
		return CVarAVBOITNiagaraBufferOverview.GetValueOnAnyThread() > 0 || CVarAVBOITBufferOverview.GetValueOnAnyThread() > 0;
	}

	int32 GetDebugSlice()
	{
		return CVarAVBOITDebugSlice.GetValueOnAnyThread() != 0 ? CVarAVBOITDebugSlice.GetValueOnAnyThread() : CVarAVBOITNiagaraDebugSlice.GetValueOnAnyThread();
	}

	bool ShouldRequireRealDraw()
	{
		return CVarAVBOITNiagaraRequireRealDraw.GetValueOnAnyThread() > 0;
	}

	bool ShouldRequireRealVertexFactory()
	{
		return CVarAVBOITNiagaraRequireRealVertexFactory.GetValueOnAnyThread() > 0;
	}

	bool ShouldRequireRealMaterial()
	{
		return CVarAVBOITNiagaraRequireRealMaterial.GetValueOnAnyThread() > 0;
	}

	bool ShouldRequireParticleAttributeHash()
	{
		return CVarAVBOITNiagaraRequireParticleAttributeHash.GetValueOnAnyThread() > 0;
	}

	bool ShouldRequireSceneColorComposite()
	{
		return CVarAVBOITNiagaraRequireSceneColorComposite.GetValueOnAnyThread() > 0;
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
