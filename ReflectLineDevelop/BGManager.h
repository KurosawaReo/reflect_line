/*
   - BGManager.h -
*/
#pragma once
#include "BG1.h"
#include "BG2.h"
//#include "BG3.h"

//背景の種類.
enum class BGType
{
	Tile,
	Space3D,
};

//背景クラス.
class BGManager final : public ManagerBase 
{
//▼ ===== 変数 ===== ▼.
private:
	vector<unique_ptr<BGBase>> bg; //背景クラス配列.

	BGType bgType{}; //どの背景を使うか.

	//画像.
	Graph* grReflectModeFrame{};

//▼ ===== 関数 ===== ▼.
public:
	//コンストラクタ.
	BGManager(){}
	//set.
	void  SetBgType(BGType type);

	void  Init()   override;
	void  Reset()  override;
	void  Update() override;
	void  Draw()   override;

	//ポーズ用.
	void  Pause();
	void  PauseEnd();

	//使用禁止.
	BGManager(const BGManager&) = delete;
	BGManager& operator=(const BGManager&) = delete;
};