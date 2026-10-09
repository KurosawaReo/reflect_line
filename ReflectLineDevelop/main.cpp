/*
   - main.cpp -
   プログラムの開始地点.
*/
#include "GameData.h"
#include "GameManager.h"
#include "BGManager.h"
#include "EffectManager.h"
#include "Item.h"
#include "LaserManager.h"
#include "Obst_Fireworks.h"
#include "Obst_MeteorManager.h"
#include "Obst_NormalLaser.h"
#include "Obst_Ripples.h"
#include "Obst_StraightLaser.h"
#include "Player.h"
#include "Stage_Endless.h"
#include "Stage_Tutorial.h"
#include "UIManager.h"

#if false
int main() {
#else
int WINAPI WinMain(
	_In_     HINSTANCE hinstance,
	_In_opt_ HINSTANCE hPrevinstance,
	_In_     LPSTR     lpCmdLine,
	_In_     int       nCmdShow
){
#endif

	//Managerクラス実体生成.
	//生成した順番に実行されるようになる.
	ManagerInsts::NewManager<InputMng>();
	ManagerInsts::NewManager<SoundMng>();
	ManagerInsts::NewManager<TimerMng>();

	ManagerInsts::NewManager<GameData>();
	ManagerInsts::NewManager<GameManager>();	//リソース読み込みをしてるため最初に.
	ManagerInsts::NewManager<BGManager>();		//背景.
	ManagerInsts::NewManager<EffectManager>();
	ManagerInsts::NewManager<ItemManager>();
	ManagerInsts::NewManager<LaserManager>();
	ManagerInsts::NewManager<Fireworks>();
	ManagerInsts::NewManager<MeteorManager>();
	ManagerInsts::NewManager<NormalLaser>();
	ManagerInsts::NewManager<Ripples>();
	ManagerInsts::NewManager<StraightLaser>();
	ManagerInsts::NewManager<Player>();
	ManagerInsts::NewManager<EndlessStage>();
	ManagerInsts::NewManager<TutorialStage>();
	ManagerInsts::NewManager<SceneMng>();		//シーンクラス.
	ManagerInsts::NewManager<UIManager>();

	//初期化処理.
	App::InitDx(WINDOW_WID, WINDOW_HEI, IS_WINDOW_MODE, FPS, false);
	//ループ処理.
	App::LoopDx();

	return 0;
}