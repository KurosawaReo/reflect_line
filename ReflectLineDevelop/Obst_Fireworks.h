/*
   - Obst_Fireworks.h -

   障害物: 花火.
*/
#pragma once

//花火(Fireworks)データ.
struct FwData 
{
	float x, y;                // 座標
	float targetX, targetY;    // 目標座標（爆発位置）
	float vx, vy;              // 速度
	float counter;             // カウンタ
	int   sparkCount;          // 火花数

	int   fallDelay;           // 落下までの遅延
	bool  hasFallen;           // 落下済みか
};

//花火管理.
class Fireworks final : public ManagerBase
{
//▼データ.
private:
	list<FwData> fireworks;
	float		 spawnTimer{};

//▼ ===== 関数 ===== ▼.
private:
	void GenerateRandomPosition   (float& x, float& y);
	bool CheckDistance            (float x, float y);
	void SpawnFireworks           (float x, float y);
	void UpdateFireworksGeneration();
	void UpdateIndividualFireworks();
	void CreateFireworksSparks    (float x, float y);

	//描画用.
	void DrawWarningEffect(list<FwData>::iterator it);

public:
	//コンストラクタ
	Fireworks(int order) : ManagerBase(order) {}

	void Init()   override;
	void Reset()  override;
	void Update() override;
	void Draw()   override;

	//使用禁止.
	Fireworks(const Fireworks*) = delete;
	Fireworks* operator=(const Fireworks*) = delete;
};