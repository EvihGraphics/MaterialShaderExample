# Egaku source: `Editor/Utils/PipelineMetaUtils.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using System.Collections;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using Egaku.Rendering.Pipeline.Common;

namespace Egaku.Rendering.Pipeline.Utility {
    
    
    
    public static class PipelineMetaUtils {
        
        public static readonly string RenderFeatureNameSuffix = "RenderFeature"; 
        public static readonly string RenderPassNodeNameSuffix = "PassNode";
        

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private static void _CheckTypeInheritanceConstraint(Type type,Type expectedBaseType) {
            if (!type.IsSubclassOf(expectedBaseType)) {
                throw new ArgumentException($"{type.Name} must be a subclass of {expectedBaseType}");
            }
        }
        
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private static void _CheckTypeInheritanceConstraint<TBase>(Type type) {
            _CheckTypeInheritanceConstraint(type,typeof(TBase));
        }
        
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private static void _CheckTypeInheritanceConstraint<T,TBase>() {
            _CheckTypeInheritanceConstraint(typeof(T),typeof(TBase));
        }
        
        private static string _GetRenderFeatureName(Type renderFeatureType) {
            _CheckTypeInheritanceConstraint<RenderFeature>(renderFeatureType);
            string className = renderFeatureType.Name;
            return className.Substring(0, className.Length - RenderFeatureNameSuffix.Length);
        }

        private static string _GetRenderPassNodeName(Type renderPassNodeType) {
            _CheckTypeInheritanceConstraint<RenderPassNode>(renderPassNodeType);
            string className = renderPassNodeType.Name;
            return className.Substring(0, className.Length - RenderPassNodeNameSuffix.Length);
        }

        public static List<RenderPassNodeInputDesc> GetRenderPassNodeInputDesc(Type renderPassNodeType) {
            _CheckTypeInheritanceConstraint<RenderPassNode>(renderPassNodeType);
            var inputFields = LangReflectionUtils.GetFieldsWithAttribute<RenderPassInputAttribute>(renderPassNodeType);
            var inputDesc = new List<RenderPassNodeInputDesc>();
            foreach (var (field,attr) in inputFields) {
                var desc = new RenderPassNodeInputDesc();
                desc.Name = field.Name;
                desc.FieldInfo = field;
                desc.ContentType = field.FieldType.GetGenericArguments()[0];
                inputDesc.Add(desc);
            }
            return inputDesc;
        }
        
        public static List<RenderPassNodeOutputDesc> GetRenderPassNodeOutputDesc(Type renderPassNodeType) {
            _CheckTypeInheritanceConstraint<RenderPassNode>(renderPassNodeType);
            var outputFields = LangReflectionUtils.GetFieldsWithAttribute<RenderPassOutputAttribute>(renderPassNodeType);
            var outputDesc = new List<RenderPassNodeOutputDesc>();
            foreach (var (field,attr) in outputFields) {
                var desc = new RenderPassNodeOutputDesc();
                desc.Name = field.Name;
                desc.FieldInfo = field;
                desc.ContentType = field.FieldType.GetGenericArguments()[0];
                outputDesc.Add(desc);
            }
            return outputDesc;
        }
        
        public static List<RenderPassNodeDesc> GetRenderPassNodeDescFromFeature(Type renderFeatureType) {
            var renderPassNodesDesc = new List<RenderPassNodeDesc>();
            var passNodeTypes = renderFeatureType.GetNestedTypes();
            foreach (var passNodeType in passNodeTypes) {
                if (!passNodeType.IsSubclassOf(typeof(RenderPassNode))) {
                    continue;
                }
                var renderPassNodeDesc = new RenderPassNodeDesc();
                renderPassNodeDesc.Name = _GetRenderPassNodeName(passNodeType);
                renderPassNodeDesc.PassNodeType = passNodeType;
                renderPassNodeDesc.InputDesc = GetRenderPassNodeInputDesc(passNodeType).ToArray();
                renderPassNodeDesc.OutputDesc = GetRenderPassNodeOutputDesc(passNodeType).ToArray();
                renderPassNodesDesc.Add(renderPassNodeDesc);
            }

            return renderPassNodesDesc;
        }
        
        public static List<RenderFeatureDesc> GetAllRenderFeatureDesc() {
            var renderFeatureTypes = LangReflectionUtils.GetAllDerivedTypesCached<RenderFeature>();
            var renderFeaturesDesc = new List<RenderFeatureDesc>();
            foreach (var type in renderFeatureTypes) {
                var renderFeatureDesc = new RenderFeatureDesc();
                renderFeatureDesc.Name = _GetRenderFeatureName(type);
                renderFeatureDesc.PassNodesDesc = GetRenderPassNodeDescFromFeature(type).ToArray();
                renderFeaturesDesc.Add(renderFeatureDesc);
            }
            return renderFeaturesDesc;
        }
        
    }
    
}
```
