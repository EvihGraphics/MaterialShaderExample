#pragma once

#include "CoreMinimal.h"

class FAVBOITFoundationManualReproCommands
{
public:
	static void RegisterCommands();
	static void UnregisterCommands();

private:
	static void SpawnTransparentSortingScene(const TArray<FString>& Args);
	static void SetOrder(const TArray<FString>& Args);
	static void SetMode(const TArray<FString>& Args);
	static void Status(const TArray<FString>& Args);
	static void CleanupTransparentSortingScene(const TArray<FString>& Args);
	static void ValidateTransparentSortingSceneAndExit(const TArray<FString>& Args);

	static class IConsoleObject* SpawnTransparentSortingSceneCommand;
	static class IConsoleObject* SetOrderCommand;
	static class IConsoleObject* SetModeCommand;
	static class IConsoleObject* StatusCommand;
	static class IConsoleObject* CleanupTransparentSortingSceneCommand;
	static class IConsoleObject* ValidateTransparentSortingSceneAndExitCommand;
};
