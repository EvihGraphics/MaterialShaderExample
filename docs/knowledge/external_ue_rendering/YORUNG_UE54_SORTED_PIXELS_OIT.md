# Yorung: UE5.4 Sorted Pixels OIT

## 元データ
* **Title:** Unreal Engine 5.4のOITを試す
* **URL:** https://qiita.com/yorung/items/a2a7a1c8ad8366d39d4b
* **Date:** 2024年頃 (UE5.4リリース時)

## 概要と要点

1. **UE5.4 Sorted Pixels OIT**
   - UE5.4からExperimentalとして導入されたネイティブのOrder Independent Transparency機能。
   - A-Bufferに似た手法で、ピクセルごとのフラグメントリストを構築し、GPU上でソートして正しいブレンドを行う。

2. **プロジェクト設定スイッチ**
   - `r.OIT.SortedPixels.Enable`
   - CVarによる有効化が必要。

3. **Experimental 状態の歴史背景**
   - 複雑な透明オブジェクト（髪の毛やガラスの交差など）の正確な描画が求められる中で導入されたが、メモリとパフォーマンスの課題からExperimental扱いとなっている。

4. **Thin Translucent 視覚差異**
   - SubstrateやThin Translucentマテリアルと組み合わせた際に、通常のZソートでは破綻する交差面が正しく描画されることが確認されている。

5. **透明オブジェクトがない場合の固定コスト**
   - OITを有効にすると、画面内に透明オブジェクトが存在しなくても、リスト構築やソート関連の初期化コスト（バッファのクリア等）が発生する。

6. **高透明カバレッジ時のパフォーマンス変化**
   - 重ね合わせ（オーバードロー）が激しいシーンでは、フラグメントリストのメモリ確保上限に達したり、ソート処理によるGPU負荷が爆発的に増加するリスクがある。

7. **Before DOF / After DOF / After Motion Blur の制約**
   - UEの透過描画パスはポストプロセスの前後に分かれているが、Sorted Pixels OITはこれらの異なるパス間でまたがってフラグメントをソートすることはできない。パスごとに独立して処理される。

8. **なぜUEネイティブOITはAVBOITの代替にならないのか**
   - ネイティブOITは「正確なソート」を目指すため、O(N log N) のソートコストと O(N) のメモリ帯域幅を必要とする。
   - **AVBOIT (P2.6T)** はソートを完全に回避し、一定のメモリ（Extinction / Transmittance ボリューム）で近似計算を行うアルゴリズムであり、パフォーマンスとスケーラビリティの観点で目的が異なるため、対比用のベースライン（Ground Truth検証）としては有用だが、研究を置き換えるものではない。

*※これらの機能やCVarは現在のUE5.7ソースコードにて引き続き存在・変更されている可能性があるため、ソース側の `r.OIT.SortedPixels` 周辺の確認が必要である。*
