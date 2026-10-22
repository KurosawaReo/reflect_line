/*
   - Scene_Menu.cpp -
*/
#include "Scene_Menu.h"

//依存関係.
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

// サムネ関係(CalcRectとDrawThumbnailで共有).
static constexpr double THUMB_EXTEND = 0.7; // 画像描画倍率.
static constexpr int    THUMB_MARGIN = 10;  // 枠を画像よりどれだけ大きくするか.

// ▼*---=[ MenuAnim ]=---*▼ //

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
	cursorRotAngle += cursorRotSpeed * MENU_CURSOR_BASE_SPEED * M_PI;
	//回転速度を標準速度(1.0)へ滑らかに戻す.
	cursorRotSpeed += (1.0 - cursorRotSpeed) * MENU_CURSOR_RETURN_SPEED;
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
		DrawModeID::None, DrawBlendModeID::Alpha, 128,
		[&]() {
			Box box = { {0, 0}, {WINDOW_WID, WINDOW_HEI}, 0x000000, 1.0f };
			box.Draw(Anchor::LU, true);
		}
	);
}

//[描画] トップテキスト.
void MenuScene::DrawTopText() {

	//基準位置.
	const DBL_XY basePos = { WINDOW_WID / 2, 80 };

	//三角形演出.
	for (const auto& a : mCursor.arrow) {
		DrawMode::Exe(
			DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * a.alpha), 
			[&]() {
				Line lines[4] = {
					//「<」.
					{ basePos + DBL_XY(-55 - 100 * a.move, 30), basePos + DBL_XY(-85 - 100 * a.move,   0), 0x00FFFF, 2.0f },
					{ basePos + DBL_XY(-85 - 100 * a.move,  0), basePos + DBL_XY(-55 - 100 * a.move, -30), 0x00FFFF, 2.0f },
					//「>」.
					{ basePos + DBL_XY(+55 + 100 * a.move, 30), basePos + DBL_XY(+85 + 100 * a.move,   0), 0x00FFFF, 2.0f },
					{ basePos + DBL_XY(+85 + 100 * a.move,  0), basePos + DBL_XY(+55 + 100 * a.move, -30), 0x00FFFF, 2.0f }
				};
				//線描画.
				for (auto& i : lines) {
					i.Draw(true);
				}
			}
		);
	}

	const INT_XY offset = { 0, -1 };

	//テキスト描画.
	DrawStr str(_T("モード選択"), basePos.ToInt() + offset, 0x00FFFF);
	str.Draw(Anchor::Mid, gameData->fonts.at("jp-size4").GetFont());
}

//[描画] 選択肢.
void MenuScene::DrawOptions() {

	const INT_XY offset = { 0, -1 };

	//テキスト & 枠線用.
	Box     box = { mLayout.menuPos, mLayout.menuSize, mColor.select1, 1.0f };
	DrawStr str = { _T(""), mLayout.menuPos.ToInt() + offset, {} };
	//選択肢テキスト.
	MY_STRING texts[] = {
		_T("ゲーム開始"),
		_T("チュートリアル"),
		_T("タイトルに戻る")
	};
	//テキスト色.
	unsigned int colors[] = {
		(mCursor.cursorIdx == 0) ? mColor.select1 : mColor.normal,
		(mCursor.cursorIdx == 1) ? mColor.select1 : mColor.normal,
		(mCursor.cursorIdx == 2) ? mColor.select1 : mColor.normal
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
			const int add = Calc::RandNum(-5, 5); //ずらす量.
			str.pos += add;						  //位置をずらす.
			alpha = 128;
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
		box.pos.y += mLayout.menuSpace;
		str.pos.y += mLayout.menuSpace;
	}
}

//[描画] カーソル.
void MenuScene::DrawCursor() {

	//基準座標.
	const DBL_XY base = mLayout.menuPos + DBL_XY(
		-mLayout.menuSize.x / 2 - 20,	//横にずらす.
		+mLayout.menuSpace * mCursor.cursorIdx	//縦にずらす.
	);

	const double wave = 10.0 * mCursor.cursorWave;

	//カーソル.
	Triangle tri = {
		base,
		base + DBL_XY(-20.0, +wave),
		base + DBL_XY(-20.0, -wave),
		{}, {}
	};
	tri.color = (mCursor.cursorWave >= 0) ? mColor.select1 : mColor.select2; //表か裏かで色を変える.
	tri.Draw();
}

//[描画] サムネイル.
void MenuScene::DrawThumbnail() {

	if (auto i = GetPlayMovie()) {
		//画像描画.
		DrawMode::Exe(
			DrawModeID::Bilinear, DrawBlendModeID::None, 255,
			[&]() { 
				i->DrawExtend(mLayout.imgPos, { THUMB_EXTEND, THUMB_EXTEND }); 
			}
		);
		//枠線描画(画像にぴったり合うように).
		Box box = { mLayout.imgPos, mRect.imgSize, mColor.frame, 1.0f };
		box.Draw(Anchor::Mid, false);
	}
}

//[描画] 装飾線.
void MenuScene::DrawGuideLines() {
	
	//画像の下端座標.
	const int imgBottomY = (int)(mLayout.imgPos.y + mRect.imgSize.y / 2);
	//線の透明度(155～255)
	const int alpha = _int_r(155 + 100 * (mCursor.linePulse + 1.0) / 2.0);

	//描画.
	DrawMode::Exe(
		DrawModeID::None, DrawBlendModeID::Alpha, alpha,
		[&]() {

			//1.メニュー項目から画像への線（メニュー項目右端から画像左端まで）
			{
				//線データ.
				Line line = {
					mLayout.menuPos + DBL_XY(mLayout.menuSize.x / 2, 0),				//始点.
					DBL_XY(mLayout.imgPos.x - mRect.imgSize.x / 2, mLayout.menuPos.y),	//終点.
					mColor.line,														//色.
					3.0f
				};
				//選択してる所にずらす.
				line.stPos.y += mLayout.menuSpace * mCursor.cursorIdx;
				line.edPos.y += mLayout.menuSpace * mCursor.cursorIdx;
				//線描画.
				line.Draw(false);
			}

			//2.画像から説明文エリアへの線（画像下端から説明文上端まで）
			{
				Line line = {
					DBL_XY(mLayout.imgPos.x - 30, imgBottomY),		//始点.
					DBL_XY(mLayout.imgPos.x - 30, mRect.descPos.y),	//終点.
					mColor.line,									//色.
					3.0f
				};
				//線1.
				line.Draw(false);
				//線2.
				line.stPos.x += 60;
				line.edPos.x += 60;
				line.Draw(false);
			}
		}
	);
}

//[描画] 操作説明パネル.
void MenuScene::DrawPanelDescOperate() {

	//枠線.
	Box box = { mRect.infoPos.ToDbl(), mRect.infoSize.ToDbl(), mColor.frame, 1.0f};
	box.Draw(Anchor::LU, false);

	//見出し.
	DrawStr str2(_T("操作"), mRect.descPos + INT_XY(10, -10), 0x00FFFF);
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

	DrawStr str = { _T(""), mRect.infoPos + mLayout.loreInner, mColor.normal };

	//1行ずつ表示.
	for (auto& i : texts) {
		str.text = i;
		str.Draw(Anchor::LU, gameData->fonts.at("jp-size2").GetFont());
		str.pos.y += mLayout.loreLineSpace; //次の行へ.
	}
}

//[描画] モード説明パネル.
void MenuScene::DrawPanelDescMode() {

	//枠線.
	Box box = { mRect.descPos.ToDbl(), mRect.descSize.ToDbl(), mColor.frame, 1.0f};
	box.Draw(Anchor::LU, false);

	//見出し.
	DrawStr str2(_T("モード説明"), mRect.descPos + INT_XY(10, -10), 0x00FFFF);
	str2.Draw(Anchor::LD, gameData->fonts.at("jp-size2").GetFont());

	//テキスト.
	DrawStr str(_T(""), mRect.descPos + mLayout.loreInner, mColor.normal);

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
				str.pos.y += mLayout.loreLineSpace; //次の行へ.
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
				str.pos.y += mLayout.loreLineSpace; //次の行へ.
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

	//サムネ枠のサイズ(動画がなければ0).
	mRect.imgSize = DBL_XY(0, 0);
	if (auto i = GetPlayMovie()) {
		mRect.imgSize = i->GetSize().ToDbl() * THUMB_EXTEND + THUMB_MARGIN;
	}

	//[説明文パネル] サムネの幅に合わせる.
	const int descWid = _int_r(mRect.imgSize.x);
	const int descHei = 260;
	mRect.descSize = INT_XY(descWid, descHei);
	mRect.descPos  = INT_XY(_int_r(mLayout.imgPos.x - descWid / 2), WINDOW_HEI - 300);

	//[操作パネル] 説明文パネルと同じ高さ・同じ縦位置.
	const int infoWid = 500;
	mRect.infoSize = INT_XY(infoWid, descHei);
	mRect.infoPos  = INT_XY(_int_r(mLayout.menuPos.x - _dbl(infoWid) / 2), mRect.descPos.y);
}

//カーソル移動時の処理.
void MenuScene::OnCursorMove() {

	isBlink = true;  //点滅させる.
	tmBlink.Start(); //点滅時間計測.

	mCursor.SetCursorRotSpeed(MENU_CURSOR_MOVE_ROT_SPEED);

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
