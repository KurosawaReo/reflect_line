/*
   - Obst_MeteorManager.cpp -

   障害物: 隕石出現管理.
*/
#include "Obst_MeteorManager.h"

//依存関係.
#include "GameData.h"
#include "GameManager.h"
#include "EffectManager.h"
//参照.
static GameData*      gameData;
static Player*        player;
static EffectManager* effectMng;
//参照(KRライブラリ)
static SoundMng*      soundMng;

// ▼*---=[ MeteorManager ]=---*▼ //

//当たった隕石を1つ取得(線と線)
Meteor* MeteorManager::GetHitMeteor(Line hit, bool isDestroy) {

	//全隕石ループ.
	for (auto& i : meteors) {
		if (i.IsHitMeteor(hit)) {
			return &i; //この隕石を返す.
		}
	}
	return nullptr; //隕石はない.
}

//当たった隕石を1つ取得(線と線)
Meteor* MeteorManager::GetHitMeteor(Circle hit, bool isDestroy) {

	//全隕石ループ.
	for (auto& i : meteors) {
		if (i.IsHitMeteor(hit)) {
			return &i; //この隕石を返す.
		}
	}
	return nullptr; //隕石はない.
}

//目標となる隕石を選ぶ.
Meteor* MeteorManager::GetTargetMeteor(DBL_XY pos) {

	Meteor* ret = nullptr; //最寄りの隕石.
	double shortest = -1;  //暫定の最短距離.

	//全隕石ループ.
	for (auto& i : meteors) {
		//破壊されてる or ターゲットされてるならスキップ.
		if (i.GetState() == MeteorState::Destroy || i.GetIsTargeting()){
			continue;
		}

		DBL_XY tmpPos = i.GetPos();              //1つずつ座標取得.
		double tmpDis = Calc::Dist(tmpPos, pos); //距離を計算.

		//初回 or より近ければ更新.
		if (shortest == -1 || tmpDis < shortest) {
			shortest = tmpDis;
			ret = &i;
		}
	}
	//この隕石をターゲット.
	if (ret) {
		ret->SetIsTargeting(true);
	}

	return ret;
}

void MeteorManager::Init() {

	gameData  = ManagerInsts::Get<GameData>();
	player    = ManagerInsts::Get<Player>();
	effectMng = ManagerInsts::Get<EffectManager>();
	soundMng  = ManagerInsts::Get<SoundMng>();
}

void MeteorManager::Reset() {

	timer = METEOR_SPAWN_SPAN; //初期時間.
	meteors.clear();           //隕石を全て消去.
}

void MeteorManager::Update() {

	//タイマーが残っていれば.
	if (timer > 0) {
		timer -= gameData->speedRate;
	}
	//タイマーが0になったら.
	else {
		SpawnMeteor(); //隕石生成.

		//次に出現するまでの時間(徐々に短くなる)
		timer = METEOR_SPAWN_SPAN * gameData->spawnRate * gameData->spawnRate;
	}

	//全隕石ループ.
	for (auto i = meteors.begin(); i != meteors.end(); ) {
		i->Update(); //更新.
		//次の要素へ.
		if (i->GetIsErase()) {
			i = meteors.erase(i);
		}
		else{
			i++;
		}
	}

	//プレイヤーとの当たり判定.
	if (GetHitMeteor(player->GetHit(), false)) {
		player->Death(); //死亡.
	}
}

void MeteorManager::Draw() {

#if defined	DEBUG_METEOR_SPAWN
	//隕石の目標地点範囲.
	{
		DrawMode _(DrawModeID::None, DrawBlendModeID::Alpha, 100);

		Box box = {{WINDOW_WID/2, WINDOW_HEI/2}, {METEOR_GOAL_RAND_RANGE*2, METEOR_GOAL_RAND_RANGE*2}, 0xFFA0A0};
		DrawBoxKR(&box, Anchor::Mid);
	}
#endif

#if defined DEBUG_OBJ_ACTIVE
	//デバッグ表示.
	DrawFormatString(0, 140, 0xFF00FF, _T("隕石　　　　　 : %d"), meteor.size());
#endif

	//全隕石ループ.
	for (auto& i : meteors) {
		i.Draw(); //描画.
	}
}

//隕石生成.
void MeteorManager::SpawnMeteor(){
	
	Meteor tmp;
	tmp.Init(); //初期化.
	meteors.push_back(tmp);
}

//隕石破壊演出.
void MeteorManager::BreakMeteor(DBL_XY pos, bool isScore, double scale) {
	
	//破片エフェクト.
	{
		EffectData data{};
		data.type = EffectType::MeteorFragment;
		data.pos  = pos;
		//いくつか出す.
		for (int i = 0; i < METEOR_BREAK_ANIM_CNT; i++) {

			double newDig = _flt(Calc::RandNum(0, 3599) / 10);			//飛ぶ方向抽選.
			data.vec   = Calc::AngToVector(newDig);						//ベクトルに反映.

			data.speed = _flt(Calc::RandNum(50, 200) / 10 * scale);		//速度抽選.
			data.len   = _flt(Calc::RandNum(30, 120) / 10 * scale);		//長さ抽選.
			data.ang   = _flt(Calc::RandNum(0, 3599) / 10);				//角度抽選.
			effectMng->SpawnEffect(&data);								//エフェクト出現.
		}
	}
	//破壊エフェクト.
	{
		EffectData data{};
		data.type = EffectType::MeteorCrash;
		data.pos  = pos;
		effectMng->SpawnEffect(&data); //エフェクト出現.
	}
	//スコアエフェクト.
	if (isScore) {
		EffectData data{};
		data.type = EffectType::Score500;
		data.pos  = pos;
		effectMng->SpawnEffect(&data); //エフェクト出現.
	}

	//サウンド.
	if (auto i = soundMng->Get(_T("MeteorBreak"))) {
		i->Play(false, 65); //再生.
	}
}