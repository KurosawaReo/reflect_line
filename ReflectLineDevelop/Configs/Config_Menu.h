/*
   - Config_Menu.h -
   メニューの設定ファイル.
*/
#pragma once
#include "Config_Main.h"

//色.
namespace MenuColor {
	constexpr UINT accent = 0x00FFFF; //枠・線・見出し・タイトル・矢印.
	constexpr UINT highlight = 0x64FFFF; //選択中の文字・選択肢の枠・カーソル表.
	constexpr UINT cursorBack = 0x3296FF; //カーソル裏.
	constexpr UINT textDim = 0x969696; //未選択の文字・説明文.
	constexpr UINT bgShade = 0x000000; //背景の暗幕.
}

//レイアウト.
namespace MenuLayout {
	//▼ タイトル.
	const     DBL_XY titlePos = { WINDOW_WID / 2, 80 };	//「モード選択」の位置.
	constexpr int    titleArrowInner = 55;						//「< >」の内側(中心から).
	constexpr int    titleArrowTip = 85;						//「< >」の先端(中心から).
	constexpr int    titleArrowHalfH = 30;						//「< >」の高さの半分.
	constexpr int    titleArrowMove = 100;						//外へ広がる最大距離.
	constexpr float  titleArrowThick = 2.0f;						//線の太さ.
	const     INT_XY textOffset = { 0, -1 };				//文字描画の位置補正(全文字共通).

	//▼ 選択肢.
	const     DBL_XY menuPos = { 370, 280 };	//モード枠の位置.
	const     DBL_XY menuSize = { 400,  70 };	//モード枠のサイズ.
	constexpr int    menuSpace = 100;			//モード枠の配置間隔.
	constexpr int    blinkShake = 5;			//点滅時のブレ幅.
	constexpr int    blinkAlpha = 128;			//点滅時の透明度.

	//▼ カーソル三角.
	constexpr int    cursorGap = 20;	//枠からの距離.
	constexpr int    cursorLen = 20;	//三角の長さ.
	constexpr int    cursorHalfH = 10;	//三角の高さの半分.

	//▼ サムネ.
	const     DBL_XY imgPos = { WINDOW_WID - 560, 410 };	//サムネの基準位置.
	constexpr double thumbScale = 0.7;							//画像描画倍率.
	constexpr int    thumbMargin = 10;							//枠を画像よりどれだけ大きくするか.

	//▼ 装飾線.
	constexpr float  lineThick = 3.0f;	//線の太さ.
	constexpr int    lineSide = 30;	//画像の中心から左右の線までの距離.
	constexpr int    lineAlphaMin = 155;	//明滅の最小透明度.
	constexpr int    lineAlphaRange = 100;	//明滅の振れ幅(最小+振れ幅が最大).

	//▼ パネル(操作説明・モード説明).
	constexpr int    panelHeight = 260;			//高さ(2枚共通).
	constexpr int    panelMarginBottom = 40;			//画面下からの余白.
	constexpr int    infoWidth = 500;			//操作パネルの幅.
	const     INT_XY headingOffset = { 10, -10 };	//見出しの位置(枠の左上から).
	constexpr int    loreInner = 15;			//説明文が内側にどれだけずれるか.
	constexpr int    loreLineSpace = 30;			//説明文の行間.

	//▼ 背景.
	constexpr int    bgAlpha = 128; //暗幕の透明度.
}

//カーソルの動き.
namespace MenuMotion {
	constexpr double cursorRotBase   = 0.02;	//カーソルの基本回転速度.
	constexpr double cursorRotReturn = 0.05;	//カーソルの減衰回転速度.
	constexpr double cursorRotMove   = 5.0;		//カーソル移動時の回転速度倍率(1.0が通常)
}