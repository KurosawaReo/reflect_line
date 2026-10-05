/*
   - GameData.cpp -
*/
#include "GameData.h"

//依存関係.
#include "GameManager.h"
//参照.
static GameManager* gameMng;

//スコア加算.
void GameData::AddScore(int score) {
	scoreNoTime += score;
}

//Time Bonusを合わせたスコアを取得.
int GameData::GetScore() const {

	//ゲーム時間.
	float fixTime;
	{
		//小数第二位以下は切り捨てる(勝手に四捨五入されてしまうため)
		const float gameTime = gameMng->GetGameScene()->GetGameTime();
		fixTime = floorf(gameTime * 10.0f) / 10.0f;
	}
	//スコアを返す.
	return scoreNoTime + _int(fixTime * 10);
}

//Time Bonusを除いたスコアを取得.
int GameData::GetScoreNoTime() const {
	//スコアを返す.
	return scoreNoTime;
}

//スコアリセット.
void GameData::ResetScore() {
	scoreNoTime = 0;
}

//初期化.
void GameData::Init() {
	gameMng = ManagerInsts::Get<GameManager>();
}

//リセット.
void GameData::Reset() {

	scoreNoTime   = 0;
	level         = 1;     //最初はLv1
	speedRate     = 1.0;   //通常は100%
	spawnRate     = 1.0;   //最初は100%
	counter       = 0;
	slowBufCntr   = 0;
	isPause       = false;
	isReflectMode = false; //最初はLv1
}