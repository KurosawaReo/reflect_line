/*
   - Config_BG2.h -
*/
#pragma once

//[設定]
namespace Config {
	//[背景2]
	namespace BG2 {

		//出現範囲(画面中心に向かう軸からの距離)
		static constexpr double SPAWN_R_MAX		= 2000.0;	//一番外側(線・星 共通).
		static constexpr double POINT_R_MIN		= 0.0;		//線: 0なら今と同じく中心にも出る.
		static constexpr double STAR_R_MIN		= 600.0;	//星: 中心を避ける距離.

		static constexpr double MOVE_SPEED		= 4.0;		//前に移動する速度.
		static constexpr double ROT_SPEED		= 0.001;	//回転する速度.
		static constexpr double POINT_LEN		= 18.0;		//飛んでくる線の長さ.
		static constexpr double Z_MIN			= 50.0;		//一番手前のz座標.
		static constexpr double Z_MAX			= 3000.0;	//一番奥のz座標.
		static constexpr double HEX_INTERVAL	= 180.0;
		static constexpr double FOV				= 500.0;
		static constexpr double FOV_HEX			= 5000.0;
	}
}