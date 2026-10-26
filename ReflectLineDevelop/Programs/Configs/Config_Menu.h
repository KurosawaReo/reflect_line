/*
   - Config_Menu.h -
*/
#pragma once
#include "../Configs/Config_Main.h"

//[設定]
namespace Config {

	//[メニュー色]
	namespace MenuColor {
		inline constexpr UINT ACCENT      = 0x00FFFF; //枠・線・見出し・タイトル・矢印.
		inline constexpr UINT HIGHLIGHT   = 0x64FFFF; //選択中の文字・選択肢の枠・カーソル表.
		inline constexpr UINT CURSOR_BACK = 0x3296FF; //カーソル裏.
		inline constexpr UINT TEXT_DIM    = 0x969696; //未選択の文字・説明文.
		inline constexpr UINT BG_SHADE    = 0x000000; //背景の暗幕.
	}

	//[メニューレイアウト]
	namespace MenuLayout {
		//▼ タイトル.
		inline const     DBL_XY TITLE_POS          = { WINDOW_WID / 2, 80 }; //「モード選択」の位置.
		inline constexpr int    TITLE_ARROW_INNER  = 55;                     //「< >」の内側(中心から).
		inline constexpr int    TITLE_ARROW_TIP    = 85;                     //「< >」の先端(中心から).
		inline constexpr int    TITLE_ARROW_HALF_H = 30;                     //「< >」の高さの半分.
		inline constexpr int    TITLE_ARROW_MOVE   = 100;                    //外へ広がる最大距離.
		inline constexpr float  TITLE_ARROW_THICK  = 2.0f;                   //線の太さ.
		inline const     INT_XY TEXT_OFFSET        = { 0, -1 };              //文字描画の位置補正(全文字共通).

		//▼ 選択肢.
		inline const     DBL_XY MENU_POS    = { 370, 280 }; //モード枠の位置.
		inline const     DBL_XY MENU_SIZE   = { 400,  70 }; //モード枠のサイズ.
		inline constexpr int    MENU_SPACE  = 100;          //モード枠の配置間隔.
		inline constexpr int    BLINK_SHAKE = 5;            //点滅時のブレ幅.
		inline constexpr int    BLINK_ALPHA = 128;          //点滅時の透明度.

		//▼ カーソル三角.
		inline constexpr int    CURSOR_GAP    = 20; //枠からの距離.
		inline constexpr int    CURSOR_LEN    = 20; //三角の長さ.
		inline constexpr int    CURSOR_HALF_H = 10; //三角の高さの半分.

		//▼ サムネ.
		inline const     DBL_XY IMG_POS      = { WINDOW_WID - 560, 410 }; //サムネの基準位置.
		inline constexpr double THUMB_SCALE  = 0.7;                       //画像描画倍率.
		inline constexpr int    THUMB_MARGIN = 10;                        //枠を画像よりどれだけ大きくするか.

		//▼ 装飾線.
		inline constexpr float  LINE_THICK       = 3.0f; //線の太さ.
		inline constexpr int    LINE_SIDE        = 30;   //画像の中心から左右の線までの距離.
		inline constexpr int    LINE_ALPHA_MIN   = 155;  //明滅の最小透明度.
		inline constexpr int    LINE_ALPHA_RANGE = 100;  //明滅の振れ幅(最小+振れ幅が最大).

		//▼ パネル(操作説明・モード説明).
		inline constexpr int    PANEL_HEIGHT        = 260;         //高さ(2枚共通).
		inline constexpr int    PANEL_MARGIN_BOTTOM = 40;          //画面下からの余白.
		inline constexpr int    INFO_WIDTH          = 500;         //操作パネルの幅.
		inline const     INT_XY HEADING_OFFSET      = { 10, -10 }; //見出しの位置(枠の左上から).
		inline constexpr int    LORE_INNER          = 15;          //説明文が内側にどれだけずれるか.
		inline constexpr int    LORE_LINE_SPACE     = 30;          //説明文の行間.

		//▼ 背景.
		inline constexpr int    BG_ALPHA = 128; //暗幕の透明度.
	}

	//[メニューカーソル]
	namespace MenuCursor {
		inline constexpr double CURSOR_ROT_BASE   = 0.02; //カーソルの基本回転速度.
		inline constexpr double CURSOR_ROT_RETURN = 0.05; //カーソルの減衰回転速度.
		inline constexpr double CURSOR_ROT_MOVE   = 5.0;  //カーソル移動時の回転速度倍率(1.0が通常)
	}
}
