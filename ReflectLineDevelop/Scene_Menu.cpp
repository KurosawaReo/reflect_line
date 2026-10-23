/*
   - Scene_Menu.cpp -
*/
#include "Scene_Menu.h"

//依存関係.
#include "Configs/Config_Menu.h"
#include "BGManager.h"
#include "GameData.h"
#include "GameManager.h"
//参照.
static GameData*    gameData;
static GameManager* gameMng;
static BGManager*   bgMng;
//参照(KRライブラリ)
static SoundMng*    soundMng;
static InputMng*    inputMng;
static SceneMng*    sceneMng;

// ▼*---=[ MenuCursor ]=---*▼ //

//set.
void MenuCursor::SetCursorRotSpeed(double speed) {
	cursorRotSpeed = speed;
}

//リセット.
void MenuCursor::Reset() {
	cursorIdx = 0;
	Update(0);
}

//更新.
void MenuCursor::Update(double counter) {

	//回転角度を速度に応じて進める.
	cursorRotAngle += cursorRotSpeed * MenuMotion::cursorRotBase * M_PI;
	//回転速度を標準速度(1.0)へ滑らかに戻す.
	cursorRotSpeed += (1.0 - cursorRotSpeed) * MenuMotion::cursorRotReturn;
	//カーソル裏表値.
	cursorWave = sin(cursorRotAngle);
	//装飾線の明滅.
	linePulse = sin(counter / 100.0 * M_PI);

	//タイトル両脇の矢印演出.
	const double delay[2] = { 0, 20 };

	for (int i = 0; i < 2; i++) {
		//進行率.
		const double rate = Calc::AnimLoopRate(counter - delay[i], 120);

		//移動量と透明度.
		arrow[i].move = Calc::AnimEase(EaseType::OutInQuad, rate);
		arrow[i].alpha = sin(rate * M_PI);
	}
}

// ▼*---=[ MenuScene ]=---*▼ //

//再生する動画を取得.
Graph* MenuScene::GetPlayMovie() {
	return grMenuMovie[mCursor.cursorIdx];
}

//初期化.
void MenuScene::Init() {

	//参照取得.
	gameData = ManagerInsts::Get<GameData>();
	gameMng  = ManagerInsts::Get<GameManager>();
	bgMng    = ManagerInsts::Get<BGManager>();
	soundMng = ManagerInsts::Get<SoundMng>();
	inputMng = ManagerInsts::Get<InputMng>();
	sceneMng = ManagerInsts::Get<SceneMng>();
	
	//動画取得.
	for (int i = 0; i < MENU_OPTION_COUNT; i++) {
		grMenuMovie[i] = GraphMng::Get(_T("menu_movie") + NumToString(i+1));
	}

	Reset();
}

//リセット.
void MenuScene::Reset() {

	counter = 0;		//経過時間を戻す.
	isBlink = false;	//点滅も戻す.

	tmBlink.Start();
	mCursor.Reset();

	//レイアウトの計算.
	CalcMenuRect();
}

//入った瞬間.
void MenuScene::Enter() {

	//レイアウトの計算.
	CalcMenuRect();

	//動画再生.
	if (auto i = GetPlayMovie()) {
		i->PlayMovie(PlayTypeID::Loop, true);
	}
}

//抜けた瞬間.
void MenuScene::Exit() {
	StopAllMovie();
}

//更新.
void MenuScene::Update() {

	//カーソル移動操作(長押しにも対応)
	{
		if (inputMng->IsPushActionTime(_T("MenuUp")) % 20 == 1) {
			//-1して、MENU_OPTION_COUNTの余りをループ.
			mCursor.cursorIdx = (mCursor.cursorIdx + MENU_OPTION_COUNT - 1) % MENU_OPTION_COUNT;
			OnCursorMove();
		}
		if (inputMng->IsPushActionTime(_T("MenuDown")) % 20 == 1) {
			//+1して、MENU_OPTION_COUNTの余りをループ.
			mCursor.cursorIdx = (mCursor.cursorIdx + 1) % MENU_OPTION_COUNT;
			OnCursorMove();
		}
	}

	//点滅終了.
	if (tmBlink.GetPassTime() <= 0) {
		isBlink = false;
	}

	//決定操作.
	if (inputMng->IsPushActionTime(_T("MenuNext")) == 1) {

		switch (mCursor.cursorIdx)
		{
			//エンドレスモードへ.
			case 0:
			{
				sceneMng->SetScene(_T("Game"));
				gameData->stage = StageType::Endless;
				//背景変更.
				bgMng->SetBgType(BGType::Space3D);

#if !defined BGM_NONE

				soundMng->StopAll();

				//BGM名を取得.
				MY_STRING bgmName = gameMng->GetGameSceneBgm();
				//BGMを再生.
				if (auto i = soundMng->Get(bgmName)) {
					i->Play(true, BGM_VOLUME_ENDLESS); //再生.
				}
#endif
			}
			break;

			//チュートリアルへ.
			case 1:
			{
				sceneMng->SetScene(_T("Game"));
				gameData->stage = StageType::Tutorial;
				//背景変更.
				bgMng->SetBgType(BGType::Tile);
				//プレイログに記録.
				gameMng->WritePlayLog(true);

#if !defined BGM_NONE
				//BGM.
				soundMng->StopAll();
				if (auto i = soundMng->Get(_T("BGM_Tutorial"))) {
					i->Play(true, BGM_VOLUME_TUTORIAL); //再生.
				}
#endif
			}
			break;

			case 2:
			{
				sceneMng->SetScene(_T("Title")); //タイトルへ.
				App::Reset();
			}
			break;

			default: assert(false); break;
		}

		//サウンド.
		if (auto i = soundMng->Get(_T("MenuOK"))) {
			i->Play(false, 70);
		}
	}

	//カウンター.
	counter += 1;
	//カーソル更新.
	mCursor.Update(counter);
}

//描画.
void MenuScene::Draw() {
	//部位ごとの描画.
	DrawBG();
	DrawTopText();
	DrawOptions();
	DrawCursor();
	DrawThumbnail();
	DrawGuideLines();
	DrawPanelDescOperate();
	DrawPanelDescMode();
}

//[描画] メニュー全体の背景.
void MenuScene::DrawBG() {
	//裏を暗くする.
	DrawMode::Exe(
		DrawModeID::None, DrawBlendModeID::Alpha, MenuLayout::bgAlpha,
		[&]() {
			Box box = { {0, 0}, {WINDOW_WID, WINDOW_HEI}, MenuColor::bgShade, 1.0f };
			box.Draw(Anchor::LU, true);
		}
	);
}

//[描画] トップテキスト.
void MenuScene::DrawTopText() {
	using namespace MenuLayout;

	//三角形演出.
	for (const auto& a : mCursor.arrow) {
		//広がり分を足した距離.
		const double inner = titleArrowInner + titleArrowMove * a.move;
		const double tip   = titleArrowTip   + titleArrowMove * a.move;
		const double h     = titleArrowHalfH;
		const UINT   col   = MenuColor::accent;

		DrawMode::Exe(
			DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * a.alpha),
			[&]() {
				Line lines[4] = {
					//「<」.
					{ titlePos + DBL_XY(-inner,  h), titlePos + DBL_XY(-tip,  0),   col, titleArrowThick },
					{ titlePos + DBL_XY(-tip,    0), titlePos + DBL_XY(-inner, -h), col, titleArrowThick },
					//「>」.
					{ titlePos + DBL_XY(+inner,  h), titlePos + DBL_XY(+tip,  0),   col, titleArrowThick },
					{ titlePos + DBL_XY(+tip,    0), titlePos + DBL_XY(+inner, -h), col, titleArrowThick }
				};
				//線描画.
				for (auto& i : lines) {
					i.Draw(true);
				}
			}
		);
	}

	//テキスト描画.
	DrawStr str(_T("モード選択"), titlePos.ToInt() + textOffset, MenuColor::accent);
	str.Draw(Anchor::Mid, gameData->fonts.at("jp-size4").GetFont());
}

//[描画] 選択肢.
void MenuScene::DrawOptions() {
	using namespace MenuLayout;

	const INT_XY offset = { 0, -1 };

	//テキスト & 枠線用.
	Box     box = { menuPos, menuSize, MenuColor::highlight, 1.0f };
	DrawStr str = { _T(""), menuPos.ToInt() + textOffset, {} };
	//選択肢テキスト.
	MY_STRING texts[] = {
		_T("ゲーム開始"),
		_T("チュートリアル"),
		_T("タイトルに戻る")
	};
	//テキスト色.
	unsigned int colors[] = {
		(mCursor.cursorIdx == 0) ? MenuColor::highlight : MenuColor::textDim,
		(mCursor.cursorIdx == 1) ? MenuColor::highlight : MenuColor::textDim,
		(mCursor.cursorIdx == 2) ? MenuColor::highlight : MenuColor::textDim
	};

	INT_XY savePos; //保存用.

	//全選択肢ループ.
	for (int i = 0; i < 3; i++) {

		str.text  = texts[i];
		str.color = colors[i];
		savePos   = str.pos;  //前の座標を保存.
		//枠.
		box.Draw(Anchor::Mid, false);

		int alpha = 255; //透明度.

		//ブレる処理.
		if (isBlink && mCursor.cursorIdx == i) {
			//位置をずらす.
			const int add = Calc::RandNum(-blinkShake, blinkShake);
			str.pos += add;
			//少し薄くする.
			alpha = blinkAlpha;
		}
		//テキスト.
		DrawMode::Exe(
			DrawModeID::None, DrawBlendModeID::Alpha, alpha,
			[&]() {
				str.Draw(Anchor::Mid, gameData->fonts.at("jp-size4").GetFont());
			}
		);

		//リセット.
		str.pos = savePos;
		//スペースを空ける.
		box.pos.y += menuSpace;
		str.pos.y += menuSpace;
	}
}

//[描画] カーソル.
void MenuScene::DrawCursor() {
	using namespace MenuLayout;

	//基準座標(選択中の枠の左).
	const DBL_XY base = menuPos + DBL_XY(-menuSize.x / 2 - cursorGap, menuSpace * mCursor.cursorIdx);
	//縦の開き(負になると裏返る).
	const double wave = cursorHalfH * mCursor.cursorWave;

	//カーソル.
	Triangle tri = {
		base,
		base + DBL_XY(-cursorLen, +wave),
		base + DBL_XY(-cursorLen, -wave),
		{}, {}
	};
	//表か裏かで色を変える.
	tri.color = (mCursor.cursorWave >= 0) ? MenuColor::highlight : MenuColor::cursorBack;
	tri.Draw();
}

//[描画] サムネイル.
void MenuScene::DrawThumbnail() {
	using namespace MenuLayout;

	if (auto i = GetPlayMovie()) {
		//画像描画.
		DrawMode::Exe(
			DrawModeID::Bilinear, DrawBlendModeID::None, 255,
			[&]() {
				i->DrawExtend(imgPos, { thumbScale, thumbScale });
			}
		);
		//枠線描画(画像にぴったり合うように).
		Box box = { imgPos, mRect.imgSize, MenuColor::accent, 1.0f };
		box.Draw(Anchor::Mid, false);
	}
}

//[描画] 装飾線.
void MenuScene::DrawGuideLines() {
	using namespace MenuLayout;

	//画像の下端座標.
	const int imgBottomY = (int)(imgPos.y + mRect.imgSize.y / 2);
	//線の透明度.
	const int alpha = _int_r(lineAlphaMin + lineAlphaRange * (mCursor.linePulse + 1.0) / 2.0);

	DrawMode::Exe(
		DrawModeID::None, DrawBlendModeID::Alpha, alpha,
		[&]() {
			//1.メニュー項目から画像への線.
			{
				Line line = {
					menuPos + DBL_XY(menuSize.x / 2, 0),                   //始点.
					DBL_XY(imgPos.x - mRect.imgSize.x / 2, menuPos.y),     //終点.
					MenuColor::accent, lineThick
				};
				//選択してる所にずらす.
				line.stPos.y += menuSpace * mCursor.cursorIdx;
				line.edPos.y += menuSpace * mCursor.cursorIdx;
				line.Draw(false);
			}
			//2.画像から説明文エリアへの2本線.
			{
				Line line = {
					DBL_XY(imgPos.x - lineSide, imgBottomY),               //始点.
					DBL_XY(imgPos.x - lineSide, mRect.descPos.y),          //終点.
					MenuColor::accent, lineThick
				};
				line.Draw(false);         //左の線.
				line.stPos.x += lineSide * 2;
				line.edPos.x += lineSide * 2;
				line.Draw(false);         //右の線.
			}
		}
	);
}

//[描画] 操作説明パネル.
void MenuScene::DrawPanelDescOperate() {

	//枠線.
	Box box = { mRect.infoPos.ToDbl(), mRect.infoSize.ToDbl(), MenuColor::accent, 1.0f};
	box.Draw(Anchor::LU, false);

	//見出し.
	DrawStr str2(_T("操作"), mRect.infoPos + MenuLayout::headingOffset, MenuColor::accent);
	str2.Draw(Anchor::LD, gameData->fonts.at("jp-size2").GetFont());

	//テキスト.
	MY_STRING texts[2] = { _T("null"), _T("null") };

	//[操作表示] 操作によって表示するテキストを変える.
#if defined INPUT_CHANGE_ARCADE
	texts[0] = _T("選択: レバー");
	texts[1] = _T("決定: 左上ボタン");
#else
	//最後の操作端末を取得.
	switch (gameMng->GetLastInputDevice())
	{
		case InputDevice::Key:
			texts[0] = _T("選択: ↑/↓/W/S");
			texts[1] = _T("決定: SPACE");
			break;

		case InputDevice::PadXbox:
			texts[0] = _T("選択: 左スティック");
			texts[1] = _T("決定: Aボタン");
			break;

		//キーボードとコントローラ以外の操作になるのはおかしい.
		default: assert(false); break;
	}
#endif

	DrawStr str = { _T(""), mRect.infoPos + MenuLayout::loreInner, MenuColor::textDim };

	//1行ずつ表示.
	for (auto& i : texts) {
		str.text = i;
		str.Draw(Anchor::LU, gameData->fonts.at("jp-size2").GetFont());
		str.pos.y += MenuLayout::loreLineSpace; //次の行へ.
	}
}

//[描画] モード説明パネル.
void MenuScene::DrawPanelDescMode() {

	//枠線.
	Box box = { mRect.descPos.ToDbl(), mRect.descSize.ToDbl(), MenuColor::accent, 1.0f};
	box.Draw(Anchor::LU, false);

	//見出し.
	DrawStr str2(_T("モード説明"), mRect.descPos + MenuLayout::headingOffset, MenuColor::accent);
	str2.Draw(Anchor::LD, gameData->fonts.at("jp-size2").GetFont());

	//テキスト.
	DrawStr str(_T(""), mRect.descPos + MenuLayout::loreInner, MenuColor::textDim);

	switch (mCursor.cursorIdx)
	{
		case 0:
		{
			MY_STRING texts[] = {
				_T("ゲームオーバーになるまで続くエンドレスモードです。"),
				_T("ハイスコアを目指しましょう！"),
				_T(""),
				_T("[スコア]"),
				_T("隕石を破壊　　: +500"),
				_T("アイテムを取る: +100"),
				_T("タイムボーナス: 1秒ごとに+10")
			};
			//1行ずつ表示.
			for (auto& i : texts) {
				str.text = i;
				str.Draw(Anchor::LU, gameData->fonts.at("jp-size2").GetFont());
				str.pos.y += MenuLayout::loreLineSpace; //次の行へ.
			}
		}
		break;

		case 1:
		{
			MY_STRING texts[] = {
				_T("基本操作やルールを確認できます。"),
				_T("初めて遊ぶ方におすすめです。"),
				_T(""),
				_T("STEP1: 基本操作"),
				_T("STEP2: アイテム"),
				_T("STEP3: 反射"),
				_T("STEP4: スコア")
			};
			//1行ずつ表示.
			for (auto& i : texts) {
				str.text = i;
				str.Draw(Anchor::LU, gameData->fonts.at("jp-size2").GetFont());
				str.pos.y += MenuLayout::loreLineSpace; //次の行へ.
			}
		}
		break;

		case 2:
		{
			str.text = _T("タイトル画面に戻ります。");
			str.Draw(Anchor::LU, gameData->fonts.at("jp-size2").GetFont());
		}
		break;
	}
}

//共有するレイアウト値を計算.
void MenuScene::CalcMenuRect() {
	using namespace MenuLayout;

	//サムネ枠のサイズ(動画がなければ0).
	mRect.imgSize = DBL_XY(0, 0);
	if (auto i = GetPlayMovie()) {
		mRect.imgSize = i->GetSize().ToDbl() * thumbScale + thumbMargin;
	}

	//[説明文パネル] サムネの幅に合わせる.
	const int descWid = _int_r(mRect.imgSize.x);
	mRect.descSize = INT_XY(descWid, panelHeight);
	mRect.descPos = INT_XY(_int_r(imgPos.x - descWid / 2), WINDOW_HEI - panelMarginBottom - panelHeight);

	//[操作パネル] 説明文パネルと同じ高さ・同じ縦位置.
	mRect.infoSize = INT_XY(infoWidth, panelHeight);
	mRect.infoPos = INT_XY(_int_r(menuPos.x - _dbl(infoWidth) / 2), mRect.descPos.y);
}

//カーソル移動時の処理.
void MenuScene::OnCursorMove() {

	isBlink = true;  //点滅させる.
	tmBlink.Start(); //点滅時間計測.

	mCursor.SetCursorRotSpeed(MenuMotion::cursorRotMove);

	//レイアウトの計算.
	CalcMenuRect();
	//全動画停止.
	StopAllMovie();
	//動画再生.
	GetPlayMovie()->PlayMovie(PlayTypeID::Loop, true);

	//サウンド.
	if (auto i = soundMng->Get(_T("MenuCursor"))) {
		i->Play(false, 70);
	}
}

//全ての動画を停止.
void MenuScene::StopAllMovie() {
	//登録された動画をループ.
	for (auto& i : grMenuMovie) {
		if (i) {
			i->StopMovie();
		}
	}
}
