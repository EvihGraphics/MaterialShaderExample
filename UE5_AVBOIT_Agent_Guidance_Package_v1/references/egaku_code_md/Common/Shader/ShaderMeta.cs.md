# Egaku source: `Common/Shader/ShaderMeta.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using Egaku.Rendering.Pipeline.Utility;
using UnityEngine;

namespace Egaku.Rendering.Pipeline.Common {


    public struct ShaderParamAttrib {
        public string label;
        public bool exposed;
        public bool hdrColor;
        public bool ranged;
        public float rangeMin;
        public float rangeMax;
    }
    
    
    
    public struct ShaderParamMeta {
        public string name;
        public string uniformName;
        public Type type;
        public string defaultValue;
        public ShaderParamAttrib attrib;
    }

    public struct ShaderParamStructAttrib {
        public string cbufferName;
        public bool instanced;
        public bool inspectorHeader;
        public string inspectorHeaderName;
    }
    
    public struct ShaderParamStructMeta {
        public string name;
        public ShaderParamMeta[] memberMetas;
        public ShaderParamStructAttrib attrib;
    }

    public class ShaderMeta {
        public ShaderConfig shaderConfig;
    }
    
    public class ShaderMetaUtils {
        private static Dictionary<Type, HLSLType> _CSharpTypeToHLSLTypeMap = new(){
            {typeof(bool) ,HLSLType.BOOL},
            {typeof(float),HLSLType.FLOAT},
            {typeof(Vector2),HLSLType.FLOAT2},
            {typeof(Vector3),HLSLType.FLOAT3},
            {typeof(Vector4),HLSLType.FLOAT4},
            {typeof(Color),HLSLType.FLOAT4},
            {typeof(Matrix4x4),HLSLType.FLOAT4X4},
            {typeof(int),HLSLType.INT},
            {typeof(Vector2Int),HLSLType.INT2},
            {typeof(Vector3Int),HLSLType.INT3},
            {typeof(Texture2D),HLSLType.TEXTURE_2D},
            {typeof(Texture3D),HLSLType.TEXTURE_3D},
            {typeof(Texture2DArray),HLSLType.TEXTURE_2D_ARRAY}
        };
        
        private static Dictionary<Type, ShaderLabType> _CSharpTypeToShaderLabTypeMap = new(){
            {typeof(bool) ,ShaderLabType.INTEGER},
            {typeof(int),ShaderLabType.INTEGER},
            {typeof(float),ShaderLabType.FLOAT},
            {typeof(Vector2),ShaderLabType.VECTOR},
            {typeof(Vector3),ShaderLabType.VECTOR},
            {typeof(Vector4),ShaderLabType.VECTOR},
            {typeof(Color),ShaderLabType.COLOR},
            {typeof(Vector2Int),ShaderLabType.VECTOR},
            {typeof(Vector3Int),ShaderLabType.VECTOR},
            {typeof(Texture2D),ShaderLabType.TWO_D},
            {typeof(Texture3D),ShaderLabType.THREE_D},
            {typeof(Texture2DArray),ShaderLabType.TWO_D_ARRAY},
            {typeof(Cubemap),ShaderLabType.CUBE},
            {typeof(CubemapArray),ShaderLabType.CUBE_ARRAY},
        };

        public static ShaderParamAttrib GetParamAttributes(FieldInfo paramFieldInfo) {
            ShaderParamAttrib shaderParamAttrib = new();
            var propertyAttr = paramFieldInfo.GetCustomAttribute<ShaderParam.ParamPropertyAttribute>();
            if (propertyAttr != null) {
                shaderParamAttrib.label = propertyAttr.label;
                shaderParamAttrib.exposed = propertyAttr.exposed;
            } else {
                shaderParamAttrib.label = "";
                shaderParamAttrib.exposed = false;
            }
            var hdrAttr = paramFieldInfo.GetCustomAttribute<ShaderParam.HDRAttribute>();
            if (hdrAttr != null) {
                shaderParamAttrib.hdrColor = true;
            } else {
                shaderParamAttrib.hdrColor = false;
            }
            var rangeAttr = paramFieldInfo.GetCustomAttribute<ShaderParam.RangeAttribute>();
            if (rangeAttr != null) {
                shaderParamAttrib.ranged = true;
                shaderParamAttrib.rangeMin = rangeAttr.min;
                shaderParamAttrib.rangeMax = rangeAttr.max;
            } else {
                shaderParamAttrib.ranged = false;
            }
            return shaderParamAttrib;
        }
  
        
        public static ShaderParamStructAttrib GetParamStructProperty(Type paramTableType) {
            ShaderParamStructAttrib shaderParamStructAttrib = new();
            var cbufferAttrib = paramTableType.GetCustomAttribute<ShaderParam.ConstantBufferAttribute>();
            var instancingAttrib = paramTableType.GetCustomAttribute<ShaderParam.InstancedAttribute>();
            var headerAttrib = paramTableType.GetCustomAttribute<ShaderParam.HeaderAttribute>();
            shaderParamStructAttrib.cbufferName = cbufferAttrib?.name;
            shaderParamStructAttrib.instanced = instancingAttrib != null;
            shaderParamStructAttrib.inspectorHeader = headerAttrib != null;
            shaderParamStructAttrib.inspectorHeaderName = headerAttrib?.title;
            return shaderParamStructAttrib;
        }
        
        public static HLSLType GetHLSLType(Type type) {
            if (_CSharpTypeToHLSLTypeMap.TryGetValue(type, out var hlslType)) {
                return hlslType;
            }
            throw new ArgumentException($"{type} is not supported.");
        }

        public static ShaderLabType GetShaderLabType(Type type) {
            if (_CSharpTypeToShaderLabTypeMap.TryGetValue(type, out var shaderLabType)) {
                return shaderLabType;
            }
            throw new ArgumentException($"{type} is not supported.");
        }
        
        public static string GetHLSLTypeToken(Type type) {
            return ShaderVariableType.GetHLSLTypeToken(GetHLSLType(type));
        }
        
        public static string GetHLSLTypeToken(HLSLType type) {
            return ShaderVariableType.GetHLSLTypeToken(type);
        }


        public static string GetShaderLabTypeToken(Type type) {
            return ShaderVariableType.GetShaderLabTypeToken(GetShaderLabType(type));
        }

        public static string GetShaderLabPropertyDefaultValueString(Type type) {
            var shaderLabType = GetShaderLabType(type);
            switch (shaderLabType) {
                case ShaderLabType.INTEGER:
                    return "0";
                case ShaderLabType.RANGE:
                    return "0.0";
                case ShaderLabType.FLOAT:
                    return "0.0";
                case ShaderLabType.COLOR:
                    return "(0, 0, 0, 0)";
                case ShaderLabType.VECTOR:
                    return "(0, 0, 0, 0)";
                case ShaderLabType.TWO_D:
                    return "\"\" {}";
                case ShaderLabType.CUBE:
                    return "\"\" {}";
                case ShaderLabType.CUBE_ARRAY:
                    return "\"\" {}";
                case ShaderLabType.THREE_D:
                    return "\"\" {}";
                case ShaderLabType.TWO_D_ARRAY:
                    return "\"\" {}";
                default:
                    throw new NotImplementedException();
            }
        }
        
        public static string GetHLSLUniformVariableName(string structName,string fieldName) {
            string prefix = structName.EndsWith("Desc") ? structName[..^4] : structName;
            return  $"_{prefix}_{fieldName}";
        }
        
        public static ShaderParamMeta GetShaderParamMeta(Type structInfo,FieldInfo fieldInfo,object defaultStructObj) {
            var paramMeta = new ShaderParamMeta();
            paramMeta.name = fieldInfo.Name;
            paramMeta.type = fieldInfo.FieldType;
            paramMeta.uniformName = GetHLSLUniformVariableName(structInfo.Name, fieldInfo.Name);
            paramMeta.defaultValue = GetShaderLabPropertyDefaultValueString(fieldInfo.FieldType);
            paramMeta.attrib = GetParamAttributes(fieldInfo);
            return paramMeta;
        }
        
        public static ShaderParamStructMeta GetShaderParamStructMeta(Type structType,object exampleObject) {
            var paramStructMeta = new ShaderParamStructMeta();
            paramStructMeta.name = structType.Name;
            paramStructMeta.attrib = GetParamStructProperty(structType);
            List<ShaderParamMeta> memberMetas = new();
            object defaultObj = LangReflectionUtils.NewObjectInstance(structType);
            foreach (var fieldInfo in structType.GetFields()) {
                memberMetas.Add(GetShaderParamMeta(structType,fieldInfo,defaultObj));
            }
            paramStructMeta.memberMetas = memberMetas.ToArray();
            return paramStructMeta;
        }
        
        private static IEnumerable<Type> GetParamDescriptionStructs(Type paramTableType) {
            var nestedTypes = paramTableType.GetNestedTypes();
            foreach (var nestedType in nestedTypes) {
                if (nestedType.IsClass is false) {
                    if (nestedType.Name.EndsWith("Desc") is false) {
                        Debug.LogWarning("Material parameter description structs naming should ends with 'Desc'");
                    }
                    yield return nestedType;
                }
            }
        }
        
        public static IEnumerable<ShaderParamStructMeta> GetShaderParamStructMetas(Type paramTableType) {
            var paramStructs = GetParamDescriptionStructs(paramTableType);
            foreach (var paramStruct in paramStructs) {
                yield return GetShaderParamStructMeta(paramStruct,LangReflectionUtils.NewObjectInstance(paramStruct));
            }
        }

        public static ShaderMeta GetShaderMetadata(Type shaderConfigType) {
            ShaderMeta meta = null;
            if (LangReflectionUtils.IsTypeDerivedFrom<ShaderConfig>(shaderConfigType)) {
                meta = new ShaderMeta();
                meta.shaderConfig = LangReflectionUtils.NewObjectInstance(shaderConfigType) as ShaderConfig;
            }
            return meta;
        }
    }
}
```
