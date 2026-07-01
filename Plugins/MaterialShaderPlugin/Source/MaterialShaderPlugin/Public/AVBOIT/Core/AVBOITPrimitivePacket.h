#pragma once

#include "CoreMinimal.h"

struct FAVBOITPrimitivePacket
{
	FMatrix44f LocalToWorld = FMatrix44f::Identity;
	FLinearColor LinearColor = FLinearColor::White;
	float Alpha = 0.5f;
	uint32 SubmissionOrder = 0;

	FBufferRHIRef VertexBuffer;
	FBufferRHIRef IndexBuffer;
	FVertexDeclarationRHIRef VertexDeclaration;

	uint32 VertexCount = 4;
	uint32 IndexCount = 6;
	uint32 PrimitiveCount = 2;

	bool IsValid() const
	{
		return VertexBuffer.IsValid()
			&& IndexBuffer.IsValid()
			&& VertexDeclaration.IsValid()
			&& VertexCount > 0
			&& IndexCount > 0
			&& PrimitiveCount > 0
			&& Alpha > 0.0f;
	}
};
