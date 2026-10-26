/*---------------------------------------------------------------------/
   - stdafx.h -

   プリコンパイル済みヘッダーとして使われるもの.
   ビルド時間を短縮することが目的.
   全てのcppファイルで、最初にこのヘッダをincludeする必要がある.

   手動で入れるのが面倒なら、自動化する方法として
   プロジェクト > プロパティ > [C/C++] > [詳細設定] に
   必ず使用されるincludeファイルを設定できる所がある.
/----------------------------------------------------------------------/
   [使い方]
   ここに事前に読み込みたいヘッダを入れる.
   自作のものでも可だが、中身があまり変更されないものだけを入れる.
   (頻繁に変えるものを入れると、逆にビルドが遅くなる)

   <cstdio>などの標準ライブラリはここに入れて
   他のcppファイルから、このstdafx.hを通してライブラリを導入する流れ.

   参考
   https://www.isus.jp/products/c-compilers/stdafxh/
/---------------------------------------------------------------------*/
#pragma once

//エラー回避用.
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include <cstdio>

#include "Pragins/KrLib_Dx/KR_Global.h"
#include "Pragins/KrLib_Dx/KR_App.h"
#include "Pragins/KrLib_Dx/KR_Calc.h"
#include "Pragins/KrLib_Dx/KR_Camera.h"
#include "Pragins/KrLib_Dx/KR_Debug.h"
#include "Pragins/KrLib_Dx/KR_DrawGraph.h"
#include "Pragins/KrLib_Dx/KR_DrawShape.h"
#include "Pragins/KrLib_Dx/KR_Input.h"
#include "Pragins/KrLib_Dx/KR_ManagerBase.h"
#include "Pragins/KrLib_Dx/KR_ManagerInsts.h"
#include "Pragins/KrLib_Dx/KR_SceneMng.h"
#include "Pragins/KrLib_Dx/KR_Sound.h"
#include "Pragins/KrLib_Dx/KR_StateMachine.h"
#include "Pragins/KrLib_Dx/KR_Timer.h"
#include "Pragins/KrLib_Dx/KR_TimerMng.h"
#include "Pragins/KrLib_cpp/KR_Calc.h"
#include "Pragins/KrLib_cpp/KR_Device.h"
#include "Pragins/KrLib_cpp/KR_File.h"
#include "Pragins/KrLib_cpp/KR_Format.h"
using namespace KR;