# Egaku source: `ShaderLibrary/Macros.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
#ifndef MACROS_HLSL
#define MACROS_HLSL

// Some shader compiler don't support to do multiple ## for concatenation inside the same macro, it require an indirection.
// This is the purpose of this macro
#define _CONCAT(X, Y) X##Y
#define CONCAT(X, Y) _CONCAT(X, Y)



#define GENERATE_INT_FLOAT_1_ARG(FunctionName, param1, FunctionBody) \
    float  FunctionName(float  param1) FunctionBody \
    uint   FunctionName(uint  param1) FunctionBody \
    int    FunctionName(int  param1) FunctionBody


#define TEMPLATE_1_FLT(FunctionName, param1, FunctionBody) \
    float  FunctionName(float  param1) FunctionBody \
    float2 FunctionName(float2 param1) FunctionBody \
    float3 FunctionName(float3 param1) FunctionBody \
    float4 FunctionName(float4 param1) FunctionBody

#define TEMPLATE_1_HALF(FunctionName, param1, FunctionBody) \
    half  FunctionName(half  param1) FunctionBody \
    half2 FunctionName(half2 param1) FunctionBody \
    half3 FunctionName(half3 param1) FunctionBody \
    half4 FunctionName(half4 param1) FunctionBody

#define TEMPLATE_1_REAL(FunctionName, param1, FunctionBody) \
    TEMPLATE_1_FLT(FunctionName, param1, FunctionBody) \
    TEMPLATE_1_HALF(FunctionName, param1, FunctionBody) 

#define TEMPLATE_1_INT(FunctionName, param1, FunctionBody) \
    int    FunctionName(int    param1) FunctionBody \
    int2   FunctionName(int2   param1) FunctionBody \
    int3   FunctionName(int3   param1) FunctionBody \
    int4   FunctionName(int4   param1) FunctionBody \
    uint   FunctionName(uint   param1) FunctionBody \
    uint2  FunctionName(uint2  param1) FunctionBody \
    uint3  FunctionName(uint3  param1) FunctionBody \
    uint4  FunctionName(uint4  param1) FunctionBody

#define TEMPLATE_2_FLT(FunctionName, param1, param2, FunctionBody) \
    float  FunctionName(float  param1, float  param2) FunctionBody \
    float2 FunctionName(float2 param1, float2 param2) FunctionBody \
    float3 FunctionName(float3 param1, float3 param2) FunctionBody \
    float4 FunctionName(float4 param1, float4 param2) FunctionBody

#define TEMPLATE_2_HALF(FunctionName, param1, param2, FunctionBody) \
    half  FunctionName(half  param1, half  param2) FunctionBody \
    half2 FunctionName(half2 param1, half2 param2) FunctionBody \
    half3 FunctionName(half3 param1, half3 param2) FunctionBody \
    half4 FunctionName(half4 param1, half4 param2) FunctionBody

#define TEMPLATE_2_REAL(FunctionName, param1, param2,  FunctionBody) \
    TEMPLATE_2_FLT(FunctionName, param1, param2, FunctionBody) \
    TEMPLATE_2_HALF(FunctionName, param1, param2, FunctionBody)


#define TEMPLATE_2_INT(FunctionName, param1, param2, FunctionBody) \
    int    FunctionName(int    param1, int    param2) FunctionBody \
    int2   FunctionName(int2   param1, int2   param2) FunctionBody \
    int3   FunctionName(int3   param1, int3   param2) FunctionBody \
    int4   FunctionName(int4   param1, int4   param2) FunctionBody \
    uint   FunctionName(uint   param1, uint   param2) FunctionBody \
    uint2  FunctionName(uint2  param1, uint2  param2) FunctionBody \
    uint3  FunctionName(uint3  param1, uint3  param2) FunctionBody \
    uint4  FunctionName(uint4  param1, uint4  param2) FunctionBody

#define TEMPLATE_3_FLT(FunctionName, param1, param2, param3, FunctionBody) \
    float  FunctionName(float  param1, float  param2, float  param3) FunctionBody \
    float2 FunctionName(float2 param1, float2 param2, float2 param3) FunctionBody \
    float3 FunctionName(float3 param1, float3 param2, float3 param3) FunctionBody \
    float4 FunctionName(float4 param1, float4 param2, float4 param3) FunctionBody

#define TEMPLATE_3_HALF(FunctionName, param1, param2, param3, FunctionBody) \
    half  FunctionName(half  param1, half  param2, half  param3) FunctionBody \
    half2 FunctionName(half2 param1, half2 param2, half2 param3) FunctionBody \
    half3 FunctionName(half3 param1, half3 param2, half3 param3) FunctionBody \
    half4 FunctionName(half4 param1, half4 param2, half4 param3) FunctionBody

#define TEMPLATE_3_INT(FunctionName, param1, param2, param3, FunctionBody) \
    int    FunctionName(int    param1, int    param2, int    param3) FunctionBody \
    int2   FunctionName(int2   param1, int2   param2, int2   param3) FunctionBody \
    int3   FunctionName(int3   param1, int3   param2, int3   param3) FunctionBody \
    int4   FunctionName(int4   param1, int4   param2, int4   param3) FunctionBody \
    uint   FunctionName(uint   param1, uint   param2, uint   param3) FunctionBody \
    uint2  FunctionName(uint2  param1, uint2  param2, uint2  param3) FunctionBody \
    uint3  FunctionName(uint3  param1, uint3  param2, uint3  param3) FunctionBody \
    uint4  FunctionName(uint4  param1, uint4  param2, uint4  param3) FunctionBody

#define TEMPLATE_3_REAL(FunctionName, param1, param2, param3, FunctionBody) \
    TEMPLATE_3_FLT(FunctionName, param1, param2, param3, FunctionBody) \
    TEMPLATE_3_HALF(FunctionName, param1, param2, param3, FunctionBody)



#endif
```
