/*
   - ItemManager.h -
   アイテム管理.
*/
#pragma once
#include "Config_Main.h"

//アイテムタイプ.
enum class ItemType
{
	Normal, //通常.
	Super,  //強化版.
};

//アイテムデータ.
struct ItemData
{
	ItemType type{};    //タイプ.
	DBL_XY   pos{};     //座標.
	float    counter{}; //経過時間.
};

//アイテム管理クラス.
class ItemManager final : public ManagerBase
{
//▼ ===== 変数 ===== ▼.
private:
	vector<ItemData> items;

	float spawnCounter{}; //召喚カウンター.

	//画像.
	Graph* grItem{};
	Graph* grItemSuper{};
	Graph* grItemSuperLight{};

//▼ ===== 関数 ===== ▼.
private:
	void ItemUpdate();	//アイテム更新.
	void ItemSpawn();	//アイテム召喚.
	void ItemReset();	//アイテムリセット.

	void ItemUse       (vector<ItemData>::iterator&, bool isSuper);	//アイテム発動.
	bool CheckHitPlayer(vector<ItemData>::iterator&, ItemType*);	//アイテム取得判定.

public:
	//コンストラクタ.
	ItemManager(){}

	void Init()   override;
	void Reset()  override;
	void Update() override;
	void Draw()   override;

	void SpawnNow(); //今すぐアイテムを出現させる.

	//使用禁止.
	ItemManager(const ItemManager&) = delete;
	ItemManager& operator=(const ItemManager&) = delete;
};