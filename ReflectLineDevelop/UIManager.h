/*
   - UIManager.h -
   UI管理.
*/
#pragma once

//UI管理.
class UIManager final : public ManagerBase
{
//▼ ===== 変数 ===== ▼.
private:
	int  dispBestScore{}; //表示ベストスコア.

	bool isShowScore{}; //スコアを表示するかどうか.

	//画像.
	Graph* grUiBackLevel{};
	Graph* grUiBackBestScore{};
	Graph* grUiBackScore{};
	Graph* grUiBackTime{};

//▼ ===== 関数 ===== ▼.
public:
	//コンストラクタ.
	UIManager(){}

	//set.
	void SetIsShowScore(bool _flag)  { isShowScore   = _flag;  }
	void SetBestScore  (int  _score) { dispBestScore = _score; }

	void Init()   override;
	void Reset()  override;
	void Update() override;
	void Draw()   override;

	//使用禁止.
	UIManager(const UIManager&) = delete;
	UIManager& operator=(const UIManager&) = delete;
};