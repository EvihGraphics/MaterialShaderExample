// Copyright

#include "AVBOIT/Raster/AVBOITTestMeshSceneProxy.h"
#include "Materials/Material.h"

#include "AVBOITRasterSceneData.h"
#include "PipelineStateCache.h"

FAVBOITTestMeshSceneProxy::FAVBOITTestMeshSceneProxy(UAVBOITTestMeshComponent* Component)
	: FPrimitiveSceneProxy(Component)
	, MaterialParams(Component->MaterialParams)
	, bIsTransparent(Component->bIsTransparent)
	, SubmissionOrder(Component->SubmissionOrder)
{
	bWillEverBeLit = false;
}

FAVBOITTestMeshSceneProxy::~FAVBOITTestMeshSceneProxy()
{
}

void FAVBOITRasterVertexBuffer::InitRHI(FRHICommandListBase& RHICmdList)
{
	TResourceArray<FVector3f, VERTEXBUFFER_ALIGNMENT> Vertices;
	Vertices.SetNumUninitialized(4);
	Vertices[0] = FVector3f(-50.f, -50.f, 0.f);
	Vertices[1] = FVector3f( 50.f, -50.f, 0.f);
	Vertices[2] = FVector3f( 50.f,  50.f, 0.f);
	Vertices[3] = FVector3f(-50.f,  50.f, 0.f);

	FRHIBufferCreateDesc CreateDesc(TEXT("AVBOITRasterVertexBuffer"), Vertices.GetResourceDataSize(), 0, BUF_Static | BUF_VertexBuffer);
	CreateDesc.SetInitActionResourceArray(&Vertices);
	VertexBufferRHI = RHICmdList.CreateBuffer(CreateDesc);
}

void FAVBOITRasterIndexBuffer::InitRHI(FRHICommandListBase& RHICmdList)
{
	TResourceArray<uint16, INDEXBUFFER_ALIGNMENT> Indices;
	Indices.SetNumUninitialized(6);
	Indices[0] = 0; Indices[1] = 1; Indices[2] = 2;
	Indices[3] = 0; Indices[4] = 2; Indices[5] = 3;

	FRHIBufferCreateDesc CreateDesc(TEXT("AVBOITRasterIndexBuffer"), Indices.GetResourceDataSize(), sizeof(uint16), BUF_Static | BUF_IndexBuffer);
	CreateDesc.SetInitActionResourceArray(&Indices);
	IndexBufferRHI = RHICmdList.CreateBuffer(CreateDesc);
}

void FAVBOITTestMeshSceneProxy::CreateRenderThreadResources(FRHICommandListBase& RHICmdList)
{
	FPrimitiveSceneProxy::CreateRenderThreadResources(RHICmdList);

	VertexBuffer.InitResource(RHICmdList);
	IndexBuffer.InitResource(RHICmdList);

	FVertexDeclarationElementList Elements;
	Elements.Add(FVertexElement(0, 0, VET_Float3, 0, sizeof(FVector3f)));
	VertexDeclaration = PipelineStateCache::GetOrCreateVertexDeclaration(Elements);

	FAVBOITRasterSceneData::Get().RegisterProxy(this);
}

void FAVBOITTestMeshSceneProxy::DestroyRenderThreadResources()
{
	FAVBOITRasterSceneData::Get().UnregisterProxy(this);

	VertexBuffer.ReleaseResource();
	IndexBuffer.ReleaseResource();
	VertexDeclaration.SafeRelease();

	FPrimitiveSceneProxy::DestroyRenderThreadResources();
}

void FAVBOITTestMeshSceneProxy::GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const
{
	// We don't draw via standard UE paths! We just provide a proxy for AVBOIT subsystem to collect and draw manually in RDG passes.
	// But we must exist in the scene to be tracked by our own custom pass.
}

FPrimitiveViewRelevance FAVBOITTestMeshSceneProxy::GetViewRelevance(const FSceneView* View) const
{
	FPrimitiveViewRelevance Result;
	Result.bDrawRelevance = IsShown(View);
	Result.bShadowRelevance = IsShadowCast(View);
	Result.bDynamicRelevance = true;
	Result.bRenderInMainPass = false; // We draw in our custom pass!
	// But wait, if it's an opaque occluder, maybe we draw it in main pass?
	if (!bIsTransparent)
	{
		Result.bRenderInMainPass = true;
		Result.bOpaque = true;
	}
	else
	{
		Result.bNormalTranslucency = true; // Just to be safe for visibility
	}
	return Result;
}

uint32 FAVBOITTestMeshSceneProxy::GetMemoryFootprint() const
{
	return sizeof(*this) + GetAllocatedSize();
}

SIZE_T FAVBOITTestMeshSceneProxy::GetTypeHash() const
{
	static size_t UniquePointer;
	return reinterpret_cast<size_t>(&UniquePointer);
}
