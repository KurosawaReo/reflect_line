/*
   - Obst_StraightLaser.h -

   障害物: 直線レーザー.
*/
#include "Obst_StraightLaser.h"

//依存関係.
#include "GameData.h"
#include "GameManager.h"
#include "LaserManager.h"
//参照.
static GameData*     gameData;
static LaserManager* laserMng;
//参照(KRライブラリ)
static SoundMng*     soundMng;

// ▼*--=<[ StraightLaserPoint ]>=--*▼ //

void StraightLaserPoint::Init() {
	
	//参照取得.
	gameData = ManagerInsts::Get<GameData>();
	laserMng = ManagerInsts::Get<LaserManager>();
	soundMng = ManagerInsts::Get<SoundMng>();
}

void StraightLaserPoint::Reset() {
	centerPos = 0;
	shotDir = 0;
}

/// <summary>
/// レーザー発射.
/// </summary>
void StraightLaserPoint::ShotLaser()
{
	//レーザー間の間隔.
	const double spacing = 20;

	DBL_XY startPos{};
	DBL_XY vel{};

	switch (shotDir)
	{
	case 0: //→.
		startPos.x = -50;
		vel = { +1.0, 0.0 };
		break;
	case 1: //←.
		startPos.x = WINDOW_WID + 50;
		vel = { -1.0, 0.0 };
		break;
	case 2: //↓.
		startPos.y = -50;
		vel = { 0.0, +1.0 };
		break;
	case 3: //↑.
		startPos.y = WINDOW_HEI + 50;
		vel = { 0.0, -1.0 };
		break;
	}

	//3つのレーザーを同時に発射.
	for (int i = 0; i < 3; i++)
	{
		DBL_XY tmpPos{};

		//水平発射.
		if (shotDir == 0 || shotDir == 1) {
			tmpPos.x = startPos.x;
			tmpPos.y = centerPos + (i - 1) * spacing; // -spacing, 0, +spacing
		}
		//垂直発射.
		else {
			tmpPos.x = centerPos + (i - 1) * spacing; // -spacing, 0, +spacing
			tmpPos.y = startPos.y;
		}
		//直線レーザーを発射.
		laserMng->SpawnLaser(tmpPos, vel, LaserType::Straight);
	}
}

/// <summary>
/// レーザー位置抽選.
/// </summary>
void StraightLaserPoint::ShotRand() {

	// 予測線表示開始時に次の発射方向と位置を決定
	shotDir = rand() % 4;

	// 次の発射位置もランダムに決定
	if (shotDir == 0 || shotDir == 1) // 水平発射
	{
		centerPos = 100 + rand() % (WINDOW_HEI - 200);
	}
	else // 垂直発射
	{
		centerPos = 100 + rand() % (WINDOW_WID - 200);
	}
}

/// <summary>
/// 予測線描画.
/// </summary>
void StraightLaserPoint::DrawPreLine(float count)
{
	Line preLine;

	//位置の設定.
	switch (shotDir)
	{
	case 0: //→.
		preLine.stPos = { -50,             centerPos };
		preLine.edPos = { WINDOW_WID + 50, centerPos };
		break;
	case 1: //←.
		preLine.stPos = { WINDOW_WID + 50, centerPos };
		preLine.edPos = { -50,             centerPos };
		break;
	case 2: //↓.
		preLine.stPos = { centerPos, -50 };
		preLine.edPos = { centerPos, WINDOW_HEI + 50 };
		break;
	case 3: //↑.
		preLine.stPos = { centerPos, WINDOW_HEI + 50 };
		preLine.edPos = { centerPos, -50 };
		break;
	}

	//予測線の透明度.
	const double alpha = Calc::AnimEase(EaseType::InQuad, count / LASER_STR_PREDICTION_TIME);

	//描画.
	DrawMode::Exe(
		DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * (1 - alpha)),
		[&](){
			preLine.color = COLOR_PRE_EFFECT;
			DrawLineKR(preLine, true);
		}
	);
}

// ▼*--=<[ StraightLaser ]>=--*▼ //

/// <summary>
/// 初期化.
/// </summary>
void StraightLaser::Init()
{
	for (auto& i : points) {
		i.Init();
	}
}

/// <summary>
/// リセット.
/// </summary>
void StraightLaser::Reset()
{
	//自動実行設定.
	SetAutoExeMode(MngAutoExe::Stop);

	counter = LASER_STR_PREDICTION_TIME + 80; //予測線が出るタイミングから開始.
	counterPrediction = 0;
	isShowPreLine = false;

	for (auto& i : points) {
		i.Reset();
	}
}

/// <summary>
/// 更新.
/// </summary>
void StraightLaser::Update()
{
	//タイマー更新.
	counter -= gameData->speedRate;

	//予測線更新.
	if (counter <= LASER_STR_PREDICTION_TIME)
	{
		if (!isShowPreLine)
		{
			//発射に関する抽選.
			for (auto& i : points) {
				i.ShotRand();
			}

			isShowPreLine = true;
		}
		counterPrediction = LASER_STR_PREDICTION_TIME - counter; // 予測線表示からの経過時間
	}
	//直線レーザー発射.
	if (counter <= 0)
	{
		//発射.
		for (auto& i : points) {
			i.ShotLaser();
		}
		//効果音再生.
		if (auto i = soundMng->Get(_T("Laser2"))) {
			i->Play(false, 75);
		}

		//タイマー再開(徐々に短くなる)
		//予測線の出る時間より短くならないよう設定.
		counter = LASER_STR_PREDICTION_TIME + LASER_STR_SPAWN_SPAN * gameData->spawnRate;

		isShowPreLine = false;
		counterPrediction = 0;
	}
}

/// <summary>
/// 描画.
/// </summary>
void StraightLaser::Draw()
{
	if (isShowPreLine) {
		//予測線の描画.
		for (auto& i : points) {
			i.DrawPreLine(counterPrediction);
		}
	}
}