/*
   - KR_DrawGraph.cpp - (DxLib)
*/
#include "KR_DrawGraph.h"

//[include] ".cpp"ファイルでのみ使うもの.
#include "KR_App.h"
#include "KR_Camera.h"
#include "../KrLib_cpp/KR_Calc.h"

//KrLib名前空間.
namespace KR
{
	constexpr int NONE_HANDLE = -1; //ハンドルなし.

	//アンカー座標.
	static const DBL_XY ANCHOR_POS[9] = {
		{0.0, 0.0}, {0.5, 0.0}, {1.0, 0.0},
		{0.0, 0.5}, {0.5, 0.5}, {1.0, 0.5},
		{0.0, 1.0}, {0.5, 1.0}, {1.0, 1.0}
	};

// ▼*--=<[ Graph ]>=--*▼ //

	//コンストラクタ.
	Graph::Graph() {
		handle = NONE_HANDLE;
	}
	//デストラクタ.
	Graph::~Graph() {
		Release();
	}

	//登録.
	void Graph::Register(int _handle) {
		handle = _handle;
		GetGraphSize(_handle, &size.x, &size.y); //サイズ取得.
	}
	//解放.
	void Graph::Release() {
		//データが登録されていれば.
		if (handle != NONE_HANDLE) {
			DeleteGraph(handle);  //解放.
			handle = NONE_HANDLE; //NONEにする.
		}
	}

	//画像描画.
	void Graph::Draw(DBL_XY pos, Anchor anc, bool isTrans, bool useFloat, bool isCameraDisp) const {

		if (handle == NONE_HANDLE) {
			throw ErrorMsg(_T("Graph::Draw"), _T("ハンドル未設定"));
			return;
		}

		//描画座標の計算.
		DBL_XY drawPos;
		{
			//カメラを考慮した座標.
			const DBL_XY newPos = (isCameraDisp) ? App::ToWorldPos(pos) : pos;

			drawPos = {
				newPos.x - (size.x - 1) * ANCHOR_POS[_int(anc)].x,
				newPos.y - (size.y - 1) * ANCHOR_POS[_int(anc)].y,
			};
		}

		//float型かどうか.
		if (useFloat) {
			//float型描画.
			int err = DrawGraphF(_flt(drawPos.x), _flt(drawPos.y), handle, isTrans);
			if (err < 0) {
				throw ErrorMsg(_T("Graph::Draw"), _T("DrawGraphFエラー"));
				return;
			}
		}
		else {
			//int型描画.
			int err = DrawGraph(_int_r(drawPos.x), _int_r(drawPos.y), handle, isTrans);
			if (err < 0) {
				throw ErrorMsg(_T("Graph::Draw"), _T("DrawGraphエラー"));
				return;
			}
		}
	}

	//画像描画(Rect)
	void Graph::DrawRect(DBL_XY pos, DBL_RECT rect, Anchor anc, bool isTrans, bool useFloat, bool isCameraDisp) const {

		if (handle == NONE_HANDLE) {
			throw ErrorMsg(_T("Graph::DrawRect"), _T("ハンドル未設定"));
			return;
		}

		//描画座標の計算.
		DBL_XY drawPos;
		{
			//カメラを考慮した座標.
			const DBL_XY newPos = (isCameraDisp) ? App::ToWorldPos(pos) : pos;

			drawPos = newPos - size.ToDbl() * ANCHOR_POS[_int(anc)];
		}

		//画像の矩形.
		INT_XY drawStart = {0, 0};
		INT_XY drawSize  = size;

		//クリッピング(画像の切り取り処理)
		if (drawPos.x < rect.left) {
			drawStart.x += _int(rect.left - drawPos.x);
			drawSize.x  -= _int(rect.left - drawPos.x);
			drawPos.x = rect.left;
		}
		if (drawPos.y < rect.up) {
			drawStart.y += _int(rect.up - drawPos.y);
			drawSize.y  -= _int(rect.up - drawPos.y);
			drawPos.y = rect.up;
		}
		if (drawPos.x + drawSize.x > rect.right) {
			drawSize.x -= _int((drawPos.x + drawSize.x) - rect.right);
		}
		if (drawPos.y + drawSize.y > rect.down) {
			drawSize.y -= _int((drawPos.y + drawSize.y) - rect.down);
		}

		//描画する範囲があるなら描画.
		if (drawSize.x > 0 && drawSize.y > 0) {
			if (useFloat) {
				//float型描画.
				int err = DrawRectGraphF(
					_flt(drawPos.x), _flt(drawPos.y), drawStart.x, drawStart.y, drawSize.x, drawSize.y, handle, isTrans
				);
				if (err < 0) {
					throw ErrorMsg(_T("Graph::DrawRect"), _T("DrawRectGraphFエラー"));
					return;
				}
			}
			else {
				//int型描画.
				int err = DrawRectGraph(
					_int_r(drawPos.x), _int_r(drawPos.y), drawStart.x, drawStart.y, drawSize.x, drawSize.y, handle, isTrans
				);
				if (err < 0) {
					throw ErrorMsg(_T("Graph::DrawRect"), _T("DrawRectGraphエラー"));
					return;
				}
			}
		}
	}
	
	//画像描画(Extend)
	void Graph::DrawExtend(DBL_XY pos, DBL_XY sizeRate, Anchor anc, bool isTrans, bool useFloat, bool isCameraDisp) const {

		if (handle == NONE_HANDLE) {
			throw ErrorMsg(_T("Graph::DrawExtend"), _T("ハンドル未設定"));
			return;
		}

		//描画座標の計算.
		DBL_XY pos1, pos2;
		{
			//カメラを考慮した座標.
			const DBL_XY newPos = (isCameraDisp) ? App::ToWorldPos(pos) : pos;

			//サイズ.
			const double width  = max(1, _int_r(size.x * sizeRate.x));
			const double height = max(1, _int_r(size.y * sizeRate.y));
			//始点.
			pos1 = {
				newPos.x - width  * ANCHOR_POS[_int(anc)].x,
				newPos.y - height * ANCHOR_POS[_int(anc)].y,
			};
			//終点.
			pos2 = {
				pos1.x + width,
				pos1.y + height
			};
		}

		//float型かどうか.
		if (useFloat) {
			//float型描画.
			int err = DrawExtendGraphF(_flt(pos1.x), _flt(pos1.y), _flt(pos2.x), _flt(pos2.y), handle, isTrans);
			if (err < 0) {
				throw ErrorMsg(_T("Graph::DrawExtend"), _T("DrawExtendGraphFエラー"));
				return;
			}
		}
		else {
			//int型描画.
			int err = DrawExtendGraph(_int_r(pos1.x), _int_r(pos1.y), _int_r(pos2.x), _int_r(pos2.y), handle, isTrans);
			if (err < 0) {
				throw ErrorMsg(_T("Graph::DrawExtend"), _T("DrawExtendGraphエラー"));
				return;
			}
		}
	}

	//画像描画(Rota)
	void Graph::DrawRota(DBL_XY pos, double extend, double ang, INT_XY pivot, bool isTrans, bool useFloat, bool isCameraDisp) const {

		if (handle == NONE_HANDLE) {
			throw ErrorMsg(_T("Graph::DrawRota"), _T("ハンドル未設定"));
			return;
		}

		DBL_XY newPos = pos; //描画座標.
		double newAng = ang; //描画角度.

		//カメラ基準に変換.
		if (isCameraDisp) { 

			//[world基準]
			//カメラ→画像 の距離.
			const double wrdDist = Calc::Dist     (Camera::GetPos(), pos);
			//カメラ→画像 の角度.
			const double wrdAng  = Calc::FacingAng(Camera::GetPos(), pos);

			//[camera基準]
			//カメラ→画像 の角度.
			const double cmrAng = wrdAng - Camera::GetAng(); //カメラの角度を0とする.
			//カメラ→画像 の位置.
			const DBL_XY cmrPos = Calc::AngToVector(cmrAng) * wrdDist; //方向 * 距離.
			
			//描画座標が確定.
			//カメラ基準にするため、画面の半分ずらす.
			newPos = cmrPos + App::GetWindowSize().ToDbl()/2;
			//描画角度が確定.
			newAng -= Camera::GetAng();
		}

		//float型かどうか.
		if (useFloat) {
			//回転軸(デフォルトは画像の中心とする)
			float cx = _flt(size.x/2 + pivot.x);
			float cy = _flt(size.y/2 + pivot.y);
			//float型描画.
			int err = DrawRotaGraphFast2F(
				_flt(newPos.x), _flt(newPos.y), cx, cy, 
				_flt(extend), _flt(_rad(newAng)), handle, isTrans
			);
			if (err < 0) {
				throw ErrorMsg(_T("Graph::DrawRota"), _T("DrawRotaGraph2Fエラー"));
				return;
			}
		}
		else {
			//回転軸(デフォルトは画像の中心とする)
			int cx = size.x/2 + pivot.x;
			int cy = size.y/2 + pivot.y;
			//int型描画.
			int err = DrawRotaGraphFast2(
				_int_r(newPos.x), _int_r(newPos.y), cx, cy, _flt(extend), 
				_flt(_rad(newAng)), handle, isTrans
			);
			if (err < 0) {
				throw ErrorMsg(_T("Graph::DrawRota"), _T("DrawRotaGraph2エラー"));
				return;
			}
		}
	}
	
	//画像描画(Modi)
	void Graph::DrawModi(DBL_XY luPos, DBL_XY ruPos, DBL_XY rdPos, DBL_XY ldPos, bool isTrans, bool useFloat, bool isCameraDisp) const {
	
		if (handle == NONE_HANDLE) {
			throw ErrorMsg(_T("Graph::DrawModi"), _T("ハンドル未設定"));
			return;
		}

		//描画座標.
		DBL_XY newPos[4] = { luPos, ruPos, rdPos, ldPos };
		//カメラ基準に変換.
		if (isCameraDisp) {
			for (auto& i : newPos) { i = App::ToWorldPos(i); }
		}

		//float型かどうか.
		if (useFloat) {
			//float型描画.
			int err = DrawModiGraphF(
				_flt(newPos[0].x), _flt(newPos[0].y), _flt(newPos[1].x), _flt(newPos[1].y),
				_flt(newPos[2].x), _flt(newPos[2].y), _flt(newPos[3].x), _flt(newPos[3].y), handle, isTrans
			);
			if (err < 0) {
				throw ErrorMsg(_T("Graph::DrawModi"), _T("DrawModiGraphFエラー"));
				return;
			}
		}
		else {
			//int型描画.
			int err = DrawModiGraph(
				_int_r(newPos[0].x), _int_r(newPos[0].y), _int_r(newPos[1].x), _int_r(newPos[1].y),
				_int_r(newPos[2].x), _int_r(newPos[2].y), _int_r(newPos[3].x), _int_r(newPos[3].y), handle, isTrans
			);
			if (err < 0) {
				throw ErrorMsg(_T("Graph::DrawModi"), _T("DrawModiGraphエラー"));
				return;
			}
		}
	}

	//動画再生.
	void Graph::PlayMovie(PlayTypeID id, bool isRestart) {

		//最初から再生するなら.
		if (isRestart) {
			SeekMovieToGraph(handle, 0); //再生位置を先頭へ.
		}
		//動画再生.
		PlayMovieToGraph(handle, _int(id));
	}
	//動画再生停止.
	void Graph::StopMovie() {
		PauseMovieToGraph(handle);
	}

// ▼*--=<[ GraphMng ]>=--*▼ //

	GraphMng GraphMng::inst; //実体生成.

	//デストラクタ.
	GraphMng::~GraphMng() {
		//画像データを全て取り出す.
		for (auto& i : graphs) {
			i.second.Release(); //各画像の解放.
		}
		graphs.clear(); //データを空にする.
	}

	//共通パスを設定.
	void GraphMng::SetPath(MY_STRING _path) {
		inst.path = _path;
	}

	//取得.
	Graph* GraphMng::Get(MY_STRING saveName) {
		
		const auto imgIt = inst.graphs.find(saveName);
		//存在しなければ.
		if (imgIt == inst.graphs.end()) {
			throw ErrorMsg(_T("GraphMng::Get"), _T("取得失敗(") + saveName + _T(")"));
			return nullptr;
		}
		return &imgIt->second;
	}
	//取得(チェックあり)
	bool GraphMng::TryGet(MY_STRING saveName, Graph* ptr) {

		const auto imgIt = inst.graphs.find(saveName);
		//存在しなければ.
		if (imgIt == inst.graphs.end()) {
			return false; //取得失敗.
		}
		ptr = &inst.graphs[saveName]; //返す.
		return true; //取得成功.
	}

	/*
	   画像読み込み.
	*/
	void GraphMng::LoadImg(MY_STRING fileName, MY_STRING saveName) {

		//既に存在すれば.
		if (inst.graphs.count(saveName) > 0) {
			throw ErrorMsg(_T("GraphMng::LoadImg"), _T("使用済みの保存名(") + saveName + _T(")"));
			return;
		}

		//パスを作成.
		const MY_STRING pathFull = inst.path + fileName;

		//ファイル読み込み(画像用)
		int handle = LoadGraph(pathFull.c_str());
		if (handle < 0) {
			throw ErrorMsg(_T("GraphMng::LoadImg"), _T("読み込み失敗(") + fileName + _T(")"));
			return;
		}
		//画像登録.
		inst.graphs[saveName].Register(handle);
	}

	/*
	   画像読み込み(分割)

	   [例]
	   GraphMng::LoadImgDiv(
	       _T("test.png"), {64, 64}, {4, 1}, //64×64のサイズで横4つ縦1つに分割.
	       {"idle","run1","jump","death"}    //登録名.
	   );
	*/
	void GraphMng::LoadImgDiv(MY_STRING fileName, INT_XY size, INT_XY cnt, vector<MY_STRING> saveName) {
		
		const int divCnt = cnt.x * cnt.y; //分割数.

		//分割数と一致しない場合.
		if (saveName.size() != divCnt) {
			throw ErrorMsg(_T("GraphMng::LoadImgDiv"), _T("保存名の数が分割数と不一致"));
			return;
		}

		//LoadDivGraphからハンドルを取り出す用.
		int* pHandle = new int[divCnt];
		//パスを作成.
		const MY_STRING pathFull = inst.path + fileName;

		//画像読み込み(分割)
		int err = LoadDivGraph(pathFull.c_str(), divCnt, cnt.x, cnt.y, size.x, size.y, pHandle);
		if (err < 0) {
			delete[] pHandle; pHandle = nullptr; //配列破棄.
			throw ErrorMsg(_T("GraphMng::LoadImgDiv"), _T("LoadDivGraphエラー"));
			return;
		}

		//読み込んだ画像をループ.
		for (int i = 0; i < divCnt; i++) {
			//既に存在すれば.
			if (inst.graphs.count(saveName[i]) > 0) {
				delete[] pHandle; pHandle = nullptr; //配列破棄.
				throw ErrorMsg(_T("GraphMng::LoadDiv"), _T("使用済みの保存名が含まれている"));
				return;
			}
			//画像登録.
			inst.graphs[saveName[i]].Register(pHandle[i]);
		}

		delete[] pHandle; pHandle = nullptr; //配列破棄.
	}

	/*
	   動画読み込み.
	*/
	void GraphMng::LoadMovie(MY_STRING fileName, MY_STRING saveName) {

		//既に存在すれば.
		if (inst.graphs.count(saveName) > 0) {
			throw ErrorMsg(_T("GraphMng::LoadMovie"), _T("使用済みの保存名(") + saveName + _T(")"));
			return;
		}

		//パスを作成.
		const MY_STRING pathFull = inst.path + fileName;

		//ファイル読み込み(動画用)
		int handle = OpenMovieToGraph(pathFull.c_str());
		if (handle < 0) {
			throw ErrorMsg(_T("GraphMng::LoadMovie"), _T("読み込み失敗(") + fileName + _T(")"));
			return;
		}
		//画像登録.
		inst.graphs[saveName].Register(handle);
	}
}