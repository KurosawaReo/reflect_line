/*
   - GameManager.h -
   ゲーム全体管理.
*/
#pragma once
#include "Scenes/Scene_Title.h"
#include "Scenes/Scene_Menu.h"
#include "Scenes/Scene_Game.h"
#include "Scenes/Scene_End.h"
#include "Obstacles/Obst_NormalLaser.h"
#include "Obstacles/Obst_NormalLaser.h"
#include "Obstacles/Obst_StraightLaser.h"

//ゲームマネージャー.
class GameManager final : public ManagerBase
{
//▼ ===== 変数 ===== ▼.
private:
	MY_STRING    gameSceneBgm{};	//ゲームシーンで使用するBGM名.

	InputDevice  lastInputDevice{};	//最後の入力端末.

	/* シーン関係 */
	TitleScene   titleScene{};
	GameScene    gameScene{};
	MenuScene    menuScene{};
	EndScene     endScene{};

#if defined DEBUG_SHOW_FPS
	TimerMicro tmFps{};	//fps計測用タイマー.
#endif

//▼ ===== 関数 ===== ▼.
private:
	//ポーズ画面.
	void DrawPause();

public:
	//コンストラクタ.
	GameManager(){}
	//set.
	void		SetLastInputDevice(InputDevice type) { lastInputDevice = type; }
	//get.
	GameScene*	GetGameScene()		       { return &gameScene;    }
	MY_STRING	GetGameSceneBgm()	 const { return gameSceneBgm;  }
	InputDevice	GetLastInputDevice() const { return lastInputDevice; }

	//基本処理.
	void Init()   override;
	void Reset()  override;
	void Update() override;
	void Draw()   override;

	//ポーズ.
	void GamePause();
	void GamePauseEnd();

	//その他.
	void GameOver();
	void ItemUsed();
	void SlowModeStart();

	void StopObjects();
	void DrawOnlyObjects();
	void RestartObjects();

	void WritePlayLog(bool isTutorial);

	//使用禁止.
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;
};