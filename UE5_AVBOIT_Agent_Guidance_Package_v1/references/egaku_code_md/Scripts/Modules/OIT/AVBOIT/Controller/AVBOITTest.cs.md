# Egaku source: `Scripts/Modules/OIT/AVBOIT/Controller/AVBOITTest.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿
using System.Collections.Generic;
using Common.Rendering;
using Modules.OIT.AVBOIT.Material;
using Modules.OIT.AVBOIT.Renderer.VirtualBlockBased;
using Unity.VisualScripting;
using UnityEngine;

namespace Modules.OIT.AVBOIT.Controller {
    
    
    [ExecuteAlways]
    public class AVBOITTest : MonoBehaviour {
        [SerializeField]
        private float _gap = 10;

        [SerializeField]
        private UnityEngine.Material _objectMaterial;

        private AVBOITShaderConfig _shaderConfig;
        
      //  private List<GameObject> _objects = new ();
        
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
          //  _shaderConfig = ScriptableObject.CreateInstance<AVBOITShaderConfig>();
          //  Shader shader = Shader.Find(_shaderConfig.ShaderURL);
         // UnityEngine.Material objectMaterial;// = new UnityEngine.Material(shader);
     //       _objects.Capacity = _colors.Count;
            ClearAllChildrenRuntime();
            List<GameObject> objects = new();
      //      _objects.Clear();
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
                if (CustomRendererManager.TryGetCustomRenderer<AVBOITRenderer>().UseGPUBlending) {
                    material.SetColor("_BaseColor", _colors[i]);
                } else {
                    material.SetColor("_Translucency_baseColor", _colors[i]);
                }
                var avboitController = obj.GetOrAddComponent<AVBOITInstanceController>();
                avboitController.SetMaterial(material);
            }
        }

        public void OnEnable() {
            foreach (GameObject obj in GetChildren()) {
                var avboitController = obj.GetOrAddComponent<AVBOITInstanceController>();
                avboitController.enabled = true;
            }
        }

        public void OnDisable() {
            foreach (GameObject obj in GetChildren()) {
                var avboitController = obj.GetComponent<AVBOITInstanceController>();
                if (avboitController != null) {
                    avboitController.enabled = false;
                }
            }
        }
    }
}
```
