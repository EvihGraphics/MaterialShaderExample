# Egaku source: `Common/CodeGenerator/VariableType.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using System.Collections.Generic;
using System.Reflection;
using UnityEngine;

namespace Egaku.Rendering.Pipeline.Common {


    
    public static class ShaderVariableType {
        
        public static string GetHLSLTypeToken(HLSLType type) {
            switch (type) {
                case HLSLType.HALF:
                    return "half";
                case HLSLType.HALF2:
                    return "half2";
                case HLSLType.HALF3:
                    return "half3";
                case HLSLType.HALF4:
                    return "half4";
                case HLSLType.HALF2X2:
                    return "half2x2";
                case HLSLType.HALF2X3:
                    return "half2x3";
                case HLSLType.HALF2X4:
                    return "half2x4";
                case HLSLType.HALF3X2:
                    return "half3x2";
                case HLSLType.HALF3X3:
                    return "half3x3";
                case HLSLType.HALF3X4:
                    return "half3x4";
                case HLSLType.HALF4X2:
                    return "half4x2";
                case HLSLType.HALF4X3:
                    return "half4x3";
                case HLSLType.HALF4X4:
                    return "half4x4";
                case HLSLType.FLOAT:
                    return "float";
                case HLSLType.FLOAT2:
                    return "float2";
                case HLSLType.FLOAT3:
                    return "float3";
                case HLSLType.FLOAT4:
                    return "float4";
                case HLSLType.FLOAT2X2:
                    return "float2x2";
                case HLSLType.FLOAT2X3:
                    return "float2x3";
                case HLSLType.FLOAT2X4:
                    return "float2x4";
                case HLSLType.FLOAT3X2:
                    return "float3x2";
                case HLSLType.FLOAT3X3:
                    return "float3x3";
                case HLSLType.FLOAT3X4:
                    return "float3x4";
                case HLSLType.FLOAT4X2:
                    return "float4x2";
                case HLSLType.FLOAT4X3:
                    return "float4x3";
                case HLSLType.FLOAT4X4:
                    return "float4x4";
                case HLSLType.INT:
                    return "int";
                case HLSLType.INT2:
                    return "int2";
                case HLSLType.INT3:
                    return "int3";
                case HLSLType.INT4:
                    return "int4";
                case HLSLType.INT2X2:
                    return "int2x2";
                case HLSLType.INT2X3:
                    return "int2x3";
                case HLSLType.INT2X4:
                    return "int2x4";
                case HLSLType.INT3X2:
                    return "int3x2";
                case HLSLType.INT3X3:
                    return "int3x3";
                case HLSLType.INT3X4:
                    return "int3x4";
                case HLSLType.INT4X2:
                    return "int4x2";
                case HLSLType.INT4X3:
                    return "int4x3";
                case HLSLType.INT4X4:
                    return "int4x4";
                case HLSLType.UINT:
                    return "uint";
                case HLSLType.UINT2:
                    return "uint2";
                case HLSLType.UINT3:
                    return "uint3";
                case HLSLType.UINT4:
                    return "uint4";
                case HLSLType.UINT2X2:
                    return "uint2x2";
                case HLSLType.UINT2X3:
                    return "uint2x3";
                case HLSLType.UINT2X4:
                    return "uint2x4";
                case HLSLType.UINT3X2:
                    return "uint3x2";
                case HLSLType.UINT3X3:
                    return "uint3x3";
                case HLSLType.UINT3X4:
                    return "uint3x4";
                case HLSLType.UINT4X2:
                    return "uint4x2";
                case HLSLType.UINT4X3:
                    return "uint4x3";
                case HLSLType.UINT4X4:
                    return "uint4x4";
                case HLSLType.BOOL:
                    return "bool";
                case HLSLType.BOOL2:
                    return "bool2";
                case HLSLType.BOOL3:
                    return "bool3";
                case HLSLType.BOOL4:
                    return "bool4";
                case HLSLType.BOOL2X2:
                    return "bool2x2";
                case HLSLType.BOOL2X3:
                    return "bool2x3";
                case HLSLType.BOOL2X4:
                    return "bool2x4";
                case HLSLType.BOOL3X2:
                    return "bool3x2";
                case HLSLType.BOOL3X3:
                    return "bool3x3";
                case HLSLType.BOOL3X4:
                    return "bool3x4";
                case HLSLType.BOOL4X2:
                    return "bool4x2";
                case HLSLType.BOOL4X3:
                    return "bool4x3";
                case HLSLType.BOOL4X4:
                    return "bool4x4";
                case HLSLType.TEXTURE_1D:
                    return "Texture1D";
                case HLSLType.TEXTURE_1D_ARRAY:
                    return "Texture1DArray";
                case HLSLType.TEXTURE_2D:
                    return "Texture2D";
                case HLSLType.TEXTURE_2D_ARRAY:
                    return "Texture2DArray";
                case HLSLType.TEXTURE_3D:
                    return "Texture3D";
                case HLSLType.TEXTURE_CUBE:
                    return "TextureCube";
                case HLSLType.TEXTURE_CUBE_ARRAY:
                    return "TextureCubeArray";
                case HLSLType.SAMPLER_1D:
                    return "sampler1D";
                case HLSLType.SAMPLER_2D:
                    return "sampler2D";
                case HLSLType.SAMPLER_3D:
                    return "sampler3D";
                case HLSLType.SAMPLER_CUBE:
                    return "samplerCUBE";
                case HLSLType.SAMPLER_STATE:
                    return "SamplerState";
                default:
                    throw new NotImplementedException();
            }
        }
        
        public static string GetShaderLabTypeToken(ShaderLabType type) {
            switch (type) {
                case ShaderLabType.INTEGER:
                    return "Integer";
                case ShaderLabType.RANGE:
                    return "Range";
                case ShaderLabType.FLOAT:
                    return "Float";
                case ShaderLabType.COLOR:
                    return "Color";
                case ShaderLabType.VECTOR:
                    return "Vector";
                case ShaderLabType.TWO_D:
                    return "2D";
                case ShaderLabType.CUBE:
                    return "Cube";
                case ShaderLabType.CUBE_ARRAY:
                    return "CubeArray";
                case ShaderLabType.THREE_D:
                    return "3D";
                case ShaderLabType.TWO_D_ARRAY:
                    return "2DArray";
                default:
                    throw new NotImplementedException();
            }
        }
        
        public static bool IsTexture(HLSLType type) {
            return type == HLSLType.TEXTURE_1D || type == HLSLType.TEXTURE_1D_ARRAY || type == HLSLType.TEXTURE_2D ||
                   type == HLSLType.TEXTURE_2D_ARRAY || type == HLSLType.TEXTURE_3D ||
                   type == HLSLType.TEXTURE_CUBE || type == HLSLType.TEXTURE_CUBE_ARRAY;
        }
    }

    public enum ShaderLabType {
        INTEGER,
        RANGE,
        FLOAT,
        COLOR,
        VECTOR,
        TWO_D,
        CUBE,
        CUBE_ARRAY,
        THREE_D,
        TWO_D_ARRAY
    }
    
    public enum HLSLType {
        HALF,
        HALF2,
        HALF3,
        HALF4,
        HALF2X2,
        HALF2X3,
        HALF2X4,
        HALF3X2,
        HALF3X3,
        HALF3X4,
        HALF4X2,
        HALF4X3,
        HALF4X4,
        FLOAT,
        FLOAT2,
        FLOAT3,
        FLOAT4,
        FLOAT2X2,
        FLOAT2X3,
        FLOAT2X4,
        FLOAT3X2,
        FLOAT3X3,
        FLOAT3X4,
        FLOAT4X2,
        FLOAT4X3,
        FLOAT4X4,
        INT,
        INT2,
        INT3,
        INT4,
        INT2X2,
        INT2X3,
        INT2X4,
        INT3X2,
        INT3X3,
        INT3X4,
        INT4X2,
        INT4X3,
        INT4X4,
        UINT,
        UINT2,
        UINT3,
        UINT4,
        UINT2X2,
        UINT2X3,
        UINT2X4,
        UINT3X2,
        UINT3X3,
        UINT3X4,
        UINT4X2,
        UINT4X3,
        UINT4X4,
        BOOL,
        BOOL2,
        BOOL3,
        BOOL4,
        BOOL2X2,
        BOOL2X3,
        BOOL2X4,
        BOOL3X2,
        BOOL3X3,
        BOOL3X4,
        BOOL4X2,
        BOOL4X3,
        BOOL4X4,
        TEXTURE_1D,
        TEXTURE_1D_ARRAY,
        TEXTURE_2D,
        TEXTURE_2D_ARRAY,
        TEXTURE_3D,
        TEXTURE_CUBE,
        TEXTURE_CUBE_ARRAY,
        SAMPLER_1D,
        SAMPLER_2D,
        SAMPLER_3D,
        SAMPLER_CUBE,
        SAMPLER_STATE
    }
    
    
   
}
```
