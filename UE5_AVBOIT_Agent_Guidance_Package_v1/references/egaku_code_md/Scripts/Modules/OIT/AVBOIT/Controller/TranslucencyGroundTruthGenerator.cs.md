# Egaku source: `Scripts/Modules/OIT/AVBOIT/Controller/TranslucencyGroundTruthGenerator.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿

using System.Collections.Generic;
using UnityEngine;

namespace Modules.OIT.AVBOIT.Controller {
    
    [ExecuteAlways]
    public class TranslucencyGroundTruthGenerator  : MonoBehaviour {
        [SerializeField]
        private float _gap = 10;
        
        
        [SerializeField]
        private UnityEngine.Material _material;
        
        
        private List<GameObject> _objects = new ();

        
        public void ClearAllChildrenRuntime()
        {
            int childCount = transform.childCount;
            for (int i = childCount - 1; i >= 0; i--)
            {
                Transform child = transform.GetChild(i);
                DestroyImmediate(child.gameObject);
            }
        }
        
        public void OnEnable() {
            UnityEngine.Material objectMaterial = new UnityEngine.Material(_material);
            List<Color> colors = FindObjectOfType<AVBOITTest>().GetTestColorList();
            _objects.Capacity = colors.Capacity;
            ClearAllChildrenRuntime();
            _objects.Clear();
            for (int i = 0; i < _objects.Capacity; i++) {
                var obj = new GameObject($"TranslucentPlane_{i}");
                obj.transform.parent = GetComponent<Transform>();
                obj.transform.localPosition = new Vector3(0,i * _gap, 0);
                obj.transform.localRotation = Quaternion.identity;
                obj.transform.localScale = new Vector3(1f, 1f, 1f);
                MeshFilter meshFilter = obj.AddComponent<MeshFilter>();
                obj.AddComponent<MeshRenderer>();
                meshFilter.mesh = GetComponent<MeshFilter>().sharedMesh;
                _objects.Add(obj);
            }

            for (int i = 0; i < _objects.Capacity; i++)
            {
                var obj = _objects[i];
                var material = new UnityEngine.Material(objectMaterial);
                material.color = colors[i];
                obj.GetComponent<MeshRenderer>().material = material;
            }
        }

        public void OnDisable() {

        }
    }
}
```
