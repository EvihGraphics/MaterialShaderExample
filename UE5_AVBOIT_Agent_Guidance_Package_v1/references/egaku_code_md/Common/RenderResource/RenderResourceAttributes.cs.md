# Egaku source: `Common/RenderResource/RenderResourceAttributes.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;


namespace Egaku.Rendering.Pipeline.Common {
    
    
    [AttributeUsage(AttributeTargets.Field,AllowMultiple = false)]
    public sealed class RenderPassInputAttribute : Attribute {
        private Type _producerPassNodeType;
        private string _name = null;
        public string Name {get => _name;set => _name = value;}
        public Type ProducerPassNodeType {get => _producerPassNodeType;
            set => _producerPassNodeType = value;
        }
        
        public RenderPassInputAttribute(Type producerPassNodeType) {
            ProducerPassNodeType = producerPassNodeType;
        }
        public RenderPassInputAttribute(Type producerPassNodeType,string name) {
            ProducerPassNodeType = producerPassNodeType;
            Name = name;
        }
    }
    
    
        
    [AttributeUsage(AttributeTargets.Field | AttributeTargets.Struct,AllowMultiple = false)]
    public sealed class RenderPassOutputAttribute : Attribute {
        private string _name = null;
        public string Name {get => _name;set => _name = value;}
        
        public RenderPassOutputAttribute() {
            Name = null;
        }
        
        public RenderPassOutputAttribute(string name) {
            Name = name;
        }
    }
    
    
    
    [AttributeUsage(AttributeTargets.Field,AllowMultiple = false)]
    public sealed class OutputResourceAttribute : Attribute {
        public string name = null;
    }
    
    
    [AttributeUsage(AttributeTargets.Field,AllowMultiple = false)]
    public sealed class SharedRenderResourceAttribute : Attribute {
        public RenderResource.Role role;
        public string name = null;
    }
    
        
    [AttributeUsage(AttributeTargets.Field,AllowMultiple = false)]
    public sealed class ReferencedRenderFeatureAttribute : Attribute {
        public Type type;
        private ReferencedRenderFeatureAttribute() {
        }
        
        public ReferencedRenderFeatureAttribute(Type type) {
            this.type = type;
        }
    }
    
    
    
    [AttributeUsage(AttributeTargets.Field,AllowMultiple = false)]
    public sealed class ReferencedRenderPassAttribute : Attribute {
        public Type type;
        private ReferencedRenderPassAttribute() {
        }
        public ReferencedRenderPassAttribute(Type type) {
            this.type = type;
        }
        
    }

    public class TestClass {
        
    }
        

    [AttributeUsage(AttributeTargets.Field,AllowMultiple = false)]
    public sealed class PipelineRenderContextAttribute : Attribute {
    }
    
    
}
```
