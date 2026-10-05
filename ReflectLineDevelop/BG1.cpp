/*
   - BG1.cpp -
*/
#include "BG1.h"

//依存関係.
#include "Global.h"
#include "GameData.h"
//参照.
static GameData* gameData;

const DBL_XY SIZE_RATE = { 0.4, 0.4 }; //タイルのサイズ倍率.

// ▼*---=[ BG_Tile ]=---*▼ //

//初期化.
void BG_Tile::Init() {
	//参照取得.
	gameData = ManagerInsts::Get<GameData>();
}

//リセット.
void BG_Tile::Reset() {
	counter = 0;
	shine   = 0;
	sinNum  = 0;
	shineTimer.Reset();
}

//更新.
void BG_Tile::Update() {

	counter += gameData->speedRate; //タイマー加算.

	//透明度計算.
	shine  = 70 + 80 * sin(M_PI * shineTimer.GetPassTime() / 3);
	sinNum = (sin(M_PI * _dbl(pos.x - pos.y + counter * 2) / (WINDOW_WID / 4)) + 1) / 2;

	//0になったら停止.
	if (shineTimer.GetPassTime() <= 0) {
		shineTimer.Reset();
	}
}

//描画(通常時)
void BG_Tile::DrawNor(double modeAlpha) {

	//タイル画像描画.
	DrawMode::Exe(
		DrawModeID::None, DrawBlendModeID::Alpha, _int(shine * sinNum * modeAlpha),
		[&]() {
			GraphMng::Get(_T("bg_tile_nor"))->DrawExtend(pos.ToDbl(), SIZE_RATE, Anchor::Mid);
		}
	);
}

//描画(反射モード)
void BG_Tile::DrawRef(double modeAlpha) {

	//タイル画像描画.
	DrawMode::Exe(
		DrawModeID::None, DrawBlendModeID::Alpha, _int(shine * sinNum * modeAlpha),
		[&]() {
			GraphMng::Get(_T("bg_tile_ref"))->DrawExtend(pos.ToDbl(), SIZE_RATE, Anchor::Mid);
		}
	);
}

//発光.
void BG_Tile::Shine() {
	//発光してないなら.
	if (shineTimer.GetState() != TimerState::Active){
		shineTimer.Start(); //開始.
	}
}

// ▼*---=[ BG1 ]=---*▼ //

//初期化.
void BG1::Init() {

	INT_XY imgSize = GraphMng::Get(_T("bg_tile_nor"))->GetSize();  //画像サイズ取得.
	INT_XY size = { _int_r(imgSize.x * SIZE_RATE.x), _int_r(imgSize.y * SIZE_RATE.y) };

	//タイルのように貼り付ける.
	for (int x = 0; x < WINDOW_WID + size.x; x += size.x) {
		for (int y = -size.y; y < WINDOW_HEI; y += size.y) {

			BG_Tile tile; //タイルを作成.

			tile.pos.x = x;
			tile.pos.y = y;
			tile.Init();
			tiles.push_back(tile); //配列に追加.
		}
	}

	tmShine.Start(); //タイマー開始.
}

//リセット.
void BG1::Reset() {

	tmShine.Reset();

	//各タイルリセット.
	for (auto& i : tiles) {
		i.Reset();
	}
}

//更新.
void BG1::Update() {

	//一定間隔ごと.
	if (tmShine.IntervalTime()) {
		int idx = Calc::RandNum(0, (int)tiles.size()-1);
		tiles[idx].Shine(); //ランダムでタイルを発光させる.
	}
	//各タイル更新.
	for (auto& i : tiles) {
		i.Update();
	}
}

//描画(通常時)
void BG1::DrawNor(double modeAlpha) {

	//各タイル描画.
	for (auto& i : tiles) {
		i.DrawNor(modeAlpha);
	}
}

//描画(反射モード)
void BG1::DrawRef(double modeAlpha) {

	//各タイル描画.
	for (auto& i : tiles) {
		i.DrawRef(modeAlpha);
	}
}

//ポーズする.
void BG1::Pause() {

	for (auto& i : tiles) {
		i.shineTimer.Pause();
	}
}

//ポーズ解除.
void BG1::PauseEnd() {

	for (auto& i : tiles) {
		//稼働中だったならリスタート.
		if (i.shineTimer.GetState() == TimerState::Pause) {
			i.shineTimer.Start();
		}
	}
}