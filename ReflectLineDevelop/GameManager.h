/*
   - GameManager.h -
   ゲーム全体管理.
*/
#pragma once
#include "Scene_Title.h"
#include "Scene_Menu.h"
#include "Scene_Game.h"
#include "Scene_End.h"
#include "Obst_NormalLaser.h"
#include "Obst_NormalLaser.h"
#include "Obst_StraightLaser.h"

//ゲームマネージャー.
class GameManager final : public ManagerBase
{
//▼ ===== 変数 ===== ▼.
private:
	MY_STRING gameSceneBgm{}; //ゲームシーンで使用するBGM名.

	/* シーン関係 */
	TitleScene titleScene{};
	GameScene  gameScene{};
	MenuScene  menuScene{};
	EndScene   endScene{};

#if defined DEBUG_SHOW_FPS
	TimerMicro tmFps{};	//fps計測用タイマー.
#endif

//▼ ===== 関数 ===== ▼.
public:
	//コンストラクタ.
	GameManager(int order) : ManagerBase(order) {}

	//get.
	GameScene* GetGameScene()    { return &gameScene; }
	MY_STRING  GetGameSceneBgm() { return gameSceneBgm; }

	//メイン処理.
	void Init()   override;
	void Reset()  override;
	void Update() override;
	void Draw()   override;

	//ポーズ画面.
	void GamePause();
	void GamePauseEnd();
	void DrawPause();

	//その他.
	void GameOver();
	void ItemUsed();
	void SlowModeStart();

	void StopObjects();
	void DrawOnlyObjects();
	void RestartObjects();

	void WritePlayLog(bool isTutorial);

	//使用禁止.
	GameManager(const GameManager*) = delete;
	GameManager* operator=(const GameManager*) = delete;
};