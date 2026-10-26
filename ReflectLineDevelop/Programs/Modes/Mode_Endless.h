/*
   - Mode_Endless.h -
*/
#pragma once

//エンドレスモード.
class EndlessMode final : public ManagerBase 
{
//▼ ===== 変数 ===== ▼.
private:
	
//▼ ===== 関数 ===== ▼.
private:
	//レベルアップ処理.
	void UpLevel1();
	void UpLevel2();
	void UpLevel3();
	void UpLevel4();
	void UpLevel5();

public:
	//コンストラクタ.
	EndlessMode(){}

	void Init()   override;
	void Reset()  override;
	void Update() override;
	void Draw()   override;

	//使用禁止.
	EndlessMode(const EndlessMode&) = delete;
	EndlessMode& operator=(const EndlessMode&) = delete;
};