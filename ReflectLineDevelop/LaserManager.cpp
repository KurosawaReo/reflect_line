/*
   - LaserManager.cpp -
   レーザー管理クラス.
*/
#include "LaserManager.h"

//依存関係.
#include "GameData.h"
#include "GameManager.h"
#include "Player.h"
#include "Obst_MeteorManager.h"
#include "Stage_Tutorial.h"
//参照.
static GameData*      gameData;
static GameManager*   gameMng;
static Player*        player;
static MeteorManager* meteorMng;
static EffectManager* effectMng;
static TutorialStage* tutorialStg;
//参照(KRライブラリ)
static SoundMng*      soundMng;

using namespace Calc; //計算機能を使用.

// ▼*---=[ LaserManager ]=---*▼ //

//初期化.
void LaserManager::Init() {

	gameData    = ManagerInsts::Get<GameData>();
	gameMng     = ManagerInsts::Get<GameManager>();
	player      = ManagerInsts::Get<Player>();
	meteorMng   = ManagerInsts::Get<MeteorManager>();
	effectMng   = ManagerInsts::Get<EffectManager>();
	tutorialStg = ManagerInsts::Get<TutorialStage>();
	soundMng    = ManagerInsts::Get<SoundMng>();
}
//リセット.
void LaserManager::Reset() {

	//自動実行設定.
	SetAutoExeMode(MngAutoExe::Stop);

	//レーザーを全て消去.
	laser.clear();
	//レーザー描画線を全て消去.
	line.clear();
}
//更新.
void LaserManager::Update() {

	plyPos = player->GetPos(); //プレイヤーの現在位置を取得.

	UpdateLaser();     //各レーザーの更新.
	UpdateLaserLine(); //各レーザー描画線の更新.
}
//描画.
void LaserManager::Draw() {

#if defined DEBUG_OBJ_ACTIVE
	//デバッグ表示.
	DrawFormatString(0, 100, 0xFF00FF, _T("レーザー　　　 : %d"), laser.size());
	DrawFormatString(0, 120, 0xFF00FF, _T("レーザー描画線 : %d"), line.size());
#endif

	//レーザー描画線.
	for (const LaserLineData& i : line) 
	{
		//時間経過で徐々に薄くする.
		int color = _int_r(255 * (1 - i.counter/LASER_LINE_DEL_TIME));
		color = max(color, 0); //最低値を0にする.

		//軌跡の線設定.
		Line tmpLine = { i.pos1, i.pos2, 0x000000, 2.0f };
		//線の色(時間経過で色が変化)
		switch (i.type)
		{
			case LaserType::Normal:
			case LaserType::Straight:
			case LaserType::Falling:
				tmpLine.color = COLOR_LASER_NORMAL(color); //通常カラー.
				break;
			case LaserType::Reflect:
			case LaserType::SuperReflect:
				tmpLine.color = COLOR_LASER_REFLECT(color); //反射カラー.
				break;

			default: assert(FALSE); break;
		}
		//加算合成モードで軌跡を描画(発光エフェクト)
		DrawMode::Exe(
			DrawModeID::None, DrawBlendModeID::Add, color,
			[&]() {
				DrawLineKR(tmpLine, true);
			}
		);
	}

	//チュートリアル限定.
	if (gameData->stage == StageType::Tutorial) {
		//レーザー本体.
		for (const LaserData& i : laser)
		{
			UINT color{};

			//色を指定.
			switch (i.type)
			{
				case LaserType::Normal:
				case LaserType::Straight:
				case LaserType::Falling:
					color = GetColor(50, 255, 255); //色の設定.
					break;
				case LaserType::Reflect:
				case LaserType::SuperReflect:
					color = GetColor(255, 0, 255); //色の設定.
					break;

				default: assert(FALSE); break;
			}

			//有効なレーザーに表示する.
			DrawStr str(_T("レーザー"), i.nowPos.ToInt(), color);
			str.Draw(Anchor::Mid, gameData->fonts["jp-size1"].GetFont());
		}
	}
}

//各レーザーの更新.
void LaserManager::UpdateLaser() {

	//各レーザーの更新.
	for (auto i = laser.begin(); i != laser.end(); )
	{
		bool isErase = false; //消去するかどうか.

		//レーザータイプ別.
		switch (i->type)
		{
			case LaserType::Normal:
			{
				//レーザーの当たり判定.
				HitLaser(i);

				//速度(時間経過で速くなる)
				const double speed = i->counter * LASER_NOR_SPEED;
				//レーザー移動.
				MoveLaser(i, speed);
			}
			break;

			case LaserType::Straight:
			{
				//レーザーの当たり判定.
				HitLaser(i);

				//速度(直線レーザーなので一定速度)
				const double speed = LASER_STR_SPEED;
				//レーザー移動.
				MoveLaser(i, speed);
			}
			break;

			case LaserType::Falling:
			{
				//ある程度薄くなるまで.
				if (i->counter <= LASER_FAL_HIT_ABLE) {
					//レーザーの当たり判定.
					HitLaser(i);
				}
				//時間を超えたら.
				if (i->counter >= LASER_FAL_DEL_TIME) {
					isErase = true; //消去する.
				}

				//重力.
				const double gravity = 0.08 * gameData->speedRate;
				i->vec.y += gravity;
				//空気抵抗.
				const double airResistance = 0.995;
				i->vec.x *= airResistance;
				//落下速度制限.
				const double maxSpeed = 8.0;
				if (i->vec.y > maxSpeed) {
					i->vec.y = maxSpeed;
				}

				//レーザー移動.
				MoveLaser(i, 1.0);
			}
			break;

			case LaserType::Reflect:
			case LaserType::SuperReflect:
			{
				//ターゲットが決まってなければ.
				if (!i->target) {
					//一定時間で目標地点を決める.
					if (i->counter >= LASER_REF_TRACK_ST_TM) {

						//ターゲットできる隕石を取得.
						Meteor* meteor = meteorMng->GetTargetMeteor(i->nowPos);
						//隕石があった場合.
						if (meteor) {
							i->target     = meteor; //隕石を登録.
							i->isGoTarget = true;   //そこへ向かう.
						}
					}
				}

				//隕石と当たっているなら.
				if (auto meteor = meteorMng->GetHitMeteor(i->hit, true)) {

					//壊れてない隕石であれば.
					if (meteor->GetState() == MeteorState::Normal) {
						meteor->Destroy(); //隕石を破壊.
					}

					const double ang = _deg(atan2(i->vec.y, i->vec.x)); //破片の飛ぶ方向.
					meteorMng->BreakMeteor(i->nowPos, ang, true);		//破壊演出.
					gameData->AddScore(SCORE_BREAK_METEOR);				//スコア加算.

					//どっちのタイプかで切り替え.
					if (i->type == LaserType::Reflect) {
						isErase = true; //消去する.
					}
					else {
						i->counter = LASER_REF_TRACK_ED_TM; //再反射後は追尾しない.
						ReflectLaser(i); //再反射.
					}
					//チュートリアルなら指示送信.
					if (gameData->stage == StageType::Tutorial) {
						tutorialStg->SetBreakMeteor(true);
					}
				}
				else {
					//レーザーの追尾処理.
					LaserRefTracking(i);

					//速度(時間経過で速くなる)
					const double speed = i->counter * LASER_REF_SPEED * gameData->speedRate;
					//レーザー移動.
					MoveLaser(i, speed);
				}
			}
			break;

			//想定外の値エラー.
			default: assert(false); break;
		}

		//画面外判定.
		const bool isOutX = (i->nowPos.x < -100) || (i->nowPos.x > WINDOW_WID + 100);
		const bool isOutY = (i->nowPos.y < -100) || (i->nowPos.y > WINDOW_HEI + 100);
		//画面外に出たレーザーを無効化.
		if (isOutX || isOutY) {
			isErase = true; //消去する.
		}

		//次のレーザーへ.
		NextLaser(i, isErase);
	}
}
//各レーザー描画線の更新.
void LaserManager::UpdateLaserLine() {

	for (auto i = line.begin(); i != line.end(); ) {

		//経過時間カウンタ増加.
		i->counter += gameData->speedRate;
		//一定フレーム経過したら消去.
		if (i->counter >= LASER_LINE_DEL_TIME) {
			i = line.erase(i);
		}
		else {
			i++;
		}
	}
}

//レーザー召喚.
void LaserManager::SpawnLaser(DBL_XY pos, DBL_XY vel, LaserType type) {

	LaserData tmp;			//レーザー作成.

	tmp.nowPos     = pos;	//初期座標.
	tmp.befPos     = pos;	//初期座標.
	tmp.vec        = vel;	//初期方向.
	tmp.counter    = 0;		//経過時間カウンタ初期化
	tmp.logNum     = 0;		//軌跡カウンタ初期化
	tmp.type       = type;	//タイプの登録

	tmp.target     = nullptr;
	tmp.isGoTarget = false;

	laser.push_back(tmp); //listに追加.
}

//次のレーザーへ.
void LaserManager::NextLaser(list<LaserData>::iterator& it, bool isErase) {

	//次の要素に進む.
	if (isErase) {
		//ターゲットが残っていれば.
		if (it->target) {
			it->target->SetIsTargeting(false); //ターゲットを解除.
		}
		//消去して次へ.
		it = laser.erase(it);
	}
	else {
		GenerateLaserLine(it);              //レーザー描画線の生成.
		it->counter += gameData->speedRate; //経過カウンター.
		it++;
	}
}

//レーザーの当たり判定.
void LaserManager::HitLaser(list<LaserData>::iterator& it) {

	//プレイヤーが無効なら中断.
	if (!player->GetActive()) {
		return;
	}

	//プレイヤーの当たり判定.
	Circle plyHit = player->GetHit();
	//反射モード中は判定を少し大きくする.
	if (player->GetMode() == PlayerMode::ItemReflect      ||
		player->GetMode() == PlayerMode::ItemReflectSuper)
	{
		plyHit.r += PLAYER_REF_ADD_SIZE;
	}

	//レーザーの当たり判定(円)
	Circle laserHit = {it->nowPos, LASER_HIT_R, {}, {}};

	//プレイヤーとレーザーの当たり判定.
	if (HitCirCir(laserHit, plyHit)) {

		//反射あり.
		switch (player->GetMode())
		{
			case PlayerMode::ItemReflect:
			{
				it->type = LaserType::Reflect; //反射モードへ.
				it->counter = 0;          //リセット.
				gameMng->SlowModeStart(); //スロー発動.
				ReflectLaser(it);         //レーザーを反射.
			}
			break;

			case PlayerMode::DashReflect:
			{
				it->type = LaserType::Reflect; //反射モードへ.
				it->counter = 0;          //リセット.
				gameMng->SlowModeStart(); //スロー発動.
				ReflectLaser(it);         //レーザーを反射.

				//ダッシュ反射演出.
				player->SetIsDashRefEffect(true); //フラグON.
				player->SpawnEffectSpark();       //スパーク演出召喚.
			}
			break;

			//反射あり(強化版)
			case PlayerMode::ItemReflectSuper:
			{
				it->type = LaserType::SuperReflect; //反射モードへ.
				it->counter = 0;               //リセット.

				gameMng->SlowModeStart(); //スロー発動.
				ReflectLaser(it);         //レーザーを反射.		
			}
			break;

			case PlayerMode::Normal:
			{
				player->Death(); //プレイヤー死亡.
			}
		    break;
		}
	}
}

//レーザー反射.
void LaserManager::ReflectLaser(list<LaserData>::iterator& it)
{
	//反射時の元の角度.
	double ang = _deg(atan2(it->vec.y, it->vec.x));
	//角度を逆方向へ(少しだけランダムでずれる)
	ang += 180 + (float)RandNum(-200, 200)/10;
	//角度反映.
	it->vec = { cos(_rad(ang)), sin(_rad(ang)) };

	//ターゲットが残っていれば.
	if (it->target) {
		it->target->SetIsTargeting(false); //ターゲットを解除.
	}

	//エフェクト.
	EffectData data{};
	data.type = EffectType::Reflect;
	data.pos  = it->nowPos;
	effectMng->SpawnEffect(&data);
	//サウンド.
	if (auto i = soundMng->Get(_T("Laser3"))) {
		i->Play(false, 58);
	}

	//チュートリアルなら指示送信.
	if (gameData->stage == StageType::Tutorial) {
		tutorialStg->SetReflectLaser(true);
	}
}

//レーザー移動.
void LaserManager::MoveLaser(list<LaserData>::iterator& it, double speed) {

	//レーザーの移動.
	it->nowPos += it->vec * speed * gameData->speedRate;
	//当たり判定の更新.
	it->hit.stPos = it->nowPos;
	it->hit.edPos = it->befPos;
}

//レーザー描画線を生成.
void LaserManager::GenerateLaserLine(list<LaserData>::iterator& it) {

	//前回描画した位置からの距離.
	const double dis = Dist(it->nowPos, it->befPos);

	//長さが一定以上あれば描画線を出す(DrawLineAAの関係上)
	if (dis >= LASER_LINE_DRAW_LEN) {
		
		LaserLineData tmp; //描画線作成.

		tmp.pos1 = it->befPos;	//描画線の座標1
		tmp.pos2 = it->nowPos;	//描画線の座標2
		tmp.type = it->type;	//レーザーのタイプに合わせる.
		tmp.counter = 0;		//経過時間.

		//落下レーザーの設定.
		if (tmp.type == LaserType::Falling) {
			//レーザーの経過時間を反映.
			//落下レーザー消滅時間が、レーザー描画線消滅時間に合わさるよう計算.
			tmp.counter = it->counter * LASER_LINE_DEL_TIME / LASER_FAL_DEL_TIME;
			//アニメーション曲線の調整.
			const double anim = AnimEase(EaseType::OutQuad, tmp.counter / LASER_LINE_DEL_TIME);
			tmp.counter *= _flt(anim);
		}

		line.push_back(tmp); //listに追加.

		//最後に描画線を出した座標を記録.
		it->befPos = it->nowPos;
	}
}

//レーザー(reflected)の隕石追尾.
void LaserManager::LaserRefTracking(list<LaserData>::iterator& it)
{
	//ターゲットがいなければ中断.
	if (!it->target) { return; }
	//目標地点に向かわないなら中断.
	if (!it->isGoTarget) { return; }

	//一定時間のみ追尾.
	if (it->counter > LASER_REF_TRACK_ST_TM &&
		it->counter < LASER_REF_TRACK_ED_TM)
	{
		//目標地点までの座標差と方角.
		const double targetAngle = atan2(
			it->target->GetPos().y - it->nowPos.y,
			it->target->GetPos().x - it->nowPos.x
		);
		//レーザーの現在の移動方向の角度.
		const double currentAngle = atan2(it->vec.y, it->vec.x);
		//角度の差分を計算.
		double angleDiff = targetAngle - currentAngle;

		//角度差分を-PI～PIの範囲に正規化.
		while (angleDiff > M_PI)
		{
			angleDiff -= 2 * M_PI;
		}
		while (angleDiff < -M_PI)
		{
			angleDiff += 2 * M_PI;
		}

		// 反射レーザーの旋回角度（通常レーザーより少し速く）.
		double maxTurn = _rad(LASER_REF_ROT_MAX) * gameData->speedRate;
		if (angleDiff > +maxTurn) angleDiff = +maxTurn;
		if (angleDiff < -maxTurn) angleDiff = -maxTurn;

		//新しい角度を計算して速度を更新
		double newAngle = currentAngle + angleDiff;

		// 方向を計算して設定.
		it->vec = { cos(newAngle), sin(newAngle) };
	}
}

//敵のレーザーが1つでも存在するかどうか.
//(未使用)
bool LaserManager::IsExistEnemyLaser(DBL_XY pos, float len) {

	//全てのレーザー.
	for (const auto& i : laser) {
		//敵のレーザーなら.
		if (i.type == LaserType::Normal   ||
			i.type == LaserType::Straight ||
			i.type == LaserType::Falling)
		{
			//消えかかってる落下レーザーは除外.
			if (i.type == LaserType::Falling &&
				i.counter > LASER_FAL_HIT_ABLE) 
			{
				continue;
			}
			//距離が範囲内ならtrueを返す.
			if (Dist(pos, i.nowPos) <= len) {
				return true; 
			}
		}
	}
	return false; //1つもない.
}

//レーザーを一括反射(未使用)
void LaserManager::LaserReflectRange(Circle cir) {
	
	//有効なレーザー.
	for (auto i = laser.begin(); i != laser.end(); i++) {
		const Circle cir2 = { i->nowPos, 1, {}, {} };
		//範囲内なら.
		if (HitCirCir(cir, cir2)) {
			ReflectLaser(i); //その場で反射.
		}
	}
}