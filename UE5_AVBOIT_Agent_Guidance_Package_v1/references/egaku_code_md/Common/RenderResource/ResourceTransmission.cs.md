# Egaku source: `Common/RenderResource/ResourceTransmission.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿
using Debug = UnityEngine.Debug;

namespace Egaku.Rendering.Pipeline.Common {

    public struct Slot<T>  {
        private T _value; 
        public void Put(T value) {
            _value = value;
        }
        
        public T Get() {
            return _value;
        }
    }
    
    public class ResourcePipe {
    
    }
    
    public class ResourcePipe<T> : ResourcePipe {
        protected bool _written;
        protected readonly int _recvPortCount;
        protected int _readCount;

        private Slot<T> _slot;
        
        public int ReaderCount { get => _recvPortCount; }
        
        public ResourcePipe(int recvPortCount) {
            _slot = new Slot<T>();
            _recvPortCount = recvPortCount;
            Reset();
        }

        public void Reset() {
            _written = false;
            _readCount = 0;
        }

 
        public bool IsWritten() {
            return _written;
        }
        
        public bool CanRead() {
            return _written && _readCount < _recvPortCount;
        }

        public bool CanWrite() {
            return IsWritten() is false;
        }

        public bool AllRead() {
            return _readCount == _recvPortCount;
        }
        
        public void Write(T value) {
            if (CanWrite()) {
                _slot.Put(value);
                _written = true;
            } else {
                throw new RenderResourceException("Tried to write a resource pipe that's not writable.");
            }
        }
        
        public T Read() {
            if (_readCount < _recvPortCount) {
                _readCount++;
                T value = _slot.Get();
                if (AllRead()) {
                    Reset();
                }
                return value;
            } else {
                throw new RenderResourceException("Tried to read resource pipe multiple times.");
            }
        }

        
        public struct SendPort {
            private ResourcePipe<T> _pipe;

            public SendPort(ResourcePipe<T> pipe) {
                _pipe = pipe;
            }
            
            
        
            public void Send(T resource) {
                if (_pipe is null) {
                    throw new RenderResourceException("This resource pipe was not registered.");
                }
                if (_pipe.CanWrite()) {
                    _pipe.Write(resource);
                } else {
                    bool noReader = _pipe.ReaderCount == 0;
                    CodeScope.Debug(() => {
                            if (noReader is false) {
                                throw new RenderResourceException("Tried to write to a written resource pipe which its resource hasn't been read by all receivers yet!");
                            } else {
                                Debug.LogWarning("Tried to write a resource pipe that has no reader.");
                            }
                        }
                        );
                }
            }

            public void SetResourcePipe(ResourcePipe<T> pipe) {
                _pipe = pipe;
            }
        }
    
        public struct RecvPort { 
            private ResourcePipe<T> _pipe;

            public RecvPort(ResourcePipe<T> pipe) {
                _pipe = pipe;
            }
        
            public T Receive() {
                if (_pipe is null) {
                    throw new RenderResourceException("This resource pipe was not registered.");
                }
                if (_pipe.CanRead()) {
                    return _pipe.Read();
                }
                throw new RenderResourceException("Resource pipe read times over receiver count.");
            }
            
            public void SetResourcePipe(ResourcePipe<T> pipe) {
                _pipe = pipe;
            }
        }
    }

    
    
}
```
