/*
   - EffectManager.cpp -
   エフェクトを管理するクラス.
*/
#include "EffectManager.h"

//依存関係.
#include "GameData.h"
//参照
static GameData* gameData;

using namespace Calc; //計算機能を使用.

// ▼*---=[ EffectManager ]=---*▼ //

void EffectManager::Init() {
	gameData = ManagerInsts::Get<GameData>();
}

void EffectManager::Reset() {
	//エフェクトを全て消去.
	effect.clear();
}

void EffectManager::Update() {
	
	//ポーズ中の更新はしない.
	if (gameData->isPause) { return; }

	//全てのエフェクト.
	for (auto i = effect.begin(); i != effect.end(); ) {

		bool isErase = false; //消去するかどうか.

		//タイプ別.
		switch (i->type) 
		{
			case EffectType::Score100:
			case EffectType::Score500:
			{
				i->counter++;

				//時間経過で消滅.
				if (i->counter >= SCORE_ANIM_TIME) {
					isErase = true;
				}
			}
			break;

			case EffectType::PlayerDeath:
			{
				i->counter++;

				//時間経過で消滅.
				if (i->counter >= PLAYER_DEATH_ANIM_TIME) {
					isErase = true;
				}
			}
			break;

			case EffectType::Reflect:
			{
				i->counter++;

				//時間経過で消滅.
				if (i->counter >= LASER_REF_ANIM_TIME) {
					isErase = true;
				}
			}
			break;

			case EffectType::PlayerDashReflect:
			{
				//カウンター計算.
				i->counter += gameData->speedRate;
				//回転.
				i->ang += 10 * gameData->speedRate;
				//減速.
				float newSpeed = i->speed / (1 + (i->counter / 5));
				//移動.
				i->pos.x += i->vec.x * newSpeed * gameData->spawnRate;
				i->pos.y += i->vec.y * newSpeed * gameData->spawnRate;

				//時間経過で消滅.
				if (i->counter >= PLAYER_DASH_SPARK_ANIM_TIME) {
					isErase = true;
				}
			}
			break;

			case EffectType::MeteorCrash:
			{
				//カウンター加算.
				i->counter += gameData->speedRate;
				//時間経過で消滅.
				if (i->counter >= METEOR_BREAK_ANIM_TIME) {
					isErase = true;
				}
			}
			break;

			case EffectType::MeteorFragment:
			{
				//カウンター加算.
				i->counter += gameData->speedRate;
				//回転.
				i->ang += 3 * gameData->speedRate;
				//減速.
				float newSpeed = i->speed/(1+(i->counter/10));
				//移動.
				i->pos.x += i->vec.x * newSpeed * gameData->speedRate;
				i->pos.y += i->vec.y * newSpeed * gameData->speedRate;

				//時間経過で消滅.
				if (i->counter >= METEOR_BREAK_ANIM_TIME) {
					isErase = true;
				}
			}
			break;

			case EffectType::Endless_Level1:
			case EffectType::Endless_Level2:
			case EffectType::Endless_Level3:
			case EffectType::Endless_Level4:
			case EffectType::Endless_Level5:
			case EffectType::Tutorial_Step1:
			case EffectType::Tutorial_Step2:
			case EffectType::Tutorial_Step3:
			case EffectType::Tutorial_Step4:
			{
				i->counter++;

				//時間経過で消滅.
				if (i->counter >= MIDDLE_ANIM_TIME) {
					isErase = true;
				}
			}
			break;

			default: assert(FALSE); break;
		}

		//次の要素に進む.
		if (isErase) {
			i = effect.erase(i); //消去して次へ.
		}
		else {
			i++;
		}
	}
}

void EffectManager::Draw() {

#if defined DEBUG_OBJ_ACTIVE
	//デバッグ表示.
	DrawFormatString(0, 80, 0xFF00FF, _T("エフェクト　　 : %d"), effect.size());
#endif

	//全てのエフェクト.
	for (const auto& i : effect) {

		//タイプ別.　
		switch (i.type)
		{
			case EffectType::Score100:
			case EffectType::Score500:
			{
				//座標.
				DBL_XY pos = {i.pos.x, i.pos.y - AnimEase(EaseType::OutQuad, i.counter/SCORE_ANIM_TIME)*30};
				//透明度.
				const int alpha = _int_r(255 * AnimEase(EaseType::OutQuad, 1 - i.counter/SCORE_ANIM_TIME));

				const double size = 0.6;

				//描画.
				DrawMode::Exe(
					DrawModeID::None, DrawBlendModeID::Alpha, alpha,
					[&]() {
						//画像切り替え.
						if (i.type == EffectType::Score100) {
							GraphMng::Get(_T("score100"))->DrawExtend(pos, { size, size }, Anchor::Mid, true, true);
						}
						else {
							GraphMng::Get(_T("score500"))->DrawExtend(pos, { size, size }, Anchor::Mid, true, true);
						}
					}
				);
			}
			break;

			case EffectType::PlayerDeath:
			{
				//透明度.
				const int alpha = _int_r(255 * AnimEase(EaseType::OutQuad, 1 - i.counter/PLAYER_DEATH_ANIM_TIME));

				Circle cir = { i.pos, PLAYER_SIZE+i.counter/2, 0xFFFFFF, 1.0f };				
				//描画.
				DrawMode::Exe(
					DrawModeID::None, DrawBlendModeID::Alpha, alpha,
					[&]() {
						DrawCircleKR(cir, Anchor::Mid, false, true);
					}
				);
			}
			break;

			case EffectType::PlayerDashReflect:
			{
				//進行度(0→1).
				const double time     = i.counter / PLAYER_DASH_SPARK_ANIM_TIME;
				//線の長さ(徐々に短くなる)
				const float  sparkLen = _flt(8 * (1 - time));
				//透明度.
				const int    alpha    = _int_r(255 * AnimEase(EaseType::OutQuad, 1 - time));

				//飛ばす線データ.
				Line line{};
				line.stPos = ArcPos(i.pos, i.ang,     sparkLen);
				line.edPos = ArcPos(i.pos, i.ang+180, sparkLen);
				line.color = 0x00FFFF;
				line.thick = 2;

				DrawMode::Exe(
					DrawModeID::None, DrawBlendModeID::Alpha, alpha,
					[&]() {
						DrawLineKR(line, true);
					}
				);
			}
			break;

			case EffectType::Reflect:
			{
				const double size  = 0.2 + 0.4 * i.counter/LASER_REF_ANIM_TIME;
				const DBL_XY pos   = i.pos;
				//透明度.
				const int    alpha = _int_r(255 * AnimEase(EaseType::OutQuad, 1 - i.counter/LASER_REF_ANIM_TIME));

				//描画.
				DrawMode::Exe(
					DrawModeID::None, DrawBlendModeID::Alpha, alpha,
					[&]() {
						GraphMng::Get(_T("reflect_effect"))->DrawExtend(pos, { size, size }, Anchor::Mid, true, true);
					}
				);
			}
			break;

			case EffectType::MeteorCrash:
			{
				//透明度.
				const int alpha = _int_r(255 * AnimEase(EaseType::OutQuad, 1 - i.counter/METEOR_BREAK_ANIM_TIME));

				Circle cir = { i.pos, i.counter, COLOR_METEOR(i.pos), 1.0f };
				//描画.
				DrawMode::Exe(
					DrawModeID::None, DrawBlendModeID::Alpha, alpha,
					[&]() {
						DrawCircleKR(cir, Anchor::Mid, false, true);
					}
				);
			}
			break;

			case EffectType::MeteorFragment:
			{
				//透明度.
				const int alpha = _int_r(255 * AnimEase(EaseType::OutQuad, 1 - i.counter/METEOR_BREAK_ANIM_TIME));

				//飛ばす線のデータ.
				Line line{};
				line.stPos = ArcPos(i.pos, i.ang,     i.len);
				line.edPos = ArcPos(i.pos, i.ang+180, i.len);
			    line.color = COLOR_METEOR(i.pos);
				line.thick = 1;

				//描画.
				DrawMode::Exe(
					DrawModeID::None, DrawBlendModeID::Alpha, alpha,
					[&]() {
						DrawLineKR(line, true);
					}
				);
			}
			break;

			case EffectType::Endless_Level1:
			case EffectType::Endless_Level2:
			case EffectType::Endless_Level3:
			case EffectType::Endless_Level4:
			case EffectType::Endless_Level5:
			case EffectType::Tutorial_Step1:
			case EffectType::Tutorial_Step2:
			case EffectType::Tutorial_Step3:
			case EffectType::Tutorial_Step4:
			{
				//透明度.
				const int alpha = _int_r(255 * AnimWave(WaveType::CosLoop, 1 - i.counter / MIDDLE_ANIM_TIME));

				//共通設定.
				DrawStr str = { _T("Unknown"), {_int_r(i.pos.x), _int_r(i.pos.y-20)}, 0xFFFFFF};
				Circle mainCir = { i.pos, i.counter*5, 0xFFFFFF, 1.0f };
				Circle lampCir[5] = {
					{{-1, i.pos.y+20}, 10, 0xFFFFFF, 1.0f},
					{{-1, i.pos.y+20}, 10, 0xFFFFFF, 1.0f},
					{{-1, i.pos.y+20}, 10, 0xFFFFFF, 1.0f},
					{{-1, i.pos.y+20}, 10, 0xFFFFFF, 1.0f},
					{{-1, i.pos.y+20}, 10, 0xFFFFFF, 1.0f}
				};
				//何個ランプを使うか.
				int lampUseCnt  = 0;
				int lampFillCnt = 0;

				switch (i.type)
				{
					case EffectType::Endless_Level1:
						str.text = _T("Level 1");
						lampUseCnt = 5;
						lampFillCnt = 1;
						break;
					case EffectType::Endless_Level2:
						str.text = _T("Level 2");
						lampUseCnt = 5;
						lampFillCnt = 2;
						break;
					case EffectType::Endless_Level3:
						str.text = _T("Level 3");
						lampUseCnt = 5;
						lampFillCnt = 3;
						break;
					case EffectType::Endless_Level4:
						str.text = _T("Level 4");
						lampUseCnt = 5;
						lampFillCnt = 4;
						break;
					case EffectType::Endless_Level5:
						str.text = _T("Level 5");
						lampUseCnt = 5;
						lampFillCnt = 5;
						break;

					case EffectType::Tutorial_Step1:
						str.text = _T("Step 1");
						lampUseCnt = 4;
						lampFillCnt = 1;
						break;
					case EffectType::Tutorial_Step2:
						str.text = _T("Step 2");
						lampUseCnt = 4;
						lampFillCnt = 2;
						break;
					case EffectType::Tutorial_Step3:
						str.text = _T("Step 3");
						lampUseCnt = 4;
						lampFillCnt = 3;
						break;
					case EffectType::Tutorial_Step4:
						str.text = _T("Step 4");
						lampUseCnt = 4;
						lampFillCnt = 4;
						break;
				}

				//描画.
				DrawMode::Exe(
					DrawModeID::None, DrawBlendModeID::Alpha, alpha,
					[&]() {

						//円.
						DrawCircleKR(mainCir, Anchor::Mid, false, true);
						//テキスト.					
						str.Draw(Anchor::Mid, gameData->fonts["en-size3"].GetFont());

						//ランプ(必要な数だけ)
						for (int j = 0; j < lampUseCnt; j++) {
							//均等になるように配置する.
							const int interval = 30; //間隔.
							lampCir[j].pos.x = i.pos.x + interval * (j - _flt(lampUseCnt - 1) / 2);
							DrawCircleKR(lampCir[j], Anchor::Mid, (lampFillCnt >= j + 1), true);
						}
					}
				);
			}
			break;

			default: assert(false); break;
		}
	}
}

//エフェクト出現.
void EffectManager::SpawnEffect(const EffectData* data) {
	effect.push_back(*data); //listに追加.
}