/*
   - BGBase.h -
   このクラスを継承して背景を作る。
*/
#pragma once

//背景基底クラス.
class BGBase
{
//▼ ===== 関数 ===== ▼.
public:
	virtual void Init   ()                 = 0; //初期化.
	virtual void Reset  ()                 = 0; //リセット.
	virtual void Update ()                 = 0; //更新.
	virtual void DrawNor(double modeAlpha) = 0; //描画(通常時)
	virtual void DrawRef(double modeAlpha) = 0; //描画(反射モード)

	//ポーズ用.
	virtual void Pause()    = 0;
	virtual void PauseEnd() = 0;
};