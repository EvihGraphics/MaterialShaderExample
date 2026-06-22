# Egaku source: `Common/CodeGenerator/HLSLCodeGenerator.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System.Text;

namespace Egaku.Rendering.Pipeline.Common {
    
    
    public class HLSLCodeGenerator : CodeGenerator {
        
        private string _includeGuardName;
        
        public HLSLCodeGenerator(string directory,string fileName):base($"{directory}/{fileName}") {
            _includeGuardName = _GetIncludeGuardName(fileName);
        }
        
        public HLSLCodeGenerator():base(null) {
        }


        private string _GetIncludeGuardName(string fileName) {
            return fileName.Replace(".", "_");
        }

        public void EmitIncludeGuardStart() {
            EmitLine($"#ifndef {_includeGuardName}");
            EmitLine($"#define {_includeGuardName}");
        }

        public void EmitCommentLine(string comment) {
            EmitLine($"// {comment}");
        }
        
        
        
        public void EmitIncludeGuardEnd() {
            EmitLine($"#endif // {_includeGuardName}");
        }

        public void EmitIncludeDirective(string fileName) {
            EmitLine($"#include \"{fileName}\"");
        }
        
        public void EmitPragmaDirective(string pragma) {
            EmitLine($"#pragma {pragma}");
        }
        
        
    

        private StringBuilder BuildStructSymbol(string name) {
            StringBuilder sb = new();
            sb.Append($"struct {name}");
            return sb;
        }
        
        public void EmitStructDefinition(string name, params (string, string)[] members) {
            EmitLine(BuildStructSymbol(name).ToString());
            BeginScope();
            foreach (var member in members) {
                EmitLine($"{member.Item1} {member.Item2};");
            }
            EndScope();
            Append(";");
        }
        
        private StringBuilder BuildFunctionSymbol(string returnType, string name) {
            StringBuilder sb = new();
            sb.Append($"{returnType} {name}()");
            return sb;
        }
        

        
        private StringBuilder BuildFunctionParameterList(params (string, string)[] parameters) {
            StringBuilder sb = new();
            int i = 0;
            sb.Append("(");
            for (; i < parameters.Length - 1; i++) {
                var (paramType,paramName) = parameters[i];
                sb.Append($"{paramType} {paramName}");
                sb.Append(", ");
            }

            if (i < parameters.Length) {
                var (paramType,paramName) = parameters[i];
                sb.Append($"{paramType} {paramName}");
            }
            sb.Append(")");
            return sb;
        }
        
        private StringBuilder BuildFunctionParameterList(params string[] parameters) {
            StringBuilder sb = new();
            int i = 0;
            sb.Append("(");
            for (; i < parameters.Length - 1; i++) {
                var paramName = parameters[i];
                sb.Append($"{paramName}");
                sb.Append(", ");
            }

            if (i < parameters.Length) {
                var paramName = parameters[i];
                sb.Append($"{paramName}");
            }
            sb.Append(")");
            return sb;
        }

        private StringBuilder BuildFunctionSymbol(string returnType, string name, params (string, string)[] parameters) {
            return BuildFunctionSymbol(returnType, name).Append(BuildFunctionParameterList(parameters));
        }
        
        public void EmitFunctionDeclaration(string returnType,string name, params (string,string)[] parameters) {
            BuildFunctionSymbol(returnType,name,parameters);
            Append(";");
        }
        
        public void EmitFunctionDefinition(CodeGenerator implementationCodeGenerator,string returnType,string name, params (string,string)[] parameters) {
            EmitLine(BuildFunctionSymbol(returnType,name,parameters).ToString());
            EmitCodeBlock(implementationCodeGenerator);
        }

        public void EmitFunctionDefinition(CodeGenerator implementationCodeGenerator,string returnType,string name) {
            EmitLine(BuildFunctionSymbol(returnType,name).ToString());
            EmitCodeBlock(implementationCodeGenerator);
        }



        public void EmitMacro(string name,string value) {
            EmitLine($"#define {name} {value}");
        }
        
        public void EmitMacroFunction(string name,string value,params string[] parameters) {
            EmitLine($"#define {name}{BuildFunctionParameterList(parameters)} {value}");
        }
        
        public void EmitVariableDeclaration(string type,string name) {
            EmitLine($"{type} {name};");
        }
        
        public void EmitInstancedVariableDeclaration(string type,string name) {
            EmitLine($"DEFINE_INSTANCED_PROP({type}, {name})");
        }
        
        public void EmitVariableAssignment(string left,string right) {
            EmitLine($"{left} = {right};");
        }

        public void EmitReturn(string returnName) {
            EmitLine($"return {returnName};");
        }
        
        public void EmitVariableDeclaration(string type,string name,string value) {
            EmitLine($"{type} {name} = {value};");
        }
        
        
    }
}
```
