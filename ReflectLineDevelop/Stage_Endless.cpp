/*
   - Stage_Endless.cpp -
*/
#include "Stage_Endless.h"

//依存関係.
#include "LaserManager.h"
#include "Obst_NormalLaser.h"
#include "Obst_StraightLaser.h"
#include "Obst_MeteorManager.h"
#include "Obst_Ripples.h"
#include "Obst_Fireworks.h"
#include "Item.h"
#include "EffectManager.h"
#include "GameData.h"
#include "GameManager.h"
//参照.
static GameData*      gameData;
static EffectManager* effectMng;
//参照(KRライブラリ)
static SoundMng*      soundMng;
static InputMng*      inputMng;

// ▼*--=<[ EndlessStage ]>=--*▼ //

//初期化.
void EndlessStage::Init() {
	//参照取得.
	gameData  = ManagerInsts::Get<GameData>();
	effectMng = ManagerInsts::Get<EffectManager>();
	soundMng  = ManagerInsts::Get<SoundMng>();
	inputMng  = ManagerInsts::Get<InputMng>();
}

//リセット.
void EndlessStage::Reset() {
}

//更新.
void EndlessStage::Update() {

	//カウンター増加.
	gameData->counter += gameData->speedRate;
	//出現間隔.
	gameData->spawnRate = 1.0f/(1.0f+(gameData->counter/8000.0f)); //100%から少しずつ減少.

	Debug::Log(_T("gameData->spawnRate"), gameData->spawnRate);

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
			if (gameData->counter >= LEVEL_2_COUNTER) {
				UpLevel2(); //Lv2にする.
			}
			break;

		case 2:
			if (gameData->counter >= LEVEL_3_COUNTER) {
				UpLevel3(); //Lv3にする.
			}
			break;

		case 3:
			if (gameData->counter >= LEVEL_4_COUNTER) {
				UpLevel4(); //Lv4にする.
			}
			break;

		case 4:
			if (gameData->counter >= LEVEL_5_COUNTER) {
				UpLevel5(); //Lv5にする.
			}
			break;

		case 5:
			break;

		default: assert(false); break;
	}
}

//描画.
void EndlessStage::Draw() {
}

//レベルアップ処理(Level1)
void EndlessStage::UpLevel1() {

	//サウンド.
	if (auto i = soundMng->Get(_T("LevelUp"))) {
		i->Play(false, 100);
	}

	//エフェクト.
	//指示付き初期化でclassのメンバ変数を初期化.
	effectMng->SpawnEffect({
		.type = EffectType::Endless_Level1,
		.pos = { WINDOW_WID / 2, WINDOW_HEI / 2 }
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
		ManagerInsts::Get<NormalLaser>()->UseLaserPointCnt(2); //レーザーは2つ.
#endif
	}

	gameData->level = 1; //Lv1へ.
}

//レベルアップ処理(Level2)
void EndlessStage::UpLevel2() {

	//サウンド.
	if (auto i = soundMng->Get(_T("LevelUp"))) {
		i->Play(false, 100);
	}

	//エフェクト.
	effectMng->SpawnEffect({
		.type = EffectType::Endless_Level2,
		.pos = { WINDOW_WID / 2, WINDOW_HEI / 2 }
	});

	//Lv2の出現設定.
#if !defined INVALID_OBST_STR_LASER
	ManagerInsts::Get<StraightLaser>()->SetAutoExeMode(MngAutoExe::Active);
#endif

	gameData->level = 2; //Lv2へ.
}

//レベルアップ処理(Level3)
void EndlessStage::UpLevel3() {

	//サウンド.
	if (auto i = soundMng->Get(_T("LevelUp"))) {
		i->Play(false, 100);
	}

	//エフェクト.
	effectMng->SpawnEffect({
		.type = EffectType::Endless_Level3,
		.pos = { WINDOW_WID / 2, WINDOW_HEI / 2 }
	});

	//Lv3の出現設定.
#if !defined INVALID_OBST_RIPPLES
	ManagerInsts::Get<Ripples>()->SetAutoExeMode(MngAutoExe::Active);
#endif

	gameData->level = 3; //Lv3へ.
}

//レベルアップ処理(Level4)
void EndlessStage::UpLevel4() {

	//サウンド.
	if (auto i = soundMng->Get(_T("LevelUp"))) {
		i->Play(false, 100);
	}

	//エフェクト.
	effectMng->SpawnEffect({
		.type = EffectType::Endless_Level4,
		.pos = { WINDOW_WID / 2, WINDOW_HEI / 2 }
	});

	//Lv4の出現設定.
#if !defined INVALID_OBST_FIREWORKS
	ManagerInsts::Get<Fireworks>()->SetAutoExeMode(MngAutoExe::Active);
#endif

	gameData->level = 4; //Lv4へ.
}

//レベルアップ処理(Level5)
void EndlessStage::UpLevel5() {

	//サウンド.
	if (auto i = soundMng->Get(_T("LevelUp"))) {
		i->Play(false, 100);
	}

	//エフェクト.
	effectMng->SpawnEffect({
		.type = EffectType::Endless_Level5,
		.pos = { WINDOW_WID / 2, WINDOW_HEI / 2 }
	});

	//Lv5の出現設定.
#if !defined INVALID_OBST_NOR_LASER
	ManagerInsts::Get<NormalLaser>()->UseLaserPointCnt(4); //レーザーは4つ.
#endif

	gameData->level = 5; //Lv5へ.
}