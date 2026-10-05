/*
   - EffectManager.h -
   エフェクトを管理するクラス.
*/
#pragma once

//エフェクトタイプ.
enum class EffectType
{
	Score100,
	Score500,
	PlayerDeath,		//プレイヤー死亡.
	PlayerDashReflect,	//反射ダッシュ.
	Reflect,			//反射.
	MeteorCrash,		//隕石破壊.
	MeteorFragment,		//隕石破片.

	Endless_Level1,		//耐久モード: Level1演出.
	Endless_Level2,		//耐久モード: Level2演出.
	Endless_Level3,		//耐久モード: Level3演出.
	Endless_Level4,		//耐久モード: Level4演出.
	Endless_Level5,		//耐久モード: Level5演出.
	Tutorial_Step1,		//チュートリアル: step1演出.
	Tutorial_Step2,		//チュートリアル: step2演出.
	Tutorial_Step3,		//チュートリアル: step3演出.
	Tutorial_Step4,		//チュートリアル: step4演出.
};

//エフェクトデータ.
struct EffectData
{
	EffectType type{};

	DBL_XY     pos{};     //座標.
	DBL_XY     vec{};     //方向.
	float      speed{};   //速さ.
	float      ang{};     //角度.
	float      len{};     //長さ.
	float      counter{}; //時間計測用.
};

//エフェクト管理クラス.
class EffectManager final : public ManagerBase
{
//▼ ===== 変数 ===== ▼.
private:
	list<EffectData> effect; //エフェクト配列.

//▼ ===== 関数 ===== ▼.
public:
	//コンストラクタ.
	EffectManager(int order) : ManagerBase(order) {}

	void Init()	  override;
	void Reset()  override;
	void Update() override;
	void Draw()	  override;

	void SpawnEffect (const EffectData* data); //エフェクト出現.

	//使用禁止.
	EffectManager(const EffectManager*) = delete;
	EffectManager* operator=(const EffectManager*) = delete;
};
