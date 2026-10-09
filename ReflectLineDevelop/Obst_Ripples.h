/*
   - Ripples.h -

   障害物: 波紋.
*/
#pragma once

//波紋データ.
struct RipplesData
{
	double x, y;       // エフェクト位置.
	int    baseSize;   // 基本サイズ
	bool   alreadyHit; 
	float  counter;    // 経過時間.
	float  duration;   // エフェクトの持続時間.
};

//波紋管理.
class Ripples final : public ManagerBase
{
//▼ ===== 変数 ===== ▼.
private:
	vector<RipplesData> ripples; //波紋配列.

	float flashTimer{}; //出現タイマー.

//▼ ===== 関数 ===== ▼.
public:
	//コンストラクタ.
	Ripples(){}

	void Init()   override;
	void Reset()  override;
	void Update() override;
	void Draw()   override;

	void Hitjudgment();
	void UpdateFlashGeneration();
	void DrawObstFlash();
	void SpawnRipples(double x, double y);

	void GenerateRandomPosition(double& x, double& y);
	bool CheckDistance(double x, double y);
	void SpawnObstaclegroup();

	int  GetEffectState   (vector<RipplesData>::iterator it);
	void DrawWarningEffect(vector<RipplesData>::iterator it);
	void DrawActiveEffect (vector<RipplesData>::iterator it);

	//使用禁止.
	Ripples(const Ripples&) = delete;
	Ripples& operator=(const Ripples&) = delete;
};