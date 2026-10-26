/*
   - Mode_Endless.cpp -
*/
#include "Mode_Endless.h"

//依存関係.
#include "../GameManager.h"
#include "../GameData.h"
#include "../LaserManager.h"
#include "../Obstacles/Obst_NormalLaser.h"
#include "../Obstacles/Obst_StraightLaser.h"
#include "../Obstacles/Obst_MeteorManager.h"
#include "../Obstacles/Obst_Ripples.h"
#include "../Obstacles/Obst_Fireworks.h"
#include "../ItemManager.h"
#include "../EffectManager.h"
//参照.
static GameData*      gameData;
static EffectManager* effectMng;
//参照(KRライブラリ)
static SoundMng*      soundMng;
static InputMng*      inputMng;

// ▼*--=<[ EndlessMode ]>=--*▼ //

//初期化.
void EndlessMode::Init() {
	//参照取得.
	gameData  = ManagerInsts::Get<GameData>();
	effectMng = ManagerInsts::Get<EffectManager>();
	soundMng  = ManagerInsts::Get<SoundMng>();
	inputMng  = ManagerInsts::Get<InputMng>();
}

//リセット.
void EndlessMode::Reset() {
}

//更新.
void EndlessMode::Update() {

	//カウンター増加.
	gameData->counter += gameData->speedRate;
	//出現間隔(100%から少しずつ減少)
	gameData->spawnRate = 1.0f/(1.0f+(gameData->counter/Config::SPAWN_RATE_HALF_COUNT));

#if defined _DEBUG //Releaseでは入れない.
	//タイマー加速(デバッグ用)
	if (inputMng->IsPushKey(KeyID::L) == 1) {
		gameData->counter += 30;
	}
#endif

	//レベル管理.
	switch (gameData->level)
	{
		case 0:
			UpLevel1(); //Lv1にする.
			break;

		case 1:
			if (gameData->counter >= Config::LEVEL_2_COUNT) {
				UpLevel2(); //Lv2にする.
			}
			break;

		case 2:
			if (gameData->counter >= Config::LEVEL_3_COUNT) {
				UpLevel3(); //Lv3にする.
			}
			break;

		case 3:
			if (gameData->counter >= Config::LEVEL_4_COUNT) {
				UpLevel4(); //Lv4にする.
			}
			break;

		case 4:
			if (gameData->counter >= Config::LEVEL_5_COUNT) {
				UpLevel5(); //Lv5にする.
			}
			break;

		case 5:
			break;

		default: assert(false); break;
	}
}

//描画.
void EndlessMode::Draw() {
}

//レベルアップ処理(Level1)
void EndlessMode::UpLevel1() {

	//サウンド.
	if (auto i = soundMng->Get(_T("LevelUp"))) {
		i->Play(false, 100);
	}

	//エフェクト.
	//指示付き初期化でclassのメンバ変数を初期化.
	effectMng->SpawnEffect({
		.type = EffectType::Endless_Level1,
		.pos = { Config::WINDOW_WID / 2, Config::WINDOW_HEI / 2 }
	});

	//出現設定.
	{
		ManagerInsts::Get<LaserManager>()->SetAutoExeMode(MngAutoExe::Active);

#if !defined INVALID_ITEM
		ManagerInsts::Get<ItemManager>()->SetAutoExeMode(MngAutoExe::Active);
#endif
#if !defined INVALID_OBST_METEOR
		ManagerInsts::Get<MeteorManager>()->SetAutoExeMode(MngAutoExe::Active);
#endif
#if !defined INVALID_OBST_NOR_LASER
		ManagerInsts::Get<NormalLaser>()->SetAutoExeMode(MngAutoExe::Active);
		ManagerInsts::Get<NormalLaser>()->EnableCount(2); //レーザーは2つ.
#endif
	}

	gameData->level = 1; //Lv1へ.
}

//レベルアップ処理(Level2)
void EndlessMode::UpLevel2() {

	//サウンド.
	if (auto i = soundMng->Get(_T("LevelUp"))) {
		i->Play(false, 100);
	}

	//エフェクト.
	effectMng->SpawnEffect({
		.type = EffectType::Endless_Level2,
		.pos = { Config::WINDOW_WID / 2, Config::WINDOW_HEI / 2 }
	});

	//Lv2の出現設定.
#if !defined INVALID_OBST_STR_LASER
	ManagerInsts::Get<StraightLaser>()->SetAutoExeMode(MngAutoExe::Active);
#endif

	gameData->level = 2; //Lv2へ.
}

//レベルアップ処理(Level3)
void EndlessMode::UpLevel3() {

	//サウンド.
	if (auto i = soundMng->Get(_T("LevelUp"))) {
		i->Play(false, 100);
	}

	//エフェクト.
	effectMng->SpawnEffect({
		.type = EffectType::Endless_Level3,
		.pos = { Config::WINDOW_WID / 2, Config::WINDOW_HEI / 2 }
	});

	//Lv3の出現設定.
#if !defined INVALID_OBST_RIPPLES
	ManagerInsts::Get<Ripples>()->SetAutoExeMode(MngAutoExe::Active);
#endif

	gameData->level = 3; //Lv3へ.
}

//レベルアップ処理(Level4)
void EndlessMode::UpLevel4() {

	//サウンド.
	if (auto i = soundMng->Get(_T("LevelUp"))) {
		i->Play(false, 100);
	}

	//エフェクト.
	effectMng->SpawnEffect({
		.type = EffectType::Endless_Level4,
		.pos = App::GetWindowSize().ToDbl() / 2
	});

	//Lv4の出現設定.
#if !defined INVALID_OBST_FIREWORKS
	ManagerInsts::Get<Fireworks>()->SetAutoExeMode(MngAutoExe::Active);
#endif

	gameData->level = 4; //Lv4へ.
}

//レベルアップ処理(Level5)
void EndlessMode::UpLevel5() {

	//サウンド.
	if (auto i = soundMng->Get(_T("LevelUp"))) {
		i->Play(false, 100);
	}

	//エフェクト.
	effectMng->SpawnEffect({
		.type = EffectType::Endless_Level5,
		.pos = App::GetWindowSize().ToDbl() / 2
	});

	//Lv5の出現設定.
#if !defined INVALID_OBST_NOR_LASER
	ManagerInsts::Get<NormalLaser>()->EnableCount(4); //レーザーは4つ.
#endif

	gameData->level = 5; //Lv5へ.
}