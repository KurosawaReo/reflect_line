/*
   - UIManager.cpp -
   UI管理.
*/
#include "UIManager.h"

//依存関係.
#include "Stage_Tutorial.h"
#include "GameData.h"
#include "GameManager.h"
//参照.
static GameManager*   gameMng;
static GameData*      gameData;
static TutorialStage* tutorialStg;

// ▼*--=<[ UIManager ]>=--*▼ //

//初期化.
void UIManager::Init() {
	//参照取得.
	gameMng     = ManagerInsts::Get<GameManager>();
	gameData    = ManagerInsts::Get<GameData>();
	tutorialStg = ManagerInsts::Get<TutorialStage>();
}

//リセット.
void UIManager::Reset() {

	//自動実行設定.
	SetAutoExeMode(MngAutoExe::Stop);

	isShowScore = false;
}

//更新.
void UIManager::Update() {

}

//描画.
void UIManager::Draw() {

#if defined DEBUG_SPAWN_RATE
	//カウンター.
	DrawFormatStringToHandle(
		10, WINDOW_HEI-75, 0xFFFFFF, gameData->font2, _T("Counter: %.2f"), gameData->counter
	);
	//出現間隔割合.
	DrawFormatStringToHandle(
		10, WINDOW_HEI-40, 0xFFFFFF, gameData->font2, _T("Spawn  : %.2f%%"), gameData->spawnRate*100
	);
#endif

	//ゲームシーン経過時間.
	const float time = gameMng->GetGameScene()->GetSceneTime();
	//白ラベルのテキストの位置修正.
	const int offset = -1;

	//ステージ別.
	switch (gameData->stage) 
	{
		case StageType::Tutorial:
		{
			//アニメーション値.
			double alpha   = Calc::AnimEase(EaseType::InOutQuad, (time-1.0)*2);
			double alpha2  = Calc::AnimEase(EaseType::InOutQuad,  time-0.2   );
			double animSin = sin(M_PI * (time-0.2));
			//テキスト設定.
			DrawStr str1({}, { WINDOW_WID/2,  70+offset }, 0xFFFFFF);
			DrawStr str2({}, { WINDOW_WID/2, 150        }, COLOR_SCORE);
			
			TCHAR text[256];
			_stprintf(text, _T("STEP %d"),    tutorialStg->GetStepNo());
			str1.text = text;
			_stprintf(text, _T("SCORE:%05d"), gameData->GetScoreNoTime());
			str2.text = text;

			// ===== STEP =====
			//背景画像.
			GraphMng::Get(_T("ui_back_level"))->DrawExtend({ WINDOW_WID/2, 70 }, {0.4, 0.35});
			//描画.
			DrawMode::Exe(
				DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * alpha),
				[&]() {
					str1.Draw(Anchor::Mid, gameData->fonts["en-size5"].GetFont());
				}
			);

			// ===== SCORE =====
			if (isShowScore) {

				DrawMode::Exe(
					DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * alpha2),
					[&]() {
						str2.Draw(Anchor::Mid, gameData->fonts["en-size4"].GetFont());
						GraphMng::Get(_T("ui_back_score"))->DrawExtend({ (double)str2.pos.x, (double)str2.pos.y + 28 }, { 0.35, 0.4 });
					}
				);
				//テキスト(光沢用)
				DrawMode::Exe(
					DrawModeID::None, DrawBlendModeID::Alpha, _int(100 * animSin),
					[&]() {
						str2.color = 0xFFFFFF;
						str2.Draw(Anchor::Mid, gameData->fonts["en-size4"].GetFont());
					}
				);
			}
		}
		break;

		case StageType::Endless:
		{
			//アニメーション値.
			const double alpha1   = Calc::AnimEase(EaseType::InOutQuad, (time - 0.1)    );
			const double alpha2   = Calc::AnimEase(EaseType::InOutQuad, (time - 0.2)    );
			const double alpha3   = Calc::AnimEase(EaseType::InOutQuad, (time - 0.3)    );
			const double alpha4   = Calc::AnimEase(EaseType::InOutQuad, (time - 1.0) * 2);
			const double animSin1 = sin(M_PI * (time - 0.1));
			const double animSin2 = sin(M_PI * (time - 0.2));
			const double animSin3 = sin(M_PI * (time - 0.3));

			//テキスト設定.
			DrawStr str[4] = {
				DrawStr({}, { WINDOW_WID/2,      70+offset }, 0xFFFFFF),
				DrawStr({}, { WINDOW_WID/2-350, 150        }, COLOR_BEST_SCORE),
				DrawStr({}, { WINDOW_WID/2,     150        }, COLOR_SCORE),
				DrawStr({}, { WINDOW_WID/2+350, 150        }, COLOR_TIME),
			};

			//ゲーム時間.
			float fixTime;
			{
				//小数第二位以下は切り捨てる(勝手に四捨五入されてしまうため)
				const float gameTime = gameMng->GetGameScene()->GetGameTime();
				fixTime = floorf(gameTime * 10.0f) / 10.0f;
			}

			TCHAR text[256];
			_stprintf(text, _T("LEVEL %d"),        gameData->level);
			str[0].text = text;
			_stprintf(text, _T("BEST SCORE:%05d"), dispBestScore);
			str[1].text = text;
			_stprintf(text, _T("SCORE:%05d"),      gameData->GetScore());
			str[2].text = text;
			_stprintf(text, _T("TIME:%.1f"),       fixTime);
			str[3].text = text;
		
			//[level]
			{
				DrawMode::Exe(
					DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * alpha4),
					[&]() {
						//画像.
						GraphMng::Get(_T("ui_back_level"))->DrawExtend({WINDOW_WID/2, 70}, {0.4, 0.35});
						//テキスト.
						str[0].Draw(Anchor::Mid, gameData->fonts["en-size5"].GetFont());
					}
				);
			}
			//[best score]
			{
				DrawMode::Exe(
					DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * alpha1),
					[&]() {
						//画像.
						GraphMng::Get(_T("ui_back_best_score"))->DrawExtend(str[1].pos.ToDbl() + DBL_XY(0, 28), { 0.35, 0.4 });
						//テキスト.
						str[1].Draw(Anchor::Mid, gameData->fonts["en-size4"].GetFont());
					}
				);
			}
			//[score]
			{
				DrawMode::Exe(
					DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * alpha2),
					[&]() {
						//画像.
						GraphMng::Get(_T("ui_back_score"))->DrawExtend(str[2].pos.ToDbl() + DBL_XY(0, 28), { 0.35, 0.4 });
						//テキスト.
						str[2].Draw(Anchor::Mid, gameData->fonts["en-size4"].GetFont());
					}
				);
			}
			//[time]
			{
				DrawMode::Exe(
					DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * alpha3),
					[&]() {
						//画像.
						GraphMng::Get(_T("ui_back_time"))->DrawExtend(str[3].pos.ToDbl() + DBL_XY(0, 28), { 0.35, 0.4 });
						//テキスト.
						str[3].Draw(Anchor::Mid, gameData->fonts["en-size4"].GetFont());
					}
				);
			}

			//テキスト(光沢用)
			str[1].color = 0xFFFFFF;
			str[2].color = 0xFFFFFF;
			str[3].color = 0xFFFFFF;
			//タイミング1.
			DrawMode::Exe(
				DrawModeID::None, DrawBlendModeID::Alpha, _int(100 * animSin1),
				[&]() {
					str[1].Draw(Anchor::Mid, gameData->fonts["en-size4"].GetFont());
				}
			);
			//タイミング2.
			DrawMode::Exe(
				DrawModeID::None, DrawBlendModeID::Alpha, _int(100 * animSin2),
				[&]() {
					str[2].Draw(Anchor::Mid, gameData->fonts["en-size4"].GetFont());
				}
			);
			//タイミング3.
			DrawMode::Exe(
				DrawModeID::None, DrawBlendModeID::Alpha, _int(100 * animSin3),
				[&]() {
					str[3].Draw(Anchor::Mid, gameData->fonts["en-size4"].GetFont());
				}
			);
		}
		break;
			
		default: assert(FALSE); break;
	}
}