/*
   - KR_Global.cpp - (DxLib)
*/
#include "KR_Global.h"

//[include] ".cpp"ファイルでのみ使うもの.
#include <WinNls.h>
#include "KR_DrawShape.h"

//KrLib名前空間.
namespace KR
{
	//ColorIDをカラーコードに変換.
	COLOR_U8 ColorIDToU8(ColorID id) {

		switch (id) {
			//                                {   B    G    R    A }
			case ColorID::Red:         return {   0,   0, 255, 255 }; 
			case ColorID::Orange:      return {   0, 128, 255, 255 };
			case ColorID::Yellow:      return {   0, 255, 255, 255 }; 
			case ColorID::Lime:        return {   0, 255, 128, 255 };
			case ColorID::Green:	   return {   0, 255,   0, 255 };
			case ColorID::SpringGreen: return { 128, 255,   0, 255 };
			case ColorID::SkyBlue:     return { 255, 255,   0, 255 };
			case ColorID::Cyan:        return { 255, 128,   0, 255 };
			case ColorID::Blue:        return { 255,   0,   0, 255 };
			case ColorID::Purple:      return { 255,   0, 128, 255 };
			case ColorID::Pink:        return { 255,   0, 255, 255 };
			case ColorID::Magenta:     return { 128,   0, 255, 255 };
			case ColorID::White:       return { 255, 255, 255, 255 };
			case ColorID::Gray:        return { 128, 128, 128, 255 }; 
			case ColorID::Black:       return {   0,   0,   0, 255 };
		}
		//不正な列挙値.
		return {0, 0, 0, 0};
	}

// ▼*--=<[ MY_COLOR ]>=--*▼ //

	//コンストラクタ.
	MY_COLOR::MY_COLOR() :                               color{ _byte(255), _byte(255), _byte(255), _byte(255) } {}
	MY_COLOR::MY_COLOR(int _r, int _g, int _b) :         color{ _byte(_b),  _byte(_g),  _byte(_r),  _byte(255) } {}
	MY_COLOR::MY_COLOR(int _r, int _g, int _b, int _a) : color{ _byte(_b) , _byte(_g),  _byte(_r),  _byte(_a)  } {}

	MY_COLOR::MY_COLOR(UINT _colorCode) {
		*this = _colorCode; //「=」演算子内で変換.
	}
	MY_COLOR::MY_COLOR(ColorID id) {
		*this = id;         //「=」演算子内で変換.
	}

	//get.
	COLOR_U8 MY_COLOR::GetColorU8() const {
		return color; //そのまま.
	}
	UINT MY_COLOR::GetColorCode() const { 
		return DxLib::GetColor(color.r, color.g, color.b); 
	}

	//代入演算子.
	void MY_COLOR::operator=(const COLOR_U8& rgba) {
		color = rgba; //そのまま.
	}
	void MY_COLOR::operator=(UINT colorCode) {
		//カラーコードをRGBに分解.
		color.r = (colorCode >> 16) & 0xFF;
		color.g = (colorCode >> 8)  & 0xFF;
		color.b = (colorCode)       & 0xFF;
		//透明度は255に.
		color.a = 255;
	}
	void MY_COLOR::operator=(ColorID id) {
		color = ColorIDToU8(id); //COLOR_U8に変換.
	}

	//比較演算子.
	bool MY_COLOR::operator==(ColorID id) const {

		const COLOR_U8 other = ColorIDToU8(id); //COLOR_U8に変換.
		//全て等しいならtrue.
		return 
			color.r == other.r && 
			color.g == other.g && 
			color.b == other.b && 
			color.a == other.a;
	}

// ▼*--=<[ 図形 : Circle ]>=--*▼ //

	//コンストラクタ.
	Circle::Circle() :
		pos(0, 0), r(10), color(ColorID::White), thick(1) //初期値.
	{}
	Circle::Circle(DBL_XY _pos, float _r, MY_COLOR _color, float _thick) :
		pos(_pos), r(_r), color(_color), thick(_thick) //初期値.
	{}
	//描画.
	void Circle::Draw(Anchor anc, bool isFill, bool isAnti, bool isCameraDisp) {
		DrawCircleKR(*this, anc, isFill, isAnti, isCameraDisp);
	}

// ▼*--=<[ 図形 : Box ]>=--*▼ //

	//コンストラクタ.
	Box::Box() :
		pos(0, 0), size(10, 10), color(ColorID::White), thick(1) //初期値.
	{}
	Box::Box(DBL_XY _pos, DBL_XY _size, MY_COLOR _color, float _thick) :
		pos(_pos), size(_size), color(_color), thick(_thick) //初期値.
	{}
	//描画.
	void Box::Draw(Anchor anc, bool isFill, bool isAnti, bool isCameraDisp) {
		DrawBoxKR(*this, anc, isFill, isAnti, isCameraDisp);
	}

// ▼*--=<[ 図形 : Triangle ]>=--*▼ //

	//コンストラクタ.
	Triangle::Triangle() :
		pos{ {0, 0}, {0, 0}, {0, 0} }, color(ColorID::White), thick(1) //初期値.
	{}
	Triangle::Triangle(DBL_XY _pos1, DBL_XY _pos2, DBL_XY _pos3, MY_COLOR _color, float _thick) :
		pos{ _pos1, _pos2, _pos3 }, color(ColorID::White), thick(1) //初期値.
	{}
	//描画.
	void Triangle::Draw(bool isFill, bool isAnti, bool isCameraDisp) {
		DrawTriangleKR(*this, isFill, isAnti, isCameraDisp);
	}

// ▼*--=<[ 図形 : Line ]>=--*▼ //

	//コンストラクタ.
	Line::Line() :
		stPos(0, 0), edPos(0, 0), color(ColorID::White), thick(1) //初期値.
	{}
	Line::Line(DBL_XY _stPos, DBL_XY _edPos, MY_COLOR _color, float _thick) :
		stPos(_stPos), edPos(_edPos), color(_color), thick(_thick) //初期値.
	{}
	//描画.
	void Line::Draw(bool isAnti, bool isCameraDisp) {
		DrawLineKR(*this, isAnti, isCameraDisp);
	}

// ▼*--=<[ 図形 : Pie ]>=--*▼ //

	//コンストラクタ.
	Pie::Pie() :
		pos(0, 0), r(10), stAng(0), arcAng(0), color(ColorID::White), thick(1) //初期値.
	{}
	Pie::Pie(DBL_XY _pos, double _r, double _stAng, double _arcAng, MY_COLOR _color, float _thick) :
		pos(_pos), r(_r), stAng(_stAng), arcAng(_arcAng), color(_color), thick(_thick) //初期値.
	{}
	//描画.
	void Pie::Draw(bool isAnti, bool isCameraDisp) {
		DrawPieKR(*this, isAnti, isCameraDisp);
	}

// ▼*--=<[ 図形 : BezierLine ]>=--*▼ //

	//コンストラクタ.
	BezierLine::BezierLine() :
		stPos(0, 0), edPos(0, 0), stContrPos(0, 0), edContrPos(0, 0), color(ColorID::White), thick(1) //初期値.
	{}
	BezierLine::BezierLine(DBL_XY _stPos, DBL_XY _edPos, DBL_XY _stContrPos, DBL_XY _edContrPos, MY_COLOR _color, float _thick) :
		stPos(_stPos), edPos(_edPos), stContrPos(_stContrPos), edContrPos(_edContrPos), color(_color), thick(_thick)
	{}
	//描画.
	void BezierLine::Draw(bool isDot, bool isAnti, bool isCameraDisp) {
		DrawBezierLineKR(*this, isDot, isAnti, isCameraDisp);
	}

// ▼*--=<[ 図形 : Spline ]>=--*▼ //

	//コンストラクタ.
	Spline::Spline() :
		points(0), color(ColorID::White), thick(1) //初期値.
	{}
	Spline::Spline(const vector<DBL_XY>& _points, MY_COLOR _color, float _thick) :
		points(_points), color(_color), thick(_thick)
	{}
	//描画.
	void Spline::Draw(bool isDot, bool isAnti, bool isCameraDisp) {
		DrawSplineKR(*this, isDot, isAnti, isCameraDisp);
	}

// ▼*--=<[ 図形 : Polygon ]>=--*▼ //

	//コンストラクタ.
	Polygon::Polygon() :
		pos(0, 0), points(), color(ColorID::White), thick(1)
	{}
	Polygon::Polygon(DBL_XY _pos, const vector<DBL_XY>& _points, MY_COLOR _color, float _thick) :
		pos(_pos), points(_points), color(_color), thick(_thick)
	{}
	//描画.
	void Polygon::Draw(bool isSurround, bool isAnti, bool isCameraDisp) {
		DrawPolygonKR(*this, isSurround, isAnti, isCameraDisp);
	}
}