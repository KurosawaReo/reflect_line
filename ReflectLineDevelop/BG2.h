/*
   - BG2.h -
*/
#pragma once
#include "BGBase.h"

//背景2.
class BG2 : public BGBase
{
//▼ ===== 変数 ===== ▼.
private:

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