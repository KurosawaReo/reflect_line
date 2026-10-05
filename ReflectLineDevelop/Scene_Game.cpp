/*
   - Scene_Game.cpp -
*/
#include "Scene_Game.h"

//依存関係.
#include "GameData.h"
#include "GameManager.h"
#include "Player.h"
#include "BGManager.h"
#include "UIManager.h"
#include "Stage_Tutorial.h"
#include "Stage_Endless.h"
//参照.
static GameData*	  gameData;
static GameManager*	  gameMng;
static Player*		  player;
static BGManager*	  bgMng;
static UIManager*	  uiMng;
static TutorialStage* tutorialStg;
//参照(KRライブラリ)
static SoundMng*      soundMng;

//初期化.
void GameScene::Init() {
	//参照取得.
	gameData    = ManagerInsts::Get<GameData>();
	gameMng     = ManagerInsts::Get<GameManager>();
	player      = ManagerInsts::Get<Player>();
	bgMng       = ManagerInsts::Get<BGManager>();
	uiMng       = ManagerInsts::Get<UIManager>();
	tutorialStg = ManagerInsts::Get<TutorialStage>();
	soundMng    = ManagerInsts::Get<SoundMng>();

	timer         = Timer(TimerMode::CountUp, 0);
	tmGameTime    = Timer(TimerMode::CountUp, 0);
	tmReflectMode = Timer(TimerMode::CountDown, REFLECT_MODE_TIME);
}

//リセット.
void GameScene::Reset() {

	//タイマーリセット.
	timer.Reset();
	tmGameTime.Reset();
	tmReflectMode.Reset();

	itemSoundCnt = 0;
	isGameStart = false;
}

//入った瞬間.
void GameScene::Enter() {

	//プレイヤーが死亡していたら.
	if (!player->GetActive()) {
		player->Revival(); //復活.
	}

	//まだ動いてなければ.
	if (timer.GetState() != TimerState::Active) {
		timer.Start(); //タイマー開始.
	}
}

//抜けた瞬間.
void GameScene::Exit() {
	//チュートリアル以外のみ.
	if (gameData->stage != StageType::Tutorial) {

		itemSoundCnt = 0;
		timer.Stop();          //演出用タイマー停止.
		tmGameTime.Stop();     //ゲーム時間停止.
		tmReflectMode.Reset(); //反射モードリセット.
	}
}

//更新.
void GameScene::Update() {

	//ゲーム開始前.
	if (!isGameStart) {

		ManagerInsts::Get<Player>()->SetAutoExeMode(MngAutoExe::Active);
		ManagerInsts::Get<UIManager>()->SetAutoExeMode(MngAutoExe::Active);

		//一定時間経ったら.
		if (timer.GetPassTime() >= GAME_START_TIME) {
			tmGameTime.Start(); //ゲーム時間計測開始.
			isGameStart = true; //ゲーム開始.
			//ステージ別.
			switch (gameData->stage)
			{
				case StageType::Tutorial:
					ManagerInsts::Get<TutorialStage>()->SetAutoExeMode(MngAutoExe::Active);
					break;
				case StageType::Endless:
					ManagerInsts::Get<EndlessStage>()-> SetAutoExeMode(MngAutoExe::Active);
					break;

				default: assert(false); break;
			}
		}
	}
	//ゲーム開始後.
	else {
		UpdateReflectMode(); //反射モード.
	}
}

//描画.
void GameScene::Draw() {
	DrawReflectMode();  //反射モード演出.
}

//ポーズ.
void GameScene::Pause() {
	tmGameTime.Pause();
	tmReflectMode.Pause();
}

//ポーズ解除.
void GameScene::PauseEnd() {
	//稼働中だったタイマーは再開.
	if (tmGameTime.GetState() == TimerState::Pause) {
		tmGameTime.Start();
	}
	if (tmReflectMode.GetState() == TimerState::Pause) {
		tmReflectMode.Start();
	}
}

//反射モードの更新.
void GameScene::UpdateReflectMode() {

	//反射モード時間判定.
	if (gameData->slowBufCntr > 0) {
		gameData->slowBufCntr--; //カウントを減らす.

		//0になったら.
		if (gameData->slowBufCntr <= 0) {
			gameData->speedRate = 1.0;         //速度倍率を戻す.
			player->SetIsDashRefEffect(false); //ダッシュ反射演出終了.
		}
	}

	//反射モード中.
	if (tmReflectMode.GetState() == TimerState::Active) {

		switch (itemSoundCnt) 
		{
			case 0:
				//3秒以下になったばかりの時.
				if (tmReflectMode.GetPassTime() <= 3) {
					if (auto i = soundMng->Get(_T("CountDown"))){
						i->Play(false, 70); //再生.
					}
					itemSoundCnt++; //次へ.
				}
				break;
			case 1:
				//2秒以下になったばかりの時.
				if (tmReflectMode.GetPassTime() <= 2) {
					if (auto i = soundMng->Get(_T("CountDown"))) {
						i->Play(false, 70); //再生.
					}
					itemSoundCnt++; //次へ.
				}
				break;
			case 2:
				//1秒以下になったばかりの時.
				if (tmReflectMode.GetPassTime() <= 1) {
					if (auto i = soundMng->Get(_T("CountDown"))) {
						i->Play(false, 70); //再生.
					}
					itemSoundCnt++; //次へ.
				}
				break;
			case 3:
				//時間切れで解除.
				if (tmReflectMode.GetPassTime() <= 0) {
					ReflectModeEnd();
				}
				break;
		}
	}
}
//反射モードの描画.
void GameScene::DrawReflectMode() {

	//カウントダウン中.
	if (tmReflectMode.GetState() == TimerState::Active &&
		tmReflectMode.GetPassTime() > 0)
	{
		//テキストの設定.
		MY_STRING text = NumToString((int)ceil(tmReflectMode.GetPassTime()));
		DrawStr str(text, { WINDOW_WID / 2, WINDOW_HEI / 2 }, COLOR_ITEM);

		//画面中央に数字を表示.
		{
			double dec  = Calc::GetDecimal(tmReflectMode.GetPassTime()); //小数だけ取り出す.
			double anim = Calc::AnimEase(EaseType::OutQuad, dec);

			//描画.
			DrawMode::Exe(
				DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * dec),
				[&]() {
					//最初の1秒.
					if (tmReflectMode.GetPassTime() > REFLECT_MODE_TIME - 1) {
						GraphMng::Get(_T("reflect_text"))->DrawExtend(
							{ WINDOW_WID / 2, WINDOW_HEI / 2 }, { 0.3 + 0.2 * anim, 0.3 + 0.2 * anim }, Anchor::Mid, true, true
						);
					}
					//最後の3秒.
					if (tmReflectMode.GetPassTime() <= 3) {
						str.Draw(Anchor::Mid, gameData->fonts["en-size5"].GetFont()); //数字.
					}
				}
			);
		}
	}
}
//反射モード終了.
void GameScene::ReflectModeEnd() {

	tmReflectMode.Reset();

	gameData->isReflectMode = false; //反射モード解除.
	gameData->speedRate = 1.0;		 //速度倍率を100%に戻す.
	gameData->slowBufCntr = 0;		 //カウンターを0に.
	itemSoundCnt = 0;
	player->SetMode(PlayerMode::Normal);  //通常状態に戻す.

	//効果終了音.
	if (auto i = soundMng->Get(_T("PowerDown"))) {
		i->Play(false, 78); //再生.
	}
	//チュートリアルなら指示送信.
	if (gameData->stage == StageType::Tutorial) {
		tutorialStg->SetReflectFinish(true); //指示を送る.
	}
}

//アイテム使用.
void GameScene::ItemUsed() {
	tmReflectMode.Start(); //反射タイマー開始.
	itemSoundCnt = 0;
}