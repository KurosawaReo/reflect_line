/*
   - Obstacle2.h -
   障害物管理.
*/
#if false
#pragma once
#include "Player.h"

#define MAX_M 100 //ミサイルが飛ぶ最大距離.
//#define PI 3.14159//円周率

class Obstacle2
{
private:
	float  hsc{};                      //エネミーの出る間隔.
	int    Mg{};                       //エネミーの画像.
	double Mx[MAX_M]{}, My[MAX_M]{};   //ミサイルのグラフィック.
	double Mv[MAX_M]{};                //ミサイルデータの使用状態(1:使用中,0:未使用)
	double Ma[MAX_M]{};                //ミサイルの角度.
	int    Mc[MAX_M]{};                //ミサイルの追尾カウンタ-

	Image img{};

	GameData* data{};
	Player* player{};

public:
	void Init(GameData*, Player*);
	void Reset();
	void Update();
	void Draw();
	void enemyMove();
};
#endif