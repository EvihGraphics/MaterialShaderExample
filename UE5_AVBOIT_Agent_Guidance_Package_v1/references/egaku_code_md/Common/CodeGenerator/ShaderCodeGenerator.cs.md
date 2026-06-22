# Egaku source: `Common/CodeGenerator/ShaderCodeGenerator.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using System.Collections.Generic;
using Egaku.Rendering.Pipeline.Common.Shader;
using Egaku.Rendering.Pipeline.Utility;
using UnityEngine;

namespace Egaku.Rendering.Pipeline.Common {
    public class ShaderCodeGenerator {

        public static void GenerateAllComputeShaderAssetFiles() {
            var computeShaderConfigTypes = LangReflectionUtils.GetAllDerivedTypesCached<ComputeShaderConfig>();
            foreach (var configType in computeShaderConfigTypes) {
                var config = LangReflectionUtils.NewObjectInstance(configType) as ComputeShaderConfig;
                GenerateComputeShaderAssetFile(config);
            }
        }
        
        public static void GenerateAllBasicCodes() {
            var shaderConfigTypes = LangReflectionUtils.GetAllDerivedTypesCached<ShaderConfig>();
     
            foreach (var configType in shaderConfigTypes) {
                var shaderMeta = ShaderMetaUtils.GetShaderMetadata(configType);
                var shaderConfig = shaderMeta.shaderConfig;
                GenerateHLSLParamTableCode(shaderConfig.ParamTableType, shaderConfig.ShaderFileDirectory,shaderConfig.ParamTableFileName);
                GenerateHLSLParamStructCode(shaderConfig.ParamTableType,shaderConfig.ShaderFileDirectory,shaderConfig.ParamStructFileName,shaderConfig.ParamTableFileName);
                GenerateShaderLabFile(shaderMeta,shaderConfig.ShaderFileDirectory, $"{shaderConfig.ShaderName}.shader");
            }
          
        }


        public static void GenerateComputeShaderAssetFile(ComputeShaderConfig config) {
            ComputeShaderCodeGenerator codeGen = new ComputeShaderCodeGenerator(config.FileDirectory,config.FileName);
            HashSet<string> includedFiles = new();
            foreach (var kernelConfig in config.KernelConfigs) {
                foreach(var includedFile in kernelConfig.IncludedFiles) {
                    includedFiles.Add(includedFile);
                }
            }
            codeGen.EmitCommentLine($"This file was generated from {config.GetType().FullName} by {typeof(ShaderCodeGenerator).FullName}.");
            codeGen.EmitCommentLine($"Please do not modify it manually.");
            codeGen.EmitBlankLine();
            foreach (var includedFile in includedFiles) {
                codeGen.EmitIncludeDirective(includedFile);
            }
            codeGen.EmitBlankLine(2);
            foreach (var kernelConfig in config.KernelConfigs) { 
                codeGen.EmitKernelDeclaration(kernelConfig.KernelSymbolName);
            }
            codeGen.SaveToFile();
        }
        
        public static void GenerateHLSLParamTableCode(Type paramTableType,string directory, string fileName) {
            var paramStructMetas = ShaderMetaUtils.GetShaderParamStructMetas(paramTableType);
            HLSLCodeGenerator codeGen = new HLSLCodeGenerator(directory,fileName);

            Dictionary<string, List<(string, string)>> cbufferMap = new();
            Dictionary<string, List<(string, string)>> instancedCBufferMap = new();
            
            foreach (var paramStructMeta in paramStructMetas) {
                string cbufferName = paramStructMeta.attrib.cbufferName;
                foreach (var memberMeta in paramStructMeta.memberMetas) {
                    HLSLType hlslType = ShaderMetaUtils.GetHLSLType(memberMeta.type);
                    string hlslTypeToken = ShaderMetaUtils.GetHLSLTypeToken(memberMeta.type);
                    ref Dictionary<string, List<(string, string)>> map = ref cbufferMap;
                    if (paramStructMeta.attrib.instanced) {
                        map = ref instancedCBufferMap;
                    }

                    // Textures do not need to be declared in cbuffer
                    string targetCBufferName = cbufferName;
                    if (ShaderVariableType.IsTexture(hlslType)) {
                        targetCBufferName = "";
                    }
                    string globalVarName = memberMeta.uniformName;
                    map.TryGetValue(targetCBufferName, out var paramList);
                    paramList ??= new List<(string, string)>();
                    paramList.Add((hlslTypeToken, globalVarName));
                    map[targetCBufferName] = paramList;
                }
            }
            
            HLSLCodeGenerator cbufferDeclCodeGen = new();
            foreach (var entry in cbufferMap) {
                if (entry.Key != "") {
                    cbufferDeclCodeGen.EmitCBufferBegin(entry.Key);
                }
                
                foreach (var (type,name) in entry.Value) {
                    cbufferDeclCodeGen.EmitVariableDeclaration(type,name);
                }

                if (entry.Key != "") {
                    cbufferDeclCodeGen.EmitCBufferEnd();
                }
       
                cbufferDeclCodeGen.EmitBlankLine(2);
            }
            
            HLSLCodeGenerator instancedCBufferDeclCodeGen = new();
   
            foreach (var entry in instancedCBufferMap) {
                instancedCBufferDeclCodeGen.EmitInstancedCBufferBegin(entry.Key);
                foreach (var (type,name) in entry.Value) {
                    instancedCBufferDeclCodeGen.EmitInstancedVariableDeclaration(type,name);
                }
                instancedCBufferDeclCodeGen.EmitInstancedCBufferEnd(entry.Key);
            }
            
            codeGen.EmitCommentLine($"This file was generated from {paramTableType.FullName} by {typeof(ShaderCodeGenerator).FullName}.");
            codeGen.EmitCommentLine($"Please do not modify it manually.");
            codeGen.EmitCommentLine($"Codes in this file are functioned as material parameter table, which is used to declare some uniform variables and macro constants.");
            codeGen.EmitBlankLine();
            codeGen.EmitIncludeGuardStart();
            
            codeGen.EmitIncludeDirective("Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Core.hlsl");
            codeGen.EmitBlankLine(2);
            codeGen.EmitCodes(cbufferDeclCodeGen);
            codeGen.EmitBlankLine(2);
            
            codeGen.EmitCodes(instancedCBufferDeclCodeGen);
            codeGen.EmitBlankLine(2);
            
            codeGen.EmitIncludeGuardEnd();
            codeGen.SaveToFile();
        }

        
        public static void GenerateShaderLabFile(ShaderMeta shaderMeta, string directory, string fileName) { 
            ShaderLabCodeGenerator codeGen = new(directory,fileName);
            codeGen.EmitShaderDeclaration(shaderMeta.shaderConfig.ShaderURL);
            
            ShaderLabCodeGenerator shaderDefCodeGen = new();
          
            // Generate properties
            ShaderLabCodeGenerator subCodeGen = new();
            ref var propertiesCodeGen = ref subCodeGen; 
            var paramStructMetas = ShaderMetaUtils.GetShaderParamStructMetas(shaderMeta.shaderConfig.ParamTableType);
            foreach (var paramStructMeta in paramStructMetas) {
                if (paramStructMeta.attrib.inspectorHeader == true) {
                    string inspectorHeaderName = paramStructMeta.attrib.inspectorHeaderName;
                    propertiesCodeGen.EmitBlankLine();
                    propertiesCodeGen.EmitAttribute($"Header({inspectorHeaderName})");
                }
                foreach (var memberMeta in paramStructMeta.memberMetas) {
                    if (memberMeta.attrib.exposed) {
                        string shaderLabTypeToken = ShaderMetaUtils.GetShaderLabTypeToken(memberMeta.type);
                        string globalVarName = memberMeta.uniformName;
                        string label = memberMeta.attrib.label;
                        string defaultValString =
                            ShaderMetaUtils.GetShaderLabPropertyDefaultValueString(memberMeta.type);
                        if (memberMeta.attrib.hdrColor) {
                            propertiesCodeGen.EmitAttribute("HDR");
                        }

                        if (memberMeta.attrib.ranged) {
                            shaderLabTypeToken = $"Range({memberMeta.attrib.rangeMin},{memberMeta.attrib.rangeMax})";
                            defaultValString = $"{memberMeta.attrib.rangeMin}";
                        }
                        propertiesCodeGen.EmitPropertyDeclaration(globalVarName,shaderLabTypeToken,label,defaultValString);
                    }
                }
            }
            shaderDefCodeGen.EmitProperties(propertiesCodeGen);
            shaderDefCodeGen.EmitBlankLine();
            
   

            
            // Generate global macro definitions and public param table declaration.
            subCodeGen.Reset();
            ref var publicIncludesCodeGen = ref subCodeGen; 
            // Generate public global macro definitions
            foreach (var macroDef in shaderMeta.shaderConfig.PublicGlobalMacros) {
                publicIncludesCodeGen.EmitMacro(macroDef.Item1,macroDef.Item2);
            }
            publicIncludesCodeGen.EmitIncludeDirective(shaderMeta.shaderConfig.ParamTableFileName);
            shaderDefCodeGen.EmitHLSLInclude(publicIncludesCodeGen);
            shaderDefCodeGen.EmitBlankLine();
            
            
            ref var subShaderCodeGen = ref subCodeGen;
            // Generate sub shaders
            foreach (var subShaderConfig in shaderMeta.shaderConfig.SubShaderConfigs) {
                subShaderCodeGen.Reset();
                subShaderCodeGen.EmitTags(subShaderConfig.Tags);
                subShaderCodeGen.EmitBlankLine();
                
                ShaderLabCodeGenerator passCodeGen = new();
                // Generate pass
                foreach (var passConfig in subShaderConfig.PassConfigs) {
                 
                    passCodeGen.Reset();

                    if (passConfig.PassName != null) {
                        passCodeGen.EmitShaderPassName(passConfig.PassName);
                    }
                    passCodeGen.EmitBlankLine();
                    
                    // Generate pipeline states
                    foreach (var pipelineStateString in passConfig.PipelineStates) {
                        passCodeGen.EmitLine(pipelineStateString);
                    }
                    passCodeGen.EmitBlankLine();
                    
                    // Generate HLSLPROGRAM block
                    ShaderLabCodeGenerator programCodeGen = new();
                    foreach (var includedFile in passConfig.IncludedFiles) {
                        programCodeGen.EmitIncludeDirective(includedFile);
                    }
                    programCodeGen.EmitBlankLine();
                    foreach (var directive in passConfig.PragmaDirectives ) {
                        programCodeGen.EmitPragmaDirective(directive);
                    }
                    programCodeGen.EmitPragmaDirective($"vertex {passConfig.VSSymbol}");
                    programCodeGen.EmitPragmaDirective($"fragment {passConfig.PSSymbol}");
                    passCodeGen.EmitHLSLProgram(programCodeGen);
                    
                    subShaderCodeGen.EmitPass(passCodeGen);
                    subShaderCodeGen.EmitBlankLine();
                }
            }
            
            shaderDefCodeGen.EmitSubShaderDefinition(subShaderCodeGen);
            
            codeGen.EmitCodeBlock(shaderDefCodeGen);
            codeGen.SaveToFile();
        }
        

        public static void GenerateHLSLParamStructCode(Type paramTableType, string directory, string fileName,
            string paramTableFileName) {
            var paramStructMetas = ShaderMetaUtils.GetShaderParamStructMetas(paramTableType);

            HLSLCodeGenerator codeGen = new HLSLCodeGenerator(directory, fileName);
            codeGen.EmitCommentLine(
                $"This file was generated from {paramTableType.FullName} by {typeof(ShaderCodeGenerator).FullName}.");
            codeGen.EmitCommentLine($"Please do not modify it manually.");

            codeGen.EmitBlankLine();

            codeGen.EmitIncludeGuardStart();
            codeGen.EmitIncludeDirective(paramTableFileName);
            codeGen.EmitBlankLine(2);

            codeGen.EmitMacroFunction("FetchParam","Get##structName()","structName");
            codeGen.EmitMacroFunction("DeclareParamFetch","structName var = FetchParam(structName)","structName","var");
            
            codeGen.EmitBlankLine(2);
            
            HLSLCodeGenerator funcCodeGen = new HLSLCodeGenerator();
            foreach (var paramStructMeta in paramStructMetas) {
                string structTypeName = paramStructMeta.name;
                
                (string, string)[] memberDeclarations = new (string, string)[paramStructMeta.memberMetas.Length];
                
                int i = 0;
                string structVarName = "result";
            
                // Generate getter function for the struct.
                funcCodeGen.EmitVariableDeclaration(structTypeName, structVarName);
                
                foreach (var paramMeta in paramStructMeta.memberMetas) {
                    string fieldName = paramMeta.name;
                    string hlslType = ShaderMetaUtils.GetHLSLTypeToken(paramMeta.type);
                    string varName;
                    if (paramStructMeta.attrib.instanced) {
                        varName = $"ACCESS_INSTANCED_PROP({paramStructMeta.attrib.cbufferName}, {paramMeta.uniformName})";
                    } else {
                        varName = paramMeta.uniformName;
                    }
                    memberDeclarations[i++] = (hlslType, fieldName);
                    funcCodeGen.EmitVariableAssignment($"{structVarName}.{fieldName}", varName);
                }

                funcCodeGen.EmitReturn(structVarName);

                codeGen.EmitCommentLine($"Declare material parameter struct {structTypeName}");
                codeGen.EmitStructDefinition(structTypeName, memberDeclarations);
                codeGen.EmitBlankLine(2);
                codeGen.EmitCommentLine($"Getter for struct {structTypeName}");
                codeGen.EmitFunctionDefinition(funcCodeGen, structTypeName, $"Get{structTypeName}");
                codeGen.EmitBlankLine(2);
                funcCodeGen.Reset();
            }
            
            codeGen.EmitBlankLine(2);
            codeGen.EmitIncludeGuardEnd();
            codeGen.SaveToFile();
        }
        
    }
}
```
