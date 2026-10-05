/*
   - Scene_End.h -
   
   [Scene]
   ・Title 
   ・Menu
   ・Game
   ・End <-
*/
#pragma once

//終了シーン.
class EndScene final : public IScene
{
private:
	Timer timer;              //シーンの経過時間.

	bool  isBestScore{};      //ベストスコア更新したか.
	bool  isBestScoreSound{}; //音を鳴らしたか.

public:
	//sign
	void SignBestScore() { isBestScore = true; }

	void Init()   override;
	void Reset()  override;
	void Enter()  override;
	void Exit()   override;
	void Update() override;
	void Draw()   override;
};