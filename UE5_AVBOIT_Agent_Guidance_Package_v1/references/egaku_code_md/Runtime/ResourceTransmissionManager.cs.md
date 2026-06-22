# Egaku source: `Runtime/ResourceTransmissionManager.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using Egaku.Rendering.Pipeline.Utility;
using UnityEngine;
using UnityEngine.Assertions;

namespace Egaku.Rendering.Pipeline.Common {


    public class ResourceTransmissionManager {
        private class ResourceTransmissionMeta {
            public ResourcePipe resourcePipe;
            public RenderPassNode producerPassNode;
            public List<RenderPassNode> consumerPassNodes;
            public Type resourcePipeType;
        }


        private readonly Dictionary<string, ResourceTransmissionMeta> _resourceTransimission = new();

        private void _CreateEntryIfNotExists(string key, Type resourcePipeType) {
            if (LangReflectionUtils.IsTypeDerivedFrom<ResourcePipe>(resourcePipeType) is false) {
                throw new RenderResourceException(
                    "Only types derived from ResourcePipe is supported for transmission.");
            }

            if (_resourceTransimission.ContainsKey(key) is true) {
                return;
            }

            _resourceTransimission[key] = new ResourceTransmissionMeta {
                resourcePipe = null,
                producerPassNode = null,
                consumerPassNodes = new List<RenderPassNode>(),
                resourcePipeType = resourcePipeType
            };
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public string GetResourceKey(Type producerPassNodeType, string name) {
            string className = LangReflectionUtils.GetClassName(producerPassNodeType);
            return $"{name}@{className}";
        }

        public ResourcePipe GetResourcePipe(string key) {
            CodeScope.Debug(() => Assert.IsTrue(_resourceTransimission.ContainsKey(key)));
            return _resourceTransimission[key].resourcePipe;
        }

        public ResourcePipe GetResourcePipe(string key, Type requiredPipeType) {
            ResourcePipe resourcePipe = GetResourcePipe(key);
            if (resourcePipe.GetType() != requiredPipeType) {
                throw new RenderResourceException("Required handle type is not the same as retrieved one.");
            }

            return resourcePipe;
        }
        
        public void RegisterPassNodeAsResourceProducer(Type resourcePipeType, string key,
            RenderPassNode producerPass) {
            _CreateEntryIfNotExists(key, resourcePipeType);
            if (_resourceTransimission[key].producerPassNode is not null) {
                throw new RenderResourceException("A resource handle can have only one producer.");
            }
            _resourceTransimission[key].producerPassNode = producerPass;
        }

        public void RegisterPassNodeAsResourceConsumer(Type resourcePipeType, string key,
            RenderPassNode consumerPass) {
            _CreateEntryIfNotExists(key, resourcePipeType);
            _resourceTransimission[key].consumerPassNodes.Add(consumerPass);
        }

        public void RegisterResourceTransmission(Type resourcePipeType, string key, RenderPassNode passNode,
            RenderPassNode.Role role) {
            switch (role) {
                case RenderPassNode.Role.Consumer:
                    RegisterPassNodeAsResourceConsumer(resourcePipeType, key, passNode);
                    break;
                case RenderPassNode.Role.Producer:
                    RegisterPassNodeAsResourceProducer(resourcePipeType, key, passNode);
                    break;
            }
        }

        public bool CheckSanity() {
            foreach (var entry in _resourceTransimission) {
                if (entry.Value.producerPassNode is null) {
                    throw new RenderResourceException($"Resource handle {entry.Key} for transmission has no producer!");
                }

                if (entry.Value.consumerPassNodes.Count == 0) {
                    Debug.LogWarning($"Resource handle {entry.Key} for transmission has no consumer!");
                }

                HashSet<RenderPassNode> consumerPassNodes = new();
                foreach (var consumerPass in entry.Value.consumerPassNodes) {
                    if (consumerPassNodes.Contains(consumerPass)) {
                        throw new RenderResourceException(
                            $"Resource handle {entry.Key} for transmission has duplicate consumer! Check names of render pass nodes are distinct.");
                    }

                    if (entry.Value.producerPassNode == consumerPass) {
                        throw new RenderResourceException(
                            $"Resource handle {entry.Key} for transmission has passes functioned as both producer and consumer! " +
                            $"Check related attributes or register function calls are properly set.");
                    }

                    consumerPassNodes.Add(consumerPass);
                }

                consumerPassNodes.Clear();
            }

            return true;
        }


        public void CreateResourcePipes() {
            foreach (var entry in  _resourceTransimission) {
                int readerCount = entry.Value.consumerPassNodes.Count;
                Type pipeType = entry.Value.resourcePipeType;
                if (readerCount == 0) {
                    CodeScope.Debug(()=>Debug.LogWarning("Resource pipe has no reader!"));
                }
                entry.Value.resourcePipe = LangReflectionUtils.NewObjectInstance<ResourcePipe>(pipeType, readerCount);
            }
        }
        
        public void Clear() {
            _resourceTransimission.Clear();
        }
    }

}
```
