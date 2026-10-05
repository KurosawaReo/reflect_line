/*
   - Scene_End.cpp -
*/
#include "Scene_End.h"

//依存関係.
#include "GameManager.h"
#include "GameData.h"
#include "Player.h"
#include "UIManager.h"
#include "BGManager.h"
//参照.
static GameManager* gameMng;
static GameData*    gameData;
static Player*      player;
static UIManager*   uiMng;
static BGManager*   bgMng;
//参照(KRライブラリ)
static SoundMng*    soundMng;
static InputMng*    inputMng;
static SceneMng*    sceneMng;

//初期化.
void EndScene::Init() {

	//参照取得.
	gameMng  = ManagerInsts::Get<GameManager>();
	gameData = ManagerInsts::Get<GameData>();
	player   = ManagerInsts::Get<Player>();
	uiMng    = ManagerInsts::Get<UIManager>();
	bgMng    = ManagerInsts::Get<BGManager>();
	soundMng = ManagerInsts::Get<SoundMng>();
	inputMng = ManagerInsts::Get<InputMng>();
	sceneMng = ManagerInsts::Get<SceneMng>();

	//タイマー.
	timer = Timer(TimerMode::CountUp, 0);
}

//リセット.
void EndScene::Reset() {
	isBestScore      = false;
	isBestScoreSound = false;
}

//入った瞬間.
void EndScene::Enter() {

	timer.Start(); //タイマー開始.

	//チュートリアル限定.
	if (gameData->stage == StageType::Tutorial) {

		//一定時間経過したら.
		ManagerInsts::Get<TimerMng>()->ReservExe(
			TUTORIAL_RESPAWN_TIME,
			[this]() {
				sceneMng->SetScene(_T("Game")); //ゲームシーンへ戻る.
			}
		);
	}
	//チュートリアル以外.
	else {
		bgMng->Pause();             //背景停止.
		gameMng->DrawOnlyObjects(); //オブジェクト動作停止.
	}
}

//抜けた瞬間.
void EndScene::Exit() {
	timer.Reset(); //タイマーリセット.
}

//更新.
void EndScene::Update() {

	//チュートリアル限定.
	if (gameData->stage == StageType::Tutorial) {
		//ゲームシーンと同じ動作をする.
		gameMng->GetGameScene()->Update();
	}
	//チュートリアル以外.
	else {
		//特定の操作でタイトルへ.
		if (inputMng->IsPushActionTime(_T("GameNext")) == 1)
		{
			uiMng->SetBestScore(gameData->bestScore);	//ベストスコア更新.
			sceneMng->SetScene(_T("Title"));			//ゲームシーンへ戻る.
			App::Reset();								//リセット.
		}
	}
}

//描画.
void EndScene::Draw() {

	//黒フィルター.
	{
		double anim = Calc::AnimEase(EaseType::InOutQuad, timer.GetPassTime()); //アニメーション値.
		Box box = { {0, 0}, {WINDOW_WID, WINDOW_HEI}, 0x000000, 1.0f };

		//描画.
		DrawMode::Exe(
			DrawModeID::None, DrawBlendModeID::Alpha, _int(128 * anim),
			[&]() {
				DrawBoxKR(box, Anchor::LU); //画面を暗くする(UI以外)
			}
		);
	}

	//チュートリアルの場合.
	if (gameData->stage == StageType::Tutorial) {

		//アニメーション値.
		double anim = Calc::AnimEase(EaseType::OutQuad, timer.GetPassTime());
		//テキスト.
		DrawStr str(_T("チュートリアルではこの場で復活します..."), { WINDOW_WID / 2, WINDOW_HEI / 2 }, 0x00FFFF);

		//描画.
		DrawMode::Exe(
			DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * anim),
			[&]() {
				//GAME OVER.
				GraphMng::Get(_T("gameover"))->DrawExtend({ WINDOW_WID / 2, 370 + 30 * anim }, { 0.5, 0.5 }, Anchor::Mid, true, true);
				//テキスト.
				str.Draw(Anchor::Mid, gameData->fonts["jp-size3"].GetFont());
			}
		);
	}
	//チュートリアル以外の場合.
	else {
		//終了案内.
		{
			//アニメーション値.
			const double anim = Calc::AnimEase(EaseType::OutQuad, timer.GetPassTime());

			//ゲーム時間.
			float fixTime;
			{
				//小数第二位以下は切り捨てる(勝手に四捨五入されてしまうため)
				const float gameTime = gameMng->GetGameScene()->GetGameTime();
 				fixTime = floorf(gameTime * 10.0f) / 10.0f;
			}

			const int font = gameData->fonts["en-size2"].GetFont();
			const int lineSpace = 40;

			//テキスト.
			MY_STRING texts[] = {
				_T("Time Bonus"),
				Format::StrFormat(
					_T("%d + %d (%.1f sec) = %d"), 
					gameData->GetScoreNoTime(), _int(fixTime * 10), fixTime, gameData->GetScore()
				),
			};

			DrawStr str = { _T(""), { WINDOW_WID / 2, WINDOW_HEI / 2 - 20 }, 0xFFFFFF };
			//テキストサイズ.
			const INT_XY textSize = GetTextSize(texts[1], font);
			//背景用四角形.
			Box box = { str.pos.ToDbl() + DBL_XY(0, lineSpace/2), DBL_XY(textSize.x + 50, 100), 0x000000, 1};

			//描画(背景)
			DrawMode::Exe(
				DrawModeID::None, DrawBlendModeID::Alpha, _int(128 * anim),
				[&]() {
					//枠線.
					DrawBoxKR(box, Anchor::Mid, true, true);
					box.color = 0xFFFFFF;
					DrawBoxKR(box, Anchor::Mid, false, true);
				}
			);
			//描画.
			DrawMode::Exe(
				DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * anim),
				[&]() {

					//GAME OVER.
					GraphMng::Get(_T("gameover"))->DrawExtend({ WINDOW_WID / 2, 370 + 30 * anim }, { 0.5, 0.5 }, Anchor::Mid, true, true); //GAME OVER

					//テキスト(1行ずつ表示)
					for (auto& i : texts) {
						str.text = i;
						str.Draw(Anchor::Mid, font);
						str.pos.y += lineSpace; //次の行へ.
					}
				}
			);
		}

		const float delay1 = 1.2f;
		const float delay2 = 1.5f;

		//一定時間が経ったら.
		if (timer.GetPassTime() > delay1) {
			//ベストスコア更新.
			if (isBestScore) {

				//アニメーション値.
				double anim = Calc::AnimEase(EaseType::OutQuad, (timer.GetPassTime() - delay1) * 2);
				//描画.
				DrawMode::Exe(
					DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * anim),
					[&]() {
						//NEW RECORD
						GraphMng::Get(_T("new_record"))->DrawExtend(
							{ WINDOW_WID / 2, WINDOW_HEI / 2 - 330 + anim * 20 }, { 0.4, 0.4 }, Anchor::Mid, true, true
						);
					}
				);
				//サウンド.
				if (!isBestScoreSound) {
					if (auto i = soundMng->Get(_T("BestScore"))) {
						i->Play(false, 65); //再生.
					}
					isBestScoreSound = true; //一度のみ.
				}
			}
		}
		//一定時間が経ったら.
		if (timer.GetPassTime() > delay2) {

			//アニメーション値.
			double anim = Calc::AnimWave(WaveType::CosLoop, timer.GetPassTime() - delay2);
			//テキスト.
			DrawStr str(_T("Push [SPACE] [A Button]"), { WINDOW_WID / 2, WINDOW_HEI / 2 + 145 }, 0xFFFFFF);

			//描画.
			DrawMode::Exe(
				DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * anim),
				[&]() {
					str.Draw(Anchor::Mid, gameData->fonts["en-size2"].GetFont()); //テキスト.
				}
			);
		}
	}
}