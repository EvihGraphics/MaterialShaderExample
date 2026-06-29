#include "AVBOITEvidencePath.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformProcess.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

FString FAVBOITEvidencePath::ResolveAVBOITMachineId()
{
	const FString JsonPath = FPaths::ProjectSavedDir() / TEXT("AVBOIT/LocalMachine/ue57-environment.json");
	FString JsonContent;
	if (FFileHelper::LoadFileToString(JsonContent, *JsonPath))
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonContent);
		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			FString MachineId;
			if (JsonObject->TryGetStringField(TEXT("MachineId"), MachineId) && !MachineId.IsEmpty())
			{
				return MachineId;
			}
		}
	}

	FString ComputerName = FPlatformProcess::ComputerName();
	if (!ComputerName.IsEmpty())
	{
		return ComputerName;
	}

	UE_LOG(LogTemp, Warning, TEXT("AVBOIT: Could not resolve machine ID, using UNKNOWN-MACHINE"));
	return TEXT("UNKNOWN-MACHINE");
}

FString FAVBOITEvidencePath::ResolveAVBOITEvidenceRoot(const FString& Stage, const FString& ExplicitRoot)
{
	if (!ExplicitRoot.IsEmpty())
	{
		FString Resolved = FPaths::IsRelative(ExplicitRoot) ? FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / ExplicitRoot) : ExplicitRoot;
		FPaths::NormalizeFilename(Resolved);
		return Resolved;
	}

	FString MachineId = ResolveAVBOITMachineId();
	FString DefaultRoot = FPaths::ProjectDir() / TEXT("LocalVisualResults/TempResults/UE57") / MachineId / Stage;
	FPaths::NormalizeFilename(DefaultRoot);
	return DefaultRoot;
}
