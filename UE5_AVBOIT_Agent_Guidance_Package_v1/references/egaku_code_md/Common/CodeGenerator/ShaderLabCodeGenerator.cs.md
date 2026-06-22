# Egaku source: `Common/CodeGenerator/ShaderLabCodeGenerator.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System.Text;

namespace Egaku.Rendering.Pipeline.Common {
    
    public class ShaderLabCodeGenerator : HLSLCodeGenerator{
        
        
        public ShaderLabCodeGenerator(string directory,string fileName):base(directory,fileName) {
        }
        
        public ShaderLabCodeGenerator() {
        }
        
        
        public void EmitShaderDeclaration(string shaderURL) {
            EmitLine($"Shader \"{shaderURL}\"");
        }

        public void EmitShaderPassName(string name) {
            EmitLine($"Name \"{name}\"");
        }
        
        public void EmitAttribute(string attribute) {
            EmitLine($"[{attribute}]");
        }
        
        public void EmitShaderDefinition(CodeGenerator implementationCodeGenerator,string shaderURL) {
            EmitShaderDeclaration(shaderURL);
            EmitCodeBlock(implementationCodeGenerator);
        }

        public void EmitPropertiesToken() {
            EmitLine($"Properties");
        }

        public void EmitProperties(CodeGenerator codeGen) {
            EmitPropertiesToken();
            EmitCodeBlock(codeGen);
        }
        

        public void EmitPropertyDeclaration(string name, string type, string label, string defaultValue,params string[] attributes) {
            foreach(var attribute in attributes) {
                EmitLine($"[{attribute}]");
            }
            EmitLine($"{name}(\"{label}\", {type}) = {defaultValue}");
        }


        public void EmitTagsToken() {
            EmitLine("Tags");
        }
        
        public void EmitTags(params (string, string)[] tags) {
            EmitTagsToken();
            BeginScope();
            foreach(var tag in tags) {
                EmitLine($"\"{tag.Item1}\" = \"{tag.Item2}\"");
            }
            EndScope();
        }

        public void EmitPassToken() {
            EmitLine("Pass");
        }
        
        public void EmitPass(CodeGenerator codeGen) {
            EmitPassToken();
            EmitCodeBlock(codeGen);
        }

        public void EmitHLSLIncludeToken() {
            EmitLine("HLSLINCLUDE");
        }
        
        public void EmitHLSLInclude(CodeGenerator codeGen) {
            EmitHLSLIncludeToken();
            EmitCodeBlock(codeGen, false);
            EmitEndHLSLToken();
        }

        public void EmitHLSLProgram(CodeGenerator codeGen) {
            EmitHLSLProgramToken();
            EmitCodeBlock(codeGen,false);
            EmitEndHLSLToken();
        }

        public void EmitCategory(CodeGenerator codeGen) {
            EmitCategoryToken();
            EmitCodeBlock(codeGen);
        }

        public void EmitShaderLabCommand(string command, string value) {
            EmitLine($"{command} {value}");
        }

        public void EmitCategoryToken() {
            EmitLine("Category");
        }
        
        public void EmitHLSLProgramToken() {
            EmitLine("HLSLPROGRAM");
        }

        public void EmitEndHLSLToken() {
            EmitLine("ENDHLSL");
        }
        
        public void EmitSubShaderToken() {
            EmitLine($"SubShader");
        }
        

        public void EmitSubShaderDefinition(CodeGenerator codeGen) {
            EmitSubShaderToken();
            EmitCodeBlock(codeGen);
        }
        

    }
}
```
