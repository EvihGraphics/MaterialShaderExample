# Monsho: Weighted Blended OIT の実装

## 元データ
* **Title:** Weighted Blended OIT の実装 (Implementation of Weighted Blended OIT)
* **URL:** https://monsho.hatenablog.com/entry/2016/06/11/124257
* **Date:** 2016-06-11

## 概要と要点

1. **歴史 UE4 バージョン背景**
   - UE4時代（おそらくUE4.11〜4.12頃）のエンジンコードを直接改造してWeighted Blended OIT (WBOIT) を実装する実験的試み。
   - 当時はSceneViewExtensionやRDGが存在せず、FRenderingCompositePassContextなどに直接手を入れる必要があった。

2. **Separate Translucency の利用方式**
   - UEには元々半透明を別のパスでレンダリングする「Separate Translucency」機能があり、これをフックしてWBOIT用のバッファに出力するアプローチを採用している。

3. **追加カラー/Alpha Render Target**
   - WBOITアルゴリズム（Morgan McGuire氏らによる手法）に従い、2つのレンダーターゲットを必要とする。
   - `Accumulation Buffer` (Color * Alpha * Weight) と `Revealage Buffer` (Alpha * Weight) を蓄積するためのバッファを作成。

4. **Blend State**
   - Accumulation側は加算合成（One, One）。
   - Revealage側も乗算または加算ベースの特殊なブレンドを適用して、順番に依存しない合成を実現。

5. **WBOIT 权重函数 (Weight Function)**
   - 深度（Z）とアルファ値に基づいたヒューリスティックな重み付け関数を使用。
   - カメラに近いほど、または不透明に近いほど重みが強くなる設計。

6. **Recombine 流程**
   - ポストプロセスフェーズで、Accumulation / Revealage を計算して最終的な SceneColor に合成（Recombine）する。

7. **なぜ旧版APIをそのままUE5.7に持ち込めないか**
   - UE5.7はRender Dependency Graph (RDG) が完全に普及しており、当時のレガシーなFRenderTargetPoolや直接のRHIコマンド操作は非推奨、あるいは削除されている。
   - SceneViewExtensionが導入されており、エンジン本体の改造なしにプラグインから描画パスを挿入できる。

8. **現在のAVBOITに対する価値**
   - OITの2パス構造（Splat/Accumulate → Resolve/Composite）の概念は不変。
   - 透明度のブレンドステートと深度テストの無効化（Z-Write Off）の挙動は現在の実装のデバッグに役立つ。

9. **The Forge WBOIT、UE Sorted Pixels、AVBOIT の違い**
   - **WBOIT (Monsho/The Forge):** ヒューリスティックな重み関数による近似OIT。順序は不要だが、重み関数の調整が難しく、交差時のアーティファクトがある。
   - **UE Sorted Pixels:** A-Bufferベースの厳密なソート。正確だが、メモリやパフォーマンスのオーバーヘッドが大きい。
   - **AVBOIT (P2.6T):** モーメントやボクセル化に近いアプローチで、WBOITよりも物理的に正確な透過/散乱を計算しつつ、完全なソートを回避するアルゴリズム。
