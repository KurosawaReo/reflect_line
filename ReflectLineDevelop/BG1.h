/*
   - BG1.h -
*/
#pragma once
#include "BGBase.h"

//背景タイル.
class BG_Tile
{
	friend class BG1; //アクセス許可.
private:
	INT_XY pos{};

	double counter{}; //経過時間.
	double shine{};
	double sinNum{};

	Timer shineTimer = Timer(TimerMode::CountDown, 3); //発光する時間.

public:
	void Init   ();                 //初期化.
	void Reset  ();                 //リセット.
	void Update ();                 //更新.
	void DrawNor(double modeAlpha); //描画(通常時)
	void DrawRef(double modeAlpha); //描画(反射モード)

	void Shine(); //発光.
};

//背景1.
class BG1 : public BGBase
{
//▼ ===== 変数 ===== ▼.
private:
	vector<BG_Tile> tiles; //背景タイルデータ.

	TimerMicro tmShine = TimerMicro(TimerMode::CountDown, 1000000/60); //光る間隔.

//▼ ===== 関数 ===== ▼.
public:
	void Init   ()                 override; //初期化.
	void Reset  ()                 override; //リセット.
	void Update ()                 override; //更新.
	void DrawNor(double modeAlpha) override; //描画(通常時)
	void DrawRef(double modeAlpha) override; //描画(反射モード)

	//ポーズ用.
	void Pause()    override;
	void PauseEnd() override;
};