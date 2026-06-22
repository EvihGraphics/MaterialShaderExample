# Egaku source: `Shaders/TerrainBaking/ComputeShader/UnderwaterVertexDisplacementShader.compute`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
#pragma kernel CSMain
#include "Assets/Shaders/HLSLInclude/UnderwaterVertexDisplacement.hlsl"
#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Utilities/ComputeShaderUtils.hlsl"

Buffer<float3> vertexPositionBuffer;
RWByteAddressBuffer vertexBuffer;

int vertexAttributeStride;
int vertexPositionOffset;

float4x4 modelMatrix;
float4x4 modelMatrixInv;
float4x4 cameraViewMatrix;
float4x4 cameraViewMatrixInv;
float3 waterBoundMinWS;
float3 waterBoundMaxWS;
float3 waterPlaneNormalWS;
float indexOfRefraction;

void SetVertexPosition(int id, float3 pos)
{
    StoreFloat3(vertexBuffer,id * vertexAttributeStride + vertexPositionOffset, pos);
}


[numthreads(32,1,1)]
void CSMain (uint id : SV_DispatchThreadID)
{
    uint vertexCount;
    vertexPositionBuffer.GetDimensions(vertexCount);
    if (id >= vertexCount){
        return;
    }

    float3 positionOS = vertexPositionBuffer[id];
    float3 positionWS = mul(modelMatrix, float4(positionOS, 1)).xyz;
    float3 newPositionOS;

    if (IsUnderwaterVertexClipped(positionWS, waterBoundMinWS, waterBoundMaxWS)){
        newPositionOS = positionOS;
    }else{
        float3 positionVS = mul(cameraViewMatrix, float4(positionWS, 1)).xyz;
        float3 waterPlaneCenterWS;
        waterPlaneCenterWS.xz = lerp(waterBoundMinWS.xz,waterBoundMaxWS.xz,0.5);
        waterPlaneCenterWS.y = waterBoundMaxWS.y;
        float3 waterPlaneCenterVS = mul(cameraViewMatrix, float4(waterPlaneCenterWS, 1)).xyz;
        float4x4 vectorViewTransMat = transpose(cameraViewMatrixInv); 
        float3 waterPlaneNormalVS = normalize(mul(vectorViewTransMat,float4(waterPlaneNormalWS, 0)).xyz);
        float3 displacement =  GetUnderwaterVertexDisplacement(float3(0,0,0), positionVS, waterPlaneNormalVS,waterPlaneCenterVS,indexOfRefraction);
        float3 newPositionVS = positionVS + displacement;
        newPositionOS = mul(mul(modelMatrixInv,cameraViewMatrixInv), float4(newPositionVS, 1)).xyz;
     //   newPositionOS = displacement;
    }
    SetVertexPosition(id , newPositionOS);
}
```
