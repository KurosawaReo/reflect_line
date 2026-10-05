/*
   - Stage_Tutorial.h -
*/
#pragma once

//チュートリアルステージ.
class TutorialStage final : public ManagerBase
{
//▼ ===== 変数 ===== ▼.
private:
	int    stepNo{};          //現在のステップ番号.
	int    stepInnerNo{};     //現在のステップ内番号.

	int    plyMoveCntr{};     //プレイヤーの移動カウンター.
	double drawAlpha{};       //描画用透明度.

	bool   isPlayerDash{};    //プレイヤーがダッシュしたかどうか.
	bool   isTakeItem{};      //アイテムを取ったかどうか.
	bool   isReflectLaser{};  //レーザーを反射したかどうか.
	bool   isReflectFinish{}; //反射モードが終わったかどうか.
	bool   isBreakMeteor{};   //隕石を壊したかどうか.

	Timer  startTimer{};      //項目開始時に計測開始.
	Timer  endTimer{};        //項目終了時に計測開始.

//▼ ===== 関数 ===== ▼.
public:
	//コンストラクタ.
	TutorialStage(int order) : ManagerBase(order) {}

	//set.
	void SetPlayerDash   (bool _flag) { isPlayerDash    = _flag; }
	void SetTakeItem     (bool _flag) { isTakeItem      = _flag; }
	void SetReflectLaser (bool _flag) { isReflectLaser  = _flag; }
	void SetReflectFinish(bool _flag) { isReflectFinish = _flag; }
	void SetBreakMeteor  (bool _flag) { isBreakMeteor   = _flag; }
	//get.
	int  GetStepNo() const { return stepNo; }

	//基本処理.
	void Init()   override;
	void Reset()  override;
	void Update() override;
	void Draw()   override;
	//Update分割.
	void UpdateStep0();
	void UpdateStep1();
	void UpdateStep2();
	void UpdateStep3();
	void UpdateStep4();
	//Draw分割.
	void DrawStep1();
	void DrawStep2();
	void DrawStep3();
	void DrawStep4();

	void DrawTopText(int line, double alpha, MY_STRING text);
	bool IsEnd(int stepNo, int stepInnerNo);

	void ResetSignFlag();
	void StepInnerEnd();
	
	//使用禁止.
	TutorialStage(const TutorialStage&) = delete;
	TutorialStage& operator=(const TutorialStage&) = delete;
};