#pragma once

#include "CoreMinimal.h"

class IConsoleObject;

class FAVBOITNiagaraValidationCommands
{
public:
	static void RegisterCommands();
	static void UnregisterCommands();

private:
	static TArray<IConsoleObject*> RegisteredCommands;
};
