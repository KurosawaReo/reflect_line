#pragma once
#include "Global.h"

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
	list<ItemData> items;

	float counter{}; //経過時間.

//▼ ===== 関数 ===== ▼.
public:
	//コンストラクタ.
	ItemManager(int order) : ManagerBase(order) {}

	void Init()   override;
	void Reset()  override;
	void Update() override;
	void Draw()   override;

	void ItemSpawn(); //アイテム召喚.
	void ItemReset(); //全てリセット.

	void ItemUse(list<ItemData>::iterator&, bool isSuper);     //アイテム発動.

	void SpawnNow();                                           //今すぐアイテムを出現させる.
	bool CheckHitPlayer(list<ItemData>::iterator&, ItemType*); //アイテム取得判定.

	//使用禁止.
	ItemManager(const ItemManager&) = delete;
	ItemManager& operator=(const ItemManager&) = delete;
};