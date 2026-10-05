/*
   - GameManager.cpp -
   ゲーム全体管理.
*/
#include "GameManager.h"

//依存関係.
#include "GameData.h"
#include "BGManager.h"
#include "LaserManager.h"
#include "Obst_NormalLaser.h"
#include "Obst_StraightLaser.h"
#include "Obst_MeteorManager.h"
#include "Obst_Ripples.h"
#include "Obst_Fireworks.h"
#include "Item.h"
#include "UIManager.h"
#include "EffectManager.h"
#include "Stage_Tutorial.h"
#include "Stage_Endless.h"
//参照.
static GameData*	  gameData;
static BGManager*	  bg;
static UIManager*	  uiMng;
static TutorialStage* tutorialStg;
//参照(KRライブラリ)
static SoundMng*      soundMng;
static InputMng*      inputMng;
static SceneMng*      sceneMng;

using namespace Calc; //計算機能を使用.

// ▼*---=[ GameManager ]=---*▼ //

//初期化(一回のみ行う)
void GameManager::Init() {

	//参照取得.
	gameData	= ManagerInsts::Get<GameData>();
	bg			= ManagerInsts::Get<BGManager>();
	uiMng		= ManagerInsts::Get<UIManager>();
	tutorialStg	= ManagerInsts::Get<TutorialStage>();
	soundMng	= ManagerInsts::Get<SoundMng>();
	inputMng	= ManagerInsts::Get<InputMng>();
	sceneMng	= ManagerInsts::Get<SceneMng>();

	srand((unsigned)time(NULL)); //乱数初期化.

	//[KrLib] カメラ.
	Camera::SetPos(App::GetWindowSize().ToDbl()/2);

	try {
		//[KrLib] 画像.
		GraphMng::SetPath(_T("Resources/Images/"));	//共通パスの設定.
		GraphMng::LoadImg(_T("logo_text_only.png"),			_T("logo"));
		GraphMng::LoadImg(_T("logo_all.png"),				_T("logo_all"));
		GraphMng::LoadImg(_T("new_record.png"),				_T("new_record"));
		GraphMng::LoadImg(_T("gameover.png"),				_T("gameover"));
		GraphMng::LoadImg(_T("reflect_text.png"),			_T("reflect_text"));
		GraphMng::LoadImg(_T("player_normal.png"),			_T("player_nor"));
		GraphMng::LoadImg(_T("player_reflect.png"),			_T("player_ref"));
		GraphMng::LoadImg(_T("player_reflect_barrier.png"),	_T("player_ref_barrier"));
		GraphMng::LoadImg(_T("item.png"),					_T("item"));
		GraphMng::LoadImg(_T("item_super.png"),				_T("item_super"));
		GraphMng::LoadImg(_T("light_color_ref.png"),		_T("light_super_item"));
		GraphMng::LoadImg(_T("light_color_nor.png"),		_T("light_nor_player"));
		GraphMng::LoadImg(_T("light_color_ref.png"),		_T("light_ref_player"));
		GraphMng::LoadImg(_T("reflect_mode_frame.png"),		_T("reflect_mode_frame"));
		GraphMng::LoadImg(_T("ui_back_level.png"),			_T("ui_back_level"));
		GraphMng::LoadImg(_T("ui_back_best_score.png"),		_T("ui_back_best_score"));
		GraphMng::LoadImg(_T("ui_back_score.png"),			_T("ui_back_score"));
		GraphMng::LoadImg(_T("ui_back_time.png"),			_T("ui_back_time"));
		GraphMng::LoadImg(_T("score100.png"),				_T("score100"));
		GraphMng::LoadImg(_T("score500.png"),				_T("score500"));
		GraphMng::LoadImg(_T("target.png"),					_T("target"));
		GraphMng::LoadImg(_T("bg_tile_nor.png"),			_T("bg_tile_nor"));
		GraphMng::LoadImg(_T("bg_tile_ref.png"),			_T("bg_tile_ref"));
		GraphMng::LoadImg(_T("bg_star_nor.png"),			_T("bg_star_nor"));
		GraphMng::LoadImg(_T("bg_star_ref.png"),			_T("bg_star_ref"));
		GraphMng::LoadImg(_T("reflect_effect.png"),			_T("reflect_effect"));

		//[KrLib] 動画.
		GraphMng::SetPath(_T("Resources/Movies/"));	//共通パスの設定.
		GraphMng::LoadMovie(_T("menu_movie_endless.mp4"),		_T("menu_movie1"));
		GraphMng::LoadMovie(_T("menu_movie_tutorial.mp4"),		_T("menu_movie2"));
		GraphMng::LoadMovie(_T("menu_movie_back_title.mp4"),	_T("menu_movie3"));
		GraphMng::LoadMovie(_T("reflect_mode_death.mp4"),		_T("reflect_mode_death"));
		GraphMng::LoadMovie(_T("dash_reflect.mp4"),				_T("dash_reflect"));

		//[KrLib] サウンド.
		soundMng->SetPath(_T("Resources/Sounds/"));	//共通パスの設定.
		soundMng->LoadFile(_T("bgm/Virtual Terminal.mp3"),		_T("BGM_Menu"));		//メニューBGM.
		soundMng->LoadFile(_T("bgm/audiostock_1603723.mp3"),	_T("BGM_Tutorial"));	//チュートリアルBGM.
		soundMng->LoadFile(_T("bgm/Scarlet Radiance.mp3"),		_T("BGM_Endless_1"));	//エンドレスモードBGM1.
		soundMng->LoadFile(_T("bgm/CODE FROST.mp3"),			_T("BGM_Endless_2"));   //エンドレスモードBGM2.
		soundMng->LoadFile(_T("bgm/Glacial brilliance.mp3"),	_T("BGM_Endless_3"));   //エンドレスモードBGM3.
		soundMng->LoadFile(_T("bgm/Crystal Reverie.mp3"),		_T("BGM_Endless_4"));   //エンドレスモードBGM3.
		soundMng->LoadFile(_T("bgm/命ナキ者ノ詩.mp3"),			_T("BGM_Over"));		//ゲームオーバーBGM.

		soundMng->LoadFile(_T("se/audiostock_1636674.mp3"),		_T("MenuCursor"));		//メニューカーソル音.
		soundMng->LoadFile(_T("se/audiostock_1636651.mp3"),		_T("MenuOK"));			//メニュー決定音.
		soundMng->LoadFile(_T("se/audiostock_461339.mp3"),		_T("ItemUse")); 		//アイテム発動.
		soundMng->LoadFile(_T("se/player_dash.mp3"),			_T("PlayerDash")); 		//ダッシュ音.
		soundMng->LoadFile(_T("se/audiostock_1677973.mp3"),     _T("PlayerParry"));     //ダッシュ反射音.
		soundMng->LoadFile(_T("se/audiostock_1116927_cut.mp3"), _T("CountDown"));		//カウントダウン.
		soundMng->LoadFile(_T("se/audiostock_63721.mp3"),		_T("PowerDown"));		//アイテム解除.
		soundMng->LoadFile(_T("se/audiostock_1296254.mp3"),		_T("Laser1"));			//レーザー(発射)
		soundMng->LoadFile(_T("se/audiostock_1296256.mp3"),		_T("Laser2"));			//レーザー(強発射)
		soundMng->LoadFile(_T("se/audiostock_218404.mp3"),		_T("Laser3"));			//レーザー(反射)
		soundMng->LoadFile(_T("se/audiostock_936158.mp3"),		_T("Ripples"));			//波紋.
		soundMng->LoadFile(_T("se/fireworks.mp3"),				_T("Fireworks"));		//花火.
		soundMng->LoadFile(_T("se/audiostock_104974.mp3"),		_T("MeteorBreak"));		//隕石破壊.
		soundMng->LoadFile(_T("se/audiostock_981051.mp3"),		_T("PlayerDeath"));
		soundMng->LoadFile(_T("se/決定ボタンを押す23.mp3"),		_T("LevelUp"));
		soundMng->LoadFile(_T("se/audiostock_184924.mp3"),		_T("BestScore"));		//最高スコア更新.
	}
	catch (const ErrorMsg& err){
		Debug::Log(_T("GameManager::Init"), err.GetMsg());
		return;
	}
	
	//[KrLib] アクション.
	{
		//キー操作.
		inputMng->AddAction(_T("GameNext"),		KeyID::Space);
		inputMng->AddAction(_T("GamePause"),	KeyID::P);
		inputMng->AddAction(_T("PlayerDash"),	KeyID::ShiftL); //固定キー機能を切っておくこと推奨.
		inputMng->AddAction(_T("PlayerDash"),	KeyID::ShiftR);
		inputMng->AddAction(_T("MenuUp"),		KeyID::W);
		inputMng->AddAction(_T("MenuUp"),		KeyID::Up);
		inputMng->AddAction(_T("MenuDown"),		KeyID::S);
		inputMng->AddAction(_T("MenuDown"),		KeyID::Down);
		inputMng->AddAction(_T("MenuNext"),		KeyID::Space);
		inputMng->AddAction(_T("MenuNext"),		KeyID::Enter);

#if defined INPUT_CHANGE_ARCADE
		//アーケード操作.
		inputMng->AddAction(_T("GameNext"),		PadArcadeID::BtnUpper1);
		inputMng->AddAction(_T("GamePause"),	PadArcadeID::BtnUpper2);
		inputMng->AddAction(_T("PlayerDash"),	PadArcadeID::BtnUpper3);
		inputMng->AddAction(_T("GameQuit"),		PadArcadeID::BtnStart);
		inputMng->AddAction(_T("MenuUp"),		PadArcadeID::Up);
		inputMng->AddAction(_T("MenuDown"),		PadArcadeID::Down);
		inputMng->AddAction(_T("MenuNext"),		PadArcadeID::BtnUpper1);
#else
		//コントローラ操作.
		inputMng->AddAction(_T("GameNext"),		PadXboxID::A);
		inputMng->AddAction(_T("GamePause"),	PadXboxID::X);
		inputMng->AddAction(_T("PlayerDash"),	PadXboxID::B);
		inputMng->AddAction(_T("GameQuit"),		PadXboxID::Menu);
		inputMng->AddAction(_T("MenuUp"),		PadXboxID::Up);
		inputMng->AddAction(_T("MenuDown"),		PadXboxID::Down);
		inputMng->AddAction(_T("MenuNext"),		PadXboxID::A);
#endif
	}

	//[KrLib] シーン.
	sceneMng->AddScene(&titleScene, _T("Title"));
	sceneMng->AddScene(&menuScene,  _T("Menu"));
	sceneMng->AddScene(&gameScene,  _T("Game"));
	sceneMng->AddScene(&endScene,   _T("End"));

	//フォント作成.
	{
		AddFontFile(_T("Resources/Fonts/Oxanium-VariableFont_wght.ttf"));
		AddFontFile(_T("Resources/Fonts/MPLUSRounded1c-Regular.ttf"));

		gameData->fonts["jp-size1"].Create(_T("Rounded Mplus 1c"), 18, 1, FontTypeID::Anti);
		gameData->fonts["jp-size2"].Create(_T("Rounded Mplus 1c"), 23, 1, FontTypeID::Anti);
		gameData->fonts["jp-size3"].Create(_T("Rounded Mplus 1c"), 30, 1, FontTypeID::Anti);
		gameData->fonts["jp-size4"].Create(_T("Rounded Mplus 1c"), 35, 1, FontTypeID::Anti);
		gameData->fonts["en-size1"].Create(_T("Oxanium ExtraLight"), 20, 1, FontTypeID::Anti);
		gameData->fonts["en-size2"].Create(_T("Oxanium ExtraLight"), 26, 1, FontTypeID::Anti);
		gameData->fonts["en-size3"].Create(_T("Oxanium ExtraLight"), 30, 1, FontTypeID::Anti);
		gameData->fonts["en-size4"].Create(_T("Oxanium ExtraLight"), 32, 1, FontTypeID::Anti);
		gameData->fonts["en-size5"].Create(_T("Oxanium ExtraLight"), 40, 1, FontTypeID::Anti);
	}

	//[score.data]
	File file;
	//ファイルがあれば開く.
	if (file.Open(FILE_DATA_SCORE, FileOpenMode::Read)) {
		gameData->bestScore = file.ReadInt();		//数字を読み込んで登録.
		uiMng->SetBestScore(gameData->bestScore);	//ベストスコア反映.
	}

	//fps表示用.
#if defined DEBUG_SHOW_FPS
	tmFps = TimerMicro(TimerMode::CountUp, 0);
	tmFps.Start();
#endif
}

//リセット(何回でも行う)
void GameManager::Reset() {

	//最初はタイトルシーン.
	sceneMng->SetScene(_T("Title"));

	//管理クラスリセット.
	StopObjects();
	bg->SetAutoExeMode(MngAutoExe::Active);

	//サウンド.
	soundMng->StopAll();

#if !defined BGM_NONE
	//メニューBGMを流す.
	if (auto i = soundMng->Get(_T("BGM_Menu"))) {
		i->Play(true, BGM_VOLUME_MENU);
	}
#endif

	//ゲームシーンのBGM抽選.
	{
		//抽選するBGM名.
		const vector<MY_STRING> bgmName = {
			_T("BGM_Endless_1"),
			_T("BGM_Endless_2"),
			_T("BGM_Endless_3"),
			_T("BGM_Endless_4"),
		};
		//何番目のBGMを使うか(bgmName配列の中から抽選)
		const int bgmNo = Calc::RandNum(0, _int(bgmName.size() - 1));
		//BGM名を保存.
		gameSceneBgm = bgmName[bgmNo];
	}
}

//更新.
void GameManager::Update() {

	//ポーズ操作.
	if (inputMng->IsPushActionTime(_T("GamePause")) == 1) {
		//ゲームシーンのみポーズ可.
		if (sceneMng->GetSceneName() == _T("Game")){

			if (gameData->isPause) {
				GamePauseEnd(); //ポーズ解除.
			}
			else {
				GamePause();    //ポーズする.
			}
		}
	}
	//特定の操作でゲーム終了
	if (inputMng->IsPushActionTime(_T("GameQuit")) >= FPS * 1) {
		App::Quit(); //ボタン長押しで終了.
	}
	else if (inputMng->IsPushKey(KeyID::Esc)) {
		App::Quit(); //ESCAPEキーを押したら即終了.
	}
}

//描画.
void GameManager::Draw() {

	//ポーズ画面.
	if (gameData->isPause) {
		DrawPause();
	}
	
	//コントローラ操作表示.
#if defined DEBUG_CONTR_INPUT
	DrawFormatString(20, WINDOW_HEI-40, 0xFFFFFF, _T("Pad Input: %d"), GetJoypadInputState(DX_INPUT_PAD1));
#endif
	//fps表示用.
#if defined DEBUG_SHOW_FPS
	DrawFormatString(20, WINDOW_HEI-40, 0xFFFFFF, _T("FPS: %f"), tmFps.GetFps());
#endif
}

//ポーズする.
void GameManager::GamePause() {

	gameData->isPause = true;

	bg->Pause();       //背景のポーズ.
	gameScene.Pause(); //ゲームシーンのポーズ.

	sceneMng->SetAutoExeMode(MngAutoExe::DrawOnly); //シーン      : 描画のみ.
	DrawOnlyObjects();                              //オブジェクト: 描画のみ.
}

//ポーズ解除.
void GameManager::GamePauseEnd() {

	gameData->isPause = false;

	bg->PauseEnd();       //背景のポーズ解除.
	gameScene.PauseEnd(); //ゲームシーンのポーズ解除.

	sceneMng->SetAutoExeMode(MngAutoExe::Active);   //シーン      : 稼働.
	RestartObjects();                               //オブジェクト: 稼働.
}
//ポーズ画面.
void GameManager::DrawPause() {

	DrawStr str(_T("PAUSE"), App::GetWindowRect().GetMid(), 0xffffff);
	str.Draw(Anchor::Mid, gameData->fonts["en-size5"].GetFont());
}

//ゲーム終了(死亡)
void GameManager::GameOver() {

	//念のため2重実行されることを防ぐ用.
	if (sceneMng->GetSceneName() != _T("End")) {

		//ステージ別.
		switch (gameData->stage)
		{
			case StageType::Tutorial:
			{
				sceneMng->SetScene(_T("End")); //終了シーンへ.
			}
			break;

			case StageType::Endless:
			{
				sceneMng->SetScene(_T("End")); //終了シーンへ.

				gameData->speedRate = 1.0; //速度倍率を100%に戻す.

				//最終スコア.
				const int finalScore = gameData->GetScore();

				//[score.data]
				if (finalScore > gameData->bestScore) {

					File file;
					//ファイルを開く.
					if (file.Open(FILE_DATA_SCORE, FileOpenMode::Write)) {
						file.WriteInt(finalScore);			//スコアを保存.
						gameData->bestScore = finalScore;	//スコア更新.
						endScene.SignBestScore();			//ハイスコアのサイン送信.
					}
				}

				//[playlog.data]
				WritePlayLog(false);

#if !defined BGM_NONE
				//元のBGMをフェードアウト.
				if (auto i = soundMng->Get(gameSceneBgm)) {
					i->FadeOutPlay(2); //再生.
				}
				//ゲームオーバーBGM再生.
				if (auto i = soundMng->Get(_T("BGM_Over"))) {
					i->Play(true, 68); //再生.
				}
#endif
			}
			break;

			default: assert(0); break;
		}
	}

}

//アイテムを使用した時.
void GameManager::ItemUsed() {

	gameData->isReflectMode = true; //反射モードにする.
	gameScene.ItemUsed();           //アイテム使用処理.
	
	//チュートリアルなら指示送信.
	if (gameData->stage == StageType::Tutorial) {
		tutorialStg->SetTakeItem(true);       //指示を送る.
		tutorialStg->SetReflectFinish(false); //falseにする(指示取り消し)
	}
}

//スローモード開始.
void GameManager::SlowModeStart() {

	//速度倍率を遅くする.
	gameData->speedRate = SLOW_MODE_SPEED;
	//一定時間スローにする.
	gameData->slowBufCntr = SLOW_MODE_BUF_F;
}

//オブジェクト停止.
void GameManager::StopObjects() {

	//管理クラス取得.
	vector<ManagerBase*> mngs = {
		ManagerInsts::Get<Player>(),
		ManagerInsts::Get<LaserManager>(),
		ManagerInsts::Get<ItemManager>(),
		ManagerInsts::Get<NormalLaser>(),
		ManagerInsts::Get<MeteorManager>(),
		ManagerInsts::Get<StraightLaser>(),
		ManagerInsts::Get<Ripples>(),
		ManagerInsts::Get<Fireworks>(),
		ManagerInsts::Get<EndlessStage>(),
		ManagerInsts::Get<TutorialStage>(),
		ManagerInsts::Get<BGManager>(),
	};
	//全ループ.
	for (auto& i : mngs) {
		i->SetAutoExeMode(MngAutoExe::Stop);
	}
}

//オブジェクト描画のみ.
void GameManager::DrawOnlyObjects() {

	//管理クラス取得.
	vector<ManagerBase*> mngs = {
		ManagerInsts::Get<Player>(),
		ManagerInsts::Get<LaserManager>(),
		ManagerInsts::Get<ItemManager>(),
		ManagerInsts::Get<NormalLaser>(),
		ManagerInsts::Get<MeteorManager>(),
		ManagerInsts::Get<StraightLaser>(),
		ManagerInsts::Get<Ripples>(),
		ManagerInsts::Get<Fireworks>(),
		ManagerInsts::Get<EndlessStage>(),
		ManagerInsts::Get<TutorialStage>(),
		ManagerInsts::Get<BGManager>(),
	};
	//全ループ.
	for (auto& i : mngs) {
		//稼働してるクラスをDrawOnlyに変更.
		if (i->GetAutoExeMode() != MngAutoExe::Stop) {
			i->SetAutoExeMode(MngAutoExe::DrawOnly);
		}
	}
}

//オブジェクト稼働再開.
void GameManager::RestartObjects() {

	//管理クラス取得.
	vector<ManagerBase*> mngs = {
		ManagerInsts::Get<Player>(),
		ManagerInsts::Get<LaserManager>(),
		ManagerInsts::Get<ItemManager>(),
		ManagerInsts::Get<NormalLaser>(),
		ManagerInsts::Get<MeteorManager>(),
		ManagerInsts::Get<StraightLaser>(),
		ManagerInsts::Get<Ripples>(),
		ManagerInsts::Get<Fireworks>(),
		ManagerInsts::Get<EndlessStage>(),
		ManagerInsts::Get<TutorialStage>(),
		ManagerInsts::Get<BGManager>(),
	};
	//全ループ.
	for (auto& i : mngs) {
		if (i->GetAutoExeMode() != MngAutoExe::Stop) {
			i->BackAutoExeMode(); //元のモードへ.
		}
	}
}

//プレイログを書き込む.
void GameManager::WritePlayLog(bool isTutorial) {

	DATEDATA date;
	GetDateTime(&date); //現在時刻取得.

	MY_STRING dateStr;
	
	if (isTutorial) {
		dateStr = Format::StrFormat(
			//フォーマット.
			_T("[%d/%0.2d/%0.2d %0.2d:%0.2d.%0.2d] DeviceName:%s / Mode:Tutorial\n"),
			//変数挿入.
			date.Year, date.Mon, date.Day, date.Hour, date.Min, date.Sec,
			Device::GetComputerNameStr().c_str()
		);
	}
	else {
		dateStr = Format::StrFormat(
			//フォーマット.
			_T("[%d/%0.2d/%0.2d %0.2d:%0.2d.%0.2d] DeviceName:%s / Mode:Endless (Level:%d, Score:%0.5d, Time:%.1f)\n"),
			//変数挿入.
			date.Year, date.Mon, date.Day, date.Hour, date.Min, date.Sec,
			Device::GetComputerNameStr().c_str(), gameData->level, gameData->GetScore(), gameScene.GetGameTime()
		);
	}

	//ファイルを開く.
	File file;
	if (file.Open(FILE_DATA_PLAYLOG, FileOpenMode::Out | FileOpenMode::App)) {
		//ファイルへ追記.
		file.WriteString(dateStr);
	}
}
