# Egaku source: `Common/RenderResource/ResourcePool.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using System.Collections.Generic;

namespace Egaku.Rendering.Pipeline.Common {
    public abstract class ResourcePool {
      
    }

   
    // ResourcePool OWNS the resource by a private-access typed PooledResource<T> struct.
    // A module is TRANSFERRED by ResourcePool and HOLDS the Handle, indicating that this module has the
    // responsibility to manage its lifecycle.
    // Typically, call 'Return()' if don't need the resource anymore.
    // A module gets the language designed object reference by Handle.Get indicates that this module uses
    // this resource but don't need to manage its lifecycle.
    // Be careful that the resource may be recycled if its Handle holder called 'Return()'.
    // A pooled resource will be recycled only when all its PooledResourceHandles are released! 

    public class PooledResourceMeta {
        private int _refCount;
        public PooledResourceMeta(int count) {
            _refCount = count;
        }
        public void SetRefCount(int refCount) {
            _refCount = refCount;
        }
        public void DecreaseRefCount() {
            if (_refCount < 0) {
                throw new Exception("Pooled resource ref count is negative.");
            }
            _refCount--;
        }
        public void IncreaseRefCount() {
            _refCount++;
        }
        public bool HasLostAllRef() {
            return _refCount == 0;
        }
    }
    public struct PooledResource<T> where T : class,new() {
        private T _resource;
        private PooledResourceMeta _meta;
        public T Instance {
            get { return _resource; }
            set { _resource = value; }
        }

        public PooledResourceMeta Meta {
            get { return _meta; }
            set { _meta = value; }
        }
        
        public PooledResource(T resourceInstance) {
            _resource = resourceInstance;
            _meta = new PooledResourceMeta(0);
        }
    }
    
    public class ResourcePool<T> : ResourcePool where T : class,new() {
        private readonly Stack<PooledResource<T>> _freedResources = new();

        public bool HasAvailableResource() {
            return _freedResources.Count != 0;
        }

        private void ExpandIfNecessary() {
            if (HasAvailableResource() is false) {
                Reserve(4);
            }
        }
        
        public void Reserve(int count) {
            for (int i = 0; i < count; i++) {
                _freedResources.Push(new PooledResource<T>(new T()));
            }
        }
        
        

        public Handle<T> Present() {
            ExpandIfNecessary();
            var resource = _freedResources.Pop();
            resource.Meta.IncreaseRefCount();
            return new Handle<T>(this, resource);
        }


        public DistributedHandle<T> Distribute(int count) {
            ExpandIfNecessary();
            var resource = _freedResources.Pop();
            resource.Meta.SetRefCount(count);
            return new DistributedHandle<T>(this, resource);
        }
        
        public void Return(in PooledResource<T> resource) {
            if (resource.Meta.HasLostAllRef() is false) {
                throw new Exception("Not all pooled resource handles are returned.");
            }
            _freedResources.Push(resource);
        }
    }
}
```
