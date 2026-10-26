/*
   - Scene_Game.h -
*/
#pragma once

//ゲームシーン.
class GameScene final : public IScene
{
//▼ ===== 変数 ===== ▼.
private:
	Timer timer;            //シーンの経過時間.

	bool  isGameStart{};	//ゲーム開始サイン.
	int   itemSoundCnt{};   //アイテムサウンドカウンター.

	Timer tmGameTime{};		//ゲーム計測時間.
	Timer tmReflectMode{};	//スロー継続時間.

	//画像.
	Graph* grReflectText{};

//▼ ===== 関数 ===== ▼.
private:
	//反射モード関係.
	void UpdateReflectMode();
	void DrawReflectMode();
	void ReflectModeEnd();

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

	//反射モード発動.
	void ReflectModeStart();
};