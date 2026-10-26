/*
   - Scene_Title.cpp -
*/
#include "Scene_Title.h"

//依存関係.
#include "../GameManager.h"
#include "../GameData.h"
#include "../Configs/Config_Title.h"
#include "../Obstacles/Obst_MeteorManager.h"
#include "../BGManager.h"
//参照.
static GameManager*   gameMng;
static GameData*      gameData;
static BGManager*     bgMng;
static MeteorManager* meteorMng;
//参照(KRライブラリ)
static InputMng*      inputMng;
static SceneMng*      sceneMng;

//startから長さdurの間に0→1へ進む値(範囲外は0か1に収まる).
static double Ease(double now, double start, double dur) {
	return Calc::AnimEase(EaseType::InOutQuad, (now - start) / dur);
}

//枠付きラベルを描画.
//(次のラベルの位置計算用に文字サイズを返す)
static DBL_XY DrawFrameLabel(const MY_STRING& text, INT_XY pos, int font) {
	using namespace Config::TitleLayout;

	//文字.
	DrawStr str(text, pos, Config::TitleColor::LABEL_TEXT);
	str.Draw(Anchor::LD, font);
	const DBL_XY size = GetTextSize(text, font).ToDbl();

	//枠線グラデーション(左下基準の4頂点).
	const DBL_XY p = pos.ToDbl() + LABEL_FRAME_OFFSET;
	const double m = LABEL_MARGIN;

	GradLine line;
	line.AddPoint(p + DBL_XY(-m,         +m),          Config::TitleColor::LABEL_GRAD_B);
	line.AddPoint(p + DBL_XY(size.x + m, +m),          Config::TitleColor::LABEL_GRAD_B);
	line.AddPoint(p + DBL_XY(size.x + m, -size.y - m), Config::TitleColor::LABEL_GRAD_A);
	line.AddPoint(p + DBL_XY(-m,         -size.y - m), Config::TitleColor::LABEL_GRAD_A);
	line.Draw(true);

	return size;
}

//操作版の表示テキスト(ラベル用).
static MY_STRING GetInputModeLabel() {
#if defined INPUT_CHANGE_ARCADE
	return _T("アーケード操作版");
#else
	return _T("PC操作版");
#endif
}

//Pushテキスト(操作端末ごとに変える).
static MY_STRING GetPushText() {
#if defined INPUT_CHANGE_ARCADE
	return _T("Push Upper Left Button");
#else
	//最後の操作端末を取得.
	switch (gameMng->GetLastInputDevice())
	{
		case InputDevice::Key:     return _T("Push [SPACE]");
		case InputDevice::PadXbox: return _T("Push [A Button]");

		//まだ操作していない場合や想定外の端末は、両方の案内を出す.
		default:                   return _T("Push [A Button] or [SPACE]");
	}
#endif
}

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
	//フォント取得.
	fontSmall = gameData->fonts.at("jp-size1").GetFont();
	fontScore = gameData->fonts.at("en-size3").GetFont();
	fontPush  = gameData->fonts.at("en-size2").GetFont();

	timer = Timer(TimerMode::CountUp, 0);
}

//リセット.
void TitleScene::Reset() {
	isBreakDone = false;
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

	InputBinding inputData; //何で操作したか取得する用.

	//特定の操作でゲーム開始.
	if (inputMng->IsPushActionTime(_T("GameNext"), &inputData) == 1) {
		//操作端末を記録.
		gameMng->SetLastInputDevice(inputData.device);
		//メニューシーンへ.
		sceneMng->SetScene(_T("Menu"));
	}

	//破片演出(一度だけ).
	if (!isBreakDone && timer.GetPassTime() >= Config::TitleTime::BREAK_START) {
		meteorMng->BreakMeteor(Config::TitleLayout::BREAK_POS, false, Config::TitleLayout::BREAK_SCALE);
		isBreakDone = true;
	}
}

//描画.
void TitleScene::Draw() {
	DrawLabels();     //左下のラベル.
	DrawLogo();       //ロゴ.
	DrawBestScore();  //ベストスコア.
	DrawPushText();   //Pushテキスト.
}

//[描画] 左下のラベル2つ.
void TitleScene::DrawLabels() {
	using namespace Config::TitleLayout;

	//バージョン.
	const DBL_XY verSize = DrawFrameLabel(Config::GAME_VERSION, LABEL_POS, fontSmall);
	//操作版(バージョンの右隣).
	DrawFrameLabel(GetInputModeLabel(), { _int(verSize.x + LABEL_NEXT_OFFSET), LABEL_POS.y }, fontSmall);
}

//[描画] タイトルロゴ.
void TitleScene::DrawLogo() {
	using namespace Config::TitleTime;
	using namespace Config::TitleLayout;

	const double t = timer.GetPassTime();

	//進行率(それぞれ0→1).
	const double fadeIn  = Ease(t, 0,                                    LOGO_FADEIN_TIME); //1枚目が出る.
	const double rise    = Ease(t, LOGO_SWAP_START,                      LOGO_SWAP_TIME);   //上へ動く.
	const double fadeOut = Ease(t, LOGO_SWAP_START + LOGO_FADEOUT_DELAY, LOGO_SWAP_TIME);   //1枚目が消える.

	//2枚のロゴで共通の位置.
	const DBL_XY pos = { CENTER_X, LOGO_Y - rise * LOGO_RISE };

	//1枚目.
	DrawMode::Exe(
		DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * fadeIn * (1 - fadeOut)),
		[&]() { grLogo->DrawExtend(pos, LOGO_SCALE, Anchor::Mid, true, true); }
	);
	//2枚目.
	DrawMode::Exe(
		DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * rise),
		[&]() { grLogoAll->DrawExtend(pos, LOGO_SCALE, Anchor::Mid, true, true); }
	);
}

//[描画] ベストスコア.
void TitleScene::DrawBestScore() {
	using namespace Config::TitleTime;
	using namespace Config::TitleLayout;

	const double t = timer.GetPassTime();

	//枠が出る前は何も描かない.
	if (t < SCORE_FRAME_START) { return; }

	//進行率(それぞれ0→1).
	const double frameAnim = Ease(t, SCORE_FRAME_START, SCORE_ANIM_TIME); //枠.
	const double textAnim  = Ease(t, SCORE_TEXT_START,  SCORE_ANIM_TIME); //文字.

	//テキスト.
	const MY_STRING text = Format::StrFormat(_T("BEST SCORE: %d"), gameData->bestScore);
	DrawStr str(text, { CENTER_X, SCORE_Y + SCORE_TEXT_OFFSET_Y }, Config::COLOR_BEST_SCORE);

	//描画(スコア)
	DrawMode::Exe(
		DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * textAnim),
		[&]() { str.Draw(Anchor::Mid, fontScore); }
	);
	//描画(線)
	const double gap = SCORE_FRAME_GAP + SCORE_FRAME_OPEN * frameAnim; //文字からの距離.
	DrawMode::Exe(
		DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * frameAnim),
		[&]() {
			grUiBackBestScore->DrawExtend({ CENTER_X, SCORE_Y + gap }, SCORE_FRAME_SCALE, Anchor::Mid, true, true);
			grUiBackBestScore->DrawExtend({ CENTER_X, SCORE_Y - gap }, SCORE_FRAME_SCALE, Anchor::Mid, true, true);
		}
	);
}

//[描画] Pushテキスト.
void TitleScene::DrawPushText() {
	using namespace Config::TitleTime;
	using namespace Config::TitleLayout;

	const double t = timer.GetPassTime();

	//表示前は処理中断.
	if (t < PUSH_START) { return; }

	//点滅(0→1の波).
	const double blink = Calc::AnimWave(WaveType::CosLoop, t - PUSH_START);

	//テキスト描画.
	DrawStr str(GetPushText(), { CENTER_X, PUSH_Y }, Config::TitleColor::PUSH_TEXT);
	DrawMode::Exe(
		DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * blink),
		[&]() { str.Draw(Anchor::Mid, fontPush); }
	);
}
