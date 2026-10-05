/*
   - Stage_Endless.h -
*/
#pragma once

//エンドレスステージ.
class EndlessStage final : public ManagerBase 
{
//▼ ===== 変数 ===== ▼.
private:
	
//▼ ===== 関数 ===== ▼.
public:
	//コンストラクタ.
	EndlessStage(int order) : ManagerBase(order) {}

	void Init()   override;
	void Reset()  override;
	void Update() override;
	void Draw()   override;

	//使用禁止.
	EndlessStage(const EndlessStage&) = delete;
	EndlessStage& operator=(const EndlessStage&) = delete;
};