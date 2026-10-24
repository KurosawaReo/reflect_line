/*
   - KR_Calc.h - (DxLib)
   ver.2026/10/15

   当たり判定などの計算機能。
*/
#pragma once
//[include] KR_Global.
#if !defined DEF_KR_DX_GLOBAL
  #include "KR_Global.h"
#endif

//KrLib名前空間.
namespace KR
{
	//計算用の関数群.
	namespace Calc
	{
		//当たり判定(図形と点)
		bool	HitCir		(const Circle& cir,   DBL_XY pos);
		bool	HitBox		(const Box&    box,   DBL_XY pos);
		bool	HitPie		(const Pie&    pie,   DBL_XY pos);

		//当たり判定(図形と図形)
		bool	HitCirCir	(const Circle& cir1,  const Circle& cir2);
		bool	HitBoxBox	(const Box&    box1,  const Box&    box2);
		bool	HitBoxCir	(const Box&    box,   const Circle& cir,  Surface* surface    = nullptr);
		bool	HitLineCir	(const Line&   line,  const Circle& cir,  DBL_XY*  nearestPos = nullptr);
		bool	HitLineLine	(const Line&   line1, const Line&   line2);

		//ベジエ曲線,スプライン曲線.
		DBL_XY	BezierPoint	(const BezierLine& bLine,  double time);
		DBL_XY	SplinePoint	(const Spline&     spline, int degree, double time);
	};
}