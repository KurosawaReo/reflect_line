/*
   - Obst_MeteorManager.h -

   障害物: 隕石出現管理.
*/
#pragma once
#include "Obst_Meteor.h"

//隕石管理.
class MeteorManager final : public ManagerBase
{
//▼ ===== 変数 ===== ▼.
private:
	list<Meteor> meteors;  //隕石配列.
	float		 timer{}; //隕石生成用.

//▼ ===== 関数 ===== ▼.
public:
	//コンストラクタ.
	MeteorManager(int order) : ManagerBase(order) {}

	//get.
	Meteor* GetHitMeteor   (Line   hit, bool isDestroy);	//範囲内の隕石を取得(1つ)
	Meteor* GetHitMeteor   (Circle hit, bool isDestroy);	//範囲内の隕石を取得(1つ)
	Meteor* GetTargetMeteor(DBL_XY pos);					//目標となる隕石を選ぶ.

	void Init()   override;
	void Reset()  override;
	void Update() override;
	void Draw()   override;

	void SpawnMeteor();                                           //隕石生成.
	void BreakMeteor(DBL_XY pos, bool isScore, double scale = 1); //隕石破壊演出.

	//使用禁止.
	MeteorManager(const MeteorManager*) = delete;
	MeteorManager* operator=(const MeteorManager*) = delete;
};