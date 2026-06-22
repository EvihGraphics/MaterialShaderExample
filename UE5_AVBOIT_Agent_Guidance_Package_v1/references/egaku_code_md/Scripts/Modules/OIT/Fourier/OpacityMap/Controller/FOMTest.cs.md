# Egaku source: `Scripts/Modules/OIT/Fourier/OpacityMap/Controller/FOMTest.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System.Collections.Generic;
using Modules.OIT.Fourier.OpacityMap.Material;
using Unity.VisualScripting;
using UnityEngine;

namespace Modules.OIT.Fourier.OpacityMap.Controller {
    
    
    public class FOMTest :MonoBehaviour{
        
        [SerializeField]
        private float _gap = 10;

        [SerializeField]
        private UnityEngine.Material _objectMaterial;

        private FOMShaderConfig _shaderConfig;
        
        
        [SerializeField]
        private List<Color> _colors = new ();
        
        public void ClearAllChildrenRuntime()
        {
            int childCount = transform.childCount;
            for (int i = childCount - 1; i >= 0; i--)
            {
                Transform child = transform.GetChild(i);
                DestroyImmediate(child.gameObject);
            }
        }

        public IEnumerable<GameObject> GetChildren() {
            
            for (int i = 0; i < transform.childCount; i++)
            {
                Transform child = transform.GetChild(i);
                GameObject childObject = child.gameObject;
                yield return childObject;
            }
        }

        public List<Color> GetTestColorList() {
            return _colors;
        }

        [ContextMenu("GenerateTestCases")]
        public void GenerateTestCases() {
            ClearAllChildrenRuntime();
            List<GameObject> objects = new();
            Mesh mesh = GetComponent<MeshFilter>().sharedMesh;
            for (int i = 0; i < _colors.Count; i++) {
                var obj = new GameObject($"TranslucentPlane_{i}");
                obj.transform.parent = GetComponent<Transform>();
                obj.transform.localPosition = new Vector3(0,i * _gap, 0);
                obj.transform.localRotation = Quaternion.identity;
                obj.transform.localScale = new Vector3(1f, 1f, 1f);
                MeshFilter meshFilter = obj.AddComponent<MeshFilter>();
                meshFilter.mesh = mesh;
                objects.Add(obj);
            }
            
            for (int i = 0; i < _colors.Count; i++)
            {
                var obj = objects[i];
                var material = new UnityEngine.Material(_objectMaterial);
                material.SetColor("_Translucency_baseColor", _colors[i]);
                var controller = obj.GetOrAddComponent<FOMInstanceController>();
                controller.SetMaterial(material);
            }
        }

        public void OnEnable() {
            foreach (GameObject obj in GetChildren()) {
                var controller = obj.GetOrAddComponent<FOMInstanceController>();
                controller.enabled = true;
            }
        }

        public void OnDisable() {
            foreach (GameObject obj in GetChildren()) {
                var controller = obj.GetComponent<FOMInstanceController>();
                if (controller != null) {
                    controller.enabled = false;
                }
            }
        }
    }
}
```
