# Egaku source: `Shaders/Mesh/CBT/CBTIndirect.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef CBT_INDIRECT_HLSL
#define CBT_INDIRECT_HLSL
#include "CBTMacroDefs.hlsl"
#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Utilities/CBT/ConcurrentBinaryTree.hlsl"

struct DrawIndirectArgs
{
    uint indexCountPerInstance;
    uint instanceCount;
    uint startIndex;
    uint baseVertexIndex;
    uint startInstance;
};

struct DispatchIndirectArgs
{
    uint threadGroupsX;
    uint threadGroupsY;
    uint threadGroupsZ;
};



DrawIndirectArgs GetMeshDrawIndirectArgs(DrawIndirectArgs oldArgs)
{
    DrawIndirectArgs indirectArgs = oldArgs;
    indirectArgs.instanceCount = cbt_NodeCount();
    return indirectArgs;
}


DrawIndirectArgs GetMeshDrawIndirectArgs(DrawIndirectArgs oldArgs,uint evalCount)
{
    DrawIndirectArgs indirectArgs = oldArgs;
    indirectArgs.instanceCount = evalCount;
    return indirectArgs;
}

DispatchIndirectArgs GetCBTUpdateDispatchIndirectArgs()
{
    uint nodeCount = cbt_NodeCount();
    uint dispatchGroupCount = nodeCount / CBT_UPDATE_GROUP_SIZE + 1;
 
    DispatchIndirectArgs indirectArgs;
    indirectArgs.threadGroupsX = dispatchGroupCount;
    indirectArgs.threadGroupsY = 1;
    indirectArgs.threadGroupsZ = 1;
    return indirectArgs;
}

DispatchIndirectArgs GetCBTUpdateDispatchIndirectArgs(uint evalCount)
{
    DispatchIndirectArgs indirectArgs;
    uint nodeCount = evalCount;
    uint dispatchGroupCount = nodeCount / CBT_UPDATE_GROUP_SIZE + 1;
    indirectArgs.threadGroupsX = dispatchGroupCount;
    indirectArgs.threadGroupsY = 1;
    indirectArgs.threadGroupsZ = 1;
    return indirectArgs;
}

DispatchIndirectArgs GetCBTEvalDispatchIndirectArgs()
{
    DispatchIndirectArgs indirectArgs;
    uint nodeCount = cbt_NodeCount();
    uint dispatchGroupCount = nodeCount / CBT_EVAL_GROUP_SIZE + 1;
 
    indirectArgs.threadGroupsX = dispatchGroupCount;
    indirectArgs.threadGroupsY = 1;
    indirectArgs.threadGroupsZ = 1;
    return indirectArgs;
}

#endif
```
