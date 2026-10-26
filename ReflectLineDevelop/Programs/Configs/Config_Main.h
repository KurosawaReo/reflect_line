/*
   - Config_Main.h -
   メインの設定ファイル.
*/
#pragma once

/*
   [デバッグ用操作]
   Mキー: 無敵
   Lキー: レベルタイマー加速
*/

/* ============================================================================== */
// ▼*--=<[ 定義 ]>=--*▼ //

/* デバッグ用 */
//#define DEBUG_SHOW_FPS            //FPS表示.
//#define DEBUG_CONTR_INPUT         //コントローラー操作表示.

/* ゲーム設定 */
//#define INPUT_CHANGE_ARCADE       //定義するとアーケード操作にする, しないとコントローラー操作に.
//#define BGM_NONE                  //定義するとBGMをoffに.

/* 撮影用 */
//#define INVALID_ITEM              //定義すると「アイテム」を無効にする.
//#define INVALID_OBST_NOR_LASER    //定義すると「追尾レーザー」を無効にする.
//#define INVALID_OBST_STR_LASER    //定義すると「直線レーザー」を無効にする.
//#define INVALID_OBST_RIPPLES      //定義すると「波紋」を無効にする.
//#define INVALID_OBST_FIREWORKS    //定義すると「花火」を無効にする.
//#define INVALID_OBST_METEOR       //定義すると「隕石」を無効にする.

/* ============================================================================== */

//[設定]
namespace Config {

    inline constexpr auto GAME_VERSION = _T("ver.2026/10/26"); //「REFLECT LINE」のバージョン.

    //[ファイル]
    inline constexpr auto FILE_DATA_SCORE   = _T("Datas/highscore.data"); //ハイスコア記録.
    inline constexpr auto FILE_DATA_PLAYLOG = _T("Datas/playlog.data");   //プレイ歴記録.

    //[DxLib設定]
    inline constexpr bool IS_WINDOW_MODE = false; //ウィンドウモードにするか.

    inline constexpr int WINDOW_WID       = 1920; //ウィンドウの横幅.
    inline constexpr int WINDOW_HEI       = 1080; //ウィンドウの縦幅.
    inline constexpr int WINDOW_COLOR_BIT = 32;   //ウィンドウのカラーbit数.

    inline constexpr int FPS = 80; //フレームレート.

    //[ゲーム設定]
    inline constexpr double GAME_START_TIME          = 1.0;  //ゲーム開始するまでの遅延.
    inline constexpr double TUTORIAL_START_WAIT_TIME = 1.5;  //チュートリアルで項目が開始した時の停止時間.
    inline constexpr double TUTORIAL_END_NEXT_TIME   = 1.0;  //チュートリアルで次に進む間隔時間.
    inline constexpr float TUTORIAL_RESPAWN_TIME     = 1.8f; //チュートリアルでプレイヤーが復活する時間.

    inline constexpr int REFLECT_MODE_TIME = 8;     //反射モード制限時間.
    inline constexpr float SLOW_MODE_SPEED = 0.20f; //スロー速度倍率.
    inline constexpr int SLOW_MODE_BUF_F   = 30;    //スロー継続時間(フレーム).

    //[BGM]
    inline constexpr int BGM_VOLUME_MENU     = 90; //メニューBGMの音量.
    inline constexpr int BGM_VOLUME_ENDLESS  = 65; //エンドレスモードBGMの音量.
    inline constexpr int BGM_VOLUME_TUTORIAL = 65; //チュートリアルモードBGMの音量.

    //[アニメーション]
    inline constexpr int MIDDLE_ANIM_TIME = 120; //レベルアップアニメーション時間.

    //[レベル]
    inline constexpr float SPAWN_RATE_HALF_COUNT = 8000.0f; //召喚間隔減少速度調整値(spawn rate値が半分(0.5f)になるcount値).

    inline constexpr float LEVEL_2_COUNT = 1500.0f; //Lv2に上がるcount値.
    inline constexpr float LEVEL_3_COUNT = 3500.0f; //Lv3に上がるcount値.
    inline constexpr float LEVEL_4_COUNT = 6000.0f; //Lv4に上がるcount値.
    inline constexpr float LEVEL_5_COUNT = 9000.0f; //Lv5に上がるcount値.

    //[スコア]
    inline constexpr int SCORE_TAKE_ITEM    = 100; //アイテムを取った時のスコア.
    inline constexpr int SCORE_BREAK_METEOR = 500; //隕石を壊した時のスコア.
    inline constexpr int SCORE_ANIM_TIME    = 50;  //スコアのアニメーション時間.

    //[メニュー]
    inline constexpr int MENU_OPTION_COUNT = 3; //選択肢の数.

    //[プレイヤー]
    inline constexpr int   PLAYER_SIZE             = 18;  //プレイヤーサイズ(半径).
    inline constexpr int   PLAYER_REF_ADD_SIZE     = 12;  //反射モード時に拡大する当たり判定.
    inline constexpr float PLAYER_REF_END_RANGE_R  = 200; //反射モード終了時の範囲反射の半径.
    inline constexpr int   PLAYER_ANIM_REFLECT_END = 15;  //反射モード終了時のアニメーション時間.

    inline constexpr double PLAYER_DRAW_SIZE       = 0.7; //プレイヤー本体の描画サイズ.
    inline constexpr double PLAYER_LIGHT_DRAW_SIZE = 0.4; //プレイヤー発光の描画サイズ.

    inline constexpr int PLAYER_MOVE_SPEED         = 8;   //プレイヤー移動速度.
    inline constexpr int PLAYER_AFT_IMG_NUM        = 9;   //残像データの数.
    inline constexpr int PLAYER_DEATH_ANIM_TIME    = 10;  //プレイヤー死亡アニメ時間.
    inline constexpr double PLAYER_MOVE_LERP_SPEED = 0.2; //プレイヤーの慣性スピードの調整値.

    //[プレイヤー > ダッシュ]
    inline constexpr double PLAYER_DASH_SPEED = 1.8; //ダッシュ速度.
    inline constexpr int PLAYER_DASH_DURATION = 20;  //ダッシュ持続時間(フレーム).
    inline constexpr int PLAYER_DASH_COOLDOWN = 30;  //ダッシュクールダウン(フレーム).

    inline constexpr int PLAYER_DASH_EFFECT_MAX      = 5;  //最大エフェクト数.
    inline constexpr int PLAYER_DASH_EFFECT_TIME     = 10; //エフェクト時間.
    inline constexpr int PLAYER_DASH_SPARK_ANIM_TIME = 20; //反射スパークの寿命(フレーム).
    inline constexpr int PLAYER_DASH_REFLECT_TIME    = 10; //ダッシュ反射の継続時間(フレーム).

    //[アイテム]
    inline constexpr int ITEM_SPAWN_TIME   = 600; //アイテム出現時間.
    inline constexpr int ITEM_SIZE         = 50;  //アイテムサイズ.
    inline constexpr int ITEM_SPEED        = 3;   //アイテム移動スピード.
    inline constexpr int ITEM_RESPAWN_TIME = 400; //アイテム復活時間.

    //[障害物 > レーザー(全体)]
    inline constexpr int LASER_REF_ROT_MAX     = 10; //レーザー(reflected)の曲がる限界角度.
    inline constexpr int LASER_REF_TRACK_ST_TM = 10; //レーザー(reflected)の追尾開始時間.
    inline constexpr int LASER_REF_TRACK_ED_TM = 30; //レーザー(reflected)の追尾終了時間.

    inline constexpr double LASER_NOR_SPEED = 0.1;  //レーザー(normal)の速度.
    inline constexpr double LASER_STR_SPEED = 50.0; //レーザー(straight)の速度.
    inline constexpr double LASER_REF_SPEED = 0.5;  //レーザー(reflected)の速度.

    inline constexpr int LASER_LINE_DEL_TIME = 64; //レーザー描画線が消える時間.
    inline constexpr int LASER_LINE_DRAW_LEN = 2;  //レーザー描画線を描く最低の長さ(DrawLineAAの関係上).
    inline constexpr int LASER_REF_ANIM_TIME = 30; //反射アニメーション時間.
    inline constexpr int LASER_HIT_R         = 3;  //レーザーのヒット半径.

    //[障害物 > 通常レーザー]
    inline constexpr int LASER_NOR_SHOT_RESET = 300; //砲台の発射リセット時間.
    inline constexpr int LASER_NOR_SHOT_START = 20;  //砲台の発射開始時間.
    inline constexpr int LASER_NOR_SHOT_SPAN  = 5;   //砲台の発射間隔時間.

    inline constexpr int LASER_NOR_FLASH_MAX         = 30;   //最大フラッシュエフェクト数.
    inline constexpr float LASER_NOR_FLASH_SIZE_INIT = 3.0f; //フラッシュエフェクトの初期サイズ.
    inline constexpr int LASER_NOR_FLASH_SIZE_SPREAD = -7;   //フラッシュエフェクトの拡大量.
    inline constexpr int LASER_NOR_FLASH_VALID_TM    = 100;  //フラッシュエフェクトの持続フレーム.
    inline constexpr int LASER_NOR_FLASH_ALPHA_TM    = 2;    //フラッシュエフェクトの透明化速度.

    inline constexpr int LASER_NOR_PRE_LASER1_SIZE = 50; //発射前エフェクト1のサイズ.
    inline constexpr int LASER_NOR_PRE_LASER2_SIZE = 20; //発射前エフェクト2のサイズ.

    //[障害物 > 直線レーザー]
    inline constexpr int LASER_STR_SPAWN_SPAN      = 350; //直線レーザーの出現間隔.
    inline constexpr int LASER_STR_PREDICTION_TIME = 60;  //直線レーザーの予測線の出る時間.

    //[障害物 > 落下レーザー(花火用)]
    inline constexpr int LASER_FAL_DELETE_TIME = 100; //消えるまでの時間.
    inline constexpr int LASER_FAL_HIT_TIME    = 60;  //↑のうち、当たり判定を持たせる時間.

    //[障害物 > 隕石]
    inline constexpr int METEOR_SPAWN_SPAN      = 200; //隕石の出現時間間隔.
    inline constexpr int METEOR_SPEED           = 2;   //移動速度.
    inline constexpr int METEOR_DEST_OUT_DIS    = 50;  //隕石が画面外で消える距離.
    inline constexpr int METEOR_DEST_TIME       = 20;  //隕石の破壊時間.
    inline constexpr int METEOR_GOAL_RAND_RANGE = 500; //隕石の目標地点の乱数範囲.
    inline constexpr int METEOR_BREAK_ANIM_TIME = 50;  //隕石破壊破片のアニメ時間.
    inline constexpr int METEOR_BREAK_ANIM_CNT  = 15;  //隕石破壊破片の数.

    //[障害物 > 隕石 > 構成線]
    inline constexpr int METEOR_LINE_CNT_MIN = 5;  //最低n角形.
    inline constexpr int METEOR_LINE_CNT_MAX = 16; //最大n角形.

    inline constexpr double METEOR_LINE_DIS_MIN = 30.0; //頂点の最短距離.
    inline constexpr double METEOR_LINE_DIS_MAX = 50.0; //頂点の最大距離.

    //[障害物 > 波紋]
    inline constexpr int RIPPLES_FLASH_MAX = 30; //最大フラッシュエフェクト数.

    inline constexpr float RIPPLES_FLASH_SIZE_INIT = 5.0f;  //フラッシュエフェクトの初期サイズ.
    inline constexpr int RIPPLES_FLASH_SIZE_SPREAD = 3;     //フラッシュエフェクトの拡大量.
    inline constexpr float RIPPLES_FLASH_BLINK_TM  = 15.0f; //フラッシュエフェクトの点滅時間.

    inline constexpr int RIPPLES_FLASH_VALID_TM = 50; //フラッシュエフェクトの持続フレーム.
    inline constexpr int RIPPLES_FLASH_ALPHA_TM = 2;  //フラッシュエフェクトの透明化速度.

    inline constexpr int RIPPLES_STATE_WARNING = 0; //予告状態.
    inline constexpr int RIPPLES_STATE_ACTIVE  = 1; //アクティブ状態.

    inline constexpr float RIPPLES_WARNING_DURATION = 120.0f; //予告の時間.
    inline constexpr float RIPPLES_ACTIVE_DURATION  = 60.0f;  //アクティブの時間.

    inline constexpr int RIPPLES_SPAWN_SPAN       = 350; //出現間隔.
    inline constexpr int RIPPLES_MAX_SIMULTANEOUS = 3;   //最大同時出現数.
    inline constexpr int RIPPLES_MIN_DISTANCE     = 100; //最小距離.

    //[障害物 > 花火]
    inline constexpr int   FIREWORKS_RND_MAX          = 2;    //花火出現最大数.
    inline constexpr int   FIREWORKS_SPAWN_SPAN       = 600;  //花火生成間隔（フレーム）.
    inline constexpr int   FIREWORKS_WARNING_DURATION = 120;  //予告を出す長さ（フレーム）
    inline constexpr int   FIREWORKS_LAUNCH_HEIGHT    = 50;   //打ち上げ高度.
    inline constexpr int   FIREWORKS_EXPLOSION_RADIUS = 100;  //爆発半径.
    inline constexpr int   FIREWORKS_SPARKS_COUNT     = 16;   //火花の数（ベース）.
    inline constexpr int   FIREWORKS_MIN_DISTANCE     = 120;  //花火同士の最小距離.
    inline constexpr float FIREWORKS_GRAVITY          = 0.3f; //重力加速度.
           
    //[カラー]
    inline const UINT COLOR_MODE_NOR    = GetColor(50, 255, 255);  //通常モードのメインカラー.
    inline const UINT COLOR_MODE_REF    = GetColor(255, 0, 255);   //反射モードのメインカラー.
    inline const UINT COLOR_PLY_REFLECT = GetColor(255, 155, 255);
    inline const UINT COLOR_PLY_DEBUG   = GetColor(255, 150, 150);
    inline const UINT COLOR_PLY_AFT_NOR = GetColor(105, 105, 105); //プレイヤーの残像色(通常).
    inline const UINT COLOR_PLY_AFT_REF = GetColor(255, 0, 255);   //プレイヤーの残像色(反射).
    inline const UINT COLOR_ITEM        = GetColor(255, 155, 255);
    inline const UINT COLOR_PRE_EFFECT  = GetColor(128, 128, 128); //予測演出.

    inline constexpr UINT COLOR_BEST_SCORE = 0x20F7DE;
    inline constexpr UINT COLOR_SCORE      = 0x00FFA0;
    inline constexpr UINT COLOR_TIME       = 0x80FF9C;

    //[カラー計算]
    inline UINT ColorLaserNormal(int color) {
        return GetColor(50, color, 255);
    }

    inline UINT ColorLaserReflect(int color) {
        return GetColor(color / 2 + 128, 0, 255);
    }

    inline UINT ColorMeteor(const DBL_XY& pos) {
        const double wave = Calc::AnimWave(WaveType::CosLoop, pos.x / 250);
        return GetColor(0, _int_r(100 + 155 * wave), 255);
    }
}