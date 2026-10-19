/*
   - KR_ManagerBase.cpp - (DxLib)
*/
#include "KR_ManagerBase.h"

//[include] ".cpp"ファイルでのみ使うもの.
#include "KR_App.h"
#include <algorithm>

//KrLib名前空間.
namespace KR
{
	//コンストラクタ.
	ManagerBase::ManagerBase(MngAutoExe _mode) :
		mode(_mode), befMode(_mode)
	{}
}