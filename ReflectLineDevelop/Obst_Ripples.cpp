/*
   - Ripples.cpp -

   障害物: 波紋.
*/
#include "Obst_Ripples.h"

//依存関係.
#include "Player.h"
#include "GameData.h"
#include "GameManager.h"
//参照.
static GameData* gameData;
static Player*   player;
//参照(KRライブラリ)
static SoundMng* soundMng;

// ▼*---=[ Ripples ]=---*▼ //

//初期化(一回のみ行う)
void Ripples::Init()
{
	gameData = ManagerInsts::Get<GameData>();
	player   = ManagerInsts::Get<Player>();
	soundMng = ManagerInsts::Get<SoundMng>();
}

//リセット(何回でも行う)
void Ripples::Reset()
{
	flashTimer = 80; //最初は少しだけ待機.
	ripples.clear(); //波紋を全て消去.
}

void Ripples::GenerateRandomPosition(double& x, double& y)
{
	//画面サイズ
	int screnWidth = WINDOW_WID;
	int screnHeight = WINDOW_HEI;
	int margin = 100;//画面端からのマージン大き目.
	x = margin + (rand() % (screnWidth - margin * 2));
	y = margin + (rand() % (screnHeight - margin * 2));
}
bool Ripples::CheckDistance(double x, double y)
{
	for (const auto& i : ripples)
	{
		double dx = x - i.x;
		double dy = y - i.y;
		double distance = dx*dx + dy*dy;
		if (distance < pow(RIPPLES_MIN_DISTANCE, 2))
		{
			return false;
		}
	}
	return true;
}
//波紋生成.
void Ripples::SpawnRipples(double x, double y)
{
	RipplesData tmp{}; //波紋生成.

	tmp.x = x;
	tmp.y = y;
	tmp.duration = RIPPLES_WARNING_DURATION + RIPPLES_ACTIVE_DURATION;
	tmp.counter  = tmp.duration;	// 持続時間から開始（カウントダウン）
	tmp.baseSize = 20;				// 適切な基本サイズに調整
	tmp.alreadyHit = false;         // 最初はまだ当たっていない.

	ripples.push_back(tmp); //listに追加.
}
void Ripples::SpawnObstaclegroup()
{
	//同時出現をランダムに決定.
	const int spawnCount = (rand() % RIPPLES_MAX_SIMULTANEOUS) + 1;
	for (int i = 0; i < spawnCount; i++)
	{
		double x{}, y{};
		bool validPosition = false;
		int attempts = 0;

		while (!validPosition && attempts < 10)
		{
			GenerateRandomPosition(x, y);
			validPosition = CheckDistance(x, y);
			attempts++;
		}
		if (validPosition)
		{
			SpawnRipples(x, y);
		}
	}
}
int Ripples::GetEffectState(list<RipplesData>::iterator it)
{
	// 残り時間がアクティブ時間より大きければ警告状態
	if (it->counter > RIPPLES_ACTIVE_DURATION) {
		return RIPPLES_STATE_WARNING;
	}
	else{
		return RIPPLES_STATE_ACTIVE;
	}
}

// 定期的にエフェクトを生成する関数を追加
void Ripples::UpdateFlashGeneration()
{
	//タイマー減少.
	flashTimer -= gameData->speedRate;
	//0以下になったら実行.
	if (flashTimer <= 0) {
		// 新しいフラッシュエフェクトを生成
		SpawnObstaclegroup();  // ランダム位置に複数生成するように変更

		// GameDataのspawnRateを使用してインターバルを調整
		flashTimer = RIPPLES_SPAWN_SPAN * gameData->spawnRate;
	}

	//全波紋.
	for (auto i = ripples.begin(); i != ripples.end(); )
	{
		//経過カウンター減少.
		i->counter -= gameData->speedRate;
		//エフェクト時間が終了したら無効化.
		if (i->counter <= 0) {
			i = ripples.erase(i);
		}
		else {
			i++;
		}
	}
}
//更新.
void Ripples::Update()
{
	UpdateFlashGeneration();
	Hitjudgment();
}
//描画.
void Ripples::Draw()
{
#if defined DEBUG_OBJ_ACTIVE
	//デバッグ表示.
	DrawFormatString(0, 160, 0xFF00FF, _T("波紋　　　　　 : %d"), ripples.size());
#endif
	DrawObstFlash();
}

//当たり判定.
void Ripples::Hitjudgment()
{
	bool isPlaySound = false; //一度のみサウンドを流す用.
	
	//全波紋.
	for (auto i = ripples.begin(); i != ripples.end(); i++) {
		if (i->counter <= 0) {
			continue;
		}

		int effectState = GetEffectState(i);
		if (effectState == RIPPLES_STATE_WARNING) {
			continue;
		}

		// 経過時間の計算
		float elapsedTime = i->duration - i->counter;
		float activeElapsedTime = elapsedTime - RIPPLES_WARNING_DURATION;
		float activeProgress = activeElapsedTime / RIPPLES_ACTIVE_DURATION;

		//波紋が広がった瞬間のみダメージ判定.
		if (effectState == RIPPLES_STATE_ACTIVE && !i->alreadyHit) {

			float sizeMultiplier = RIPPLES_FLASH_SIZE_INIT + (activeProgress * RIPPLES_FLASH_SIZE_SPREAD);
			int effectSize = (int)(i->baseSize * sizeMultiplier);

			DBL_XY playerPos = player->GetPos();
			double dx = playerPos.x - i->x;
			double dy = playerPos.y - i->y;
			double distance = sqrt(dx * dx + dy * dy);
			float playerRadius = 10.0f;

			if (distance < (effectSize * 0.8f + playerRadius)) {
				player->Death();
			}

			//サウンド.
			if (!isPlaySound) {
				if (auto i = soundMng->Get(_T("Ripples"))) {
					i->Play(false, 73); //再生.
				}
				isPlaySound = true; //もう再生しない.
			}

			//波紋のダメージ判定終了.
			i->alreadyHit = true;
		}
	}
}

void Ripples::DrawObstFlash()
{
	//全波紋.
	for (auto i = ripples.begin(); i != ripples.end(); i++)
	{
		int effetState = GetEffectState(i);
		if (effetState == RIPPLES_STATE_WARNING)
		{
			// 予告状態の描画（赤い点滅エフェクト）
			DrawWarningEffect(i);
		}
		else
		{
			// アクティブ状態の描画（元のフラッシュエフェクト）
			DrawActiveEffect(i);
		}
	}
}

void Ripples::DrawWarningEffect(list<RipplesData>::iterator it)
{
	//残り時間から経過時間を計算.
	float elapsedTime = it->duration - it->counter;

	//1: 透明度の計算(点滅)
	int alphaValue;
	//攻撃前になるまで点滅.
	if (elapsedTime < 90) {
		double blinkPhase = fmod(elapsedTime, RIPPLES_FLASH_BLINK_TM);
		double blinkAlpha = sin(blinkPhase * M_PI/RIPPLES_FLASH_BLINK_TM); //0.0～1.0を往復するっぽい.
		alphaValue = (int)(255 - 200*blinkAlpha);
	}
	else {
		alphaValue = 255; //最大値.
	}

	//2: サイズの計算.
	int warningSize;
	//攻撃前になったら脈動開始.
	if (elapsedTime >= 90) {
		const float pulseRate = 4.0f;
		double pulseFactor = 1.0f + 0.4f * sin(elapsedTime * pulseRate * M_PI/60.0f);
		warningSize = (int)(it->baseSize * pulseFactor);
	}
	else {
		warningSize = it->baseSize; //固定サイズ.
	}

	//予告エフェクト.
	DrawMode::Exe(
		DrawModeID::None, DrawBlendModeID::Alpha, alphaValue,
		[&]() {
			Circle cir;
			cir = { {it->x, it->y}, (float)warningSize,   GetColor(150, 150, 150), 1.0f };
			DrawCircleKR(cir, Anchor::Mid, false, true);
			cir = { {it->x, it->y}, (float)warningSize/2, GetColor(200, 200, 200), 1.0f };
			DrawCircleKR(cir, Anchor::Mid, false, true);
			cir = { {it->x, it->y}, (float)warningSize+5, GetColor(120, 120, 120), 1.0f }; // 外周リング
		}
	);
}

void Ripples::DrawActiveEffect(list<RipplesData>::iterator it)
{
	// 残り時間から経過時間を計算
	float elapsedTime = it->duration - it->counter;
	float activeElapsedTime = elapsedTime - RIPPLES_WARNING_DURATION;

	// アクティブ状態での進行度
	float activeProgress = activeElapsedTime / RIPPLES_ACTIVE_DURATION;

	// 透明度を時間に応じて計算
	float alpha = 1.0f - (activeProgress * RIPPLES_FLASH_ALPHA_TM);
	int alphaValue = (int)(255 * max(alpha, 0.0f));

	// エフェクトのサイズを時間に応じて拡大
	float sizeMultiplier = RIPPLES_FLASH_SIZE_INIT + (activeProgress * RIPPLES_FLASH_SIZE_SPREAD);
	int effectSize = (int)(it->baseSize * sizeMultiplier);
	int innerSize = effectSize / 2;

	//アクティブエフェクト.
	DrawMode::Exe(
		DrawModeID::None, DrawBlendModeID::Alpha, alphaValue,
		[&]() {
			Circle cir;
			cir = {{it->x, it->y}, (float)effectSize, GetColor(0, 255, 255), 1.0f};
			DrawCircleKR(cir, Anchor::Mid, false, true);
			cir = {{it->x, it->y}, (float)innerSize,  GetColor(0, 255, 200), 1.0f};
			DrawCircleKR(cir, Anchor::Mid, false, true);
		}
	);
}