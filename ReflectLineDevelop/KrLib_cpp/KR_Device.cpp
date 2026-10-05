/*
   - KR_Device.cpp - (C++)
*/
#pragma once
#include "KR_Device.h"

//[include] ".cpp"ファイルでのみ使うもの.
#include <Windows.h>

//KrLib名前空間.
namespace KR
{
	//デバイス情報を取得する関数群.
	namespace Device
	{
		//デバイス名を取得する.
		MY_STRING GetComputerNameStr() {

			MY_CHAR name[MAX_COMPUTERNAME_LENGTH + 1];
			DWORD size = MAX_COMPUTERNAME_LENGTH + 1;

			//デバイス名取得.
			if (GetComputerName(name, &size)) {
				return name;
			}
			return _T("Unknown"); //不明.
		}
	}
}