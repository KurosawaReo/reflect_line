/*
   - KR_TimerMng.h - (DxLib)
   ver.2026/07/02

   時間管理機能。[試作品]
*/
#pragma once
//[include] KR_Global.
#if !defined DEF_KR_DX_GLOBAL
  #include "KR_Global.h"
#endif
//[include] ".h"ファイルで使うもの.
#include "KR_ManagerBase.h"

//KrLib名前空間.
namespace KR
{
	//予約実行関数.
	struct ReservFunc
	{
		function<void()> func{}; //関数ポインタ.
		float            time{}; //何秒後に実行するか.
	};

	class TimerMng : public ManagerBase
	{
	private:
		vector<ReservFunc> functions{}; //予約リスト.
		Timer              gameTimer{}; //計測タイマー.

	public:
		//コンストラクタ.
		TimerMng(int order) : ManagerBase(order) {}

		void Init()   override;
		void Reset()  override {}
		void Update() override;
		void Draw()   override {}

		//予約実行する.
		template<std::invocable Func>
		void ReservExe(float timer, Func&& func) {

			//不正な時間は中断.
			if (timer < 0) { return; }

			//予約設定.
			ReservFunc newFunc;
			newFunc.func = func;
			newFunc.time = gameTimer.GetPassTime() + timer; //現在時刻 + 何秒後か.
			//予約リストに追加.
			functions.push_back(newFunc);
		}
		//全ての予約を中止する.
		void ReservCancelAll();
	};
}