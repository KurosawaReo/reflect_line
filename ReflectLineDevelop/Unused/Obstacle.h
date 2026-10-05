/*
   - Obstacle.h -
   障害物管理.
*/
#pragma once

#if false
class Obstacle
{
private:
	Line  line{};
	float ang{}; //角度.
	float len;   //長さ.
	float speed; //速さ.

	GameData* data  {};
	Player*   player{};

public:
	//コンストラクタ.
	Obstacle(float _len, float _speed, UINT _color) {
		len        = _len;
		speed      = _speed;
		line.color = _color;
	}

	void Init  (GameData*, Player*);
	void Reset (DBL_XY _pos, float _ang);
	void Update();
	void Draw  ();
};
#endif