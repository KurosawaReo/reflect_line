/*
   - Obst_Fireworks.cpp -

   障害物: 花火.
*/
#include "Obst_Fireworks.h"

//依存関係.
#include "Player.h"
#include "LaserManager.h"
#include "GameData.h"
//参照.
static GameData*     gameData;
static LaserManager* laserMng;
//参照(KRライブラリ)
static SoundMng*     soundMng;

// ▼*---=[ Fireworks ]=---*▼ //

// 初期化
void Fireworks::Init() {

	gameData = ManagerInsts::Get<GameData>();
	laserMng = ManagerInsts::Get<LaserManager>();
	soundMng = ManagerInsts::Get<SoundMng>();
}

// リセット
void Fireworks::Reset() {

	spawnTimer = 120;  //最初は少し待機.
	fireworks.clear(); //花火を全て消去.
}

// ランダム位置生成
void Fireworks::GenerateRandomPosition(float& x, float& y) {

	int margin = 80;
	x = (float)(margin + (rand() % (WINDOW_WID - margin * 2)));
	y = (float)(margin + (rand() % (WINDOW_HEI - margin * 2 - FIREWORKS_LAUNCH_HEIGHT)));
}

// 距離チェック(近すぎる花火がなければtrue)
bool Fireworks::CheckDistance(float x, float y) {

	//全花火.
	for (const auto& i: fireworks) {

		float dx = x - i.targetX;
		float dy = y - i.targetY;
		float distance = dx*dx + dy*dy;
		//最小距離内なら.
		if (distance < pow(FIREWORKS_MIN_DISTANCE, 2)) {
			return false;
		}
	}
	return true;
}

// 花火生成.
void Fireworks::SpawnFireworks(float x, float y) {

	FwData tmp{}; //花火生成.

	tmp.targetX = x;
	tmp.targetY = y;
	tmp.x = x;
	tmp.y = y;
	tmp.vx = 0.0f;
	tmp.vy = 0.0f;
	tmp.counter = FIREWORKS_WARNING_DURATION;
	tmp.sparkCount = FIREWORKS_SPARKS_COUNT + (rand() % 8);

	fireworks.push_back(tmp); //listに追加.
}

// 花火生成更新
void Fireworks::UpdateFireworksGeneration() {
	spawnTimer -= gameData->speedRate;

	if (spawnTimer <= 0) {
		//1～3個の花火をランダム生成.
		int spawnCount = 1 + (rand() % FIREWORKS_RND_MAX);

		for (int i = 0; i < spawnCount; i++) {
			float x, y;
			bool validPosition = false;
			int attempts = 0;

			while (!validPosition && attempts < 10) {
				GenerateRandomPosition(x, y);
				validPosition = CheckDistance(x, y);
				attempts++;
			}

			if (validPosition) {
				SpawnFireworks(x, y);
			}
		}
		//出現カウンターリセット.
		spawnTimer = FIREWORKS_SPAWN_SPAN * gameData->spawnRate;
	}
}

// 個別花火更新
void Fireworks::UpdateIndividualFireworks() {

	bool isPlaySound = false; //一度のみサウンドを流す用.

	for (auto i = fireworks.begin(); i != fireworks.end(); ) {

		i->counter -= gameData->speedRate; //カウンター減少.

		//警告表示.
		if (i->counter > 0) {
			i++; //次の要素へ.
		}
		//爆発.
		else {
			//花火のレーザーを生成.
			CreateFireworksSparks(i->x, i->y);

			//爆発音.
			if (!isPlaySound) {
				if (auto i = soundMng->Get(_T("Fireworks"))) {
					i->Play(false, 100); //再生.
				}
				isPlaySound = true; //もう再生しない.
			}

			i = fireworks.erase(i); //消去.
		}
	}
}

// 花火の火花作成（LaserManagerを使用）- 落下効果付き
void Fireworks::CreateFireworksSparks(float x, float y) {

	//仮で0番の火花数を使用(←追記:これがなにかは不明)
	const int sparkCount = fireworks.begin()->sparkCount;

	for (int i = 0; i < sparkCount; i++) {
		float angle = _flt((2.0f * M_PI * i) / sparkCount);

		// 初期速度をランダムに調整
		float baseSpeed = 2.5f + (float)(rand() % 3); // 2.5~4.5の範囲
		float speedVariation = 0.8f + (float)(rand() % 4) / 10.0f; // 0.8~1.1の範囲
		float speed = baseSpeed * speedVariation;

		// 角度に少しランダム性を追加
		float angleVariation = _flt((float)(rand() % 40 - 20) * M_PI / 180.0f); // ±20度
		angle += angleVariation;

		DBL_XY pos = { x, y };
		DBL_XY vel = {
			cos(angle) * speed,
			sin(angle) * speed - 0.5f // 上向きにバイアスをかける
		};

		// 落下するレーザーとして生成
		laserMng->SpawnLaser(pos, vel, LaserType::Falling);
	}
}

// 更新
void Fireworks::Update() {
	UpdateFireworksGeneration();
	UpdateIndividualFireworks();
}

// 描画
void Fireworks::Draw() {

#if defined DEBUG_OBJ_ACTIVE
	//デバッグ表示.
	DrawFormatString(0, 180, 0xFF00FF, _T("花火　　　　　 : %d"), fireworks.size());
#endif

	//全花火.
	for (auto i = fireworks.begin(); i != fireworks.end(); i++) {
		DrawWarningEffect(i);
	}
}

// 予告エフェクト描画
void Fireworks::DrawWarningEffect(list<FwData>::iterator it) {

	const float sepTime     = 30; //区切り時間.
	const float elapsedTime = it->counter;

	// 点滅エフェクト
	int alphaValue;
	if (elapsedTime < sepTime) {
		double blinkPhase = fmod(elapsedTime, 20);
		double blinkAlpha = sin(blinkPhase * M_PI / 20);
		alphaValue = (int)(255 - 150 * blinkAlpha);
	}
	else {
		alphaValue = 255;
	}
	// 脈動サイズ
	int warningSize;
	if (elapsedTime >= sepTime) {
		double pulseFactor = 1.0f + 0.3f * sin(elapsedTime * 4.0f * M_PI / 60.0f);
		warningSize = (int)(30 * pulseFactor);
	}
	else {
		warningSize = 30;
	}

	float centerX = it->targetX;
	float centerY = it->targetY;
	float size = (float)warningSize;

	unsigned int color = GetColor(128, 128, 128);

	//描画.
	DrawMode::Exe(
		DrawModeID::None, DrawBlendModeID::Alpha, alphaValue,
		[&]() {
			//上向きの三角形.
			Triangle tri;
			tri.pos[0] = { centerX, centerY - size }; // 上の頂点
			tri.pos[1] = { centerX - size * 0.866f, centerY + size * 0.5f }; // 左下の頂点
			tri.pos[2] = { centerX + size * 0.866f, centerY + size * 0.5f }; // 右下の頂点
			tri.color = color;
			DrawTriangleKR(tri, false, true);

			//下向きの三角形.
			Triangle tri2;
			tri2.pos[0] = { centerX, centerY + size }; // 下の頂点
			tri2.pos[1] = { centerX - size * 0.866f, centerY - size * 0.5f }; // 左上の頂点
			tri2.pos[2] = { centerX + size * 0.866f, centerY - size * 0.5f }; // 右上の頂点
			tri2.color = color;
			DrawTriangleKR(tri2, false, true);
		}
	);
}