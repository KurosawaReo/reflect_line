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
	//引数で実行順(order値)を入力する.
	ManagerInsts::NewManager<InputMng>(0);
	ManagerInsts::NewManager<SoundMng>(1);
	ManagerInsts::NewManager<TimerMng>(2);

	ManagerInsts::NewManager<GameData>();
	ManagerInsts::NewManager<GameManager>(3);	//リソース読み込みをしてるため最初に.
	ManagerInsts::NewManager<BGManager>(4);		//背景.
	ManagerInsts::NewManager<EffectManager>(5);
	ManagerInsts::NewManager<ItemManager>(6);
	ManagerInsts::NewManager<LaserManager>(7);
	ManagerInsts::NewManager<Fireworks>(8);
	ManagerInsts::NewManager<MeteorManager>(9);
	ManagerInsts::NewManager<NormalLaser>(10);
	ManagerInsts::NewManager<Ripples>(11);
	ManagerInsts::NewManager<StraightLaser>(12);
	ManagerInsts::NewManager<Player>(13);
	ManagerInsts::NewManager<EndlessStage>(14);
	ManagerInsts::NewManager<TutorialStage>(15);
	ManagerInsts::NewManager<SceneMng>(16);		//シーンクラス.
	ManagerInsts::NewManager<UIManager>(17);

	//初期化処理.
	App::InitDx(WINDOW_WID, WINDOW_HEI, IS_WINDOW_MODE, FPS, false);
	//ループ処理.
	App::LoopDx();

	return 0;
}