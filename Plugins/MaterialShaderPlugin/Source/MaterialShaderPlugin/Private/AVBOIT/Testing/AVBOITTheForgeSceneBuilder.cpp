#include "AVBOITTheForgeSceneBuilder.h"
#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonSerializer.h"

static const FName THE_FORGE_TAG = TEXT("AVBOIT_TheForgeIntegration");

static FVector ForgeToUEPosition(const TArray<TSharedPtr<FJsonValue>>& Array)
{
	if (Array.Num() >= 3)
	{
		float X = Array[0]->AsNumber();
		float Y = Array[1]->AsNumber();
		float Z = Array[2]->AsNumber();
		return FVector(Z * 100.0f, X * 100.0f, Y * 100.0f);
	}
	return FVector::ZeroVector;
}

static FVector ForgeToUEScale(const TArray<TSharedPtr<FJsonValue>>& Array)
{
	if (Array.Num() >= 3)
	{
		float X = Array[0]->AsNumber();
		float Y = Array[1]->AsNumber();
		float Z = Array[2]->AsNumber();
		// Scale mapping is the same as position without the 100 multiplier?
		// Wait, no. A cube of scale 1x1x1 in Forge is 1 unit. In UE, standard cube is 100x100x100 units.
		// If standard UE cube is used, its scale 1 means 100 units.
		// ForgeScale = [100.0, 0.5, 100.0] -> 100 units wide.
		// UE cube is 100 units wide. So a scale of 1.0 in UE is 100 units.
		// If Forge cube is 100 units wide, its UE scale should be 1.0.
		// So UE_Scale = Forge_Scale. Wait, let's map coordinates:
		// ForgeX maps to UEY, ForgeY maps to UEZ, ForgeZ maps to UEX.
		return FVector(Z, X, Y);
	}
	return FVector::OneVector;
}

static FLinearColor ParseColor(const TArray<TSharedPtr<FJsonValue>>& Array)
{
	if (Array.Num() >= 4)
	{
		return FLinearColor(Array[0]->AsNumber(), Array[1]->AsNumber(), Array[2]->AsNumber(), Array[3]->AsNumber());
	}
	return FLinearColor::White;
}

void FAVBOITTheForgeSceneBuilder::BuildSceneFromContract(UWorld* World)
{
	if (!World) return;

	// Load JSONs
	FString SceneJsonPath = FPaths::ProjectConfigDir() / TEXT("AVBOIT/TheForge15TransparencyScene.json");
	FString CameraJsonPath = FPaths::ProjectConfigDir() / TEXT("AVBOIT/TheForge15TransparencyCamera.json");

	FString SceneJsonContent, CameraJsonContent;
	FFileHelper::LoadFileToString(SceneJsonContent, *SceneJsonPath);
	FFileHelper::LoadFileToString(CameraJsonContent, *CameraJsonPath);

	TSharedPtr<FJsonObject> SceneJson, CameraJson;
	TSharedRef<TJsonReader<>> SceneReader = TJsonReaderFactory<>::Create(SceneJsonContent);
	TSharedRef<TJsonReader<>> CameraReader = TJsonReaderFactory<>::Create(CameraJsonContent);
	FJsonSerializer::Deserialize(SceneReader, SceneJson);
	FJsonSerializer::Deserialize(CameraReader, CameraJson);

	// Basic meshes
	UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
	UStaticMesh* SphereMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	UMaterialInterface* BaseMat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));

	if (SceneJson.IsValid() && SceneJson->HasTypedField<EJson::Array>(TEXT("Objects")))
	{
		auto ObjectsArray = SceneJson->GetArrayField(TEXT("Objects"));
		for (auto& ObjVal : ObjectsArray)
		{
			auto Obj = ObjVal->AsObject();
			FString Name = Obj->GetStringField(TEXT("Name"));
			FString Type = Obj->GetStringField(TEXT("Type"));
			FVector Pos = ForgeToUEPosition(Obj->GetArrayField(TEXT("ForgePosition")));
			FVector Scale = ForgeToUEScale(Obj->GetArrayField(TEXT("ForgeScale")));
			FLinearColor Color = ParseColor(Obj->GetArrayField(TEXT("Color")));

			AStaticMeshActor* Actor = World->SpawnActor<AStaticMeshActor>(Pos, FRotator::ZeroRotator);
			Actor->Tags.Add(THE_FORGE_TAG);
			Actor->SetActorLabel(Name);
			
			UStaticMeshComponent* SMC = Actor->GetStaticMeshComponent();
			SMC->SetStaticMesh(Type == TEXT("Cube") ? CubeMesh : SphereMesh);
			SMC->SetWorldScale3D(Scale);
			
			UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(BaseMat, SMC);
			MID->SetVectorParameterValue(TEXT("Color"), Color);
			// Assume it's unlit translucent for testing or simple opaque depending on alpha
			SMC->SetMaterial(0, MID);
		}
	}

	if (CameraJson.IsValid() && CameraJson->HasTypedField<EJson::Object>(TEXT("Camera")))
	{
		auto CamObj = CameraJson->GetObjectField(TEXT("Camera"));
		FVector UEPos = FVector(CamObj->GetArrayField(TEXT("UEPosition"))[0]->AsNumber(),
								CamObj->GetArrayField(TEXT("UEPosition"))[1]->AsNumber(),
								CamObj->GetArrayField(TEXT("UEPosition"))[2]->AsNumber());
		FVector UELookAt = FVector(CamObj->GetArrayField(TEXT("UELookAt"))[0]->AsNumber(),
								   CamObj->GetArrayField(TEXT("UELookAt"))[1]->AsNumber(),
								   CamObj->GetArrayField(TEXT("UELookAt"))[2]->AsNumber());
		
		FRotator Rot = (UELookAt - UEPos).Rotation();
		ACameraActor* CamActor = World->SpawnActor<ACameraActor>(UEPos, Rot);
		CamActor->Tags.Add(THE_FORGE_TAG);
		CamActor->SetActorLabel(TEXT("TheForgeCamera"));
		CamActor->GetCameraComponent()->SetFieldOfView(CamObj->GetNumberField(TEXT("HorizontalFOV")));
		CamActor->GetCameraComponent()->AspectRatio = CamObj->GetNumberField(TEXT("Aspect"));
	}
}
