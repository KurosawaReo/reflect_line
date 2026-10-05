/*
   - KR_Draw.h - (DxLib)
   ver.2026/07/19

   描画に関する機能。
*/
#pragma once
//[include] KR_Global.
#if !defined DEF_KR_DX_GLOBAL
  #include "KR_Global.h"
#endif

//KrLib名前空間.
namespace KR
{
	//フォントタイプID(入力しやすくする用)
	enum class FontTypeID
	{
		Normal      = DX_FONTTYPE_NORMAL,
		Edge        = DX_FONTTYPE_EDGE,
		Anti        = DX_FONTTYPE_ANTIALIASING,      //アンチエイリアス(滑らかになる)
		AntiEdge    = DX_FONTTYPE_ANTIALIASING_EDGE, //アンチエイリアス & エッジ.

		None        = -1, //指定なし.
	};
	//描画モードID(入力しやすくする用)
	enum class DrawModeID
	{
		None        = DX_DRAWMODE_NEAREST,      //指定なし.
		Nearest		= DX_DRAWMODE_NEAREST,		//ネアレストネイバー法  (通常)
		Bilinear	= DX_DRAWMODE_BILINEAR,		//バイリニア法          (画像を縮小しても滑らかになる)
		Anisotropic	= DX_DRAWMODE_ANISOTROPIC,	//異方性フィルタリング法.
	};
	enum class DrawBlendModeID
	{
		None		= DX_BLENDMODE_NOBLEND,		//デフォルト.
		Alpha		= DX_BLENDMODE_ALPHA,		//αブレンド  (重なると透過する)
		Add			= DX_BLENDMODE_ADD,			//加算ブレンド(重なると明度が明るくなる)
		Sub			= DX_BLENDMODE_SUB,			//減算ブレンド(重なると明度が暗くなる)
		Mul			= DX_BLENDMODE_MUL			//乗算ブレンド.
	};

	//テキスト描画クラス.
	class DrawStr
	{
	public:
		MY_STRING text;  //テキスト.
		INT_XY    pos;   //画面のどこに描画するか.
		MY_COLOR  color; //文字の色.

		//コンストラクタ.
		DrawStr(MY_STRING _text, INT_XY _pos, MY_COLOR _color) : text(_text), pos(_pos), color(_color) {}

		//描画.
		void Draw	 (                                                                         Anchor anc = Anchor::Mid, int font = -1, bool isCameraDisp = true);
		void DrawRota(INT_XY extend, INT_XY pivot, double ang, bool isVertical,                                          int font = -1, bool isCameraDisp = true);
		void DrawModi(INT_XY luPos, INT_XY ruPos, INT_XY rdPos, INT_XY ldPos, bool isVertical,                           int font = -1, bool isCameraDisp = true);
	};

	//フォントクラス.
	class Font
	{
	private:
		int handle;

	public:
		//コンストラクタ, デストラクタ.
		Font();
		~Font();
		//get.
		int GetFont() const { return handle; }

		//フォント作成.
		void Create(MY_STRING fontName, int size, int thick, FontTypeID id = FontTypeID::None);

		//使用禁止(「=」で実体が複製されて、意図せずデストラクタが実行されるのを防ぐため)
		Font& operator=(const Font&) = delete;
	};

	//描画モード設定.
	class DrawMode
	{
	//▼ ===== 実体 ===== ▼.
	private:
		static DrawMode inst; //実体を入れる用.

	//▼ ===== 関数 ===== ▼.
	public:
		//描画モードを設定して描画.
		template<std::invocable Func>
		static void Exe(DrawModeID mode1, DrawBlendModeID mode2, int mode2Param, Func&& func) {

			//現在の設定を保存.
			int oldMode1 = GetDrawMode();
			int oldMode2, oldMode2Param;
			GetDrawBlendMode(&oldMode2, &oldMode2Param);

			//modeを設定.
			SetDrawMode(_int(mode1));
			SetDrawBlendMode(_int(mode2), mode2Param);

			try {
				//関数実行.
				func();
			}
			catch (...) {
				//設定を戻す.
				SetDrawMode(oldMode1);
				SetDrawBlendMode(oldMode2, oldMode2Param);
				throw;
			}
			//設定を戻す.
			SetDrawMode(oldMode1);
			SetDrawBlendMode(oldMode2, oldMode2Param);
		}
	};

	//テキストのサイズ取得.
	INT_XY GetTextSize(MY_STRING text, int font = -1);
}