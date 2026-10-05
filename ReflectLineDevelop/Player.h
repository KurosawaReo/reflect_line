/*
   - Player.h -
   プレイヤー管理.
*/
#pragma once
#include "EffectManager.h"
#include "Global.h"

//エフェクトデータ.
struct AfterEffect
{
	DBL_XY pos;
	double ang;
	bool   isDash;   //ダッシュエフェクトにするか.
	bool   isActive;
};

//プレイヤーモード.
enum class PlayerMode
{
	Normal,           //通常モード.
	ItemReflect,      //アイテム反射モード.
	ItemReflectSuper, //アイテム反射モード強化版.
	DashReflect,      //反射ダッシュ.
	DashReflectSpark, //ダッシュエフェクト.
};

//プレイヤー.
class Player final : public ManagerBase
{
//▼ ===== 変数 ===== ▼.
private:
	PlayerMode		mode{};					//モード.
	
	Circle			hit{};					//プレイヤーの当たり判定円.
	DBL_XY			velocity{};				//移動速度.
	DBL_XY			lastInputVec{};			//最後の入力方向.
	bool			active{};				//有効か.

	float			dashEndEffectTimer{};	//エフェクト残り時間.

	bool			isDashing{};			//ダッシュ中かどうか.
	bool			isDashRefEffect{};		//ダッシュ反射エフェクトを出すか.
	bool			isDashEndEffect{};		//エフェクト発動フラグ.
	bool            isNoDeath{};            //無敵かどうか(チュートリアル用)
	bool			isDebug{};				//デバッグ用.

	//残像.
	float			afterCntr{};	 				//残像用時間カウンター.
	AfterEffect		after[PLAYER_AFT_IMG_NUM]{};	//残像位置の履歴.

	//ダッシュ.
	float			dashTimer{};			//ダッシュの残り時間.
	float			dashCooldown{};			//ダッシュのクールダウン.

	//画像.
	double			imgRot{};				//プレイヤーの画像角度.


//▼ ===== 関数 ===== ▼.
public:
	//コンストラクタ.
	Player(int order) : ManagerBase(order) {}

	//set.
	void       SetPos            (DBL_XY     _pos ) { hit.pos         = _pos;  }
	void       SetActive         (bool       _flag) { active          = _flag; }
	void       SetMode           (PlayerMode _mode) { mode            = _mode; }
	void       SetIsNoDeath      (bool       _flag) { isNoDeath       = _flag; }
	void       SetIsDashRefEffect(bool       _flag) { isDashRefEffect = _flag; }
	//get. 
	DBL_XY     GetPos()       const { return hit.pos;   }
	bool       GetActive()    const { return active;    }
	PlayerMode GetMode()      const { return mode;      }
	Circle     GetHit()       const { return hit;       }
	bool       GetIsDashing() const { return isDashing; }
	
	bool IsMoved()       const;
	bool IsDashReflect() const;

	void Init  () override;
	void Reset () override;
	void Update() override;
	void Draw  () override;

	//Update系.
	void UpdateDash   ();	//ダッシュ更新.
	void UpdateAfter  ();	//残像更新.
	//Draw系.
	void DrawAfter    ();									 //残像描画.
	void DrawAfterNor (int idx);						  	 //残像描画(通常時)
	void DrawAfterDash(int idx, double anim1, double anim2); //残像描画(ダッシュ時)
	void DrawPlayer   ();									 //本体描画.

	void Move();		//プレイヤー移動.
	void Death();		//プレイヤー死亡.
	void Revival();		//プレイヤー復活.

	void SpawnEffectSpark(); //ダッシュ反射エフェクト生成.

	//使用禁止.
	Player(const Player*) = delete;
	Player* operator=(const Player*) = delete;
};