# Egaku source: `Common/CodeGenerator/CodeGenerator.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using UnityEngine;

namespace Egaku.Rendering.Pipeline.Common {
    
    
    public class CodeGenerator {
        private List<string> _codeLines = new();
        private StringBuilder _code = new();
        private int _indentLevel = 0;
        private string _fileName;
        private static string _projectPath = Application.dataPath;
        
        public CodeGenerator(string fileName) {
            _fileName = fileName;
            Reset();
        }
        
        public CodeGenerator() {
            Reset();
        }

        private string _GetAbsoluteFilePath() {
            return $"{_projectPath}/{_fileName}";
        }
        
        public List<string> GetCodeLines() {
            return _codeLines;
        }

        public void Reset() {
            _indentLevel = 0;
            _code.Clear();
            _codeLines.Clear();
        }

        public string GetIndent() {
            return new string('\t', _indentLevel);
        }
        
        
        public void Append(string code) {
            if (_codeLines.Count == 0) {
                _codeLines.Add(code);
            } else {
                string old = _codeLines.Last();
                _codeLines[_codeLines.Count - 1] = $"{old}{code}";
            }
        }
        
        public void EmitLine(string code) {
            _codeLines.Add($"{GetIndent()}{code}");
        }
        
        public void EmitCBufferBegin(string cbufferName) {
            EmitLine($"CBUFFER_BEGIN({cbufferName})");
            BeginScope(false);
        }
        
        public void EmitCBufferEnd() {
            EndScope(false);
            EmitLine("CBUFFER_END");
        }
        
        public void EmitInstancedCBufferBegin(string cbufferName) {
            EmitLine($"INSTANCING_BUFFER_BEGIN({cbufferName})");
            BeginScope(false);
        }
        
        public void EmitInstancedCBufferEnd(string cbufferName) {
            EndScope(false);
            EmitLine($"INSTANCING_BUFFER_END({cbufferName})");
        }
        
        public void EmitBlankLine(int count = 1) {
            while (count > 0) {
                _codeLines.Add("");
                count--;
            }
        }
        
        public void BeginScope(bool withBrace = true) {
            if (withBrace) {
                EmitLine($"{{");
            }
            PushIndent();
        }
        
        public void EndScope(bool withBrace = true) {
            PopIndent();
            if (withBrace) {
                EmitLine($"}}");
            }
        }

        public void EmitCodeBlock(CodeGenerator codeGenerator,bool withBrace = true) {
            BeginScope(withBrace);
            foreach (var line in codeGenerator.GetCodeLines()) {
                EmitLine(line);
            }
            EndScope(withBrace);
        }
        
        public void EmitCodes(CodeGenerator codeGenerator) {
            foreach (var line in codeGenerator.GetCodeLines()) {
                EmitLine(line);
            }
        }
        
        public void PushIndent(int count = 1) {
            while (count > 0) {
                _indentLevel++;
                count--;
            }
        }
        
        public void PopIndent(int count = 1) {
            while (count > 0) {
                _indentLevel--;
                count--;
            }
        }

        public void SetIndent(int indentLevel) {
            _indentLevel = indentLevel;
        }

        public void ClearIndent() {
            SetIndent(0);
        }


        public void SaveToFile() {
            using (var writer = new StreamWriter(_GetAbsoluteFilePath())) {
                foreach (var line in _codeLines) {
                    writer.WriteLine(line);
                }
            }
            
        }
        
        #region UNUSED_CODES

        class Person {
            private int a;
        }
        public static void Gen() {
            Expression param = Expression.Parameter(typeof(Person), "sdfg");
            Expression ds = Expression.Field(param,"a");
            Expression t = Expression.Assign(ds,Expression.Constant(1));
            Expression t2 = Expression.Assign(ds,Expression.Variable(typeof(int)));
        }

        #endregion

        
    }
}
```
