/*
   - Scene_Menu.h -

   [Scene]
   ・Title
   ・Menu <-
   ・Game
   ・End
*/
#pragma once
#include "Global.h"

/*------------------------------------------*/
//【編集用】メニューのデザインを変えたい時はここをいじる.

//メニュー設定(レイアウト)
struct MenuLayout
{
	DBL_XY   menuPos       = {370, 280};			//モード枠の位置.
	DBL_XY   menuSize      = {400,  70};			//モード枠のサイズ.
	int      menuSpace     = 100;					//モード枠の配置間隔.
	int      loreInner     = 15;					//説明文が内側にどれだけずれるか.
	int      loreLineSpace = 30;					//説明文の行間.
	DBL_XY   imgPos        = {WINDOW_WID-560, 410};	//サムネの基準位置.
};
//メニュー設定(色)
struct MenuColor 
{
	UINT text    = GetColor(255, 255, 255);	//テキスト色.
	UINT frame   = GetColor(  0, 255, 255);	//枠色.
	UINT normal  = GetColor(150, 150, 150);	//未選択色.
	UINT select1 = GetColor(100, 255, 255);	//カーソル表.
	UINT select2 = GetColor( 50, 150, 255);	//カーソル裏.
	UINT line    = GetColor(  0, 255, 255);	//線の色.
};
/*------------------------------------------*/

//メニューシーン.
class MenuScene final : public IScene
{
//▼ ===== 実体 ===== ▼.
public:
	static MenuScene& GetInst() {
		static MenuScene inst; //自身のインスタンス.
		return inst;
	}

//▼ ===== 変数 ===== ▼.
private:
	int       cursorIdx = 0;  //カーソルの選択位置.
	float     counter = 0;    //経過時間.

	//点滅アニメーション.
	Timer     tmBlink = Timer(TimerMode::CountDown, 0.1f); //点滅間隔.
	Timer     tmTitle = Timer(TimerMode::CountDown, 2.5f); //タイトルアニメ用.
	bool      isBlink = false;                             //点滅させる用.

	//描画用設定.
	const MenuLayout mLayout;
	const MenuColor  mColor;

//▼ ===== 関数 ===== ▼.
private:
	MY_STRING GetPlayMovieName(); //再生する動画名.

public:
	void Init()   override;
	void Reset()  override;
	void Enter()  override;
	void Exit()   override;
	void Update() override;
	void Draw()   override;

	void OnCursorMove(); //カーソル移動時の処理.
	void StopAllMovie(); //全ての動画を停止.
};