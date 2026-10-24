/*
   - BG2.cpp -
*/
#include "BG2.h"

//依存関係.
#include "Configs/Config_Main.h"
#include "Configs/Config_BG2.h"
#include "GameData.h"
#include <cmath>
//参照.
static GameData* gameData;

/* ============================================================================== */
// ▼*--=<[ 関数 ]>=--*▼ //

//出現位置を「向き」と「半径」で決める(rMin未満には出さない).
static void RandSpawnXY(double& x, double& y, double rMin) {
	const double ang = Calc::RandNum(0.0, M_PI * 2); //向き.
	//2乗の範囲で乱数を取り平方根にすると、面積あたりの密度が均等になる.
	const double r = sqrt(Calc::RandNum(pow(rMin, 2), pow(Config::BG2::SPAWN_R_MAX, 2)));
	x = cos(ang) * r;
	y = sin(ang) * r;
}

/* ============================================================================== */

//初期化.
void BG2::Init()
{
	//参照取得.
	gameData = ManagerInsts::Get<GameData>();
	//画像取得.
	grBgStarNor    = GraphMng::Get(_T("bg_star_nor"));
	grBgStarRef    = GraphMng::Get(_T("bg_star_ref"));
	grBgSpace3DNor = GraphMng::Get(_T("bg_space3d_nor"));
	grBgSpace3DRef = GraphMng::Get(_T("bg_space3d_ref"));
}

//リセット.
void BG2::Reset()
{
	angle = 0.0;

	//六角形.
	for (int i = 0; i < HEX_NUM; i++)
	{
		hex[i].z = Config::BG2::Z_MIN + i * Config::BG2::HEX_INTERVAL;
	}
	//線: 初期はz全域にばらまく.
	for (auto& i : point)
	{
		Respawn(i, Config::BG2::POINT_R_MIN, Calc::RandNum(Config::BG2::Z_MIN, Config::BG2::Z_MAX));
		i.oldZ = i.z;
	}
	//星: 初期はz全域にばらまく.
	for (auto& i : star)
	{
		Respawn(i, Config::BG2::STAR_R_MIN, Calc::RandNum(Config::BG2::Z_MIN, Config::BG2::Z_MAX));
	}
}

//更新.
void BG2::Update()
{
	angle += Config::BG2::ROT_SPEED * gameData->speedRate;

	for (auto& i : hex)
	{
		//移動.
		i.z -= Config::BG2::MOVE_SPEED * gameData->speedRate;

		//手前へ来たら奥へ戻す.
		if (i.z < Config::BG2::Z_MIN) {
			i.z += HEX_NUM * Config::BG2::HEX_INTERVAL;
		}
	}
	for (auto& i : point)
	{
		//移動.
		i.z -= Config::BG2::MOVE_SPEED * gameData->speedRate;
		i.oldZ = i.z + Config::BG2::POINT_LEN;

		//手前へ来たら奥へ戻す.
		if (i.z < Config::BG2::Z_MIN)
		{
			Respawn(i, Config::BG2::POINT_R_MIN, Config::BG2::Z_MAX);
			i.oldZ = i.z;
		}
	}
	for (auto& i : star)
	{
		i.z -= Config::BG2::MOVE_SPEED * gameData->speedRate;

		//手前へ来たら奥へ戻す.
		if (i.z < Config::BG2::Z_MIN)
		{
			Respawn(i, Config::BG2::STAR_R_MIN, Config::BG2::Z_MAX);
		}
	}
}

//描画(通常時)
void BG2::DrawNor(double modeAlpha)
{
	DrawAtmosphere(modeAlpha, false);
	DrawHexagons  (modeAlpha, Config::COLOR_MODE_NOR);
	DrawPoints    (modeAlpha, Config::COLOR_MODE_NOR);
	DrawStars     (modeAlpha, false);
}

//描画(反射モード)
void BG2::DrawRef(double modeAlpha)
{
	DrawAtmosphere(modeAlpha, true);
	DrawHexagons  (modeAlpha, Config::COLOR_MODE_REF);
	DrawPoints    (modeAlpha, Config::COLOR_MODE_REF);
	DrawStars     (modeAlpha, true);
}

//描画(光のグラデーション)
void BG2::DrawAtmosphere(double modeAlpha, bool isReflect)
{
	DrawMode::Exe(
		DrawModeID::None, DrawBlendModeID::Alpha, _int(255 * modeAlpha),
		[&]()
		{
			//反射モードかどうか.
			if (isReflect) {
				grBgSpace3DRef->Draw(App::GetWindowRect().GetMid().ToDbl()); //反射モード.
			}
			else {
				grBgSpace3DNor->Draw(App::GetWindowRect().GetMid().ToDbl()); //通常モード.
			}
		}
	);
}

//描画(線)
void BG2::DrawPoints(double modeAlpha, MY_COLOR color)
{
	//画面中央.
	const DBL_XY center = App::GetWindowRect().GetMid().ToDbl();

	//==============================
	// 疑似3D ワープライン
	//==============================

	double c = cos(angle * 0.3);
	double s = sin(angle * 0.3);

	for (auto& i : point)
	{
		//奥すぎる場合は描画しない.
		if (i.z <= 1.0) {
			continue;
		}

		//--------------------------------
		// 回転
		//--------------------------------
		double rx = i.x * c - i.y * s;
		double ry = i.x * s + i.y * c;

		//--------------------------------
		// 現在座標
		//--------------------------------
		double scale = Config::BG2::FOV / i.z;

		DBL_XY now =
		{
			center.x + rx * scale,
			center.y + ry * scale
		};

		//画面外なら描画しない.
		if (now.x < -100 || now.x > Config::WINDOW_WID + 100) {
			continue;
		}
		if (now.y < -100 || now.y > Config::WINDOW_HEI + 100) {
			continue;
		}

		//--------------------------------
		// 前フレーム座標
		//--------------------------------
		double oldScale = Config::BG2::FOV / i.oldZ;

		DBL_XY old =
		{
			center.x + rx * oldScale,
			center.y + ry * oldScale
		};

		//--------------------------------
		// 線の太さ
		//--------------------------------
		float thick = _flt(5.0 * (1.0 - i.z / Config::BG2::Z_MAX));

		//--------------------------------
		// 描画
		//--------------------------------
		Line line;
		line.stPos = old;
		line.edPos = now;
		line.color = color;
		line.thick = thick;

		//透明度を計算(奥ほど薄くする)
		const double alpha = 128 * (1.0 - i.z / Config::BG2::Z_MAX) * modeAlpha;

		DrawMode::Exe(
			DrawModeID::None, DrawBlendModeID::Alpha, _int(alpha),
			[&]() {
				line.Draw(true);
			}
		);
	}
}

//描画(六角形).
void BG2::DrawHexagons(double modeAlpha, MY_COLOR color)
{
	//画面中央.
	const DBL_XY center = App::GetWindowRect().GetMid().ToDbl();

	//頂点の方向を事前計算.
	DBL_XY directions[6];
	for (int i = 0; i < 6; i++)
	{
		const double a = angle + i * M_PI / 3;
		directions[i] = { cos(a), sin(a) };
	}

	//六角形を描画.
	for (auto& h : hex)
	{
		const double scale = Config::BG2::FOV_HEX / h.z;
		const double radius = scale * 80;

		//小さすぎる六角形は描画しない.
		if (radius < 3) {
			continue;
		}

		//ポリゴンの作成.
		KR::Polygon poly;
		poly.color = color;
		poly.thick = 2;

		//事前計算した方向ベクトルから頂点を作成.
		for (const auto& d : directions)
		{
			poly.points.push_back({
				center.x + d.x * radius,
				center.y + d.y * radius
			});
		}

		//奥ほど透明にする.
		const double alpha = 80 * (1.0 - h.z / Config::BG2::Z_MAX) * modeAlpha;

		DrawMode::Exe(
			DrawModeID::None, DrawBlendModeID::Alpha, _int(alpha),
			[&]()
			{
				DrawPolygonKR(poly, true, true);
			}
		);
	}
}

//描画(星)
void BG2::DrawStars(double modeAlpha, bool isReflect)
{
	//画面中央.
	const DBL_XY center = App::GetWindowRect().GetMid().ToDbl();

	for (auto& i : star)
	{
		//奥行きに応じた拡大率.
		const double scale = Config::BG2::FOV / i.z;
		//奥ほど小さく、手前ほど大きくする.
		const double size = 0.3 * scale;
		//奥ほど透明にする.
		const double alpha = 128 * (1.0 - i.z / Config::BG2::Z_MAX) * modeAlpha;

		//座標.
		const DBL_XY pos = {
			center.x + i.x * scale,
			center.y + i.y * scale
		};

		//画面外なら描画しない.
		if (pos.x < -100 || pos.x > Config::WINDOW_WID + 100) { continue; }
		if (pos.y < -100 || pos.y > Config::WINDOW_HEI + 100) { continue; }

		DrawMode::Exe(
			DrawModeID::None, DrawBlendModeID::Add, _int(alpha),
			[&]()
			{
				//反射モードになったら変色.
				if (isReflect) {
					grBgStarRef->DrawExtend(
						pos, { size, size }, Anchor::Mid, true, true
					);
				}
				else {
					grBgStarNor->DrawExtend(
						pos, { size, size }, Anchor::Mid, true, true
					);
				}
			}
		);
	}
}

//ポーズする.
void BG2::Pause() {
}

//ポーズ解除.
void BG2::PauseEnd() {
}

//出現位置の再抽選(線・星の共通処理).
void BG2::Respawn(WarpPos& obj, double rMin, double z) {
	RandSpawnXY(obj.x, obj.y, rMin);
	obj.z = z;
}