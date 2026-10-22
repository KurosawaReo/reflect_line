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
	EndlessStage(){}

	void Init()   override;
	void Reset()  override;
	void Update() override;
	void Draw()   override;

	//レベルアップ処理.
	void UpLevel1();
	void UpLevel2();
	void UpLevel3();
	void UpLevel4();
	void UpLevel5();

	//使用禁止.
	EndlessStage(const EndlessStage&) = delete;
	EndlessStage& operator=(const EndlessStage&) = delete;
};