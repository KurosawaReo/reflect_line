/*
   - main.cpp -
   プログラムの開始地点.
*/
#include "Programs/GameData.h"
#include "Programs/GameManager.h"
#include "Programs/BGManager.h"
#include "Programs/EffectManager.h"
#include "Programs/ItemManager.h"
#include "Programs/LaserManager.h"
#include "Programs/Obst_Fireworks.h"
#include "Programs/Obst_MeteorManager.h"
#include "Programs/Obst_NormalLaser.h"
#include "Programs/Obst_Ripples.h"
#include "Programs/Obst_StraightLaser.h"
#include "Programs/Player.h"
#include "Programs/Stage_Endless.h"
#include "Programs/Stage_Tutorial.h"
#include "Programs/UIManager.h"

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

	/*
	   [管理クラス生成]
	   生成した順にInitやUpdateなどが自動で実行される.
	*/
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
	App::InitDx(Config::WINDOW_WID, Config::WINDOW_HEI, Config::IS_WINDOW_MODE, Config::FPS, false);
	//ループ処理.
	App::LoopDx();

	return 0;
}