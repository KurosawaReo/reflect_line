/*
   - BG3.h -
   3Dの背景は画面酔いするリスクがあることに注意。
*/
#pragma once
#include "BGBase.h"

//背景3.
class BG3 : public BGBase
{
//▼ ===== 変数 ===== ▼.
private:
	//疑似3Dオブジェクト.
	struct WarpPoint
	{
		double x;
		double y;
		double z;

		double oldZ;
	};
	struct WarpHex
	{
		double z;
	};
	struct WarpStar
	{
		double x;
		double y;
		double z;
	};

	static constexpr int POINT_NUM = 220;
	WarpPoint point[POINT_NUM];

	static constexpr int HEX_NUM = 20;
	WarpHex hex[HEX_NUM];

	static constexpr int STAR_NUM = 80;
	WarpStar star[STAR_NUM];

	double angle; //角度.


//▼ ===== 関数 ===== ▼.
public:
	void Init   ()                 override; //初期化.
	void Reset  ()                 override; //リセット.
	void Update ()                 override; //更新.
	void DrawNor(double modeAlpha) override; //描画(通常時)
	void DrawRef(double modeAlpha) override; //描画(反射モード)

	//パーツ別描画.
	void DrawPoints  (double modeAlpha, MY_COLOR color);
	void DrawHexagons(double modeAlpha, MY_COLOR color);
	void DrawStars   (double modeAlpha, bool isReflect);

	//ポーズ用.
	void Pause()    override;
	void PauseEnd() override;
};