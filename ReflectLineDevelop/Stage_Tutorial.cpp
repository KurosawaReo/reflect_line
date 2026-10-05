/*
   - Stage_Tutorial.cpp -   
*/
#include "Stage_Tutorial.h"

//依存関係.
#include "LaserManager.h"
#include "Obst_NormalLaser.h"
#include "Obst_StraightLaser.h"
#include "Obst_MeteorManager.h"
#include "Obst_Ripples.h"
#include "Obst_Fireworks.h"
#include "Item.h"
#include "Player.h"
#include "EffectManager.h"
#include "UIManager.h"
#include "GameData.h"
#include "GameManager.h"
//参照.
static GameData*		gameData;
static GameManager*		gameMng;
static LaserManager*	laserMng;
static MeteorManager*	meteorMng;
static Ripples*			ripples;
static ItemManager*		itemMng;
static Player*			player;
static Fireworks*		fireworksMng;
static EffectManager*	effectMng;
static UIManager*		uiMng;
//参照(KRライブラリ)
static SoundMng*		soundMng;
static SceneMng*		sceneMng;
static TimerMng*		timerMng;

constexpr int MOVIE_BACK_ALPHA = 160; //動画再生中の暗転の透明度.

// ▼*--=<[ TutorialStage ]>=--*▼ //

//初期化.
void TutorialStage::Init() {

	//参照取得.
	gameData	 = ManagerInsts::Get<GameData>();
	gameMng		 = ManagerInsts::Get<GameManager>();
	laserMng	 = ManagerInsts::Get<LaserManager>();
	meteorMng	 = ManagerInsts::Get<MeteorManager>();
	ripples		 = ManagerInsts::Get<Ripples>();
	itemMng		 = ManagerInsts::Get<ItemManager>();
	player		 = ManagerInsts::Get<Player>();
	fireworksMng = ManagerInsts::Get<Fireworks>();
	effectMng	 = ManagerInsts::Get<EffectManager>();
	uiMng        = ManagerInsts::Get<UIManager>();
	soundMng	 = ManagerInsts::Get<SoundMng>();
	sceneMng	 = ManagerInsts::Get<SceneMng>();
	timerMng	 = ManagerInsts::Get<TimerMng>();

	//タイマー
	startTimer = Timer(TimerMode::CountUp, 0);
	endTimer   = Timer(TimerMode::CountUp, 0);
}

//リセット.
void TutorialStage::Reset() {

	startTimer.Reset();
	endTimer.Reset();

	//後からstep1にする.
	stepNo      = 0;
	stepInnerNo = 0;

	plyMoveCntr = 0; 
	ResetSignFlag(); //signフラグoff.
}

//更新.
void TutorialStage::Update() {

	//透明度計算.
	{
		//開始タイマーと終了タイマーの組み合わせでアニメーションを作る.
		double alpha1 = Calc::AnimEase(EaseType::InQuad, (startTimer.GetPassTime() - 0.5) * 2);
		double alpha2 = Calc::AnimEase(EaseType::OutQuad, endTimer.GetPassTime() * 2);
		//同時に作動しても繋がるようにする.
		drawAlpha = alpha1 * (1 - alpha2);
	}

	//step進行後一定時間が経過 & エンドタイマーが起動してない.
	if (startTimer.GetPassTime() >= TUTORIAL_START_WAIT_TIME &&
		endTimer.GetState() != TimerState::Active
	){
		//終了条件を満たしていれば.
		if (IsEnd(stepNo, stepInnerNo)) {

			endTimer.Start();

#if !defined BGM_NONE
			//最後のみ.
			if (stepNo == 4 && stepInnerNo == 1) {
				//BGMフェードアウト.
				if (auto i = soundMng->Get(_T("BGM_Tutorial"))) {
					i->FadeOutPlay(TUTORIAL_END_NEXT_TIME);
				}
			}
#endif
		}
	}

	//ステップ別.
	switch (stepNo)
	{
		case 0: UpdateStep0(); break;
		case 1: UpdateStep1(); break;
		case 2: UpdateStep2(); break;
		case 3: UpdateStep3(); break;
		case 4: UpdateStep4(); break;

		default: assert(FALSE); break;
	}
}

//描画.
void TutorialStage::Draw() {

	//ステップ別.
	switch (stepNo)
	{
		case 0:              break; //描画なし.
		case 1: DrawStep1(); break;
		case 2: DrawStep2(); break;
		case 3: DrawStep3(); break;
		case 4: DrawStep4(); break;

		default: assert(false); break;
	}
}

//更新:step0
void TutorialStage::UpdateStep0() {

	//サウンド.
	if (auto i = soundMng->Get(_T("LevelUp"))) {
		i->Play(false, 100); //再生.
	}
	//エフェクト.
	EffectData data{};
	data.type = EffectType::Tutorial_Step1;
	data.pos  = { WINDOW_WID/2, WINDOW_HEI/2 };
	effectMng->SpawnEffect(&data);

	//オブジェクト.
	ManagerInsts::Get<LaserManager>()->SetAutoExeMode(MngAutoExe::Active);

	startTimer.Start(); //開始.
	stepNo++;           //次のステップ.
	stepInnerNo = 0;
}

//更新:step1
void TutorialStage::UpdateStep1() {

	//ステップ内項目.
	switch (stepInnerNo)
	{
		case 0:
		{
			//移動したらカウンターを+1
			if (player->IsMoved()) {
				plyMoveCntr += 1;
			}

			//次に進行する時の処理.
			if (endTimer.GetPassTime() >= TUTORIAL_END_NEXT_TIME) {
				//終了処理.
				StepInnerEnd();
				stepInnerNo++;
			}
		}
		break;

		case 1:
		{
			//次に進行する時の処理.
			if (endTimer.GetPassTime() >= TUTORIAL_END_NEXT_TIME) {
				
				//オブジェクト.
				ManagerInsts::Get<NormalLaser>()->UseLaserPointCnt(1); //レーザー数 = 1.
				//オブジェクト(遅延実行)
				timerMng->ReservExe(
					1.0f, []() {
						ManagerInsts::Get<NormalLaser>()->SetAutoExeMode(MngAutoExe::Active);
					}
				);
	
				//終了処理.
				StepInnerEnd();
				stepInnerNo++;
			}
		}
		break;

		case 2:
		{
			//次に進行する時の処理.
			if (endTimer.GetPassTime() >= TUTORIAL_END_NEXT_TIME) {

				//サウンド.
				if (auto i = soundMng->Get(_T("LevelUp"))) {
					i->Play(false, 100); //再生.
				}

				//エフェクト.
				EffectData data{};
				data.type = EffectType::Tutorial_Step2;
				data.pos  = {WINDOW_WID/2, WINDOW_HEI/2};
				effectMng->SpawnEffect(&data);

				//オブジェクト.
				ManagerInsts::Get<NormalLaser>()->SetAutoExeMode(MngAutoExe::Stop);
				ManagerInsts::Get<ItemManager>()->SpawnNow();
				//オブジェクト(遅延実行)
				timerMng->ReservExe(
					1.0f, []() {
						ManagerInsts::Get<ItemManager>()->SetAutoExeMode(MngAutoExe::Active);
					}
				);

				//終了処理.
				StepInnerEnd();
				stepNo++;
				stepInnerNo = 0;
			}
		}
		break;

		default: assert(false); break;
	}
}

//更新:step2
void TutorialStage::UpdateStep2() {

	//ステップ内項目.
	switch (stepInnerNo) 
	{
		case 0:
		{
			//次に進行する時の処理.
			if (endTimer.GetPassTime() >= TUTORIAL_END_NEXT_TIME) {

				//オブジェクト.
				ManagerInsts::Get<ItemManager>()->SetAutoExeMode(MngAutoExe::Stop);

				//終了処理.
				StepInnerEnd();
				stepInnerNo++;
			}
		}
		break;

		case 1:
		{
			//次に進行する時の処理.
			if (endTimer.GetPassTime() >= TUTORIAL_END_NEXT_TIME) {

				//サウンド.
				if (auto i = soundMng->Get(_T("LevelUp"))) {
					i->Play(false, 100); //再生.
				}

				//エフェクト.
				EffectData data{};
				data.type = EffectType::Tutorial_Step3;
				data.pos  = {WINDOW_WID/2, WINDOW_HEI/2};
				effectMng->SpawnEffect(&data);

				//オブジェクト.
				ManagerInsts::Get<ItemManager>()->SpawnNow();
				//オブジェクト(遅延実行)
				timerMng->ReservExe(
					1.0f, []() {
						ManagerInsts::Get<ItemManager>()->SetAutoExeMode(MngAutoExe::Active);
					}
				);

				//終了処理.
				StepInnerEnd();
				stepNo++;
				stepInnerNo = 0;
			}
		}
		break;

		default: assert(false); break;
	}
}

//更新:step3
void TutorialStage::UpdateStep3() {

	//ステップ内項目.
	switch (stepInnerNo) 
	{
		case 0:
		{
			//次に進行する時の処理.
			if (endTimer.GetPassTime() >= TUTORIAL_END_NEXT_TIME) {

				//オブジェクト.
				ManagerInsts::Get<NormalLaser>()->Reset();             //リセットしておく.
				ManagerInsts::Get<NormalLaser>()->UseLaserPointCnt(1); //レーザー数 = 1.
				//オブジェクト(遅延実行)
				timerMng->ReservExe(
					1.0f, []() {
						ManagerInsts::Get<NormalLaser>()->SetAutoExeMode(MngAutoExe::Active);
					}
				);
				
				//終了処理.
				StepInnerEnd();
				stepInnerNo++;
			}
		}
		break;

		case 1:
		{
			//次に進行する時の処理.
			if (endTimer.GetPassTime() >= TUTORIAL_END_NEXT_TIME) {

				//オブジェクト.
				ManagerInsts::Get<MeteorManager>()->SetAutoExeMode(MngAutoExe::Active);
				//オブジェクト(遅延実行)
				timerMng->ReservExe(
					1.0f, []() {
						ManagerInsts::Get<ItemManager>()->SpawnNow();
					}
				);

				//終了処理.
				StepInnerEnd();
				stepInnerNo++;
			}
		}
		break;

		case 2:
		{
			//次に進行する時の処理.
			if (endTimer.GetPassTime() >= TUTORIAL_END_NEXT_TIME) {

				//動画用.
				timerMng->ReservExe(
					1.0f, []() {
						//動画.
						GraphMng::Get(_T("reflect_mode_death"))->PlayMovie(PlayTypeID::Loop);
						//ゲーム速度.
						gameData->speedRate = 0.1f;
						//無敵にする.
						player->SetIsNoDeath(true);
					}
				);

				//終了処理.
				StepInnerEnd();
				stepInnerNo++;
			}
		}
		break;


		case 3:
		{
			//次に進行する時の処理.
			if (endTimer.GetPassTime() >= TUTORIAL_END_NEXT_TIME) {

				//動画.
				GraphMng::Get(_T("reflect_mode_death"))->StopMovie();

				timerMng->ReservExe(
					1.0f, []() {
						GraphMng::Get(_T("dash_reflect"))->PlayMovie(PlayTypeID::Loop);
					}
				);

				//終了処理.
				StepInnerEnd();
				stepInnerNo++;
			}
		}
		break;

		case 4:
		{
			//次に進行する時の処理.
			if (endTimer.GetPassTime() >= TUTORIAL_END_NEXT_TIME) {

				//サウンド.
				if (auto i = soundMng->Get(_T("LevelUp"))) {
					i->Play(false, 100); //再生.
				}

				//エフェクト.
				EffectData data{};
				data.type = EffectType::Tutorial_Step4;
				data.pos  = {WINDOW_WID/2, WINDOW_HEI/2};
				effectMng->SpawnEffect(&data);

				//スコア表示.
				gameData->ResetScore();
				uiMng->SetIsShowScore(true);

				//動画用.
				GraphMng::Get(_T("dash_reflect"))->StopMovie();
				gameData->speedRate = 1.0f;  //ゲーム速度.
				player->SetIsNoDeath(false); //無敵解除.

				//終了処理.
				StepInnerEnd();
				stepNo++;
				stepInnerNo = 0;
			}
		}
		break;

		default: assert(false); break;
	}
}

//更新:step4
void TutorialStage::UpdateStep4() {

	//ステップ内項目.
	switch (stepInnerNo) 
	{
		case 0:
		{
			//次に進行する時の処理.
			if (endTimer.GetPassTime() >= TUTORIAL_END_NEXT_TIME) {

				//オブジェクト.
				ManagerInsts::Get<ItemManager>()->SetAutoExeMode(MngAutoExe::Stop);
				ManagerInsts::Get<NormalLaser>()->SetAutoExeMode(MngAutoExe::Stop);
				ManagerInsts::Get<LaserManager>()->SetAutoExeMode(MngAutoExe::Stop);
				ManagerInsts::Get<MeteorManager>()->SetAutoExeMode(MngAutoExe::Stop);

				//スコア非表示.
				uiMng->SetIsShowScore(false);

				//終了処理.
				StepInnerEnd();
				stepInnerNo++;
			}
		}
		break;

		case 1:
		{
			//チュートリアル終了.
			if (endTimer.GetPassTime() >= TUTORIAL_END_NEXT_TIME) {

				sceneMng->SetScene(_T("Title"));
				App::Reset(); //全てリセット.
			}
		}
		break;

		default: assert(false); break;
	}
}

//描画:step1
void TutorialStage::DrawStep1() {

    //ステップ内項目.
    switch (stepInnerNo)
    {
        case 0:
        {
            DrawTopText(1, drawAlpha, _T("移動する"));
            DrawTopText(2, drawAlpha, _T("キーボード　　 : WASD / ↑↓←→"));
            DrawTopText(3, drawAlpha, _T("コントローラー : 左スティック　 "));
			DrawTopText(4, drawAlpha, _T("アーケード　　 : レバー　　　　 "));
        }
        break;

		case 1:
		{
			DrawTopText(1, drawAlpha, _T("ダッシュする"));
			DrawTopText(2, drawAlpha, _T("キーボード　　 : SHIFTキー "));
			DrawTopText(3, drawAlpha, _T("コントローラー : Bボタン　 "));
			DrawTopText(4, drawAlpha, _T("アーケード　　 : 右上ボタン"));
		}
		break;

        case 2:
        {
            DrawTopText(1, drawAlpha, _T("障害物をさける"));
            DrawTopText(2, drawAlpha, _T("青いものは障害物です。当たるとゲームオーバーになります。"));
        }
        break;

		default: assert(false); break;
    }
}

//描画:step2
void TutorialStage::DrawStep2() {

    //ステップ内項目.
    switch (stepInnerNo)
    {
        case 0:
        {
            DrawTopText(1, drawAlpha, _T("アイテムをとる"));
            DrawTopText(2, drawAlpha, _T("アイテムは上から降ってきます。触れるだけで取れます。"));
        }
        break;

        case 1:
        {
            DrawTopText(1, drawAlpha, _T("アイテム発動"));
            DrawTopText(2, drawAlpha, _T("アイテムを取ると効果が発動し、一定時間経つと解除されます。"));
        }
        break;

		default: assert(false); break;
    }
}

//描画:step3
void TutorialStage::DrawStep3() {

    //ステップ内項目.
    switch (stepInnerNo) 
    {
        case 0:
        {
            DrawTopText(1, drawAlpha, _T("反射モード"));
            DrawTopText(2, drawAlpha, _T("アイテムを取ると「反射モード」へ変化します。"));
        }
        break;

        case 1:
        {
            DrawTopText(1, drawAlpha, _T("レーザーを反射する"));
            DrawTopText(2, drawAlpha, _T("反射モード中は、「レーザー」に当たると跳ね返せます。"));
        }
        break;

		case 2:
		{
			DrawTopText(1, drawAlpha, _T("隕石をこわす"));
			DrawTopText(2, drawAlpha, _T("跳ね返したレーザーは、「隕石」に向かって自動で飛んでいきます。"));
		}
		break;

		case 3:
		{
			//背景描画.
			DrawMode::Exe(
				DrawModeID::None, DrawBlendModeID::Alpha, _int(MOVIE_BACK_ALPHA * drawAlpha),
				[]() {
					Box box = { App::GetWindowRect().GetMid().ToDbl(), App::GetWindowSize().ToDbl(), 0x000000, 0 };
					DrawBoxKR(box);
				}
			);
			
			DrawTopText(1, drawAlpha, _T("反射モードの注意"));
			DrawTopText(2, drawAlpha, _T("反射モード中でも、レーザー以外に当たると死にます。"));
			DrawTopText(3, drawAlpha, _T("無敵ではありません。"));

			//動画描画.
			DrawMode::Exe(
				DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * drawAlpha),
				[&]() {
					//座標 & サイズ.
					const DBL_XY pos  = App::GetWindowRect().GetMid().ToDbl() + DBL_XY(0, 100);
					const DBL_XY size = GraphMng::Get(_T("reflect_mode_death"))->GetSize().ToDbl();

					//動画.
					GraphMng::Get(_T("reflect_mode_death"))->Draw(pos);
					//枠線.
					GradLine gradLine;
					gradLine.AddPoint(pos + DBL_XY(-size.x/2, -size.y/2), {0, 255, 255, _int_r(255 * drawAlpha)});
					gradLine.AddPoint(pos + DBL_XY(+size.x/2, -size.y/2), { 0, 100, 255, _int_r(255 * drawAlpha) });
					gradLine.AddPoint(pos + DBL_XY(+size.x/2, +size.y/2), { 0, 255, 255, _int_r(255 * drawAlpha) });
					gradLine.AddPoint(pos + DBL_XY(-size.x/2, +size.y/2), { 0, 100, 255, _int_r(255 * drawAlpha) });
					gradLine.Draw(true);
				}
			);
		}
		break;

		case 4:
		{
			//背景描画.
			DrawMode::Exe(
				DrawModeID::None, DrawBlendModeID::Alpha, _int(MOVIE_BACK_ALPHA * drawAlpha),
				[]() {
					Box box = { App::GetWindowRect().GetMid().ToDbl(), App::GetWindowSize().ToDbl(), 0x000000, 0 };
					DrawBoxKR(box);
				}
			);

			DrawTopText(1, drawAlpha, _T("ダッシュ反射"));
			DrawTopText(2, drawAlpha, _T("ダッシュした瞬間にレーザーに当たっても跳ね返せます。"));
			DrawTopText(3, drawAlpha, _T("腕に自信がある人は挑戦してみてください！"));

			//動画描画.
			DrawMode::Exe(
				DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * drawAlpha),
				[&]() {
					//座標 & サイズ.
					const DBL_XY pos = App::GetWindowRect().GetMid().ToDbl() + DBL_XY(0, 100);
					const DBL_XY size = GraphMng::Get(_T("dash_reflect"))->GetSize().ToDbl();

					//動画.
					GraphMng::Get(_T("dash_reflect"))->Draw(pos);
					//枠線.
					GradLine gradLine;
					gradLine.AddPoint(pos + DBL_XY(-size.x / 2, -size.y / 2), { 0, 255, 255, _int_r(255 * drawAlpha) });
					gradLine.AddPoint(pos + DBL_XY(+size.x / 2, -size.y / 2), { 0, 100, 255, _int_r(255 * drawAlpha) });
					gradLine.AddPoint(pos + DBL_XY(+size.x / 2, +size.y / 2), { 0, 255, 255, _int_r(255 * drawAlpha) });
					gradLine.AddPoint(pos + DBL_XY(-size.x / 2, +size.y / 2), { 0, 100, 255, _int_r(255 * drawAlpha) });
					gradLine.Draw(true);
				}
			);
		}
		break;

		default: assert(false); break;
    }
}

//描画:step4
void TutorialStage::DrawStep4() {

    //ステップ内項目.
    switch (stepInnerNo) 
    {
        case 0:
        {
            DrawTopText(2, drawAlpha, _T("最後にスコアを2000点稼いでみましょう。"));
			DrawTopText(3, drawAlpha, _T("アイテムをとる:+100  隕石をこわす:+500"));
        }
        break;

		case 1:
		{
			DrawTopText(2, drawAlpha, _T("これでチュートリアルは以上です。"));
			DrawTopText(3, drawAlpha, _T("ハイスコアを目指して頑張ってください！"));
		}
		break;

		default: assert(false); break;
    }
} 

/*
   画面上にテキストを出す.
   alpha: 0.0～1.0
   line : 行数
*/
void TutorialStage::DrawTopText(int line, double alpha, MY_STRING text) {

	//テキスト設定.
	DrawStr str(text, {WINDOW_WID/2, 0}, {});
	switch (line) {
		case 1: 
			str.pos.y = 155;
			str.color = {  0, 255, 255};
			break;
		case 2: 
			str.pos.y = 155+70;
			str.color = {255, 255, 255};
			break;
		case 3: 
			str.pos.y = 155+70+55;
			str.color = {255, 255, 255};
			break;
		case 4: 
			str.pos.y = 155+70+55+55; 
			str.color = {255, 255, 255};
			break;
	}

	//フォント.
	const int useFont = (line == 1) ? gameData->fonts["jp-size3"].GetFont() : gameData->fonts["jp-size2"].GetFont();

	{
		const int margin = 24;
		//テキストサイズ.
		const INT_XY textSize = GetTextSize(str.text, useFont);

		const DBL_XY pos  = (str.pos - textSize/2).ToDbl() + DBL_XY(-margin/2, -margin/2);
		const DBL_XY size = (textSize + margin).ToDbl();
		const Box    box  = {pos, size, 0x000000, 1.0f};

		//枠背景.
		DrawMode::Exe(
			DrawModeID::None, DrawBlendModeID::Alpha, _int(100 * alpha),
			[&]() {
				DrawBoxKR(box, Anchor::LU);
			}
		);
		//テキスト.
		DrawMode::Exe(
			DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * alpha),
			[&]() {
				str.Draw(Anchor::Mid, useFont);
			}
		);
		//枠線グラデーション.
		GradLine gradLine;
		if (line == 1) {
			gradLine.AddPoint(pos,                          {  0, 255, 255, _int_r(255*alpha)});
			gradLine.AddPoint(pos + DBL_XY(size.x,      0), {  0, 100, 255, _int_r(255*alpha)});
			gradLine.AddPoint(pos + DBL_XY(size.x, size.y), {  0, 255, 255, _int_r(255*alpha)});
			gradLine.AddPoint(pos + DBL_XY(     0, size.y), {  0, 100, 255, _int_r(255*alpha)});
		}
		else {
			gradLine.AddPoint(pos,                          {  0, 255, 255, _int_r(255*alpha)});
			gradLine.AddPoint(pos + DBL_XY(size.x,      0), {  0,   0,   0, _int_r(255*alpha)});
			gradLine.AddPoint(pos + DBL_XY(size.x, size.y), {255,   0, 255, _int_r(255*alpha)});
			gradLine.AddPoint(pos + DBL_XY(     0, size.y), {  0,   0,   0, _int_r(255*alpha)});
		}
		gradLine.Draw(true);
	}
}

//このステップを終了するか判定.
bool TutorialStage::IsEnd(int stepNo, int stepInnerNo) {

	bool ret = false; //終了条件を満たしたか.

	//step別.
	switch (stepNo) 
	{
		case 1:
		{
			//[step1]
			switch (stepInnerNo) 
			{
				//一定時間移動したら.
				case 0: 
					ret = plyMoveCntr >= 20;
					break;

				//ダッシュしたら.
				case 1:
					ret = isPlayerDash;
					break;

				//一定時間経過したら.
				case 2:
					ret = startTimer.GetPassTime() >= 8;
					break;

				default: assert(false); break;
			}
		}
		break;

		case 2:
		{
			//[step2]
			switch (stepInnerNo)
			{
				//アイテムを取ったら.
				case 0:
					ret = isTakeItem;
					break;

				//反射モードが終わったら.
				case 1:
					ret = isReflectFinish;
					break;

				default: assert(false); break;
			}

		}
		break;

		case 3:
		{
			//[step3]
			switch (stepInnerNo)
			{
				//アイテムを取ったら.
				case 0:
					ret = isTakeItem;
					break;

				//レーザーを反射した & 反射モードが終わったら.
				case 1:
					ret = isReflectLaser && isReflectFinish;
					break;

				//隕石を壊した & 反射モードでなければ.
				case 2:
					ret = isBreakMeteor && player->GetMode() == PlayerMode::Normal;
					break;

				//一定時間が経過したら.
				case 3:
					ret = startTimer.GetPassTime() >= 8.0;
					break;

				//一定時間が経過したら.
				case 4:
					ret = startTimer.GetPassTime() >= 8.0;
					break;

				default: assert(false); break;
			}
		}
		break;

		case 4:
		{
			//[step4]
			switch (stepInnerNo)
			{
				//一定スコアを越える & 反射モードでなければ.
				case 0:
					ret = gameData->GetScoreNoTime() >= 2000 && player->GetMode() == PlayerMode::Normal;
					break;

				//一定時間が経過したら.
				case 1:
					ret = startTimer.GetPassTime() >= 5.0;
					break;

				default: assert(false); break;
			}
		}
		break;

		default: assert(false); break;
	}

	return ret; //終了するかを返す.
}

//サインフラグのリセット.
void TutorialStage::ResetSignFlag() {

	isPlayerDash    = false;
	isTakeItem      = false;
	isReflectLaser  = false;
	isReflectFinish = false;
	isBreakMeteor   = false;
}

//項目が終わる度の処理.
void TutorialStage::StepInnerEnd() {

	//リセット.
	startTimer.Reset();
	endTimer.Reset();
	//開始.
	startTimer.Start();

	ResetSignFlag(); //signフラグoff.
}