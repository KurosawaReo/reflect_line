/*
   - Obst_NormalLaser.cpp -

   障害物: レーザー発射台(継承元)
*/
#include "Obst_NormalLaser.h" //自身のヘッダ.

//依存関係.
#include "Player.h"
#include "LaserManager.h"
#include "Obst_MeteorManager.h"
#include "GameData.h"
#include "GameManager.h"
//参照.
static GameData*      gameData;
static Player*        player;
static LaserManager*  laserMng;
static MeteorManager* meteorMng;
static NormalLaser*   normalLaser;
//参照(KRライブラリ)
static SoundMng*      soundMng;
static SceneMng*      sceneMng;

using namespace Calc; //計算機能を使用.

// ▼*--=<[ NormalLaserPoint ]>=--*▼ //

//方向取得.
DBL_XY NormalLaserPoint::GetVec() const {

	switch (move.dir) {
		//                 (move.isLeft) ? 左回り         : 右回り.
		case Left:  return (move.isLeft) ? DBL_XY( 0, +1) : DBL_XY( 0, -1);
		case Up:    return (move.isLeft) ? DBL_XY(-1,  0) : DBL_XY(+1,  0);
		case Right: return (move.isLeft) ? DBL_XY( 0, -1) : DBL_XY( 0, +1);
		case Down:  return (move.isLeft) ? DBL_XY(+1,  0) : DBL_XY(-1,  0);
	}
	return { 0, 0 }; //不正な値.
}

void NormalLaserPoint::Reset() {

	speed     = 3;
	validFlag = false;

	MoveRand();
}

void NormalLaserPoint::Update() {

	//有効なもののみ.
	if (validFlag) {

		//移動(方向 * 速度 * 速度倍率)
		pos += GetVec() * speed * gameData->speedRate;
		//画面サイズ.
		DBL_RECT winSize = App::GetWindowRect().ToDbl();

		//画面から少しでもはみ出たら.
		if (Calc::IsOutInArea(pos, { 0, 0 }, winSize.AddSize(1), false)) {
			//画面内に補正.
			Calc::FixPosInArea(&pos, { 0, 0 }, winSize);
			//回転.
			if (move.isLeft) {
				move.dir = static_cast<MoveDir>((move.dir + 3) % MoveDir::Count); //左回り.
			}
			else {
				move.dir = static_cast<MoveDir>((move.dir + 1) % MoveDir::Count); //右回り.
			}
		}
	}
}

//予告描画.
void NormalLaserPoint::DrawPrediction(float count) {

	//有効なもののみ.
	if (validFlag) {

		//点滅.
		float blinkProgress = (60 - count) / 60.0f; //0.0～1.0
		int   blinkAlpha    = _int_r(128 + 127 * sin(blinkProgress * M_PI * 8));

		//サイズを徐々に大きく.
		float dotSize  = (float)(3 + AnimEase(EaseType::OutQuad, blinkProgress) * LASER_NOR_PRE_LASER1_SIZE);
		float dotSize2 = (float)(3 + AnimEase(EaseType::OutQuad, blinkProgress) * LASER_NOR_PRE_LASER2_SIZE);
		//円情報.
		Circle cir = { pos, 0, COLOR_PRE_EFFECT, 1.0f };

		//描画.
		DrawMode::Exe(
			DrawModeID::None, DrawBlendModeID::Alpha, blinkAlpha,
			[&]() {

				cir.r = dotSize;
				DrawCircleKR(cir, Anchor::Mid, false, true);
				cir.r = dotSize2;
				DrawCircleKR(cir, Anchor::Mid, false, true);
			}
		);
	}
}

//レーザー発射.
void NormalLaserPoint::Shot() {

	//有効なもののみ.
	if (validFlag) {

		//プレイヤー座標.
		const DBL_XY plyPos = player->GetPos();
		//プレイヤー方向への初期角度計算.
		const double angle  = atan2(plyPos.y - pos.y, plyPos.x - pos.x);
		const DBL_XY vel    = { cos(angle), sin(angle) };

		laserMng->SpawnLaser(pos, vel, LaserType::Normal); //通常レーザー召喚.
		normalLaser->CreateFlashEffect(pos.x, pos.y); //エフェクトを出す.
	}
}

//レーザー移動抽選.
void NormalLaserPoint::MoveRand()
{
	//画面のどの辺にいくか.
	move.dir = static_cast<MoveDir>(rand() % MoveDir::Count);
	//どっち周りか.
	move.isLeft = (rand() % 2 == 0);

	//座標抽選.
	switch (move.dir)
	{
	case Left: //左.
		pos.x = 0;
		pos.y = _dbl(Calc::RandNum(0, WINDOW_HEI - 1));
		break;
	case Up: //上.
		pos.x = _dbl(Calc::RandNum(0, WINDOW_WID - 1));
		pos.y = 0;
		break;
	case Right: //右.
		pos.x = WINDOW_WID;
		pos.y = _dbl(Calc::RandNum(0, WINDOW_HEI - 1));
		break;
	case Down: //下.
		pos.x = _dbl(Calc::RandNum(0, WINDOW_WID - 1));
		pos.y = WINDOW_HEI;
		break;
	}
}

// ▼*--=<[ NormalLaser ]>=--*▼ //

//初期化.
void NormalLaser::Init(){
	
	gameData    = ManagerInsts::Get<GameData>();
	player      = ManagerInsts::Get<Player>();
	laserMng    = ManagerInsts::Get<LaserManager>();
	meteorMng   = ManagerInsts::Get<MeteorManager>();
	normalLaser = ManagerInsts::Get<NormalLaser>();
	soundMng    = ManagerInsts::Get<SoundMng>();
	sceneMng    = ManagerInsts::Get<SceneMng>();
}

//リセット.
void NormalLaser::Reset()
{
	counter   = LASER_NOR_SHOT_START + 100;
	counterTm = LASER_NOR_SHOT_START;

	//発射台.
	for (auto& i : points) {
		i.Reset();
	}
	//フラッシュ.
	for (int i = 0; i < LASER_NOR_FLASH_MAX; i++) {
		flash[i].validFlag = false; //全て無効に.
	}
}

//更新.
void NormalLaser::Update()
{
	counter -= gameData->speedRate; //経過カウンター.

	//発射台更新.
	for (auto& i : points) {
		i.Update();
	}

	//タイミングが来たらレーザー発射.
	if (counter <= counterTm) {
		//発射.
		for (auto& i : points) {
			i.Shot();
		}
		//効果音再生.
		if (auto i = soundMng->Get(_T("Laser1"))) {
			i->Play(false, 60);
		}
		//次のレーザーを発射するタイミング.
		counterTm -= LASER_NOR_SHOT_SPAN;
	}

	//0を下回ったらタイマー再開.
	if (counter <= 0) {
		//発射開始時間 + 待機時間(待機時間は徐々に短くなる)
		counter = LASER_NOR_SHOT_START + LASER_NOR_SHOT_RESET * gameData->spawnRate;
		//発射開始時間.
		counterTm = LASER_NOR_SHOT_START;
		//レーザー移動抽選.
		for (auto& i : points) {
			i.MoveRand();
		}
	}

	//エフェクト.
	for (int i = 0; i < LASER_NOR_FLASH_MAX; i++) {
		//有効なら.
		if (flash[i].validFlag) {
			flash[i].counter += gameData->speedRate;
		}
	}
}

//描画.
void NormalLaser::Draw()
{
	//レーザー発射前なら.
	if (counter <= counterTm + 60) {
		//予告を出す.
		for (auto& i : points) {
			i.DrawPrediction(counter);
		}
	}

	DrawObstFlash(); //発射エフェクトの描画.
}

//発射エフェクトの描画.
void NormalLaser::DrawObstFlash() {

	for (int i = 0; i < LASER_NOR_FLASH_MAX; i++)
	{
		//無効ならスキップ.
		if (!flash[i].validFlag) {
			continue;
		}

		//エフェクトの透明度を時間に応じて計算.
		float alpha = 1.0f - (
			flash[i].counter * LASER_NOR_FLASH_ALPHA_TM / flash[i].Duration
		);
		int alphaValue = _int_r(255 * alpha);
		alphaValue = max(alphaValue, 0); //下限は0.

		//エフェクトのサイズを時間に応じて拡大.
		float sizeMultiplier = LASER_NOR_FLASH_SIZE_INIT + (
			flash[i].counter * LASER_NOR_FLASH_SIZE_SPREAD / flash[i].Duration
		);
		int effectSize = _int_r(flash[i].BaseSize * sizeMultiplier);
		int innerSize = effectSize / 2;

		//プレイヤーの方向を計算.
		double angle = flash[i].angle;
		double cos_a = cos(angle);
		double sin_a = sin(angle);

#if false
		//エフェクトを時間経過でプレイヤーの方へ進ませる.
		float progress = flashEffect[i].Counter / flashEffect[i].Duration;
		double currentX = flashEffect[i].x + cos_a * progress * 1000; // nピクセル分移動
		double currentY = flashEffect[i].y + sin_a * progress * 1000;
#else
		double currentX = flash[i].x;
		double currentY = flash[i].y;
#endif

		Triangle tri;
		//三角形の3点.
		tri.pos[0].x = currentX + cos_a * effectSize; //先端.
		tri.pos[0].y = currentY + sin_a * effectSize;
		tri.pos[1].x = currentX - cos_a * effectSize/3 + sin_a * effectSize/2; //左後.
		tri.pos[1].y = currentY - sin_a * effectSize/3 - cos_a * effectSize/2;
		tri.pos[2].x = currentX - cos_a * effectSize/3 - sin_a * effectSize/2; //右後.
		tri.pos[2].y = currentY - sin_a * effectSize/3 + cos_a * effectSize/2;

		Line line1, line2;
		//三角形の2辺を線にする.
		line1.stPos = tri.pos[1];
		line1.edPos = tri.pos[0];
		line2.stPos = tri.pos[0];
		line2.edPos = tri.pos[2];
		line1.color = GetColor(0, 255, 255);
		line2.color = GetColor(0, 255, 255);
		//描画.
		DrawMode::Exe(
			DrawModeID::None, DrawBlendModeID::Alpha, alphaValue,
			[&]() {
				DrawLineKR(line1, true);
				DrawLineKR(line2, true);
			}
		);

		//エフェクト時間が終了したら無効化
		if (flash[i].counter >= flash[i].Duration)
		{
			flash[i].validFlag = false;
		}
	}
}

//光るeffectの生成.
void NormalLaser::CreateFlashEffect(double fx, double fy)
{
	DBL_XY pPos = player->GetPos(); //プレイヤー座標取得.

	//未使用のエフェクトスロットを探す.
	for (int i = 0; i < LASER_NOR_FLASH_MAX; i++)
	{
		if (!flash[i].validFlag)
		{
			double dx = pPos.x - fx;
			double dy = pPos.y - fy;
			double angle = atan2(dy, dx);

			//エフェクトデータの設定.
			flash[i].x = fx;
			flash[i].y = fy;
			flash[i].angle = angle; // プレイヤーへの角度を保存
			flash[i].counter = 0;
			flash[i].Duration = LASER_NOR_FLASH_VALID_TM; //一定フレーム光る.
			flash[i].BaseSize = 20; //基本サイズ
			flash[i].validFlag = 1;
			break;
		}
	}
}

//何個発射台を使うか.
void NormalLaser::UseLaserPointCnt(int count) {
	
	int tmp = count;

	//全ての発射台.
	for (auto& i : points) {
		i.Reset();               //一旦リセット.
		i.SetValidFlag(tmp > 0); //必要な数だけ有効に.
		tmp--;
	}
}