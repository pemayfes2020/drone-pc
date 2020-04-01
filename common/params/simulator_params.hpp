#pragma once

#include "unit.hpp"

#include <cmath>

namespace Params
{

namespace Simulator
{

inline constexpr int update_division = 10;

inline Vel takeoff_vz = 0.5_m / 1.0_s;
inline Length takeoff_z_threshold = 1.0_m;
inline Angular takeoff_tlit = M_PI / 4.0 * 1.0_rad;

inline Time vz_time_constant = 5.0_ms;   // z方向速度時定数
inline Time vxy_time_constant = 1.0_ms;  // xy平面内速度時定数
inline Time vr_time_constant = 0.1_ms;   // yaw速度時定数

}  // namespace Simulator

}  // namespace Params
