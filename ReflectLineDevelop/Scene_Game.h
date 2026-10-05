/*
   - Scene_Game.h -

   [Scene]
   ・Title 
   ・Menu
   ・Game <-
   ・End
*/
#pragma once

//タイトルシーン.
class GameScene final : public IScene
{
//▼ ===== 変数 ===== ▼.
private:
	Timer timer;            //シーンの経過時間.

	bool  isGameStart{};	//ゲーム開始サイン.
	int   itemSoundCnt{};   //アイテムサウンドカウンター.

	Timer tmGameTime{};		//ゲーム計測時間.
	Timer tmReflectMode{};	//スロー継続時間.

//▼ ===== 関数 ===== ▼.
public:
	//get.
	float GetSceneTime()       const { return timer.GetPassTime(); }
	float GetGameTime()        const { return tmGameTime.GetPassTime(); }
	float GetReflectModeTime() const { return tmReflectMode.GetPassTime(); }

	void Init()   override;
	void Reset()  override;
	void Enter()  override;
	void Exit()   override;
	void Update() override;
	void Draw()   override;

	//ポーズ用.
	void Pause();
	void PauseEnd();
	
	//反射モード関係.
	void UpdateReflectMode();
	void DrawReflectMode();
	void ReflectModeEnd();

	void ItemUsed();
};