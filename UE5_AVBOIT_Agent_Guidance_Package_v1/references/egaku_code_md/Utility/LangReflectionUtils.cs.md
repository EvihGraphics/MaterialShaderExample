# Egaku source: `Utility/LangReflectionUtils.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Reflection;
using System.Runtime.CompilerServices;

using UnityEditor;


namespace Egaku.Rendering.Pipeline.Utility {

    
    public static class LangReflectionUtils {

        [Obsolete]
        public static IEnumerable<Type> GetAllDerivedTypes<T>() {
            Type baseType = typeof(T);
            Assembly assembly = Assembly.GetExecutingAssembly();
            foreach (Type type in assembly.GetTypes()) {
                if (type.BaseType != null && type.BaseType == baseType) {
                    yield return type;
                }
            }
        }
        
        
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static IEnumerable<Type> GetAllDerivedTypesCached<T>() {
            return TypeCache.GetTypesDerivedFrom<T>();
        }
        
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static IEnumerable<Type> GetAllDerivedTypesCached<T>(string assemblyName) {
            return TypeCache.GetTypesDerivedFrom<T>(assemblyName);
        }
        
        public static IEnumerable<(FieldInfo,TAttribute)> GetFieldsWithAttribute<TAttribute>(Type objectType) where TAttribute : Attribute {
            foreach (FieldInfo fieldInfo in objectType.GetFields()) {
                TAttribute attribute = fieldInfo.GetCustomAttribute<TAttribute>();
                if (attribute != null) {
                    yield return (fieldInfo, attribute);
                }
            }
          
        } 
        
        public static IEnumerable<(Type,TAttribute)> GetTypesWithAttribute<TAttribute>() where TAttribute : Attribute {
            IEnumerable<Type> types = TypeCache.GetTypesWithAttribute<TAttribute>();
            foreach (var type in types) {
                TAttribute attribute = type.GetCustomAttribute(typeof(TAttribute)) as TAttribute;
                if (attribute != null) {
                    yield return (type,attribute);
                }
            }
          
        } 
        
        
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static IEnumerable<(FieldInfo,TAttribute)> GetFieldsWithAttribute<TObject, TAttribute>() where TAttribute : Attribute {
            return GetFieldsWithAttribute<TAttribute>(typeof(TObject));
        }

        
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static TObject NewObjectInstance<TObject>() {
            return Activator.CreateInstance<TObject>();
        }
        
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static TObject NewObjectInstance<TObject>(Type type, params object[] args) {
            return (TObject)Activator.CreateInstance(type, args);
        }
        

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static object NewObjectInstance(Type type, params object[] args) {
            return Activator.CreateInstance(type, args);
        }
        
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static TObject NewObjectInstance<TObject>(Type objectType) where TObject :class, new(){
            return Activator.CreateInstance(objectType) as TObject;
        }
        
        

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool IsTypeDerivedFrom<T>(Type type) {
            return type.IsSubclassOf(typeof(T));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool HasAttribute<TAttribute>(FieldInfo fieldInfo) where TAttribute : Attribute{
            return fieldInfo.GetCustomAttribute<TAttribute>() is not null;
        }

   
        public static string GetClassName(Type type) {
            string nameWithNamespace = type.FullName;
            string classnameWithNested = nameWithNamespace.Substring(nameWithNamespace.LastIndexOf('.') + 1);
            return classnameWithNested.Replace('+', '.');
        }
        
        public static StackFrame GetThisFunctionCaller() {
             return new StackFrame(2);
        }
    }

}
```
