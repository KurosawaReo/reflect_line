/*
   - Obst_StraightLaser.h -

   障害物: 直線レーザー.
*/
#pragma once

//直線レーザー発射台.
class StraightLaserPoint
{
private:
	double centerPos{};	//レーザー発射位置.
	int    shotDir{};	//発射方向.

public:
	void Init();
	void Reset();

	void ShotLaser();	//レーザー発射.
	void ShotRand();	//レーザー位置抽選.

	void DrawPreLine(float count);
};

//直線レーザー.
class StraightLaser final : public ManagerBase
{
//▼ ===== 変数 ===== ▼.
private:
	StraightLaserPoint points[2];

	float  counter{};			//レーザー発射タイマー.
	float  counterPrediction{};	//予測線タイマー.
	bool   isShowPreLine{};		//予測線表示フラグ.

//▼ ===== 関数 ===== ▼.
public:
	//コンストラクタ.
	StraightLaser(int order) : ManagerBase(order) {}

	void Init()   override;
	void Reset()  override;
	void Update() override;
	void Draw()   override;

	//使用禁止.
	StraightLaser(const StraightLaser&) = delete;
	StraightLaser& operator=(const StraightLaser&) = delete;
};