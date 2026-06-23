#pragma once

#include "CoreMinimal.h"

class FAVBOITEvidencePath
{
public:
	static FString ResolveAVBOITMachineId();
	static FString ResolveAVBOITEvidenceRoot(const FString& Stage, const FString& ExplicitRoot);
};
