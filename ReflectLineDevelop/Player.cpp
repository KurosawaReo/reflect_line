/*
   - Player.cpp -
   プレイヤー管理.
*/
#include "Player.h"

//依存関係.
#include "LaserManager.h"
#include "Obst_NormalLaser.h"
#include "Stage_Tutorial.h"
#include "GameManager.h"
#include "GameData.h"

#include "KrLib_Dx/KR_ManagerInsts.h"
#include "KrLib_Dx/KR_TimerMng.h"
//参照.
static GameData*      gameData;
static GameManager*   gameMng;
static EffectManager* effectMng;
static TutorialStage* tutorialStg;
//参照(KRライブラリ)
static SoundMng*      soundMng;
static InputMng*      inputMng;

// ▼*--=<[ Player ]>=--*▼ //

//移動したか.
bool Player::IsMoved() const {
	//移動距離が0より大きければtrue.
	return Calc::Dist(hit.pos, after[1].pos) > 0;
}

//ダッシュ反射可能か.
bool Player::IsDashReflect() const {
	//継続時間内であれば.
	if (dashTimer > PLAYER_DASH_DURATION - PLAYER_DASH_REFLECT_TIME) {
		return true; //可能.
	}
	return false; //不可能.
}

//初期化(一回のみ行う)
void Player::Init()
{
	//参照取得.
	gameData    = ManagerInsts::Get<GameData>();
	gameMng     = ManagerInsts::Get<GameManager>();
	effectMng   = ManagerInsts::Get<EffectManager>();
	tutorialStg = ManagerInsts::Get<TutorialStage>();
	soundMng    = ManagerInsts::Get<SoundMng>();
	inputMng    = ManagerInsts::Get<InputMng>();

	isDebug = false;
}

//リセット(何回でも行う)
void Player::Reset()
{
	hit       = { { WINDOW_WID / 2, WINDOW_HEI / 2 + 200 }, PLAYER_SIZE, {}, {} };
	mode      = PlayerMode::Normal;
	afterCntr = 1;
	active    = true;

	//初期速度.
	velocity = {0,0};
	//最後の移動方向(上方向にしておく)
	lastInputVec = {0, -1};

	// ダッシュ関連の初期化.
	dashTimer    = 0;
	dashCooldown = 0;

	//ダッシュ関係.
	dashEndEffectTimer = 0;

	//フラグリセット.
	isDashing       = false;
	isDashRefEffect = false;
	isDashEndEffect = false;

	//残像配列のリセット.
	for (int i = 0; i < _countof(after); i++) {
		after[i].pos      = hit.pos; //初期位置と同じ.
		after[i].isActive = false;
	}
}

//更新.
void Player::Update()
{
#if defined _DEBUG //Releaseでは入れない.
	//無敵モード.
	if (inputMng->IsPushKeyTime(KeyID::M) == 1) {
		isDebug = !isDebug;
	}
#endif

	//有効なら.
	if (active) {

		//画像回転.
		imgRot += (1.0 + velocity.Dist()*0.5) * gameData->speedRate;

		UpdateDash();  //ダッシュ更新.
		UpdateAfter(); //残像更新.
		Move();		   //プレイヤー移動.
	}
}

//描画.
void Player::Draw()
{
#if defined _DEBUG //Releaseでは入れない.
	//無敵モード表示.
	if (isDebug) {
		DrawStr str(_T("[Debug] 無敵モード"), {WINDOW_WID/2, WINDOW_HEI/2+300}, COLOR_PLY_DEBUG);
		str.Draw(Anchor::Mid, gameData->fonts["jp-size3"].GetFont());
	}
#endif

	//有効なら.
	if (active) {

		//残像描画.
		DrawAfter();
		//本体描画.
		DrawPlayer();
	}
}

//ダッシュ処理.
void Player::UpdateDash()
{
	//ダッシュクールダウン減少.
	if (dashCooldown > 0){
		dashCooldown -= 1 * gameData->speedRate;
	}

	//ダッシュ中なら.
	if (isDashing)
	{
		dashTimer -= 1 * gameData->speedRate;

		//ダッシュ反射終了.
		if (!IsDashReflect())
		{
			if (mode == PlayerMode::DashReflect)
			{
				mode = PlayerMode::Normal;
			}
		}
		//ダッシュ時間切れ.
		if (dashTimer <= 0)
		{
			isDashing = false; //ダッシュ終了.
		}
	}
	//ダッシュしてないなら.
	else
	{
		if (dashCooldown <= 0)
		{
			bool dashkey = (inputMng->IsPushActionTime(_T("PlayerDash")) == 1);
			//ダッシュ開始.
			if (dashkey)
			{
				//ダッシュ音再生.
				if (auto i = soundMng->Get(_T("PlayerDash"))) {
					i->Play(false, 50); //再生.
				}
				dashTimer    = PLAYER_DASH_DURATION;
				dashCooldown = PLAYER_DASH_COOLDOWN;
				isDashing    = true;

				//ダッシュ反射開始.
				if (mode == PlayerMode::Normal)
				{
					mode = PlayerMode::DashReflect;
				}

				//チュートリアルなら.
				if (gameData->stage == StageType::Tutorial) {
					tutorialStg->SetPlayerDash(true);
				}
			}
		}
	}
}

//残像更新.
void Player::UpdateAfter()
{
	afterCntr -= gameData->speedRate;

	//残像を残すタイミングになったら(基本は毎フレーム)
	if (afterCntr <= 0) {
		afterCntr = 1;

		//残像データを後ろにずらす.
		for (int i = PLAYER_AFT_IMG_NUM - 1; i > 0; i--)
		{
			after[i] = after[i - 1];
		}

		//一旦無効にする.
		after[0].isActive = false;

		//ある程度移動したら.
		if (fabs(hit.pos.x - after[1].pos.x) >= 1.0 ||
			fabs(hit.pos.y - after[1].pos.y) >= 1.0
		){
			//1フレーム目の情報登録.
			after[0].pos = hit.pos;                                     //プレイヤー座標.
			after[0].ang = Calc::FacingAng(after[0].pos, after[1].pos); //移動方向.
			after[0].isDash = isDashing;                                //ダッシュ中ならダッシュエフェクトに.
			after[0].isActive = true;                                   //残像を出す.
		}
	}
}

//残像描画.
void Player::DrawAfter()
{
	//残像処理.
	for (int i = 0; i < PLAYER_AFT_IMG_NUM; i++)
	{
		if (!after[i].isActive) { continue; }

		if (hit.pos.x != after[i].pos.x || hit.pos.y != after[i].pos.y) {

			//アニメーション値.
			const double anim1 = _dbl(i) / PLAYER_AFT_IMG_NUM;
			const double anim2 = Calc::AnimEase(EaseType::OutQuad, anim1); //遅めに変化する版.

			//描画.
			DrawMode::Exe(
				DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * (1 - anim1)),
				[&]() {

					//ダッシュエフェクト.
					if (after[i].isDash) {
						DrawAfterDash(i, anim1, anim2);
					}
					//通常エフェクト.
					else {
						DrawAfterNor(i);
					}
				}
			);
		}
	}
}

//残像描画(通常時)
void Player::DrawAfterNor(int idx) {

	MY_COLOR color;

	//反射カラー.
	if (mode == PlayerMode::ItemReflect ||
		mode == PlayerMode::ItemReflectSuper
	){
		color = COLOR_PLY_AFT_REF;
	}
	//通常カラー.
	else
	{
		color = COLOR_PLY_AFT_NOR;
	}
	//円描画.
	Circle cir = { after[idx].pos, PLAYER_SIZE, color, 1.0f };
	DrawCircleKR(cir, Anchor::Mid, false, true);
}

//残像描画(ダッシュ時)
void Player::DrawAfterDash(int idx, double anim1, double anim2) {

	//三角形データ.
	DBL_XY   pos1 = after[idx].pos + Calc::AngToVector(after[idx].ang     ) * (30 * (1 - anim1));
	DBL_XY   pos2 = after[idx].pos + Calc::AngToVector(after[idx].ang + 90) * (20 * (1 - anim1));
	DBL_XY   pos3 = after[idx].pos + Calc::AngToVector(after[idx].ang - 90) * (20 * (1 - anim1));
	GradLine line;
	
	//モード別.
	switch (mode)
	{
		case PlayerMode::ItemReflect:
		case PlayerMode::ItemReflectSuper:
		{
			//三角形のグラデーション線を作成.
			line.AddPoint(pos2, { 255,   0, 255, _int_r(255 * (1 - anim1)) });
			line.AddPoint(pos1, { 100,   0, 100, _int_r(255 * (1 - anim1)) });
			line.AddPoint(pos3, { 255,   0, 255, _int_r(255 * (1 - anim1)) });
		}
		break;

		case PlayerMode::Normal:
		case PlayerMode::DashReflect:
		{
			//ダッシュ反射してたら色付き.
			if (isDashRefEffect) {

				//色の変化.
				MY_COLOR color = {
					_int_r(80 * (1 - anim2)),	//R
					_int_r(255 - 200 * anim2),	//G
					_int_r(255 * anim2),		//B
					_int_r(255 * (1 - anim1))   //A(透明度)
				};
				//三角形のグラデーション線を作成.
				line.AddPoint(pos2, color);
				line.AddPoint(pos1, color);
				line.AddPoint(pos3, color);
			}
			//してなければ通常カラー.
			else {
				//三角形のグラデーション線を作成.
				line.AddPoint(pos2, { 255, 255, 255, _int_r(255 * (1 - anim1)) });
				line.AddPoint(pos1, { 100, 100, 100, _int_r(255 * (1 - anim1)) });
				line.AddPoint(pos3, { 255, 255, 255, _int_r(255 * (1 - anim1)) });
			}
		}
		break;
	}

	//ダッシュエフェクト描画.
	line.Draw();
}

//本体描画.
void Player::DrawPlayer() {

	//透明度計算.
	const double alpha  = Calc::AnimEase(EaseType::OutQuad, dashTimer / PLAYER_DASH_DURATION);
	const double alpha2 = Calc::AnimWave(WaveType::CosLoop, gameMng->GetGameScene()->GetReflectModeTime() * 2);

	//使用する画像名.
	MY_STRING imgDash   = _T("null");
	MY_STRING imgPlayer = _T("null");

	if (mode == PlayerMode::ItemReflect ||
		mode == PlayerMode::ItemReflectSuper
	){
		//ダッシュ演出.
		if (isDashing) {
			imgDash = _T("light_ref_player");
		}
		//プレイヤー本体.
		imgPlayer = _T("player_ref");
	}
	else 
	{
		//ダッシュ演出.
		if (isDashing) {
			imgDash = _T("light_nor_player");
		}
		//プレイヤー本体.
		imgPlayer = _T("player_nor");
	}

	//ダッシュ時.
	if (isDashing) {
		//発光.
		DrawMode::Exe(
			DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * alpha),
			[&]() {
				GraphMng::Get(imgDash)->DrawExtend(
					hit.pos, { PLAYER_LIGHT_DRAW_SIZE, PLAYER_LIGHT_DRAW_SIZE }, Anchor::Mid, true, true
				);
			}
		);
	}

	//反射モード時.
	if (mode == PlayerMode::ItemReflect ||
		mode == PlayerMode::ItemReflectSuper
	){
		//反射バリア.
		DrawMode::Exe(
			DrawModeID::None, DrawBlendModeID::Alpha, _int(80 + 128 * alpha2),
			[&]() {
				GraphMng::Get(_T("player_ref_barrier"))->DrawExtend(
					hit.pos, { PLAYER_DRAW_SIZE, PLAYER_DRAW_SIZE }, Anchor::Mid, true, true
				);
			}
		);
	}
	else {
		//TODO: ダッシュ反射の継続時間が分かる演出をどうするか?
		/*
		//ダッシュ反射円.
		Circle cir = { hit.pos, _flt(40 * alpha), COLOR_MODE_REF, 1 };
		DrawCircleKR(cir, Anchor::Mid, false, true);
		*/
	}

	//プレイヤー本体.
	GraphMng::Get(imgPlayer)->DrawRota(
		hit.pos, PLAYER_DRAW_SIZE, imgRot, { 0, 0 }, true, true
	);

	//チュートリアル用.
	if (gameData->stage == StageType::Tutorial) {
		DrawStr str(_T("プレイヤー"), hit.pos.ToInt() + INT_XY(0, -40), 0xFFFFFF);
		str.Draw(Anchor::Mid, gameData->fonts["jp-size1"].GetFont());
	}
}

//移動処理(斜め対応)
void Player::Move()
{
	float speed = PLAYER_MOVE_SPEED;

	//目標速度.
	DBL_XY targetVel{};
	//入力操作.
	DBL_XY input = inputMng->GetKey4Dir() + inputMng->GetPadStick();
	//入力があれば更新.
	if (input.x != 0 || input.y != 0) {
		lastInputVec = input;
	}

	//ダッシュ中なら.
	if (isDashing) {
		//段々減速.
		const double rate = Calc::AnimEase(EaseType::OutQuad, dashTimer / PLAYER_DASH_DURATION);
		//速度変化.
		speed *= _flt(1.0 + PLAYER_DASH_SPEED * rate);
		//目標速度変更.
		targetVel = lastInputVec * speed;
	}
	else {
		//目標速度変更.
		targetVel = input * speed;
	}

	//Lerp速度.
	const double moveLerp = PLAYER_MOVE_LERP_SPEED;
	//Lerpで目標速度に近づける.
	velocity.x += (targetVel.x - velocity.x) * moveLerp;
	velocity.y += (targetVel.y - velocity.y) * moveLerp;
	//誤差は無視.
	if (fabs(velocity.x) < 0.01f) { velocity.x = 0; }
	if (fabs(velocity.y) < 0.01f) { velocity.y = 0; }

	//移動.
	hit.pos += velocity * gameData->speedRate;
	//移動限界.
	Calc::FixPosInArea(&hit.pos, { PLAYER_SIZE * 2, PLAYER_SIZE * 2 }, { 0, 0, WINDOW_WID - 1, WINDOW_HEI - 1 });
}

//死亡処理.
void Player::Death() {

	//デバッグモード中は無敵.
	if (isDebug) { return; }
	//死亡済なら中断.
	if (!active) { return; }
	//無敵なら中断.
	if (isNoDeath) { return; }

	//サウンド.
	if (auto i = soundMng->Get(_T("PlayerDeath"))) {
		i->Play(false, 70); //再生.
	}
	//エフェクト.
	EffectData data{};
	data.type = EffectType::PlayerDeath;
	data.pos = hit.pos;
	effectMng->SpawnEffect(&data);

	gameMng->GameOver(); //ゲーム終了.

	isDashing = false;
	dashTimer = 0;
	dashCooldown = 0;
	active = false;
}

//プレイヤー復活.
void Player::Revival()
{
	//位置・速度リセット.
	hit.pos = { WINDOW_WID / 2, WINDOW_HEI / 2 };
	velocity = { 0, 0 };
	active = true;

	//残像配列のリセット.
	for (int i = 0; i < _countof(after); i++) {
		after[i].pos = hit.pos;
		after[i].isActive = false;
	}
}

//反射スパーク生成.
void Player::SpawnEffectSpark()
{
	//何個出すか.
	const int sparkNum = 4;

	const double baseAng = Calc::FacingAng({ 0,0 }, lastInputVec);

	for (int s = 0; s < sparkNum; s++)
	{
		//角度抽選.
		const double ang   = baseAng + Calc::RandNum(-30, 30);
		//速度抽選.
		const double speed = Calc::RandNum(4.0, 15.0);

		//サウンド.
		if (auto i = soundMng->Get(_T("PlayerParry"))) {
			i->Play(false, 50);
		}
		//エフェクト.
		EffectData data{};
		data.type  = EffectType::PlayerDashReflect;
		data.pos   = hit.pos;
		data.vec   = Calc::AngToVector(ang);
		data.speed = _flt(speed);
		data.ang   = _flt(ang);
		effectMng->SpawnEffect(&data);
	}
}
