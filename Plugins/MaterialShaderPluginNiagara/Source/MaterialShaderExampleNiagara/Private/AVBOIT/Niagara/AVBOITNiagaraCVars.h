#pragma once

#include "CoreMinimal.h"

namespace AVBOITNiagara
{
	bool IsAVBOITEnabled();
	bool IsNiagaraEnabled();
	bool IsUnlitOnly();
	bool IsDebugEnabled();
	bool ShouldValidateBindings();
	bool ShouldCaptureInputs();
	bool IsBufferOverviewEnabled();
	int32 GetDebugSlice();
	bool ShouldRequireRealDraw();
	bool IsTintEnabled();
	FLinearColor GetTintColor();
	void SetTintEnabled(bool bEnabled);
	void SetTintColor(const FLinearColor& Color);
	void ResetTint();
}
