/*
   - LaserManager.h -
   レーザー管理クラス.
*/
#pragma once
#include "Obst_Meteor.h"

//レーザータイプ.
enum class LaserType
{
	Normal,       //通常レーザー.
	Straight,     //直線レーザー.
	Reflect,      //反射レーザー.
	SuperReflect, //反射レーザー強化版.
	Falling,      //落下レーザー(花火用)
};

//レーザー本体.
struct LaserData
{
	LaserType type;       //レーザータイプ.

	Line      hit;        //当たり判定用.

	DBL_XY    nowPos;     //現在の座標.
	DBL_XY    befPos;     //前回描画した時にいた座標.
	DBL_XY    vec;        //進行方向ベクトル.

	Meteor*   target;     //目標隕石.
	bool      isGoTarget; //目標地点に向かって進むか.

	int       logNum;     //記録した軌跡の数.
	float     counter;    //経過時間.
};

//レーザーが描く描画線.
struct LaserLineData
{
	LaserType type;         //レーザータイプ.

	DBL_XY pos1, pos2;		//描くラインの座標.
	float  counter;         //描くラインの色決定用値.
};

//レーザー管理用.
class LaserManager final : public ManagerBase
{
//▼ ===== 変数 ===== ▼.
private:
	//listで適宜サイズを増減する.
	list<LaserData>     laser; //レーザー.
	list<LaserLineData> line;  //レーザー描画線.

	DBL_XY plyPos{}; //プレイヤー座標保管用.

//▼ ===== 関数 ===== ▼.
public:
	//コンストラクタ.
	LaserManager(int order) : ManagerBase(order) {}

	void Init()   override;
	void Reset()  override;
	void Update() override;
	void Draw()   override;

	void UpdateLaser();													//各レーザーの更新.
	void UpdateLaserLine();												//各レーザー描画線の更新.
	
	void SpawnLaser       (DBL_XY pos, DBL_XY vel, LaserType type);		//召喚.
	void NextLaser        (list<LaserData>::iterator&, bool isErase);	//次のレーザーへ.
	void HitLaser		  (list<LaserData>::iterator&);					//当たり判定.
	void ReflectLaser     (list<LaserData>::iterator&);					//反射.
	void MoveLaser		  (list<LaserData>::iterator&, double speed);	//レーザー移動.

	void GenerateLaserLine(list<LaserData>::iterator&);					//レーザー描画線を生成.
	void LaserRefTracking (list<LaserData>::iterator&);					//反射レーザーの隕石追尾.

	bool IsExistEnemyLaser(DBL_XY pos, float len);						//敵のレーザーが1つでも存在するかどうか.

	//未使用.
	void LaserReflectRange(Circle cir);									//レーザーを一括反射.

	//使用禁止.
	LaserManager(const LaserManager&) = delete;
	LaserManager& operator=(const LaserManager&) = delete;
};