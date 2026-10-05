/*
   - Item.cpp -
   Item管理.
*/
#include "Item.h"

//依存関係.
#include "Player.h"
#include "LaserManager.h"
#include "EffectManager.h"
#include "GameData.h"
#include "GameManager.h"
//参照.
static GameData*      gameData;
static GameManager*   gameMng;
static Player*        player;
static LaserManager*  laserMng;
static EffectManager* effectMng;
//参照(KRライブラリ)
static SoundMng*      soundMng;

using namespace Calc; //計算機能を使用.

// ▼*---=[ ItemManager ]=---*▼ //

//初期化.
void ItemManager::Init() 
{
	gameData  = ManagerInsts::Get<GameData>();
	gameMng   = ManagerInsts::Get<GameManager>();
	player    = ManagerInsts::Get<Player>();
	laserMng  = ManagerInsts::Get<LaserManager>();
	effectMng = ManagerInsts::Get<EffectManager>();
	soundMng  = ManagerInsts::Get<SoundMng>();
}

//リセット.
void ItemManager::Reset()
{
	ItemReset();
}

//更新.
void ItemManager::Update()
{
	//反射モード中は処理しない.
	if (gameData->isReflectMode) {
		return;
	}

	counter -= gameData->speedRate; //時間経過.

	//アイテム出現.
	if (counter <= 0) {
		ItemSpawn();
		counter = ITEM_SPAWN_TIME;
	}

	//全アイテムループ.
	for (auto i = items.begin(); i != items.end(); ) {

		//カウンター.
		i->counter += gameData->speedRate;
		//落下.
		i->pos.y += ITEM_SPEED * gameData->speedRate;

		ItemType type;

		//当たり判定.
		if (CheckHitPlayer(i, &type)){
			//効果発動.
			switch (type)
			{
				case ItemType::Normal:
					ItemUse(i, false);
					break;
				case ItemType::Super:
					ItemUse(i, true);
					break;

				default: assert(false); break;
			}
			break;
		}

		//次の要素へ, 画面下を超えたら消滅.
		if (i->pos.y > WINDOW_HEI + ITEM_SIZE + 50) {
			i = items.erase(i);
		}
		else {
			i++;
		}
	}
}

//描画.
void ItemManager::Draw()
{
	//全アイテムループ.
	for (auto& i : items) {

		//透明度.
		const int alpha = _int(155 + 100 * AnimWave(WaveType::CosLoop, i.counter / 30));

		const DBL_XY size = {
			ITEM_SIZE / GraphMng::Get(_T("item"))->GetSize().ToDbl().x,
			ITEM_SIZE / GraphMng::Get(_T("item"))->GetSize().ToDbl().y
		};

		//点滅描画.
		DrawMode::Exe(
			DrawModeID::None, DrawBlendModeID::Alpha, alpha,
			[&]() {
				//強化演出.
				if (i.type == ItemType::Super) {
					//アイテム発光.
					GraphMng::Get(_T("light_super_item"))->DrawExtend(i.pos, { 0.4, 0.4 });
					//アイテム本体.
					GraphMng::Get(_T("item_super"))->DrawExtend(i.pos, size, Anchor::Mid, true, true);
				}
				else {
					//アイテム本体.
					GraphMng::Get(_T("item"))->DrawExtend(i.pos, size, Anchor::Mid, true, true);
				}
			}
		);

		//チュートリアル用.
		if (gameData->stage == StageType::Tutorial) {
			DrawStr str(_T("アイテム"), i.pos.ToInt() + INT_XY(0, -35), COLOR_ITEM);
			str.Draw(Anchor::Mid, gameData->fonts["jp-size1"].GetFont());
		}
	}
}

//アイテム召喚.
void ItemManager::ItemSpawn() {

	ItemData item; //新規アイテム.

	//座標の設定.
	item.pos.x = _dbl(Calc::RandNum(ITEM_SIZE, WINDOW_WID-ITEM_SIZE));
	item.pos.y = -ITEM_SIZE;

	//タイプの設定.
	if (gameData->level < 5) {
		item.type = ItemType::Normal;
	}
	else {
		item.type = ItemType::Super; //Lv5からは強化版へ.
	}

	items.push_back(item); //配列に追加.
}

//全てリセット.
void ItemManager::ItemReset() {
	
	//全てのアイテムを消去.
	items.clear();
	//カウンターリセット.
	counter = ITEM_SPAWN_TIME;
}

//アイテム発動.
void ItemManager::ItemUse(list<ItemData>::iterator& it, bool isSuper)
{
	//エフェクト召喚.
	EffectData effect{};
	effect.type = EffectType::Score100;
	effect.pos  = it->pos;
	effectMng->SpawnEffect(&effect);
	//スコア加算.
	gameData->AddScore(SCORE_TAKE_ITEM);

	//プレイヤーのモード設定.
	if (isSuper) {
		player->SetMode(PlayerMode::ItemReflectSuper);
	}
	else {
		player->SetMode(PlayerMode::ItemReflect);
	}

	ItemReset();

	//アイテムを使用.
	gameMng->ItemUsed();
	//サウンド.
	if (auto i = soundMng->Get(_T("ItemUse"))) {
		i->Play(false, 70); //ポワーン.
	}
}

//今すぐアイテムを出現させる.
void ItemManager::SpawnNow() {
	counter = 0;
}

//プレイヤーとの当たり判定.
bool ItemManager::CheckHitPlayer(list<ItemData>::iterator& it, ItemType* type)
{
	//プレイヤーが無効なら中断.
	if (!player->GetActive()) {
		return false;
	}

	//プレイヤーの判定を取得.
	const Circle plyHit = player->GetHit();
	//当たり判定.
	const Box itemBox = { it->pos, {ITEM_SIZE, ITEM_SIZE}, {}, {} };
	
	//当たったら.
	if (Calc::HitBoxCir(itemBox, plyHit)) {
		*type = it->type; //種類を返す.
		return true;      //取った.
	}
	return false; //取ってない.
}