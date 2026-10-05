/*
   - Obst_NormalLaser.h -

   障害物: レーザー発射台(継承元)
*/
#pragma once
#include "Global.h"

//移動方向(+1すれば右回転)
enum MoveDir
{
	Left,
	Up,
	Right,
	Down,

	Count, //総数.
};
struct MoveInfo
{
	MoveDir dir;    //画面のどの辺にいるか.
	bool    isLeft; //true = 左回り, false = 右回り.
};

//予告エフェクト.
struct FlashEffect
{
	double	x, y;		// エフェクト位置.
	float	counter;	// 経過時間.
	float	Duration;	// エフェクトの持続時間.
	int		BaseSize;   // 基本サイズ
	bool	validFlag;  // 有効フラグ

	double	angle;
};

//通常レーザー発射台.
class NormalLaserPoint
{
private:
	DBL_XY   pos{};       //位置.
	MoveInfo move{};      //移動データ.
	float    speed{};     //移動速度.
	bool	 validFlag{}; //有効フラグ.

public:
	//set.
	void SetValidFlag(bool _flag) { validFlag = _flag; }
	//get.
	DBL_XY GetVec() const; //方向取得.

	void Reset();
	void Update();

	void DrawPrediction(float count); //予告描画.
	
	void Shot();					//レーザー発射.
	void MoveRand();				//レーザー移動抽選.
};

//通常レーザー.
class NormalLaser final : public ManagerBase
{
//▼ ===== 変数 ===== ▼.
private:
	NormalLaserPoint points[4]{}; //レーザー発射台.
	FlashEffect flash[LASER_NOR_FLASH_MAX]{};

	float counter{};   //経過カウンター.
	float counterTm{}; //発射するタイミング.


//▼ ===== 変数 ===== ▼.
public:
	//コンストラクタ.
	NormalLaser(int order) : ManagerBase(order) {}

	//基本処理.
	void Init  () override;
	void Reset () override;
	void Update() override;
	void Draw  () override;

	//描画系.
	void DrawObstFlash();
 
	void CreateFlashEffect(double x, double y);

	//何個発射台を使うか.
	void UseLaserPointCnt(int count);

	//使用禁止.
	NormalLaser(const NormalLaser&) = delete;
	NormalLaser& operator=(const NormalLaser&) = delete;
};
