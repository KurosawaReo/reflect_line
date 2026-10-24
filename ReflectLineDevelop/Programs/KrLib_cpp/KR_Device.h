/*
   - KR_Device.h - (C++)
   ver.2026/07/18

   デバイス情報の取得機能。
*/
#pragma once
//[include] KR_Global.
#if !defined DEF_KR_CPP_GLOBAL
  #include "KR_Global.h"
#endif

//KrLib名前空間.
namespace KR
{
	//デバイス情報を取得する関数群.
	namespace Device 
	{
		MY_STRING GetComputerNameStr();
	}
}