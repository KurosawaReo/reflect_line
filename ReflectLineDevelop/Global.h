/*
   - Global.h -

   REFLINE用のグローバル.
   KR_Library用のグローバルは分類済み.
*/
#pragma once

//再定義防止用(TODO:なぜかこれを外すと再定義エラーになる)
#if !defined GLOBAL
#define GLOBAL

//====================================================================//
//【設定】

//デバッグ用操作.
//Mキー: 無敵.
//Lキー: レベルタイマー加速.

/* デバッグ用 */
//#define DEBUG_OBJ_ACTIVE		//オブジェクト出現数表示.
//#define DEBUG_SPAWN_RATE		//召喚間隔表示.
//#define DEBUG_SHOW_FPS		//FPS表示.　
//#define DEBUG_CONTR_INPUT		//コントローラ操作表示.

/* ゲーム設定 */
//#define INPUT_CHANGE_ARCADE   //定義するとアーケード操作にする, しないとコントローラ操作に.
//#define BGM_NONE              //定義するとBGMをoffに.

/* 撮影用 */
//#define INVALID_ITEM            //定義すると「アイテム」　　を無効にする.
//#define INVALID_OBST_NOR_LASER  //定義すると「追尾レーザー」を無効にする.
//#define INVALID_OBST_STR_LASER  //定義すると「直線レーザー」を無効にする.
//#define INVALID_OBST_RIPPLES    //定義すると「波紋」　　　　を無効にする.
//#define INVALID_OBST_FIREWORKS  //定義すると「花火」　　　　を無効にする.
//#define INVALID_OBST_METEOR     //定義すると「隕石」　　　　を無効にする.

//====================================================================//
//【定数】

//DxLib設定.
#define FILE_DATA_SCORE					_T("Data/highscore.data")	//ハイスコア記録.
#define FILE_DATA_PLAYLOG				_T("Data/playlog.data")		//プレイ歴記録.

#define IS_WINDOW_MODE					(false)			//ウィンドウモードにするか.

#define WINDOW_WID						(1920)			//ウィンドウの横幅.
#define WINDOW_HEI						(1080)			//ウィンドウの縦幅.
#define WINDOW_COLOR_BIT				(32)			//ウィンドウのカラーbit数.

#define FPS								(80)			//フレームレート.

//ゲーム設定.
#define GAME_START_TIME					(1.0)			//ゲーム開始するまでの遅延.
#define TUTORIAL_START_WAIT_TIME        (1.5)           //チュートリアルで項目が開始した時の停止時間.
#define TUTORIAL_END_NEXT_TIME          (1.0)           //チュートリアルで次に進む間隔時間.
#define TUTORIAL_RESPAWN_TIME           (1.8f)          //チュートリアルでプレイヤーが復活する時間.

#define REFLECT_MODE_TIME				(8)             //反射モード制限時間.
#define SLOW_MODE_SPEED					(0.20f)			//スロー速度倍率.
#define SLOW_MODE_DIS_LEN				(50)			//スローになるプレイヤーとレーザーの距離範囲.
#define SLOW_MODE_BUF_F                 (30)            //スローが継続するフレーム数.

#define BGM_VOLUME_MENU					(90)			//メニューBGMの音量.
#define BGM_VOLUME_ENDLESS				(65)			//エンドレスモードBGMの音量.
#define BGM_VOLUME_TUTORIAL				(65)			//チュートリアルモードBGMの音量.

#define MIDDLE_ANIM_TIME				(120)			//レベルアップアニメーション時間.

//プレイヤー.
#define PLAYER_SIZE						(18)			//プレイヤーサイズ(半径)
#define PLAYER_REF_ADD_SIZE				(12)			//反射モード時に拡大する当たり判定.
#define PLAYER_DRAW_SIZE				(0.7)			//プレイヤー本体の描画サイズ.
#define PLAYER_LIGHT_DRAW_SIZE			(0.4)			//プレイヤー発光の描画サイズ.
#define PLAYER_MOVE_SPEED				(8)				//プレイヤー移動速度.
#define PLAYER_AFT_IMG_NUM				(9)             //残像データの数.
#define PLAYER_DEATH_ANIM_TIME			(10)			//プレイヤー死亡アニメ時間.
#define PLAYER_MOVE_LERP_SPEED          (0.2)           //プレイヤーの慣性スピードの調整.
//プレイヤー > ダッシュ.
#define PLAYER_DASH_SPEED               (1.8)			//ダッシュ速度.
#define PLAYER_DASH_DURATION            (20)            //ダッシュ持続時間(フレーム)
#define PLAYER_DASH_COOLDOWN            (30)            //ダッシュクールダウン(フレーム)
#define PLAYER_DASH_EFFECT_MAX          (5)             //最大エフェクト数.
#define PLAYER_DASH_EFFECT_TIME			(10)			//エフェクト時間.
#define PLAYER_DASH_SPARK_ANIM_TIME     (20)            //反射スパークの寿命(フレーム).
#define PLAYER_DASH_REFLECT_TIME		(10)			//ダッシュ反射の継続時間.

//アイテム.
#define ITEM_SPAWN_TIME					(600)			//アイテムの出現時間.
#define ITEM_SIZE						(50)			//アイテムサイズ.
#define ITEM_SPEED						(3)				//アイテム移動スピード.  
#define ITEM_RESPAWN_TIME				(400)           //アイテム復活時間.

//レーザー総合.
#define LASER_REF_ROT_MAX				(10)			//レーザー(reflected)の曲がる限界角度.
#define LASER_REF_TRACK_ST_TM			(10)			//レーザー(reflected)の追尾開始時間.
#define LASER_REF_TRACK_ED_TM			(30)			//レーザー(reflected)の追尾終了時間.
#define LASER_NOR_SPEED					(0.1)		    //レーザー(normal)   の速度.
#define LASER_STR_SPEED					(50.0)		    //レーザー(straight) の速度.
#define LASER_REF_SPEED					(0.5)		    //レーザー(reflected)の速度.
#define LASER_LINE_DEL_TIME				(64)			//レーザー描画線が消える時間.
#define LASER_LINE_DRAW_LEN				(2)				//レーザー描画線を描く最低の長さ(DrawLineAAの関係上)
#define LASER_REF_ANIM_TIME				(30)			//反射アニメーション時間.
#define LASER_HIT_R						(3)				//レーザーのヒット半径.

//障害物: 通常レーザー.
#define LASER_NOR_SHOT_RESET			(300)			//砲台の発射リセット時間.
#define LASER_NOR_SHOT_START			(20)			//砲台の発射開始時間.
#define LASER_NOR_SHOT_SPAN				(5)				//砲台の発射間隔時間.

#define LASER_NOR_FLASH_MAX				(30)            //最大フラッシュエフェクト数.
#define LASER_NOR_FLASH_SIZE_INIT		(3.0f)          //フラッシュエフェクトの初期サイズ.
#define LASER_NOR_FLASH_SIZE_SPREAD		(-7)            //フラッシュエフェクトの拡大量.
#define LASER_NOR_FLASH_VALID_TM		(100)           //フラッシュエフェクトの持続フレーム.
#define LASER_NOR_FLASH_ALPHA_TM		(2)             //フラッシュエフェクトの透明化速度.
#define LASER_NOR_PRE_LASER1_SIZE		(50)            //発射前エフェクト1のサイズ.
#define LASER_NOR_PRE_LASER2_SIZE		(20)            //発射前エフェクト2のサイズ.

//障害物: 直線レーザー.
#define LASER_STR_SPAWN_SPAN			(350)			//直線レーザーの出現間隔.
#define LASER_STR_PREDICTION_TIME		(60)			//直線レーザーの予測線の出る時間.

//障害物: 落下レーザー(花火用)
#define LASER_FAL_DEL_TIME				(100)			//消えるまでの時間.
#define LASER_FAL_HIT_ABLE				(60)			//↑のうち、当たり判定を持たせる時間.

//障害物: 隕石.
#define METEOR_SPAWN_SPAN				(200)			//隕石の出現時間間隔.
#define METEOR_SPEED					(2)				//移動速度.
#define METEOR_DEST_OUT_DIS				(50)			//隕石が画面外で消える距離.
#define METEOR_DEST_TIME				(20)			//隕石の破壊時間.
#define METEOR_GOAL_RAND_RANGE			(500)			//隕石の目標地点の乱数範囲.
#define METEOR_BREAK_ANIM_TIME			(50)			//隕石破壊破片のアニメ時間.
#define METEOR_BREAK_ANIM_CNT			(15)			//隕石破壊破片の数.

#define METEOR_LINE_CNT_MIN				(5)				//最低n角形.
#define METEOR_LINE_CNT_MAX				(16)			//最大n角形.
#define METEOR_LINE_DIS_MIN				(30)			//頂点の最短距離.
#define METEOR_LINE_DIS_MAX				(50)			//頂点の最大距離.

//障害物: 波紋.
#define RIPPLES_FLASH_MAX				(30)            //最大フラッシュエフェクト数.
#define RIPPLES_FLASH_SIZE_INIT			(5.0f)          //フラッシュエフェクトの初期サイズ.
#define RIPPLES_FLASH_SIZE_SPREAD		(3)             //フラッシュエフェクトの拡大量.
#define RIPPLES_FLASH_BLINK_TM			(15.0f)         //フラッシュエフェクトの点滅時間.
#define RIPPLES_FLASH_VALID_TM			(50)            //フラッシュエフェクトの持続フレーム.
#define RIPPLES_FLASH_ALPHA_TM			(2)             //フラッシュエフェクトの透明化速度.
#define RIPPLES_STATE_WARNING			(0)				//予告状態.
#define RIPPLES_STATE_ACTIVE			(1)				//アクティブ状態.
#define RIPPLES_WARNING_DURATION		(120.0f)		//予告の時間.
#define RIPPLES_ACTIVE_DURATION			(60.0f)			//アクティブの時間.
#define RIPPLES_SPAWN_SPAN				(350)			//出現間隔.
#define RIPPLES_MAX_SIMULTANEOUS		(3)				//最大同時出現数.
#define RIPPLES_MIN_DISTANCE			(100)			//最小距離.

//障害物: 花火.
#define FIREWORKS_RND_MAX				(2)				//花火出現最大数
#define FIREWORKS_SPAWN_SPAN			(600)			//花火生成間隔（フレーム）
#define FIREWORKS_WARNING_DURATION		(120)			//予告を出す長さ（フレーム）
#define FIREWORKS_LAUNCH_HEIGHT			(50)			//打ち上げ高度
#define FIREWORKS_EXPLOSION_RADIUS		(100)			//爆発半径
#define FIREWORKS_SPARKS_COUNT			(16)			//火花の数（ベース）
#define FIREWORKS_MIN_DISTANCE			(120)			//花火同士の最小距離
#define FIREWORKS_GRAVITY				(0.3f)			//重力加速度

//メニュー.
#define MENU_ELECTR_MOVE_SPEED			(0.1)			//電気の動く速度.

//スコア.
#define SCORE_TAKE_ITEM					(100)			//アイテムを取った時のスコア.
#define SCORE_BREAK_METEOR				(500)			//隕石を壊した時のスコア.
#define SCORE_ANIM_TIME					(50)			//スコアのアニメーション時間.

//カラー.
#define COLOR_MODE_NOR					(GetColor(50, 255, 255))		//通常モードのメインカラー.
#define COLOR_MODE_REF					(GetColor(255, 0, 255))			//反射モードのメインカラー.
#define COLOR_LASER_NORMAL(color)		(GetColor(50, color, 255))		//レーザー(通常)
#define COLOR_LASER_REFLECT(color)		(GetColor(color/2+128, 0, 255)) //レーザー(反射)
#define COLOR_PLY_REFLECT				(GetColor(255, 155, 255))
#define COLOR_PLY_DEBUG					(GetColor(255, 150, 150))
#define COLOR_PLY_AFT_NOR				(GetColor(105, 105, 105))		//プレイヤーの残像色(通常)
#define COLOR_PLY_AFT_REF				(GetColor(255,   0, 255))		//プレイヤーの残像色(反射)
#define COLOR_ITEM						(GetColor(255, 155, 255))
#define COLOR_PRE_EFFECT				(GetColor(128, 128, 128))		//予測演出.
#define COLOR_METEOR(pos)				(GetColor(0, _int_r(100 + 155 * Calc::AnimWave(WaveType::CosLoop, pos.x/250)), 255)) //隕石.
#define COLOR_BEST_SCORE				(0x20F7DE)
#define COLOR_SCORE						(0x00FFA0)
#define COLOR_TIME						(0x80FF9C)

#endif