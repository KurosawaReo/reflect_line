/*
   - KR_TimerMng.cpp - (DxLib)
*/
#include "KR_TimerMng.h"

//KrLib名前空間.
namespace KR
{
	//初期化.
	void TimerMng::Init() {
		gameTimer.Start(); //タイマー開始.
	}

	//更新.
	void TimerMng::Update() {

		//予約がない時は処理しない.
		if (functions.empty()) { return; }

		//現在時刻取得.
		const float nowTime = gameTimer.GetPassTime();
		//実行する関数.
		vector<function<void()>> exeFuncs;

		//予約リスト全ループ.
		for (auto it = functions.begin(); it != functions.end(); ) {
			//予約時間になったら.
			if (it->time <= nowTime) {
				exeFuncs.push_back(move(it->func)); //後で実行する.
				it = functions.erase(it);           //予約削除.
			}
			else {
				it++;
			}
		}
		//後で実行.
		for (auto& i : exeFuncs) {
			i();
		}
	}

	//全ての予約を中止する.
	void TimerMng::ReservCancelAll() {
		functions.clear();
	}
}