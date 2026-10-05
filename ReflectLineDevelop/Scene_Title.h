/*
   - Scene_Title.h -

   [Scene]
   ・Title <-
   ・Menu
   ・Game
   ・End
*/
#pragma once

//タイトルシーン.
class TitleScene final : public IScene
{
private:
	Timer timer;         //シーンの経過時間.
	bool  isTitleAnim{}; //破片アニメーションを出したか.

public:
	void Init()   override;
	void Reset()  override;
	void Enter()  override;
	void Exit()   override;
	void Update() override;
	void Draw()   override;
};