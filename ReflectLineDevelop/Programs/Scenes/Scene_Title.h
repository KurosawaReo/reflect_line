/*
   - Scene_Title.h -
*/
#pragma once

//タイトルシーン.
class TitleScene final : public IScene
{
//▼ ===== 変数 ===== ▼.
private:
	Timer timer;          //シーンの経過時間.
	bool  isBreakDone{};  //破片アニメーションを出したか.

	//画像.
	Graph* grLogo{};
	Graph* grLogoAll{};
	Graph* grUiBackBestScore{};
	//フォント.
	int fontSmall{};  //ラベル用.
	int fontScore{};  //ベストスコア用.
	int fontPush{};   //Pushテキスト用.

//▼ ===== 関数 ===== ▼.
private:
	//描画系.
	void DrawLabels();     //左下のラベル2つ.
	void DrawLogo();       //タイトルロゴ.
	void DrawBestScore();  //ベストスコア.
	void DrawPushText();   //Pushテキスト.

public:
	void Init()   override;
	void Reset()  override;
	void Enter()  override;
	void Exit()   override;
	void Update() override;
	void Draw()   override;
};
