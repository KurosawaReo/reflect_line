/*
   - BGManager.h -
*/
#pragma once
#include "BG1.h"
#include "BG2.h"
#include "BG3.h"

//背景クラス.
class BGManager final : public ManagerBase 
{
//▼ ===== 変数 ===== ▼.
private:
	vector<unique_ptr<BGBase>> bg; //背景クラス配列.

	int useBgNo{}; //何番目の背景を使うか.

//▼ ===== 関数 ===== ▼.
public:
	//コンストラクタ.
	BGManager(int order) : ManagerBase(order) {}

	//set.
	void  SetBgNo(int _no) { useBgNo = _no; }

	void  Init()   override;
	void  Reset()  override;
	void  Update() override;
	void  Draw()   override;

	//ポーズ用.
	void  Pause();
	void  PauseEnd();

	//使用禁止.
	BGManager(const BGManager*) = delete;
	BGManager* operator=(const BGManager*) = delete;
};