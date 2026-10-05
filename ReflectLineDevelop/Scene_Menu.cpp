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

//初期化.
void MenuScene::Init() {

	//参照取得.
	gameData = ManagerInsts::Get<GameData>();
	gameMng  = ManagerInsts::Get<GameManager>();
	bgMng    = ManagerInsts::Get<BGManager>();
	soundMng = ManagerInsts::Get<SoundMng>();
	inputMng = ManagerInsts::Get<InputMng>();
	sceneMng = ManagerInsts::Get<SceneMng>();

	Reset();
}

//リセット.
void MenuScene::Reset() {
	cursorIdx = 0;
	tmBlink.Start();
}

//入った瞬間.
void MenuScene::Enter() {
	//動画再生.
	GraphMng::Get(GetPlayMovieName())->PlayMovie(PlayTypeID::Loop, true);
}

//抜けた瞬間.
void MenuScene::Exit() {
	StopAllMovie();
}

//更新.
void MenuScene::Update() {

	//カーソル移動操作.
	if (inputMng->IsPushActionTime(_T("MenuUp")) % 20 == 1) {
		cursorIdx = (cursorIdx + 3 - 1) % 3; //-1して、3の余り(0～2)をループ.
		OnCursorMove();
	}
	if (inputMng->IsPushActionTime(_T("MenuDown")) % 20 == 1) { //長押しにも対応.
		cursorIdx = (cursorIdx + 1) % 3;     //+1して、3の余り(0～2)をループ.
		OnCursorMove();
	}
	//点滅終了.
	if (tmBlink.GetPassTime() <= 0) {
		isBlink = false;
	}

	//決定操作.
	if (inputMng->IsPushActionTime(_T("MenuNext")) == 1) {

		switch (cursorIdx)
		{
			case 0:
			{
				//耐久モードへ.
				sceneMng->SetScene(_T("Game"));
				gameData->stage = StageType::Endless;
				//背景変更.
				bgMng->SetBgNo(3);

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

			case 1:
			{
				//チュートリアルへ.
				sceneMng->SetScene(_T("Game"));
				gameData->stage = StageType::Tutorial;
				//背景変更.
				bgMng->SetBgNo(1);
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

			default: assert(FALSE); break;
		}

		//サウンド.
		if (auto i = soundMng->Get(_T("MenuOK"))) {
			i->Play(false, 70);
		}
	}

	//経過時間.
	counter += 1;
}

//描画.
void MenuScene::Draw() {

	//アニメーション値.
	//この値を基準にメニューのアニメーションを制御する.
	const double anim1 = sin(counter/ 50 * M_PI);
	const double anim2 = sin(counter/100 * M_PI);
	const double anim3 = Calc::AnimEase(EaseType::OutInQuad, fmod(counter, 120)/120);
	const double anim4 = sin(fmod(counter, 120)/120 * M_PI);
	const double anim5 = Calc::AnimEase(EaseType::OutInQuad, fmod(counter-20, 120)/120);
	const double anim6 = sin(fmod(counter-20, 120)/120 * M_PI);

	//▼メニュー全体の背景.
	DrawMode::Exe(
		DrawModeID::None, DrawBlendModeID::Alpha, 128,
		[&]() {
			Box box = { {0, 0}, {WINDOW_WID, WINDOW_HEI}, 0x000000, 1.0f };
			DrawBoxKR(box, Anchor::LU, true);
		}
	);
	//▼メニュータイトル.
	{
		//基準位置.
		const DBL_XY basePos = { WINDOW_WID / 2, 80 };

		//三角形演出1.
		DrawMode::Exe(
			DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * anim4),
			[&]() {
				Line lines[4] = {
					//「<」.
					{ basePos + DBL_XY(-55 - 100 * anim3, 30), basePos + DBL_XY(-85 - 100 * anim3,   0), 0x00FFFF, 2.0f },
					{ basePos + DBL_XY(-85 - 100 * anim3,  0), basePos + DBL_XY(-55 - 100 * anim3, -30), 0x00FFFF, 2.0f },
					//「>」.
					{ basePos + DBL_XY(+55 + 100 * anim3, 30), basePos + DBL_XY(+85 + 100 * anim3,   0), 0x00FFFF, 2.0f },
					{ basePos + DBL_XY(+85 + 100 * anim3,  0), basePos + DBL_XY(+55 + 100 * anim3, -30), 0x00FFFF, 2.0f }
				};
				//線描画.
				for (auto& i : lines) {
					DrawLineKR(i, true);
				}
			}
		);
		//三角形演出2.
		DrawMode::Exe(
			DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * anim6),
			[&]() {
				Line lines[4] = {
					//「<」.
					{ basePos + DBL_XY(-55 - 100 * anim5, 30), basePos + DBL_XY(-85 - 100 * anim5,   0), 0x00FFFF, 2.0f },
					{ basePos + DBL_XY(-85 - 100 * anim5,  0), basePos + DBL_XY(-55 - 100 * anim5, -30), 0x00FFFF, 2.0f },
					//「>」.
					{ basePos + DBL_XY(+55 + 100 * anim5, 30), basePos + DBL_XY(+85 + 100 * anim5,   0), 0x00FFFF, 2.0f },
					{ basePos + DBL_XY(+85 + 100 * anim5,  0), basePos + DBL_XY(+55 + 100 * anim5, -30), 0x00FFFF, 2.0f }
				};
				//線描画.
				for (auto& i : lines) {
					DrawLineKR(i, true);
				}
			}
		);

		const INT_XY offset = { 0, -1 };

		//テキスト描画.
		DrawStr str(_T("モード選択"), basePos.ToInt() + offset, 0x00FFFF);
		str.Draw(Anchor::Mid, gameData->fonts["jp-size4"].GetFont());
	}

	//▼各選択肢.
	{
		const INT_XY offset = {0, -1};

		//テキスト & 枠線用.
		Box     box = { mLayout.menuPos, mLayout.menuSize, mColor.select1, 1.0f };
		DrawStr str = { _T(""), mLayout.menuPos.ToInt() + offset, {}};
		//選択肢テキスト.
		MY_STRING texts[] = {
			_T("ゲーム開始"), 
			_T("チュートリアル"), 
			_T("タイトルに戻る")
		};
		//テキスト色.
		unsigned int colors[] = {
			(cursorIdx == 0) ? mColor.select1 : mColor.normal,
			(cursorIdx == 1) ? mColor.select1 : mColor.normal,
			(cursorIdx == 2) ? mColor.select1 : mColor.normal
		};

		INT_XY savePos; //保存用.

		//全選択肢ループ.
		for (int i = 0; i < 3; i++) {

			str.text  = texts[i];
			str.color = colors[i];
			savePos   = str.pos;  //前の座標を保存.
			//枠.
			DrawBoxKR(box, Anchor::Mid, false);
			
			int alpha = 255; //透明度.
			//ブレる処理.
			if (isBlink && cursorIdx == i) {
				const int add = Calc::RandNum(-5, 5); //ずらす量.
				str.pos += add;						  //位置をずらす.
				alpha = 128;
			}
			//テキスト.
			DrawMode::Exe(
				DrawModeID::None, DrawBlendModeID::Alpha, alpha,
				[&]() {
					str.Draw(Anchor::Mid, gameData->fonts["jp-size4"].GetFont());
				}
			);

			//リセット.
			str.pos = savePos;
			//スペースを空ける.
			box.pos.y += mLayout.menuSpace;
			str.pos.y += mLayout.menuSpace;
		}
	}

	//▼カーソルの三角.
	{
		//基準座標.
		DBL_XY base = mLayout.menuPos + DBL_XY(
			-mLayout.menuSize.x/2 - 20,		//横にずらす.
			+mLayout.menuSpace * cursorIdx	//縦にずらす.
		);

		Triangle tri = { base, base + DBL_XY(-20, 10 * anim1), base + DBL_XY(-20, -10 * anim1), {}, {} };
		tri.color = (anim1 >= 0) ? mColor.select1 : mColor.select2; //表か裏かで色を変える.
		DrawTriangleKR(tri, true, true);
	}

	//画像サイズ保存用.
	DBL_XY imgSize;

	//▼サムネ.
	{
		const double extend = 0.7; //画像描画倍率.
		const int    margin = 10;  //枠を画像よりどれだけ大きくするか.
	
		//画像を取得できたら.
		if (auto i = GraphMng::Get(GetPlayMovieName())) {

			//画像描画.
			DrawMode::Exe(
				DrawModeID::Bilinear, DrawBlendModeID::None, 255,
				[&]() {
					i->DrawExtend(mLayout.imgPos, { extend , extend });
				}
			);
			//画像の描画サイズを計算.
			imgSize = i->GetSize().ToDbl() * extend + margin;
			//枠線描画(画像にぴったり合うように)
			Box box = { mLayout.imgPos, imgSize, mColor.frame, 1.0f };
			DrawBoxKR(box, Anchor::Mid, false);
		}
	}

	//▼説明文の枠（右下）- 画像の幅に合わせる
	int textBoxWidth  = _int_r(imgSize.x);
	int textBoxHeight = 260;
	int textBoxX      = _int_r(mLayout.imgPos.x - textBoxWidth/2);  // 画像と同じ中心位置
	int textBoxY      = WINDOW_HEI - 300;

	//▼選択項目から画像、説明文エリアまでの線を描画
	{
		int imgLeftX   = (int)(mLayout.imgPos.x - imgSize.x/2); //画像の左端座標.
		int imgCenterY = (int) mLayout.imgPos.y;
		int imgBottomY = (int)(mLayout.imgPos.y + imgSize.y/2); //画像の下端座標.

		// 説明文エリアの上端中央座標
		int textBoxCenterX = textBoxX + textBoxWidth / 2;
		int textBoxTopY = textBoxY;

		//線の透明度(155～255)
		const int alpha = _int_r(155 + 100 * (anim2 + 1.0) / 2.0);
		//描画.
		DrawMode::Exe(
			DrawModeID::None, DrawBlendModeID::Alpha, alpha,
			[&]() {

				//1.メニュー項目から画像への線（メニュー項目右端から画像左端まで）
				{
					//線データ.
					Line line = {
						mLayout.menuPos + DBL_XY(mLayout.menuSize.x/2, 0),       //始点.
						DBL_XY(mLayout.imgPos.x-imgSize.x/2, mLayout.menuPos.y), //終点.
						mColor.line,                                             //色.
						3.0f
					};
					//選択してる所にずらす.
					line.stPos.y += mLayout.menuSpace * cursorIdx;
					line.edPos.y += mLayout.menuSpace * cursorIdx;
					//線描画.
					DrawLineKR(line, false);
				}

				//2.画像から説明文エリアへの線（画像下端から説明文上端まで）
				{
					Line line = {
						DBL_XY(mLayout.imgPos.x-30, imgBottomY),  //始点.
						DBL_XY(mLayout.imgPos.x-30, textBoxTopY), //終点.
						mColor.line,							  //色.
						3.0f
					};
					//線1.
					DrawLineKR(line, false);
					//線2.
					line.stPos.x += 60;
					line.edPos.x += 60;
					DrawLineKR(line, false);
				}
			}
		);
	}

	//▼操作説明（左下）
	{
		int infoWidth = 500;
		int infoHeight = textBoxHeight;
		int infoX = _int_r(mLayout.menuPos.x - _dbl(infoWidth)/2);
		int infoY = textBoxY;

		Box box = { DBL_XY(infoX, infoY), DBL_XY(infoWidth, infoHeight), mColor.select1, 1.0f };

		DrawStr str2(_T("操作"), { infoX + 10, infoY - 10 }, 0x00FFFF);
		str2.Draw(Anchor::LD, gameData->fonts["jp-size2"].GetFont());

		DrawBoxKR(box, Anchor::LU, false);

		DrawStr str = { _T(""), INT_XY(infoX, infoY) + mLayout.loreInner, mColor.normal };

		MY_STRING texts[] = {
			_T("選択: ↑/↓/W/S"),
			_T("決定: SPACE/ENTER/Aボタン"),
		};
		//1行ずつ表示.
		for (auto& i : texts) {
			str.text = i;
			str.Draw(Anchor::LU, gameData->fonts["jp-size2"].GetFont());
			str.pos.y += mLayout.loreLineSpace; //次の行へ.
		}
	}

	//▼モード説明タイトル（説明文枠の上に表示）
	{
		DrawStr str2(_T("モード説明"), { textBoxX+10, textBoxY-10 }, 0x00FFFF);
		str2.Draw(Anchor::LD, gameData->fonts["jp-size2"].GetFont());

		// 説明文枠の枠線（水色）	
		Box box = { DBL_XY(textBoxX, textBoxY), DBL_XY(textBoxWidth, textBoxHeight), mColor.frame, 1.0f };
		DrawBoxKR(box, Anchor::LU, false);

		//説明文用.
		DrawStr str(_T(""), INT_XY(textBoxX, textBoxY)+mLayout.loreInner, mColor.normal);

		switch (cursorIdx)
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
					str.Draw(Anchor::LU, gameData->fonts["jp-size2"].GetFont());
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
					str.Draw(Anchor::LU, gameData->fonts["jp-size2"].GetFont());
					str.pos.y += mLayout.loreLineSpace; //次の行へ.
				}
			}
			break;

		case 2:
			str.text = _T("タイトル画面に戻ります。");
			str.Draw(Anchor::LU, gameData->fonts["jp-size2"].GetFont());
			break;
		}
	}
}

//再生する動画名.
MY_STRING MenuScene::GetPlayMovieName() {
	return _T("menu_movie") + NumToString(cursorIdx + 1);
}

//カーソル移動時の処理.
void MenuScene::OnCursorMove() {

	isBlink = true;  //点滅させる.
	tmBlink.Start(); //点滅時間計測.

	//全動画停止.
	StopAllMovie();
	//動画再生.
	GraphMng::Get(GetPlayMovieName())->PlayMovie(PlayTypeID::Loop, true);

	//サウンド.
	if (auto i = soundMng->Get(_T("MenuCursor"))) {
		i->Play(false, 70);
	}
}

//全ての動画を停止.
void MenuScene::StopAllMovie() {

	GraphMng::Get(_T("menu_movie1"))->StopMovie();
	GraphMng::Get(_T("menu_movie2"))->StopMovie();
	GraphMng::Get(_T("menu_movie3"))->StopMovie();
}
