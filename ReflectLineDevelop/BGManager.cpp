/*
   - BGManager.cpp -
*/
#include "BGManager.h"

//依存関係.
#include "GameManager.h"
#include "GameData.h"
//参照.
static GameManager* gameMng;
static GameData*    gameData;

// ▼*---=[ BGManager ]=---*▼ //

//初期化.
void BGManager::Init() {

	//参照取得.
	gameMng  = ManagerInsts::Get<GameManager>();
	gameData = ManagerInsts::Get<GameData>();

	//背景クラス生成.
	bg.push_back(make_unique<BG1>());
	bg.push_back(make_unique<BG2>());
	bg.push_back(make_unique<BG3>());
	//初期化.
	for (auto& i : bg) {
		i->Init();
	}
}

//リセット.
void BGManager::Reset() {

	useBgNo = 1;

	//リセット.
	for (auto& i : bg) {
		i->Reset();
	}
}

//更新.
void BGManager::Update() {

	//ポーズ中の更新はしない.
	if (gameData->isPause) { return; }

	//更新.
	if (useBgNo != 0) {
		bg[useBgNo - 1]->Update();
	}
}

//描画.
void BGManager::Draw() {

	//スローモード経過時間.
	const float refTime = gameMng->GetGameScene()->GetReflectModeTime();
	//透明度の計算.
	double modeAlpha = 0.5 - (refTime - (REFLECT_MODE_TIME - 0.5)); //最初の0.5秒.
	modeAlpha = Calc::AnimEase(EaseType::OutQuad, modeAlpha);       //0.0～1.0の範囲にする.

	//描画(通常時)
	if (modeAlpha < 1.0) {
		//背景別.
		if (useBgNo != 0) {
			bg[useBgNo - 1]->DrawNor(1-modeAlpha);
		}
	}

	//描画(反射モード)
	if (modeAlpha > 0.0) {

		double alpha3count = 1.0;

		//背景別.
		if (useBgNo != 0) {
			bg[useBgNo - 1]->DrawRef(modeAlpha);
		}
		//残り3秒になったら.
		if (refTime <= 3.0) {
			alpha3count = 0.5 + 0.5 * Calc::AnimWave(WaveType::CosLoop, refTime*4);
		}

		DrawMode::Exe(
			DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * modeAlpha * alpha3count),
			[&]() {
				//グラデーション
				GraphMng::Get(_T("reflect_mode_frame"))->Draw({ WINDOW_WID / 2, WINDOW_HEI / 2 });
				//枠線.
				Box box = { {WINDOW_WID / 2, WINDOW_HEI / 2}, { WINDOW_WID * modeAlpha, WINDOW_HEI * modeAlpha }, COLOR_PLY_REFLECT, 1.0f };
				DrawBoxKR(box, Anchor::Mid, false, true);
			}
		);
	}
}

//ポーズする.
void BGManager::Pause() {

	if (useBgNo != 0) {
		bg[useBgNo - 1]->Pause();
	}
}

//ポーズ解除.
void BGManager::PauseEnd() {

	if (useBgNo != 0) {
		bg[useBgNo - 1]->PauseEnd();
	}
}