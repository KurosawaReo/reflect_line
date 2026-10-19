/*
   - Scene_Title.cpp -
*/
#include "Scene_Title.h"

//依存関係.
#include "GameManager.h"
#include "GameData.h"
#include "BGManager.h"
#include "Obst_MeteorManager.h"
//参照.
static GameManager*   gameMng;
static GameData*      gameData;
static BGManager*     bgMng;
static MeteorManager* meteorMng;
//参照(KRライブラリ)
static InputMng*      inputMng;
static SceneMng*      sceneMng;

//初期化.
void TitleScene::Init() {

	//参照取得.
	gameMng   = ManagerInsts::Get<GameManager>();
	gameData  = ManagerInsts::Get<GameData>();
	bgMng     = ManagerInsts::Get<BGManager>();
	meteorMng = ManagerInsts::Get<MeteorManager>();
	inputMng  = ManagerInsts::Get<InputMng>();
	sceneMng  = ManagerInsts::Get<SceneMng>();
	//画像取得.
	grLogo            = GraphMng::Get("logo");
	grLogoAll         = GraphMng::Get("logo_all");
	grUiBackBestScore = GraphMng::Get("ui_back_best_score");

	timer = Timer(TimerMode::CountUp, 0);
}

//リセット.
void TitleScene::Reset() {
	isTitleAnim = false;
}

//入った瞬間.
void TitleScene::Enter() {
	timer.Start();					//タイマー開始.
	bgMng->SetBgType(BGType::Tile);	//背景を設定.
}

//抜けた瞬間.
void TitleScene::Exit() {
	timer.Reset(); //タイマーリセット.
}

//更新.
void TitleScene::Update() {

	InputData inputData; //何で操作したか取得する用.

	//特定の操作でゲーム開始.
	if (inputMng->IsPushActionTime(_T("GameNext"), &inputData) == 1) {
		//操作端末を記録.
		gameMng->SetLastInputDevice(inputData.device);
		//メニューシーンへ.
		sceneMng->SetScene(_T("Menu"));
	}
}

//描画.
void TitleScene::Draw() {

	//画面左下の操作方法表記.
	DrawStr howPlay(_T(""), { 30, WINDOW_HEI - 30 }, 0x00FFFF);
	{
		const int font = gameData->fonts.at("jp-size1").GetFont();

		//テキスト切り替え.
#if defined INPUT_CHANGE_ARCADE
		howPlay.text = _T("アーケード操作版");
#else
		howPlay.text = _T("PC操作版");
#endif
		howPlay.Draw(Anchor::LD, font);

		//枠線グラデーション.
		const DBL_XY howPlayPos  = howPlay.pos.ToDbl();
		const DBL_XY howPlaySize = GetTextSize(howPlay.text, font).ToDbl();
		const DBL_XY offset      = {0, 1};
		const int    margin      = 6;

		GradLine line;
		line.AddPoint(howPlayPos + offset + DBL_XY(-margin, margin),                                 { 0, 255, 255 });
		line.AddPoint(howPlayPos + offset + DBL_XY(howPlaySize.x + margin, margin),                  { 0, 100, 255 });
		line.AddPoint(howPlayPos + offset + DBL_XY(howPlaySize.x + margin, -howPlaySize.y - margin), { 0, 255, 255 });
		line.AddPoint(howPlayPos + offset + DBL_XY(-margin, -howPlaySize.y - margin),                { 0, 100, 255 });
		line.Draw(true);
	}

	//アニメーション切り替わりポイント.
	const float delay1 = 1;
	const float delay2 = 1.4f;
	const float delay3 = 1.6f;
	const float delay4 = 3;
	const float delay5 = 3;

	//画像の表示.
	{
		const int    logoY = WINDOW_HEI / 2 - 80;
		const DBL_XY imgSize = { 0.7, 0.7 };

		//切り替え前.
		if (timer.GetPassTime() < delay1) {
			//アニメーション値.
			double anim = Calc::AnimEase(EaseType::InOutQuad, timer.GetPassTime() / delay1);
			//ロゴ1枚目.
			DrawMode::Exe(
				DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * anim),
				[&]() {
					grLogo->DrawExtend(
						DBL_XY{ WINDOW_WID / 2, _dbl(logoY) }, imgSize, Anchor::Mid, true, true
					);
				}
			);
		}
		//切り替え後.
		else {
			//アニメーション値.
			//1枚目と2枚目の切り替えが自然になるよう、anim2は遅延ありにする.
			double anim1 = Calc::AnimEase(EaseType::InOutQuad, (timer.GetPassTime() - delay1)       / 1.8);
			double anim2 = Calc::AnimEase(EaseType::InOutQuad, (timer.GetPassTime() - delay1 - 0.4) / 1.8);
			//ロゴ1枚目.
			DrawMode::Exe(
				DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * (1 - anim2)),
				[&]() {
					grLogo->DrawExtend({ WINDOW_WID / 2, logoY - anim1 * 80 }, imgSize, Anchor::Mid, true, true);
				}
			);
			//ロゴ2枚目.
			DrawMode::Exe(
				DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * anim1),
				[&]() {
					grLogoAll->DrawExtend({ WINDOW_WID / 2, logoY - anim1 * 80 }, imgSize, Anchor::Mid, true, true);
				}
			);
		}
	}

	//best score.
	if (timer.GetPassTime() >= delay2) {

		const int drawY = WINDOW_HEI / 2 + 130;

		//アニメーション値.
		double anim1 = Calc::AnimEase(EaseType::InOutQuad, (timer.GetPassTime() - delay3) / 1.5);
		double anim2 = Calc::AnimEase(EaseType::InOutQuad, (timer.GetPassTime() - delay2) / 1.5);
		//テキスト.
		TCHAR text[256];
		_stprintf(text, _T("BEST SCORE: %d"), gameData->bestScore); //ベストスコア.
		DrawStr str(text, { WINDOW_WID / 2, drawY-2 }, COLOR_BEST_SCORE);

		//描画(スコア)
		DrawMode::Exe(
			DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * anim1),
			[&]() {
				str.Draw(Anchor::Mid, gameData->fonts.at("en-size3").GetFont()); //スコア値.
			}
		);
		//描画(画像)
		DrawMode::Exe(
			DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * anim2),
			[&]() {
				grUiBackBestScore->
					DrawExtend({ WINDOW_WID / 2, drawY + (10 + 18 * anim2) }, { 0.45, 0.4 }, Anchor::Mid, true, true);
				grUiBackBestScore->
					DrawExtend({ WINDOW_WID / 2, drawY - (10 + 18 * anim2) }, { 0.45, 0.4 }, Anchor::Mid, true, true);
			}
		);
	}
	//PUSH SPACE.
	if (timer.GetPassTime() >= delay4) {

		const int drawY = WINDOW_HEI / 2 + 310;

		//アニメーション値.
		double anim = Calc::AnimWave(WaveType::CosLoop, timer.GetPassTime() - delay4);
		//テキスト.
		DrawStr str(_T(""), { WINDOW_WID / 2, drawY }, 0xFFFFFF);

		//[操作表示] 操作によって表示するテキストを変える.
#if defined INPUT_CHANGE_ARCADE
		str.text = _T("Push Upper Left Button");
#else
		//最後の操作端末を取得.
		switch (gameMng->GetLastInputDevice())
		{
			case InputDevice::None:
				str.text = _T("Push [A Button] or [SPACE]");
				break;

			case InputDevice::Key:
				str.text = _T("Push [SPACE]");
				break;

			case InputDevice::PadXbox:
				str.text = _T("Push [A Button]");
				break;

			//キーボードとコントローラ以外の操作になるのはおかしい.
			default: assert(false); break;
		}
#endif

		//描画.
		DrawMode::Exe(
			DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * anim),
			[&]() {
				str.Draw(Anchor::Mid, gameData->fonts.at("en-size2").GetFont()); //テキスト.
			}
		);
	}
	//破片アニメーション.
	if (!isTitleAnim) {
		if (timer.GetPassTime() >= delay5) {
			//演出召喚.
			meteorMng->BreakMeteor({ 960, 850 }, false, 1.4);
			//一度きり.
			isTitleAnim = true;
		}
	}
}