/*
   - Obst_Meteor.cpp -

   障害物: 隕石.
*/
#include "Obst_Meteor.h"

//依存関係.
#include "GameData.h"
//参照.
static GameData* gameData;

void Meteor::Init() {
	//取得.
	gameData = ManagerInsts::Get<GameData>();
	//隕石出現処理.
	Spawn();
}

void Meteor::Reset() {

}

void Meteor::Update() {

	//移動.
	pos.x += vel.x * METEOR_SPEED * gameData->speedRate;
	pos.y += vel.y * METEOR_SPEED * gameData->speedRate;
	//回転.
	ang += gameData->speedRate;

	//状態別処理.
	switch (state)
	{
		case MeteorState::Normal:
			//画面外で消去.
			if (Calc::IsOutInArea(pos, { METEOR_LINE_DIS_MAX*2, METEOR_LINE_DIS_MAX*2 }, {0, 0, WINDOW_WID, WINDOW_HEI}, true)){
				isErase = true; //消去する.
			}
			break;

		case MeteorState::Destroy:
			//破壊量の度合.
			destroyCntr += gameData->speedRate;
			//時間が終了したら.
			if (destroyCntr >= METEOR_DEST_TIME) {
				state   = MeteorState::Normal; //元に戻す.
				isErase = true;          //消去する.
			}
			break;

		default: assert(FALSE); break;
	}
	//隕石構成線の更新.
	UpdateMeteoLine();
}

void Meteor::Draw() {
	
	int alpha = 255; //透明度.

	//破壊中はだんだん薄くする.
	if (state == MeteorState::Destroy) {
		alpha = _int_r(255 * (1-destroyCntr/METEOR_DEST_TIME)); //少しずつ減少(255→0)
	}

	//描画.
	DrawMode::Exe(
		DrawModeID::None, DrawBlendModeID::Alpha, alpha,
		[&](){

			//全ての描画線.
			for (auto& i : shape.line) {
				i.color = COLOR_METEOR(pos);
				DrawLineKR(i, true);
			}

			//チュートリアル限定.
			if (gameData->stage == StageType::Tutorial) {
				//ターゲットされてなければ.
				if (!isTargeting) {
					DrawStr str(_T("隕石"), pos.ToInt(), COLOR_METEOR(pos));
					str.Draw(Anchor::Mid, gameData->fonts["jp-size1"].GetFont());
				}
			}
		}
	);

	//ターゲットマーク.
	if (isTargeting) {
		const double size = 0.34;
		GraphMng::Get(_T("target"))->DrawExtend(pos, { size, size }, Anchor::Mid, true, true);
	}
}

//隕石出現処理.
void Meteor::Spawn() {

	int rnd1 = Calc::RandNum(0, 99);
	int rnd2 = Calc::RandNum(0, 99);

	INT_XY goalPos{}; //目標座標.

	//50%:上下端から出現.
	if (rnd1 < 50) {
		pos.x = Calc::RandNum(0, WINDOW_WID);                                             //xの設定.
		pos.y = (rnd2 < 50) ? 0 - METEOR_LINE_DIS_MAX : WINDOW_HEI + METEOR_LINE_DIS_MAX; //yの設定.
	}
	//50%:左右端から出現.
	else {
		pos.x = (rnd2 < 50) ? 0 - METEOR_LINE_DIS_MAX : WINDOW_WID + METEOR_LINE_DIS_MAX; //xの設定.
		pos.y = Calc::RandNum(0, WINDOW_HEI);                                             //yの設定.
	}

	//目標地点の抽選.
	{
		goalPos.x = Calc::RandNum(WINDOW_WID/2 - METEOR_GOAL_RAND_RANGE, WINDOW_WID/2 + METEOR_GOAL_RAND_RANGE);
		goalPos.y = Calc::RandNum(WINDOW_HEI/2 - METEOR_GOAL_RAND_RANGE, WINDOW_HEI/2 + METEOR_GOAL_RAND_RANGE);
		//目標地点までの角度を求める.
		double ang = (goalPos.ToDbl() - pos).Angle();
		//xとyのvectorに分解.
		vel = Calc::AngToVector(ang);
	}

	//隕石の設定.
	{
		//①何角形にするか抽選.
		const int lineCnt = Calc::RandNum(METEOR_LINE_CNT_MIN, METEOR_LINE_CNT_MAX);
		shape.line.    resize(lineCnt);
		shape.lineDist.resize(lineCnt);

		//②頂点の位置を抽選.
		for (auto& i : shape.lineDist) {
			//小数第1位まで抽選.
			i = _flt(
				Calc::RandNum(METEOR_LINE_DIS_MIN*10, METEOR_LINE_DIS_MAX*10)/10
			);
		}
		
		//線の設定.
		for (auto& i : shape.line) {
			i.thick = 3.0f;
		}
	}
}

//隕石破壊.
void Meteor::Destroy() {
	state = MeteorState::Destroy; //破壊モードに.
	destroyCntr = 0;        //0から開始.
}

//隕石の当たり判定(線と線)
bool Meteor::IsHitMeteor(Line hit) const {

	//破壊されてない隕石なら.
	if (state == MeteorState::Normal) {
		//全ての線で判定.
		for (const auto& i : shape.line) {
			if (Calc::HitLineLine(i, hit)) {
				return true; //当たった.
			}
		}
	}

	return false; //当たっていない.
}

//隕石の当たり判定(線と円)
bool Meteor::IsHitMeteor(Circle hit) const {

	//破壊されてない隕石なら.
	if (state == MeteorState::Normal) {
		//全ての線で判定.
		for (const auto& i : shape.line) {
			if (Calc::HitLineCir(i, hit)) {
				return true; //当たった.
			}
		}
	}

	return false; //当たっていない.
}

//隕石を構成する線の更新.
void Meteor::UpdateMeteoLine() {

	//何度ずつずれるか.
	float rot = (float)360/shape.line.size(); //360度÷描く線の数.

	//回転しながら始点と終点を設定していく.
	for (int i = 0; i < shape.line.size(); i++) {

		//要素数が0未満なら最大値へ移動する.
		int bef = ((i-1) < 0) ? (_int(shape.line.size())-1) : (i-1);

		shape.line[i].stPos = Calc::ArcPos(pos, ang+  i*rot, shape.lineDist[i]);   //始点: 現在の角度から計算.
		shape.line[i].edPos = Calc::ArcPos(pos, ang+bef*rot, shape.lineDist[bef]); //終点: 1つ前の角度から計算.

		//破壊時の回転アニメーション.
		if (state == MeteorState::Destroy) {

			//①隕石を構成する線の情報.
			DBL_XY lineMidPos   = Calc::MidPos   (shape.line[i].stPos, shape.line[i].edPos); //中点の位置.
			double lineLen      = Calc::Dist     (shape.line[i].stPos, lineMidPos);			 //長さの半分.
			double lineAng      = Calc::FacingAng(lineMidPos, shape.line[i].stPos);			 //角度.
			//②隕石の中央からどんどん離していく.
			double pivotDis     = Calc::Dist     (pos, lineMidPos);							 //隕石の中央からの距離.
			double pivotAng     = Calc::FacingAng(pos, lineMidPos);							 //隕石の中央から見た角度.
			DBL_XY newPos       = Calc::ArcPos   (pos, pivotAng, pivotDis+destroyCntr);		 //距離を増やす.
			//③新たな線の始点と終点.
			shape.line[i].stPos = Calc::ArcPos   (newPos, lineAng    +destroyCntr, lineLen);
			shape.line[i].edPos = Calc::ArcPos   (newPos, lineAng+180+destroyCntr, lineLen);
		}
	}
}