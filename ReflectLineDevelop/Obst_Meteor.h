/*
   - Obst_Meteor.h -
   
   障害物: 隕石.
*/
#pragma once
#include "Player.h"

//隕石の状態.
enum MeteorState
{
	Normal,
	Destroy, //破壊演出.
};
//隕石の形データ.
struct MeteorShape
{
	vector<float> lineDist;  //隕石の中心からの頂点の距離.
	vector<Line>  line;      //隕石を構成する線.
};

//隕石(単体)
class Meteor
{
private:
	MeteorState state{};		//隕石の状態.
	MeteorShape shape{};		//隕石の形データ.

	DBL_XY      pos{};			//中心座標.
	float       ang{};			//角度.
	DBL_XY      vel{};			//速度.

	bool        isErase{};		//消去するかどうか.
	bool        isTargeting{};	//レーザーに標的にされているか.

	float       destroyCntr{};	//破壊の度合.

public:
	//get.
	DBL_XY      GetPos()		 const { return pos; }
	MeteorState GetState()		 const { return state; }
	bool        GetIsErase()	 const { return isErase; }
	bool        GetIsTargeting() const { return isTargeting; }
	//set.
	void        SetIsTargeting(bool flag) { isTargeting = flag; }

	//その他.
	void Init();
	void Reset();
	void Update();
	void Draw();

	void Spawn();						//隕石出現処理. 
	void Destroy();						//隕石破壊.
	bool IsHitMeteor(Line   hit) const;	//隕石の当たり判定.
	bool IsHitMeteor(Circle hit) const;	//隕石の当たり判定.

	void UpdateMeteoLine();				//隕石を構成する線の更新.
};