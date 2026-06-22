# Egaku source: `Runtime/EgakuRenderResourceManager.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿
using System;
using System.Collections.Generic;
using System.Reflection;
using Egaku.Rendering.Pipeline.Common;
using Egaku.Rendering.Pipeline.Utility;



namespace Egaku.Rendering.Pipeline.Runtime {
    public class EgakuRenderResourceManager : RenderResourceManager {
        
        private class RenderResourceEntry {
            public RenderResource resourceInstance;
            public string key;
            public string name;
            public int producerCount;
            public int consumerCount;
            public int processorCount;
        }
    
        private class RenderContextManager {
                
            private readonly List<RenderContext> _renderContexts = new();
            
            public int Register(RenderContext context) {
                _renderContexts.Add(context);
                return _renderContexts.Count - 1;
            }

            public RenderContext GetRenderContext(int index) {
                return _renderContexts[index];
            }

            public void Clear() {
                _renderContexts.Clear();
            }
        }
        
        private ResourceTransmissionManager _resourceTransmissionManager = new();
        
        private RenderContextManager _renderContextManager = new();

        private int _currentRenderContextIndex;
        
        public RenderContext CurrentRenderContext => _renderContextManager.GetRenderContext(_currentRenderContextIndex);

        private class RenderResourceReference {
            public string producer;
            public string consumer;
            public string name;
        }

        
        public EgakuRenderResourceManager()
        {
        }


        public int RegisterRenderContext(RenderContext context) {
            return _renderContextManager.Register(context);
        }

        public void ClearRenderContext() {
            _renderContextManager.Clear();
        }


        public void ClearResourceTransmissionRegistry() {
            _resourceTransmissionManager.Clear();
        }
        
        public void SetCurrentRenderContext(int index) {
            _currentRenderContextIndex = index;
        }
        
        public override RenderContext GetCurrentRenderContext() {
            return _renderContextManager.GetRenderContext(_currentRenderContextIndex);
        }
        
        
        public void RegisterResourceTransmission(Type resourcePipeType,RenderPassNode producerPassNode,string name) {
            var producerType = producerPassNode.GetType();
            var key = _resourceTransmissionManager.GetResourceKey(producerType,name);
            _resourceTransmissionManager.RegisterPassNodeAsResourceProducer(resourcePipeType,key, producerPassNode);
        }
        
        public ResourcePipe GetResourcePipe(Type producerPassNodeType,string name) { 
            var key = _resourceTransmissionManager.GetResourceKey(producerPassNodeType,name);
            return _resourceTransmissionManager.GetResourcePipe(key);
        }
        

        public void RegisterResourceTransmission(Type resourcePipeType,RenderPassNode consumerPassNode,string name,Type producerPassNodeType) {
            var producerType = producerPassNodeType;
            var key = _resourceTransmissionManager.GetResourceKey(producerType,name);
             _resourceTransmissionManager.RegisterPassNodeAsResourceConsumer(resourcePipeType,key, consumerPassNode);
        }

        public void RegisterResourceTransmission(Type resourcePipeType,RenderPassNode consumerPassNode,string name,RenderPassNode producerPassNode) {
            RegisterResourceTransmission(resourcePipeType,consumerPassNode, name, producerPassNode.GetType());
        }

        public void AutoRegisterResourceTransmissions(IEnumerable<RenderPassNode> passes) {
            foreach (var pass in passes) {
                AutoRegisterResourceTransmissions(pass);
            }
            if (_resourceTransmissionManager.CheckSanity() is false) {
                throw new RenderResourceException("Check sanity failed.");
            }
        }

        public void CreateResourcePipes() {
            _resourceTransmissionManager.CreateResourcePipes();
        }
        
        
        public void AutoRegisterResourceTransmissions(RenderPassNode pass) {
            Type passType = pass.GetType();

            Type GetResourcePipeType(FieldInfo fieldInfo) {
                Type pipeType = fieldInfo.FieldType.DeclaringType;
                Type contentType = fieldInfo.FieldType.GetGenericArguments()[0];
                return pipeType.MakeGenericType(contentType);
            }
            
            foreach (var (fieldInfo, attr)
                     in LangReflectionUtils.GetFieldsWithAttribute<RenderPassOutputAttribute>(passType)) {
                var name = attr.Name;
                if (name is null) {
                    name = fieldInfo.Name;
                }

                
                RegisterResourceTransmission(GetResourcePipeType(fieldInfo),producerPassNode: pass, name);
            }
            
            foreach (var (fieldInfo, attr)
                     in LangReflectionUtils.GetFieldsWithAttribute<RenderPassInputAttribute>(passType)) {
                var name = attr.Name;
                if (name is null) {
                    name = fieldInfo.Name;
                }
                // attr.ProducerPassNodeType won't be null.
                RegisterResourceTransmission(GetResourcePipeType(fieldInfo),consumerPassNode:pass,name,attr.ProducerPassNodeType);
            }
        }
        
        public void AutoSetResourcePipePorts(RenderPassNode pass) {
            Type passType = pass.GetType();
            foreach (var (fieldInfo, attr)
                     in LangReflectionUtils.GetFieldsWithAttribute<RenderPassOutputAttribute>(passType)) {
                var name = attr.Name;
                if (name is null) {
                    name = fieldInfo.Name;
                }
                var resourcePipeInstance = GetResourcePipe(pass.GetType(), name);
                fieldInfo.SetValue(pass,LangReflectionUtils.NewObjectInstance(fieldInfo.FieldType,resourcePipeInstance));
            }
            
            foreach (var (fieldInfo, attr)
                     in LangReflectionUtils.GetFieldsWithAttribute<RenderPassInputAttribute>(passType)) {
                var name = attr.Name;
                if (name is null) {
                    name = fieldInfo.Name;
                }
                // attr.ProducerPassNodeType won't be null.
                var resourcePipeInstance = GetResourcePipe(attr.ProducerPassNodeType, name);
                fieldInfo.SetValue(pass,LangReflectionUtils.NewObjectInstance(fieldInfo.FieldType,resourcePipeInstance));
            }
        }
        
        public void AutoSetResourcePipePorts(IEnumerable<RenderPassNode> passes) {
            foreach (var pass in passes) {
                AutoSetResourcePipePorts(pass);
            }
        }
    }
}
```
