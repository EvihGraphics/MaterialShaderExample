# Egaku source: `Common/RenderResource/ResourceHandle.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿

using System;
using System.Runtime.ConstrainedExecution;
using UnityEngine.Rendering;

namespace Egaku.Rendering.Pipeline.Common {


    public interface IHandle<T>: IDisposable {
        
    }

  
    // MUST use this struct in using() scope, or call Dispose() / Release() manually to release the resource.
    // MUST use Share() / Transfer() to explicitly share or transfer the resource ownership.
    // It's strictly forbidden to use assignment '=' to assign to a new Handle, this will cause an untracked
    // resource ownership sharing and finally cause a bug.
    // If using instance is needed, use ResourceRef to get a language-based object reference as it will function as 
    // a weak reference.

    public struct Handle<T> : IHandle<T> where T : class,new() {
        private ResourcePool<T> _pool;
        private PooledResource<T> _pooledResource;
        private bool _disposed;
        public Handle(ResourcePool<T> pool,in PooledResource<T> pooledResource) {
            _pooledResource = pooledResource;
            _pool = pool;
            _disposed = false;
        }

        public T ResourceRef => _pooledResource.Instance;
        
        public void Release() {
            Dispose();
        }

        public Handle<T> Share() {
            _pooledResource.Meta.IncreaseRefCount();
            return this;
        }

        public Handle<T> Transfer() {
            Handle<T> copy = this;
            Dispose();
            return copy;
        }
        
        
        public void Dispose() {
            if (_disposed is false) {
                _pooledResource.Meta.DecreaseRefCount();
                if (_pooledResource.Meta.HasLostAllRef()) {
                    _pool.Return(_pooledResource);
                }
                _pooledResource.Instance = null;
                _pooledResource.Meta = null;
                _pool = null;
                _disposed = true;
            } 
        }
    }

    public struct DistributedHandle<T> :IHandle<T> where T : class,new() {
        private ResourcePool<T> _pool;
        private PooledResource<T> _pooledResource;
        private bool _disposed;
        public T ResourceRef => _pooledResource.Instance;
        public DistributedHandle(ResourcePool<T> pool,in PooledResource<T> pooledResource) {
            _pool = pool;
            _pooledResource = pooledResource;
            _disposed = false;
        }
        
        public void Release() {
            Dispose();
        }
        
        public void Dispose() {
            if (_disposed) {
                return;
            }
            _pooledResource.Meta.DecreaseRefCount();
            if (_pooledResource.Meta.HasLostAllRef()) {
                _pool.Return(_pooledResource);
            }
            _pooledResource.Instance = null;
            _pooledResource.Meta = null;
            _pool = null;
            _disposed = true;
        }
        
    }

    /*
    public class ResourceHandle<T> where T : class {

        private T _instanceRef;


        public ResourceHandle(T instance) {
            _instanceRef = instance;

        }

        public ResourceHandle(ResourceHandle<T> other) {
            _instanceRef = other.Instance;
        }

        public ResourceHandle(ref ResourceHandle other) {
            //  _instanceRef = other.Instance;
        }

        public T Instance {
            get => _instanceRef;
            set => _instanceRef = value;
        }
    }
    */
    
    
    


}
```
