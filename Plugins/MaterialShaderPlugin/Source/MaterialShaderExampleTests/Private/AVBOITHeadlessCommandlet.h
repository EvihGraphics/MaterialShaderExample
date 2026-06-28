// Copyright

#pragma once

#include "Commandlets/Commandlet.h"
#include "AVBOITHeadlessCommandlet.generated.h"

UCLASS()
class UAVBOITHeadlessCommandlet : public UCommandlet
{
	GENERATED_BODY()

public:
	UAVBOITHeadlessCommandlet();

	virtual int32 Main(const FString& Params) override;
};
