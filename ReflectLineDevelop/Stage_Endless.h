/*
   - Stage_Endless.h -
*/
#pragma once

//エンドレスステージ.
class EndlessStage final : public ManagerBase 
{
//▼ ===== 変数 ===== ▼.
private:
	bool isStarted{}; //最初の1フレーム処理用.
	
//▼ ===== 関数 ===== ▼.
public:
	//コンストラクタ.
	EndlessStage(){}

	void Init()   override;
	void Reset()  override;
	void Update() override;
	void Draw()   override;

	//使用禁止.
	EndlessStage(const EndlessStage&) = delete;
	EndlessStage& operator=(const EndlessStage&) = delete;
};