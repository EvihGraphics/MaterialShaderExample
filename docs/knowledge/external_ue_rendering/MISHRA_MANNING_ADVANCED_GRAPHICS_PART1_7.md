# Manning: Advanced Graphics Programming in Unreal Part 1-7

## Source
* **Author:** Matt Manning (Mishra / W27)
* **URL:** https://medium.com/@manning.w27/advanced-graphics-programming-in-unreal-part-7-d3500d4b8195 (and Parts 1-6)

## Part Overview and AVBOIT Mapping

| Part | 核心主题 (Core Topic) | 与 AVBOIT 的对应关系 (AVBOIT Relevance) |
|---|---|---|
| 1 | 工程设置、Shader 目录、调试環境 (Project Setup, Shader Dirs, Debug) | プラグイン構成とシェーダーのパスマッピング (`/Plugin/MaterialShaderPlugin/Shaders/` -> `AVBOIT/Raster/`) |
| 2 | RHI、RDG、スレッド、Proxy ライフサイクル (RHI, RDG, Threads, Proxy Lifecycle) | AVBOITパスのCPU(Game Thread)からGPU(Render Thread)へのデータ転送と実行 (`FAVBOITRasterSceneData`) |
| 3 | Scene、View、SceneViewExtension (SVE) | AVBOITパスの注入ポイントとマルチViewのライフサイクル (`AVBOITSceneViewExtension.cpp`) |
| 4 | Global Shader | 画面全体の処理：Clear, Integrate, Composite などの Compute / Fullscreen パス |
| 5 | Simple Material Shader | 将来的なマテリアルパラメータ化の基盤 |
| 6 | Advanced Material Shader | カスタムマテリアル出力とコンパイル環境 |
| 7 | Mesh-Material Shader、Mesh Pass | 現在テスト用の Quad に限定されている Proxy 実装を置き換え、任意の Mesh (Cube, Sphere 等) を描画するための手法。 |

## Concept Mapping (概念 -> ファイル/API -> UE5.7 実際のインターフェース)

* **Global Shader** -> `FAVBOITClearCS`, `FAVBOITIntegrateCS` -> `DECLARE_GLOBAL_SHADER`, `IMPLEMENT_GLOBAL_SHADER`
* **Scene View Extension** -> `FAVBOITSceneViewExtension` -> `FSceneViewExtensionBase`, `SubscribeToPostProcessingPass`
* **RDG Pass Setup** -> `FAVBOITRasterRenderer::AddPasses` -> `FRDGBuilder`, `FComputeShaderUtils::AddPass`, `GraphBuilder.AddPass`
* **Mesh Pass Processor (Part 7)** -> *Currently missing in AVBOIT, uses direct RHICmdList DrawIndexedPrimitive* -> UE5.7 `FMeshPassProcessor`, `AddSimpleMeshPass`, `AddDrawDynamicMeshPass`

*Note: Medium articles serve as conceptual guides. The actual UE5.7 C++ implementation requires verification against Engine source, especially regarding RDG Builder API changes and Mesh Pass structural updates.*
