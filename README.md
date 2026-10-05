<!-- 
   GitHubの表示を基準にするため、VScode上での表示とは異なる点に注意
-->

<p align="center">
  <img src="_readme_img/card_image.webp"><br>
</p>

## 操作説明
### キーボード
<img src="_readme_img/lore_keyboard.png">

### Xboxコントローラ
<img src="_readme_img/lore_xbox_controller.png">
<br>

## ゲーム概要
<img src="_readme_img/game_image.png">
<table>
  <tr>
    <th>開発期間</th>
    <td>2025/4 ~ 2025/10 (現在も改良中)</td>
  </tr>
  <tr>
    <th>開発人数</th>
    <td>2人</td>
  </tr>
  <tr>
    <th>プレイ人数</th>
    <td>1人</td>
  </tr>
  <tr>
    <th>ジャンル</th>
    <td>反撃型回避アクション</td>
  </tr>
  <tr>
    <th>使用技術</th>
    <td>DxLib, C++, Sourcetree</td>
  </tr>
</table>

## ルール
以下のサイクルをゲームオーバーになるまで繰り返すエンドレスゲームです。
最終スコアでハイスコアを競います。

<div style="display:flex; align-items:center; gap:16px;">
  <img src="_readme_img/gif/rule1_sakeru.gif" width=200>
  <div>
    <b>1. 避ける</b><br>
    障害物に当たると即ゲームオーバー。<br>
    <font color=00ffff>青いもの</font>が障害物の特徴です。
  </div>
</div>
<br>

<div style="display:flex; align-items:center; gap:16px;">
  <img src="_readme_img/gif/rule2_toru.gif" width=200>
  <div>
    <b>2. 取る</b><br>
    上から降ってくるアイテムを取ると、反射モードに変化。<br>
    画面が<font color=ff00ff>ピンク色</font>になると、反射モード発動中のサインです。
  </div>
</div>
<br>

<div style="display:flex; align-items:center; gap:16px;">
  <img src="_readme_img/gif/rule3_hansya.gif" width=200>
  <div>
    <b>3. 反射</b><br>
    反射モード中は、レーザー系の障害物に当たると跳ね返します。<br>
    反射モードには制限時間があります。
  </div>
</div>
<br>

<div style="display:flex; align-items:center; gap:16px;">
  <img src="_readme_img/gif/rule4_kowasu.gif" width=200>
  <div>
    <b>4. 壊す</b><br>
    反射したレーザーは、近くの隕石に自動で追尾します。<br>
    隕石に命中すると破壊し、スコアを獲得できます。
  </div>
</div>
<br>

これらのルールは、ゲーム本編の「**チュートリアル**」から確認できます。
<img src="_readme_img/menu_tutorial.png">

## こだわりポイント

> ### ネオン風のデザイン

「きれい × かっこいい」をコンセプトにした、個性的なデザインにしています。
<font color=00ffff>青色</font>と<font color=ff00ff>ピンク色</font>の対比が特徴です。

<div style="display: flex; flex-wrap: wrap;">
  <p style="width: 45%; margin: 1%;">
    <font color=00ffff><b>通常モード</b></font>
    <img src="_readme_img/color_blue.png">
  </p>
  <p style="width: 45%; margin: 1%;">
    <font color=ff00ff><b>反射モード</b></font>
    <img src="_readme_img/color_pink.png">
  </p>
</div>

> ### "動き" を作るアニメーション
ゲームには、常に動いてるものがないと寂しく感じます。
そのため、画面の至る所で常に動きが出るよう、アニメーションを実装しています。

<div style="display: flex; flex-wrap: wrap;">
  <p style="width: 45%; margin: 1%;">
    <img src="_readme_img/gif/anim_levelup.gif">
  </p>
  <p style="width: 45%; margin: 1%;">
    <img src="_readme_img/gif/gameover.gif">
  </p>
</div>
<br>

> ### 隕石のランダム生成
隕石の形にバリエーションを作るため、独自の隕石生成アルゴリズムを実装しています。
1. 頂点の数を抽選
2. 隕石の中心から各頂点までの距離を抽選
3. 頂点を順番に線で結ぶ

<img src="_readme_img/meteor1.png"> <img src="_readme_img/meteor2.png"> <img src="_readme_img/meteor3.png">

> ### 隕石をこわす爽快感

ガラスを壊すような破片の演出と効果音を取り入れています。

現実では、ガラスを壊すことはあまりありません。
ゲームならではの爽快感を表現するため、このような演出を採用しています。

<img src="_readme_img/gif/anim_break_meteor.gif">
<br>

> ### ポートフォリオサイト
詳しくはポートフォリオサイトをご覧ください。

https://portfolio-kurosawareo.netlify.app/