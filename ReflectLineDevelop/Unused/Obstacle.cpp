/*
   - Obstacle.cpp -
   障害物管理.
*/
#if false
#include "Player.h"
#include "Obstacle.h"

void Obstacle::Init(GameData* _data, Player* _player) {
	//実体のアドレスをもらう.
	data   = _data;
	player = _player;
}

void Obstacle::Reset(DBL_XY _pos, float _ang) {
	line.stPos = _pos;
	ang        = _ang;
}

void Obstacle::Update() {

	//回転速度.
	ang += speed * (float)((data->isSlow) ? SLOW_MODE_SPEED : 1);
	//線の回転.
	line.edPos = CalcArcPos(line.stPos, ang, len);

	//当たったら死亡.
	if (HitCheckLine(&line, player->GetHit())) {
		player->PlayerDeath();
	}
}

void Obstacle::Draw() {

	//デバッグ表示.
	if (CheckHitKey(KEY_INPUT_0)) {
		
		DBL_XY mdPos = CalcMidPos(line.stPos, line.edPos);
		DrawCircle(_int_r(mdPos.x), _int_r(mdPos.y), _int_r(len/2), 0x303030);

		Line debugLine1 = {line.stPos, CalcArcPos(line.stPos, ang, 1000),       0x303030};
		Line debugLine2 = {line.stPos, CalcArcPos(line.stPos, ang + 180, 1000), 0x303030};
		DrawLineST(&debugLine1);
		DrawLineST(&debugLine2);

		
		
		(0, 455, _T("[Debug] 線の当たり判定を表示"), 0xFFFFFF);
	}

	//線の描画.
	DrawLineST(&line);
}
#endif