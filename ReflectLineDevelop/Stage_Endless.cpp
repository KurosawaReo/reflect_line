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

// ▼*--=<[ StraightLaser ]>=--*▼ //

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

	//最初のみ.
	if (gameData->counter == 0) {
		//サウンド.
		if (auto i = soundMng->Get(_T("LevelUp"))) {
			i->Play(false, 100);
		}
		//エフェクト.
		EffectData data{};
		data.type = EffectType::Endless_Level1;
		data.pos = { WINDOW_WID/2, WINDOW_HEI/2 };
		effectMng->SpawnEffect(&data);

		//Lv1の出現設定.
		{
			ManagerInsts::Get<LaserManager> ()->SetAutoExeMode(MngAutoExe::Active);

#if !defined INVALID_ITEM
			ManagerInsts::Get<ItemManager>  ()->SetAutoExeMode(MngAutoExe::Active);
#endif
#if !defined INVALID_OBST_METEOR
			ManagerInsts::Get<MeteorManager>()->SetAutoExeMode(MngAutoExe::Active);
#endif
#if !defined INVALID_OBST_NOR_LASER
			ManagerInsts::Get<NormalLaser>  ()->SetAutoExeMode(MngAutoExe::Active);
			ManagerInsts::Get<NormalLaser>  ()->UseLaserPointCnt(2); //レーザーは2つ.
#endif
		}
	}
	else {
#if defined _DEBUG //Releaseでは入れない.
		//タイマー加速(Debug)
		if (inputMng->IsPushKey(KeyID::L) == 1) {
			gameData->counter += 30;
		}
#endif
	}

	//カウンター増加.
	gameData->counter += gameData->speedRate;
	//出現間隔.
	gameData->spawnRate = 1.0f/(1+(gameData->counter/8000)); //100%から少しずつ減少.
	//レベル管理.
	switch (gameData->level)
	{
		case 0:
			break;
		case 1:
			if (gameData->counter >= 1500) { //1500 = 出現間隔約??%地点.
				gameData->level = 2; //Lv2へ.

				//サウンド.
				if (auto i = soundMng->Get(_T("LevelUp"))) {
					i->Play(false, 100);
				}
				//エフェクト.
				EffectData data{};
				data.type = EffectType::Endless_Level2;
				data.pos  = {WINDOW_WID/2, WINDOW_HEI/2};
				effectMng->SpawnEffect(&data);

				//Lv2の出現設定.
#if !defined INVALID_OBST_STR_LASER
				ManagerInsts::Get<StraightLaser>()->SetAutoExeMode(MngAutoExe::Active);
#endif
			}
			break;
		case 2:
			if (gameData->counter >= 3500) { //3500 = 出現間隔約??%地点.
				gameData->level = 3; //Lv3へ.

				//サウンド.
				if (auto i = soundMng->Get(_T("LevelUp"))) {
					i->Play(false, 100);
				}
				//エフェクト.
				EffectData data{};
				data.type = EffectType::Endless_Level3;
				data.pos  = {WINDOW_WID/2, WINDOW_HEI/2};
				effectMng->SpawnEffect(&data);

				//Lv3の出現設定.
#if !defined INVALID_OBST_RIPPLES
				ManagerInsts::Get<Ripples>()->SetAutoExeMode(MngAutoExe::Active);
#endif
			}
			break;
		case 3:
			if (gameData->counter >= 6000) { //6000 = 出現間隔約??%地点.
				gameData->level = 4; //Lv4へ.

				//サウンド.
				if (auto i = soundMng->Get(_T("LevelUp"))) {
					i->Play(false, 100);
				}
				//エフェクト.
				EffectData data{};
				data.type = EffectType::Endless_Level4;
				data.pos  = {WINDOW_WID/2, WINDOW_HEI/2};
				effectMng->SpawnEffect(&data);

				//Lv4の出現設定.
#if !defined INVALID_OBST_FIREWORKS
				ManagerInsts::Get<Fireworks>()->SetAutoExeMode(MngAutoExe::Active);
#endif
			}
			break;
		case 4:
			if (gameData->counter >= 9000) { //9000 = 出現間隔約??%地点.
				gameData->level = 5; //Lv5へ.

				//サウンド.
				if (auto i = soundMng->Get(_T("LevelUp"))) {
					i->Play(false, 100);
				}
				//エフェクト.
				EffectData data{};
				data.type = EffectType::Endless_Level5;
				data.pos  = {WINDOW_WID/2, WINDOW_HEI/2};
				effectMng->SpawnEffect(&data);

				//Lv5の出現設定.
#if !defined INVALID_OBST_NOR_LASER
				ManagerInsts::Get<NormalLaser>()->UseLaserPointCnt(4); //レーザーは4つ.
#endif
			}
			break;
		case 5:
			break;

		default: assert(FALSE); break;
	}
}
//描画.
void EndlessStage::Draw() {
}