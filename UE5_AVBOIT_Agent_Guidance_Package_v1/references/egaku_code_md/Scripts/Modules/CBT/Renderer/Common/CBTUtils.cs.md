# Egaku source: `Scripts/Modules/CBT/Renderer/Common/CBTUtils.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using Modules.CBT.Renderer.Common;
using Unity.Collections;
using UnityEngine;
using UnityEngine.Assertions;
using UnityEngine.Rendering;
using UnityEngine.UIElements;

namespace Modules.CBT.Renderer {
    
    public static class CBTUtils {
        

        
        public static int GetCBTBufferSizeInBytes(int cbtMaxDepth) {
            Assert.IsTrue(cbtMaxDepth >= 5 && cbtMaxDepth <= 31);
            return 1 << (cbtMaxDepth - 1);
        }
        
        public static int GetCBTBufferSizeIn4Bytes(int cbtMaxDepth) {
            Assert.IsTrue(cbtMaxDepth >= 5 && cbtMaxDepth <= 31);
            return 1 << (cbtMaxDepth - 3);
        }

        private static int _GetNodeDepth(int nodeID) {
            int x = nodeID;
            int msb = 0;
            while (x > 1) {
                x>>=1;
                msb++;
            }
            return msb;
        }

        public static int GetCBTLevelNodeCount(int maxDepth) {
            return 1 << maxDepth;
        }


        private static int _GetNodeBitCount(int nodeDepth, int maxDepth) {
            return maxDepth - nodeDepth + 1;
        }


        private static int _NodeBitOffset(in CBTNode node,int maxDepth) {
            var tmp1 = 2 << node.depth;
            var tmp2 = 1 + maxDepth - node.depth;
            return tmp1 + node.id * tmp2;
        }

        public static CBTNode GetCeilNode(in CBTNode node,int maxDepth) {
            CBTNode ceilNode;
            ceilNode.id = node.id <<(maxDepth - node.depth);
            ceilNode.depth = maxDepth;
            return ceilNode;
        }
        
        private static int _LeafNodeBitfieldOffset(in CBTNode node,int maxDepth) {
            return _NodeBitOffset(GetCeilNode(node,maxDepth), maxDepth);
        }


        private static void _SetBit(ref ulong buf,int bitOffset,bool bit) {
            ulong bitMask = ~(1ul << bitOffset);
            buf &= bitMask; // clear bit
            buf |= (bit ? (1ul << bitOffset) : 0); //  set bit
        }
        
        private static void _SetBits(ref ulong buf,int bitOffset,ulong bits,int bitCount) {
            ulong bitMask = ~(~(0xFFFFFFFFFFFFFFFFul << bitCount) << bitOffset);
            buf &= bitMask; // clear bit
            buf |= (bits << bitOffset);
        }
        
        public static void SetLeafNodeBitfield(in Span<ulong> cbtBufferBytes,in CBTNode node, int maxDepth, bool bit) {
            int bitfieldOffset = _LeafNodeBitfieldOffset(node, maxDepth);
            int alignment = 8 * sizeof(ulong);
            _SetBit(ref cbtBufferBytes[bitfieldOffset /  alignment], bitfieldOffset %  alignment, bit);
        }
        
        public static void SetNodeBits(in Span<ulong> cbtBuffer,in CBTNode node, int maxDepth,ulong bits,int bitCount) {
            Assert.IsTrue(bitCount <= 64);
            int bitfieldOffset = _NodeBitOffset(node, maxDepth);
            int alignment = sizeof(ulong) * 8;
            ref ulong lowerBits = ref cbtBuffer[bitfieldOffset / alignment];
            int lbOffset = bitfieldOffset % alignment;
            int lowerBitCount = Math.Min(alignment - lbOffset, bitCount);
            _SetBits(ref lowerBits, lbOffset, bits, lowerBitCount);
            if (bitfieldOffset / alignment + 1 < cbtBuffer.Length) {
                ref ulong higherBits = ref cbtBuffer[bitfieldOffset / alignment + 1];
                int higherBitCount = bitCount - lowerBitCount;
                _SetBits(ref higherBits, 0, bits >> lowerBitCount, higherBitCount);
            }
        }
        
                
        public static void SetNodeBits(in Span<ulong> cbtBuffer,in CBTNode node, int maxDepth,ulong bits) {
            int bitCount = _GetNodeBitCount(node.depth, maxDepth);
            SetNodeBits(cbtBuffer, node, maxDepth, bits, bitCount);
        }

        

        public static GraphicsBuffer SetupCBTBuffer(int cbtMaxDepth) {
            int cbtBufferSize = GetCBTBufferSizeInBytes(cbtMaxDepth);
            int stride = sizeof(uint);
            Assert.IsTrue(cbtBufferSize % stride == 0);
            GraphicsBuffer cbtBuffer = new GraphicsBuffer(GraphicsBuffer.Target.Structured, cbtBufferSize / stride, stride);
            var cbtBufferUint64 = new NativeArray<ulong>(cbtBufferSize / sizeof(ulong), Allocator.Temp);
       
            cbtBufferUint64[0] = (ulong)GetCBTLevelNodeCount(cbtMaxDepth);
            for (int i = 1; i < cbtBufferUint64.Length; i++) {
                cbtBufferUint64[i] = 0;
            }
            CBTNode root = new CBTNode { id = 1, depth = 0 };
            CBTNode initLeafNodeLeft = new CBTNode{ id = root.id<<1, depth = 1}; 
            CBTNode initLeafNodeRight = new CBTNode{ id = (root.id<<1) | 1, depth = 1};
   
            var cbtBufferView = cbtBufferUint64.AsSpan();
            SetLeafNodeBitfield(cbtBufferView, initLeafNodeLeft, cbtMaxDepth, true);
            SetLeafNodeBitfield(cbtBufferView, initLeafNodeRight, cbtMaxDepth, true);
            
            // sum-reduction for initial CBT two leaf nodes at depth 2.
            CBTNode node1 = GetCeilNode(initLeafNodeLeft, cbtMaxDepth);
            CBTNode node2 = GetCeilNode(initLeafNodeRight, cbtMaxDepth);
            for (int i = cbtMaxDepth; i >= 1; i--) {
                SetNodeBits(cbtBufferView, node1, cbtMaxDepth, 1);
                SetNodeBits(cbtBufferView, node2, cbtMaxDepth, 1);
                node1.id >>= 1;
                node1.depth -= 1;
                node2.id >>= 1;
                node2.depth -= 1;
             
                var bytes2 = MemoryMarshal.AsBytes(cbtBufferUint64.AsReadOnlySpan());
            }
            SetNodeBits(cbtBufferView, new CBTNode{ id = 1, depth = 0}, cbtMaxDepth, 2);
            cbtBuffer.SetData(cbtBufferUint64);
            var bytes = MemoryMarshal.AsBytes(cbtBufferUint64.AsReadOnlySpan());
            cbtBuffer.name = "CBT Buffer";
            return cbtBuffer;
        }
        
        private static Matrix4x4 _GetLEBMatrix(in CBTNode node) {
            Matrix4x4 _GetSplittingMatrix(int bit) {
                float b = bit;
                float c = 1.0f - b;
                return new Matrix4x4 {
                    m00 = c, m01 = b,m02 = 0,m03 = 0,
                    m10 = 0.5f,m11 = 0,m12 = 0.5f,m13 = 0,
                    m20 = 0,m21 = c,m22 = b,m23 = 0,
                    m30 = 0,m31 = 0,m32 = 0,m33 = 1
                };
            }
            
            // wind for clockwise triangle vertices.
            Matrix4x4 _GetWindingMatrix(int bit) {
                float b = bit;
                float c = 1.0f - b;
                return new Matrix4x4 {
                    m00 = c, m01 = 0,m02 = b,m03 = 0,
                    m10 = 0f,m11 = 1,m12 = 0,m13 = 0,
                    m20 = b,m21 = 0,m22 = c,m23 = 0,
                    m30 = 0,m31 = 0,m32 = 0,m33 = 1
                };
            }

            Matrix4x4 m = Matrix4x4.identity;
            for (int i = node.depth - 1; i >= 0; i--) {
                int bit = (node.id >> i) & 1;
                var mi = _GetSplittingMatrix(bit);
                m = mi * m;
            }

            var w = _GetWindingMatrix(node.depth & 1);
            return w*m;
        }
        
        
        public static Mesh BuildMeshletUInt32(int meshletSubdivisionLevel) {
            int triangleSubdivDepth = meshletSubdivisionLevel * 2;
            int triangleCount = 1 << triangleSubdivDepth;
            int edgeTessFactor = 1 << meshletSubdivisionLevel;
            int vertexCount = (1 << triangleSubdivDepth) + 2;
            
            NativeArray<Vector2> vertexBuf = new NativeArray<Vector2>(vertexCount, Allocator.Temp);
            int indexCount = triangleCount*3;
            NativeArray<uint> indexBuf = new NativeArray<uint>(indexCount, Allocator.Temp);
       
            Vector4[] vertices = {
                // A.x B.x C.x
                new(0,  0,  1, 0), 
                // A.y B.y C.y
                new(1,  0,  0, 0)
            };
            int vertexBufIter = 0;
            int indexBufIter = 0;
            
            Dictionary<int, uint> vertMap = new();
            for (int i = 0; i < triangleCount; i++) {
                CBTNode node = new CBTNode { id = triangleCount + i, depth = triangleSubdivDepth };
                Matrix4x4 m = _GetLEBMatrix(node);
                Vector4 x = m * vertices[0];
                Vector4 y = m * vertices[1];
                for (int j = 0; j < 3; j++) {
                    Vector2 vertex = new Vector2(x[j], y[j]);
                    int key = (int)(vertex.x * (edgeTessFactor + 1) +
                                vertex.y * (edgeTessFactor + 1) * (edgeTessFactor + 1));
                    if (vertMap.TryGetValue(key, out uint vertexIndex) == false) {
                        vertexIndex = (uint)vertexBufIter;
                        vertexBuf[vertexBufIter] = vertex;
                        vertMap.Add(key, vertexIndex);
                        vertexBufIter++;
                    }
                    indexBuf[indexBufIter] = vertexIndex;
                    indexBufIter++;
                }
            }
            
            Mesh mesh = new Mesh();
            var layout = new[] {
                new VertexAttributeDescriptor(VertexAttribute.Position, VertexAttributeFormat.Float32, 2)
            };
            mesh.SetVertexBufferParams(vertexCount, layout);
            mesh.SetVertexBufferData(vertexBuf, 0, 0, vertexBuf.Length);
            mesh.SetIndexBufferParams(indexCount, IndexFormat.UInt32);
            mesh.SetIndexBufferData(indexBuf, 0, 0, indexBuf.Length);
            mesh.subMeshCount = 1;
            mesh.SetSubMesh(0, new SubMeshDescriptor(0, indexCount, MeshTopology.Triangles), MeshUpdateFlags.DontRecalculateBounds);
            mesh.name = "CBT Meshlet";
            return mesh;
        }
        
        public static Mesh BuildMeshletUInt16(int meshletSubdivisionLevel) {
            int triangleSubdivDepth = meshletSubdivisionLevel * 2;
            int triangleCount = 1 << triangleSubdivDepth;
            int edgeTessFactor = 1 << meshletSubdivisionLevel;
            int vertexCount = (1 << triangleSubdivDepth) + 2;
            
            NativeArray<Vector2> vertexBuf = new NativeArray<Vector2>(vertexCount, Allocator.Temp);
            int indexCount = triangleCount*3;
            Assert.IsTrue(indexCount < ushort.MaxValue);
   
            NativeArray<ushort> indexBuf = new NativeArray<ushort>(indexCount, Allocator.Temp);
       
            Vector4[] vertices = {
                // A.x B.x C.x
                new(0,  0,  1, 0), 
                // A.y B.y C.y
                new(1,  0,  0, 0)
            };
            int vertexBufIter = 0;
            int indexBufIter = 0;
            
            Dictionary<int, ushort> vertMap = new();
            for (int i = 0; i < triangleCount; i++) {
                CBTNode node = new CBTNode { id = triangleCount + i, depth = triangleSubdivDepth };
                Matrix4x4 m = _GetLEBMatrix(node);
                Vector4 x = m * vertices[0];
                Vector4 y = m * vertices[1];
                for (int j = 0; j < 3; j++) {
                    Vector2 vertex = new Vector2(x[j], y[j]);
                    int key = (int)(vertex.x * (edgeTessFactor + 1) +
                                vertex.y * (edgeTessFactor + 1) * (edgeTessFactor + 1));
                    if (vertMap.TryGetValue(key, out ushort vertexIndex) == false) {
                        vertexIndex = (ushort)vertexBufIter;
                        vertexBuf[vertexBufIter] = vertex;
                        vertMap.Add(key, vertexIndex);
                        vertexBufIter++;
                    }
                    indexBuf[indexBufIter] = vertexIndex;
                    indexBufIter++;
                }
            }
            
            Mesh mesh = new Mesh();
            var layout = new[] {
                new VertexAttributeDescriptor(VertexAttribute.Position, VertexAttributeFormat.Float32, 2)
            };
            mesh.SetVertexBufferParams(vertexCount, layout);
            mesh.SetVertexBufferData(vertexBuf, 0, 0, vertexBuf.Length);
            mesh.SetIndexBufferParams(indexCount, IndexFormat.UInt16);
            mesh.SetIndexBufferData(indexBuf, 0, 0, indexBuf.Length);
            mesh.subMeshCount = 1;
            mesh.SetSubMesh(0, new SubMeshDescriptor(0, indexCount, MeshTopology.Triangles), MeshUpdateFlags.DontRecalculateBounds);
            mesh.name = "CBT Meshlet";
            return mesh;
        }
        
        public static float GetLodBaseFactor(int targetVisPixelCount,int meshletSubdivisionLevel,float cameraFoVy,float viewportHeight) {
            float t = targetVisPixelCount * 2 * Mathf.Tan(Angle.Degrees(cameraFoVy).ToRadians() * 0.5f)/viewportHeight;
            int a = 1<<meshletSubdivisionLevel; // divisor of meshlet real triangle edge length.
            return -2.0f * Mathf.Log(t * a)/Mathf.Log(2) + 2.0f;
        }
    }
}
```
