/*
   - Scene_Menu.h -

   [Scene]
   ・Title
   ・Menu <-
   ・Game
   ・End
*/
#pragma once
#include "Config_Main.h"

//メニューカーソル.
struct MenuCursor
{
//▼ ===== 変数 ===== ▼.
	int    cursorIdx = 0;		//カーソル選択位置.
	double cursorWave = 0;		//カーソル裏表値(-1～1).
	double cursorRotSpeed = 1;	//カーソル回転速度.
	double cursorRotAngle = 0;	//カーソル回転角度.

	double linePulse = 0;		//装飾線の明滅(-1～1).

	//タイトル両脇の「< >」演出.
	struct Arrow {
		double move  = 0;	//外へ広がる量(0～1).
		double alpha = 0;	//透明度(0～1).
	};
	Arrow arrow[2];

//▼ ===== 関数 ===== ▼.
	//set.
	void SetCursorRotSpeed(double speed);

	void Reset();				 //リセット.
	void Update(double counter); //更新.
};

//カーソル位置で決まるレイアウト値.
struct MenuRect
{
	DBL_XY imgSize{};   //サムネ枠のサイズ(動画なしなら0).
	INT_XY descPos{};   //説明文パネルの左上.
	INT_XY descSize{};  //説明文パネルのサイズ.
	INT_XY infoPos{};   //操作パネルの左上.
	INT_XY infoSize{};  //操作パネルのサイズ.
};

//メニューシーン.
class MenuScene final : public IScene
{
//▼ ===== 変数 ===== ▼.
private:
	float	   counter = 0; //経過時間.

	MenuCursor mCursor;
	MenuRect   mRect;

	//点滅アニメーション.
	Timer      tmBlink = Timer(TimerMode::CountDown, 0.1f); //点滅間隔.
	bool       isBlink = false;                             //点滅させる用.

	//動画.
	Graph* grMenuMovie[Config::MENU_OPTION_COUNT]{};

//▼ ===== 関数 ===== ▼.
private:
	Graph* GetPlayMovie(); //再生する動画を取得.

	void DrawBG();
	void DrawTopText();
	void DrawOptions();
	void DrawCursor();
	void DrawThumbnail();
	void DrawGuideLines();
	void DrawPanelDescOperate();
	void DrawPanelDescMode();
		
	void CalcMenuRect(); //共有するレイアウト値を計算.

	void OnCursorMove(); //カーソル移動時の処理.
	void StopAllMovie(); //全ての動画を停止.

public:
	void Init()   override;
	void Reset()  override;
	void Enter()  override;
	void Exit()   override;
	void Update() override;
	void Draw()   override;
};