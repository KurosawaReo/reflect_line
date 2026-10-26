/*
   - Config_Title.h -
   タイトルシーンの設定ファイル.
*/
#pragma once
#include "Config_Main.h"

namespace Config {

	//[タイトル: 時間(秒)]
	namespace TitleTime {
		inline constexpr double LOGO_FADEIN_TIME   = 1.0; //ロゴ1枚目が出てくる時間.
		inline constexpr double LOGO_SWAP_START    = 1.0; //ロゴの切り替え開始.
		inline constexpr double LOGO_SWAP_TIME     = 1.8; //切り替えにかかる時間(上へ動く・1枚目が消える)
		inline constexpr double LOGO_FADEOUT_DELAY = 0.4; //1枚目が消え始めるまでの遅れ(切り替え開始から)
		inline constexpr double SCORE_FRAME_START  = 1.4; //ベストスコアの枠が出る.
		inline constexpr double SCORE_TEXT_START   = 1.6; //ベストスコアの文字が出る.
		inline constexpr double SCORE_ANIM_TIME    = 1.5; //ベストスコアの枠・文字が出きるまでの時間.
		inline constexpr double PUSH_START         = 3.0; //Pushテキスト表示.
		inline constexpr double BREAK_START        = 3.0; //破片演出.
	}

	//[タイトル: 配置]
	namespace TitleLayout {
		//▼ 共通.
		inline constexpr int    CENTER_X = WINDOW_WID / 2; //画面中央のx座標.

		//▼ 左下のラベル(バージョン / 操作版).
		inline const     INT_XY LABEL_POS          = { 30, WINDOW_HEI - 30 }; //1つ目(バージョン)の位置. 左下基準.
		inline constexpr int    LABEL_NEXT_OFFSET  = 50;                      //2つ目の位置: 1つ目の文字幅にこの値を足したx.
		inline constexpr double LABEL_MARGIN       = 6;                       //文字と枠の余白.
		inline const     DBL_XY LABEL_FRAME_OFFSET = { 0, 1 };                //枠の位置補正.

		//▼ ロゴ.
		inline constexpr int    LOGO_Y     = WINDOW_HEI / 2 - 80; //切り替え前のy座標.
		inline constexpr int    LOGO_RISE  = 80;                  //切り替え時に上へ動く量.
		inline const     DBL_XY LOGO_SCALE = { 0.7, 0.7 };        //描画倍率.

		//▼ ベストスコア.
		inline constexpr int    SCORE_Y             = WINDOW_HEI / 2 + 130; //基準のy座標.
		inline constexpr int    SCORE_TEXT_OFFSET_Y = -2;                   //文字のy補正.
		inline constexpr int    SCORE_FRAME_GAP     = 10;                   //枠(上下2枚)の中心からの最初の距離.
		inline constexpr int    SCORE_FRAME_OPEN    = 18;                   //枠が上下に開く量.
		inline const     DBL_XY SCORE_FRAME_SCALE   = { 0.45, 0.4 };        //枠画像の描画倍率.

		//▼ Pushテキスト.
		inline constexpr int    PUSH_Y = WINDOW_HEI / 2 + 310; //y座標.

		//▼ 破片演出.
		inline const     DBL_XY BREAK_POS   = { WINDOW_WID / 2, 850 }; //出現位置.
		inline constexpr double BREAK_SCALE = 1.4;                     //大きさ倍率.
	}

	//[タイトル: 色]
	namespace TitleColor {
		inline constexpr UINT LABEL_TEXT   = 0x00FFFF; //ラベルの文字色.
		inline constexpr UINT LABEL_GRAD_A = 0x00FFFF; //ラベル枠のグラデーション色A.
		inline constexpr UINT LABEL_GRAD_B = 0x0064FF; //ラベル枠のグラデーション色B.
		inline constexpr UINT PUSH_TEXT    = 0xFFFFFF; //Pushテキストの文字色.
	}
}
