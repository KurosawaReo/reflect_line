/*
   - GameData.h -
*/
#pragma once
#include "Global.h"

//ステージの種類.
enum class StageType
{
	Endless,
	Tutorial,
};

//操作方法の種類.
enum class OperateType
{

};

//ゲームデータ[継承不可]
class GameData final : public ManagerBase
{
//▼ ===== 変数 ===== ▼.
private:
	int		   scoreNoTime{};	//スコア(Time Bonus除く)

public:
	StageType  stage{};			//ステージ種類.
	int		   level{};			//レベル.
	int		   bestScore{};		//ベストスコア.

	float	   speedRate{};		//ゲーム進行速度倍率.
	float	   spawnRate{};		//障害物の出現時間割合.

	float	   counter{};		//経過時間カウンター(スローの影響を受ける)
	float      slowBufCntr{};	//スロー続行時間.

	bool       isPause{};       //ポーズ中かどうか.
	bool       isReflectMode{};	//反射モードかどうか.

	umap<string, Font> fonts;	//フォントデータ.
	
//▼ ===== 関数 ===== ▼.
public:
	//コンストラクタ.
	GameData() : ManagerBase(0) {}

	//add.
	void AddScore(int score);
	//get.
	int  GetScore()       const;
	int  GetScoreNoTime() const;

	void ResetScore();

	//未使用.
	void Init()   override;
	void Reset()  override;
	void Update() override{};
	void Draw()   override{};
};