// Copyright

#include "AVBOIT/Raster/AVBOITTestMeshComponent.h"
#include "AVBOIT/Raster/AVBOITTestMeshSceneProxy.h"

UAVBOITTestMeshComponent::UAVBOITTestMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bTickInEditor = false;
	bSelectable = true;
}

FPrimitiveSceneProxy* UAVBOITTestMeshComponent::CreateSceneProxy()
{
	return new FAVBOITTestMeshSceneProxy(this);
}

FBoxSphereBounds UAVBOITTestMeshComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	FBox Box(ForceInit);
	if (bUseCustomLocalVertices)
	{
		for (const FVector3f& Vertex : CustomLocalVertices)
		{
			Box += FVector(Vertex);
		}
	}
	else
	{
		Box = FBox(FVector(-50.0f, -50.0f, 0.0f), FVector(50.0f, 50.0f, 0.0f));
	}
	return FBoxSphereBounds(Box).TransformBy(LocalToWorld);
}
