/*
   - KR_Object.cpp - (DxLib)
*/
#include "KR_Object.h"

//[include] ".cpp"ファイルでのみ使うもの.
#include "KR_Calc.h"
#include "KR_DrawGraph.h"
#include "KR_DrawShape.h"
#include "KR_Input.h"
#include "KR_ManagerInsts.h"
#include "../KrLib_cpp/KR_Calc.h"

//KrLib名前空間.
namespace KR
{
// ▼*--=<[ ObjectShape ]>=--*▼ //

	//画像.
	void ObjectShape::SetImage(MY_STRING name) {
		useImg.clear();         //リセット.
		useImg.push_back(name); //画像名を登録.
		useImgNo = 0;           //1枚しかない場合は0で固定.
	}
	void ObjectShape::SetImages(vector<MY_STRING> names, float changeTime) {
		//同じものでなければ.
		if (useImg != names) {
			useImg = names; //画像名配列を登録.
			useImgNo = 0;   //最初は0番目から.
			//切り替え時間の設定.
			tmImgAnim = Timer(TimerMode::CountDown, changeTime);
		}
	}
	//画像アニメーション設定.
	//ポーズ画面などに使う想定.
	void ObjectShape::StopImageAnim(bool isStop) {
		if (isStop) {
			tmImgAnim.Pause(); //停止する(ポーズ)
		}
		else {
			tmImgAnim.Start(); //再生する.
		}
	}
	//画像更新.
	void ObjectShape::UpdateImage() {
		//複数の画像がある場合のみ.
		if (useImg.size() > 1) {
			//一定時間で画像切り替え.
			if (tmImgAnim.IntervalTime()) {
				useImgNo = (useImgNo + 1) % useImg.size(); //(0～size-1)を順番にループ.
			}
		}
	}

	//移動限界を越えないよう位置修正.
	void ObjectShape::FixPosInArea(DBL_RECT rect) {
		Calc::FixPosInArea(GetPosPtr(), GetSize().ToInt(), rect);
	}
	//エリアを越えているかどうか.
	bool ObjectShape::IsOutInArea(DBL_RECT rect, bool isCompOut, Surface* surface) {
		return Calc::IsOutInArea(GetPos(), GetSize().ToInt(), rect, isCompOut, surface);
	}
	//距離を求める.
	double ObjectShape::Dist(DBL_XY pos) {
		return Calc::Dist(GetPos(), pos);
	}
	//角度と長さから円周上の座標を求める.
	DBL_XY ObjectShape::ArcPos(double ang, double len) {
		return Calc::ArcPos(GetPos(), ang, len);
	}
	//対象座標を見た時の方向を求める.
	double ObjectShape::FacingAng(DBL_XY targetPos) {
		return Calc::FacingAng(GetPos(), targetPos);
	}

	//移動操作.
	void ObjectShape::MoveKey4Dir(float speed, bool isWASD, bool isArrow, bool isUpDown, bool isLeftRight) {
		//入力取得.
		const DBL_XY input = ManagerInsts::Get<InputMng>()->GetKey4Dir(isWASD, isArrow, isUpDown, isLeftRight);
		//現在地 + 入力 * 速度.
		SetPos(GetPos() + input * speed);
	}
	void ObjectShape::MovePad4Dir(float speed) {
		//入力取得.
		const DBL_XY input = ManagerInsts::Get<InputMng>()->GetPad4Dir();
		//現在地 + 入力 * 速度.
		SetPos(GetPos() + input * speed);
	}
	void ObjectShape::MovePadStick(float speed) {
		//入力取得.
		const DBL_XY input = ManagerInsts::Get<InputMng>()->GetPadStick();
		//現在地 + 入力 * 速度.
		SetPos(GetPos() + input * speed);
	}
	void ObjectShape::MoveMousePos(bool isMoveX, bool isMoveY) {
		//入力取得.
		const DBL_XY input = ManagerInsts::Get<InputMng>()->GetMousePos();
		//有効ならマウス座標を反映.
		double x = (isMoveX) ? input.x : GetPos().x;
		double y = (isMoveY) ? input.y : GetPos().y;
		SetPos({x, y});
	}
	
	//DrawGraph描画.
	void ObjectShape::DrawGraph(Anchor anc, bool useFloat, bool isCameraDisp) {

		//非アクティブなら描画しない.
		if (!isActive) { return; }

		UpdateImage(); //画像更新.

		//画像名を登録しているなら.
		if (useImg.size() > 0) {
			//画像データがある場合.
			if (auto pImg = GraphMng::Get(useImg[useImgNo])) {
				//座標にoffsetを足す.
				DBL_XY pos = GetPos() + offset;
				//描画.
				try {
					pImg->Draw(pos, anc, true, useFloat, isCameraDisp);
				}
				catch (const ErrorMsg& err) {
					throw ErrorMsg(_T("ObjectShape::DrawGraph"), err.GetMsg());
					return;
				}
			}
			else {
				DrawShape(); //代わりに図形を描画.
				throw ErrorMsg(_T("ObjectShape::DrawGraph"), _T("不明な画像名"));
				return;
			}
		}
		else {
			DrawShape(); //代わりに図形を描画.
			throw ErrorMsg(_T("ObjectShape::DrawGraph"), _T("画像未登録"));
			return;
		}
	}

	//DrawRectGraph描画.
	void ObjectShape::DrawRectGraph(DBL_RECT rect, Anchor anc, bool useFloat, bool isCameraDisp) {

		//非アクティブなら描画しない.
		if (!isActive) { return; }

		UpdateImage(); //画像更新.

		//画像名を登録しているなら.
		if (useImg.size() > 0) {
			//画像データがある場合.
			if (auto pImg = GraphMng::Get(useImg[useImgNo])) {
				//座標にoffsetを足す.
				DBL_XY pos = GetPos() + offset;
				//描画.
				try {
					pImg->DrawRect(pos, rect, anc, true, useFloat, isCameraDisp);
				}
				catch (const ErrorMsg& err) {
					throw ErrorMsg(_T("ObjectShape::DrawRectGraph"), err.GetMsg());
					return;
				}
			}
			else {
				DrawShape(); //代わりに図形を描画.
				throw ErrorMsg(_T("ObjectShape::DrawRectGraph"), _T("不明な画像名"));
				return;
			}
		}
		else {
			DrawShape(); //代わりに図形を描画.
			throw ErrorMsg(_T("ObjectShape::DrawRectGraph"), _T("画像未登録"));
			return;
		}
	}

	//DrawExtendGraph描画.
	void ObjectShape::DrawExtendGraph(DBL_XY sizeRate, Anchor anc, bool useFloat, bool isCameraDisp) {

		//非アクティブなら描画しない.
		if (!isActive) { return; }

		UpdateImage(); //画像更新.

		//画像名を登録しているなら.
		if (useImg.size() > 0) {
			//画像データがある場合.
			if (auto pImg = GraphMng::Get(useImg[useImgNo])) {
				//座標にoffsetを足す.
				DBL_XY pos = GetPos() + offset;
				//描画.
				try {
					pImg->DrawExtend(pos, sizeRate, anc, true, useFloat, isCameraDisp);
				}
				catch (const ErrorMsg& err) {
					throw ErrorMsg(_T("ObjectShape::DrawExtendGraph"), err.GetMsg());
					return;
				}
			}
			else {
				DrawShape(); //代わりに図形を描画.
				throw ErrorMsg(_T("ObjectShape::DrawExtendGraph"), _T("不明な画像名"));
				return;
			}
		}
		else {
			DrawShape(); //代わりに図形を描画.
			throw ErrorMsg(_T("ObjectShape::DrawExtendGraph"), _T("画像未登録"));
			return;
		}
	}

	//DrawRotaGraph描画.
	void ObjectShape::DrawRotaGraph(double ang, double sizeRate, INT_XY pivot, bool useFloat, bool isCameraDisp) {

		//非アクティブなら描画しない.
		if (!isActive) { return; }

		UpdateImage(); //画像更新.

		//画像名を登録しているなら.
		if (useImg.size() > 0) {
			//画像データがある場合.
			if (auto pImg = GraphMng::Get(useImg[useImgNo])) {
				//座標にoffsetを足す.
				DBL_XY pos = GetPos() + offset;
				//描画.
				try {
					pImg->DrawRota(pos, sizeRate, ang, pivot, true, useFloat, isCameraDisp);
				}
				catch (const ErrorMsg& err) {
					throw ErrorMsg(_T("ObjectShape::DrawRotaGraph"), err.GetMsg());
					return;
				}
			}
			else {
				DrawShape(); //代わりに図形を描画.
				throw ErrorMsg(_T("ObjectShape::DrawRotaGraph"), _T("不明な画像名"));
				return;
			}
		}
		else {
			DrawShape(); //代わりに図形を描画.
			throw ErrorMsg(_T("ObjectShape::DrawRotaGraph"), _T("画像未登録"));
			return;
		}
	}

// ▼*--=<[ ObjectCir ]>=--*▼ //

	//円との判定.
	bool ObjectCir::HitCheckCir(const Circle& cir) const {
		return Calc::HitCirCir(cir, this->cir);
	}
	//四角形との判定.
	bool ObjectCir::HitCheckBox(const Box& box) const {
		return Calc::HitBoxCir(box, this->cir);
	}
	//線との当たり判定.
	bool ObjectCir::HitCheckLine(const Line& line, DBL_XY* nearestPos) const {
		return Calc::HitLineCir(line, this->cir, nearestPos);
	}

	//図形(円)を描画.
	void ObjectCir::DrawShape(bool isFill, bool isAnti, bool isCameraDisp) const {

		//非アクティブなら描画しない.
		if (!isActive) { return; }

		Circle tmpCir = cir;
		//座標にoffsetを足す.
		tmpCir.pos += offset;
		//描画.
		try {
			DrawCircleKR(tmpCir, Anchor::Mid, isFill, isAnti, isCameraDisp);
		}
		catch (const ErrorMsg& err) {
			throw ErrorMsg(_T("ObjectCir::DrawShape"), err.GetMsg());
			return;
		}
	}

// ▼*--=<[ ObjectBox ]>=--*▼ //

	//円との判定.
	bool ObjectBox::HitCheckCir(const Circle& cir) const {
		return Calc::HitBoxCir(this->box, cir);
	}
	//四角形との判定.
	bool ObjectBox::HitCheckBox(const Box& box) const {
		return Calc::HitBoxBox(this->box, box);
	}

	//図形(四角形)を描画.
	void ObjectBox::DrawShape(bool isFill, bool isAnti, bool isCameraDisp) const {

		//非アクティブなら描画しない.
		if (!isActive) { return; }

		Box tmpBox = box;
		//座標にoffsetを足す.
		tmpBox.pos += offset;
		//描画.
		try {
			DrawBoxKR(tmpBox, Anchor::Mid, isFill, isAnti, isCameraDisp);
		}
		catch (const ErrorMsg& err) {
			throw ErrorMsg(_T("ObjectBox::DrawShape"), err.GetMsg());
			return;
		}
	}

// ▼*--=<[ ObjectPoint ]>=--*▼ //

	//図形(点)を描画.
	void ObjectPoint::DrawShape(bool isFill, bool isAnti, bool isCameraDisp) const {

		//非アクティブなら描画しない.
		if (!isActive) { return; }

		//小さい円を描画する.
		Circle tmpCir;
		tmpCir.pos   = pos + offset;
		tmpCir.r     = 5;
		tmpCir.color = ColorID::Red;

		//描画.
		try {
			DrawCircleKR(tmpCir, Anchor::Mid, isFill, isAnti, isCameraDisp);
		}
		catch (const ErrorMsg& err) {
			throw ErrorMsg(_T("ObjectPoint::DrawShape"), err.GetMsg());
			return;
		}
	}
}