# Egaku source: `Common/CodeGenerator/CSharpRuntimeCodeBuilder.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System.Linq.Expressions;
using System.Reflection;
using UnityEngine.Rendering;
using System;
using System.Reflection;
using System.Reflection.Emit;
namespace Egaku.Rendering.Pipeline.Common {


public class DynamicClassCreator
{
    public static Type CreatePersonType()
    {
        // 1. 定义一个动态程序集
        var assemblyName = new AssemblyName("DynamicAssembly");
        var assemblyBuilder = AssemblyBuilder.DefineDynamicAssembly(assemblyName, AssemblyBuilderAccess.Run);

        // 2. 定义一个动态模块
        var moduleBuilder = assemblyBuilder.DefineDynamicModule("MainModule");

        // 3. 定义一个类
        var typeBuilder = moduleBuilder.DefineType(
            "DynamicPerson", 
            TypeAttributes.Public, 
            typeof(object) // 父类
        );

        // 4. 定义私有字段
        var nameField = typeBuilder.DefineField("_name", typeof(string), FieldAttributes.Private);
        var ageField = typeBuilder.DefineField("_age", typeof(int), FieldAttributes.Private);

        // 5. 定义属性 (Name)
        var nameProperty = typeBuilder.DefineProperty("Name", PropertyAttributes.HasDefault, typeof(string), null);
        
        // 5.1 定义 getter 方法
        var getNameMethod = typeBuilder.DefineMethod(
            "get_Name",
            MethodAttributes.Public | MethodAttributes.SpecialName | MethodAttributes.HideBySig,
            typeof(string),
            Type.EmptyTypes
        );
        var getNameIl = getNameMethod.GetILGenerator();
        getNameIl.Emit(OpCodes.Ldarg_0);           // 加载 'this'
        getNameIl.Emit(OpCodes.Ldfld, nameField);  // 加载 _name 字段的值
        getNameIl.Emit(OpCodes.Ret);               // 返回
        nameProperty.SetGetMethod(getNameMethod);

        // 5.2 定义 setter 方法
        var setNameMethod = typeBuilder.DefineMethod(
            "set_Name",
            MethodAttributes.Public | MethodAttributes.SpecialName | MethodAttributes.HideBySig,
            null,
            new Type[] { typeof(string) }
        );
        var setNameIl = setNameMethod.GetILGenerator();
        setNameIl.Emit(OpCodes.Ldarg_0);           // 加载 'this'
        setNameIl.Emit(OpCodes.Ldarg_1);           // 加载 value 参数
        setNameIl.Emit(OpCodes.Stfld, nameField);  // 将 value 存入 _name 字段
        setNameIl.Emit(OpCodes.Ret);
        nameProperty.SetSetMethod(setNameMethod);
        
        // 6. 定义另一个属性 (Age)，过程类似
        var ageProperty = typeBuilder.DefineProperty("Age", PropertyAttributes.HasDefault, typeof(int), null);
        
        var getAgeMethod = typeBuilder.DefineMethod("get_Age", MethodAttributes.Public | MethodAttributes.SpecialName | MethodAttributes.HideBySig, typeof(int), Type.EmptyTypes);
        var getAgeIl = getAgeMethod.GetILGenerator();
        getAgeIl.Emit(OpCodes.Ldarg_0);
        getAgeIl.Emit(OpCodes.Ldfld, ageField);
        getAgeIl.Emit(OpCodes.Ret);
        ageProperty.SetGetMethod(getAgeMethod);

        var setAgeMethod = typeBuilder.DefineMethod("set_Age", MethodAttributes.Public | MethodAttributes.SpecialName | MethodAttributes.HideBySig, null, new Type[] { typeof(int) });
        var setAgeIl = setAgeMethod.GetILGenerator();
        setAgeIl.Emit(OpCodes.Ldarg_0);
        setAgeIl.Emit(OpCodes.Ldarg_1);
        setAgeIl.Emit(OpCodes.Stfld, ageField);
        setAgeIl.Emit(OpCodes.Ret);
        ageProperty.SetSetMethod(setAgeMethod);

        // 7. 定义一个自定义方法 (Greet)
        var greetMethod = typeBuilder.DefineMethod(
            "Greet",
            MethodAttributes.Public,
            typeof(string),
            Type.EmptyTypes
        );
        var greetIl = greetMethod.GetILGenerator();
        greetIl.Emit(OpCodes.Ldstr, "Hello, my name is ");
        greetIl.Emit(OpCodes.Ldarg_0);
        greetIl.EmitCall(OpCodes.Callvirt, getNameMethod, null); // 调用 get_Name
        greetIl.Emit(OpCodes.Call, typeof(string).GetMethod("Concat", new Type[] { typeof(string), typeof(string) }));
        greetIl.Emit(OpCodes.Ret);
        // 现在 greetMethod 已经定义好

        // 8. 创建类型
        return typeBuilder.CreateType();
    }
}

// 使用示例
class Program
{
    static void Main()
    {
        // 创建动态类型
        Type dynamicPersonType = DynamicClassCreator.CreatePersonType();

        // 创建该类型的实例
        object instance = Activator.CreateInstance(dynamicPersonType);

        // 设置属性
        dynamicPersonType.GetProperty("Name").SetValue(instance, "Alice");
        dynamicPersonType.GetProperty("Age").SetValue(instance, 30);

        // 获取属性
        string name = (string)dynamicPersonType.GetProperty("Name").GetValue(instance);
        int age = (int)dynamicPersonType.GetProperty("Age").GetValue(instance);
        Console.WriteLine($"Name: {name}, Age: {age}");

        // 调用方法
        object result = dynamicPersonType.GetMethod("Greet").Invoke(instance, null);
        Console.WriteLine(result); // 输出: Hello, my name is Alice
    }
}
    public class CSharpRuntimeCodeBuilder {

        public static BinaryExpression MakeSimpleBinary<T>(ExpressionType type,string leftName,string rightName) {
            return Expression.MakeBinary(type,Expression.Variable(typeof(T),leftName),Expression.Variable(typeof(T),rightName));
        }



    }
}
```
