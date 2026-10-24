/*
   - KR_DrawGraph.h - (DxLib)
   ver.2026/08/04

   画像描画に関する機能。
*/
#pragma once
//[include] KR_Global.
#if !defined DEF_KR_DX_GLOBAL
  #include "KR_Global.h"
#endif
//[include] ".h"ファイルで使うもの.
#include "KR_Draw.h"

//KrLib名前空間.
namespace KR
{
	//動画再生タイプID.
	enum class PlayTypeID
	{
		Normal = DX_PLAYTYPE_NORMAL, //ノーマル再生.
		Back   = DX_PLAYTYPE_BACK,	 //バックグラウンド再生.
		Loop   = DX_PLAYTYPE_LOOP 	 //ループ再生.
	};

	//画像・動画クラス.
	class Graph
	{
		friend class GraphMng; //アクセス許可.

	//▼ ===== 変数 ===== ▼.
	private:
		int    handle; //ハンドル.
		INT_XY size;   //画像のサイズ.

	//▼ ===== 関数 ===== ▼.
	private:
		void Register(int _handle); //登録.
		void Release();             //解放.

	public:
		//コンストラクタ, デストラクタ.
		//std::mapを使う関係でpublicに入れておく.
		Graph();
		~Graph();
		//get.
        INT_XY GetSize() const { return size; }

		//描画(画像/動画)
		void Draw      (DBL_XY pos,                                                   Anchor anc = Anchor::Mid, bool isTrans = true, bool useFloat = false, bool isCameraDisp = true) const;
		void DrawRect  (DBL_XY pos, DBL_RECT rect,                                    Anchor anc = Anchor::Mid, bool isTrans = true, bool useFloat = false, bool isCameraDisp = true) const;
		void DrawExtend(DBL_XY pos, DBL_XY sizeRate,                                  Anchor anc = Anchor::Mid, bool isTrans = true, bool useFloat = false, bool isCameraDisp = true) const;
		void DrawRota  (DBL_XY pos, double extend, double ang, INT_XY pivot = {0, 0},                           bool isTrans = true, bool useFloat = false, bool isCameraDisp = true) const;
		void DrawModi  (DBL_XY luPos, DBL_XY ruPos, DBL_XY rdPos, DBL_XY ldPos,                                 bool isTrans = true, bool useFloat = false, bool isCameraDisp = true) const;
		//動画専用.
		void PlayMovie (PlayTypeID id, bool isRestart = true);
		void StopMovie ();

		//使用禁止(「=」で実体が複製されて、意図せずデストラクタが実行されるのを防ぐため)
		Graph& operator=(const Graph&) = delete;
	};

	//画像・動画管理クラス.
	class GraphMng final
	{
	//▼ ===== 実体 ===== ▼.
	private:
		static GraphMng inst; //実体を入れる用.

	//▼ ===== 変数 ===== ▼.
	private:
		umap<MY_STRING, Graph> graphs; //画像・動画データ.
		MY_STRING              path;   //共通パス.

	//▼ ===== 関数 ===== ▼.
	private:
		//コンストラクタ.
		GraphMng(){}
		//デストラクタ.
		~GraphMng();

	public:
		//set.
		static void   SetPath   (MY_STRING path);
		//get.
		static Graph* Get       (MY_STRING saveName);
		static bool   TryGet    (MY_STRING saveName, Graph* ptr);
		
		//読み込み.
		static void   LoadImg   (MY_STRING fileName, MY_STRING saveName);
		static void   LoadImgDiv(MY_STRING fileName, INT_XY size, INT_XY cnt, vector<MY_STRING> saveName);
		static void   LoadMovie (MY_STRING fileName, MY_STRING saveName);

		//使用禁止.
		GraphMng(const GraphMng&) = delete;
		GraphMng& operator=(const GraphMng&) = delete;
	};
}