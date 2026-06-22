# Egaku source: `Common/RenderResource/RenderResource.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;

namespace Egaku.Rendering.Pipeline.Common {


    public class Resource {
        
    }
    
    public class RenderResource : IDisposable{
        
        
        public enum LifecyclePolicy {
            Persistent,
            PerFrame,
            Temporal,
            Transient,
            Manual
        }
        
        public enum Role {
            Producer,
            Consumer,
            Processor
        }
        
        
        
        public virtual void Create() {
            
        }

        public virtual void BeforeProduction() {
            
        }

        public virtual void AfterProduction() {
            
        }
        
        public virtual void BeforeConsumption() {
            
        }
        
        public virtual void AfterConsumption() {
            
        }
        
        public virtual void BeforeProcessing() {
            
        }
        
        public virtual void AfterProcessing() {
            
        }

        public virtual void AfterLosingAllReferences() {
            
        }
        
        public virtual void Destroy() {
            
        }

        public virtual void Dispose() {
            throw new NotImplementedException();
        }
    }

    public class RenderResource<TResource> : RenderResource where TResource : class { 
        private TResource _resourceInstance;
        public TResource ResourceInstance => _resourceInstance;
    }

    public class RenderResourceException : Exception {
        
        public RenderResourceException(string message) : base(message) {
            
        }
    }

    

    
}
```
