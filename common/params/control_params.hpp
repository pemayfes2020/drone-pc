#pragma once

#include "unit.hpp"
#include <Eigen/Core>
namespace Params
{
// region PID value {
inline constexpr double Kp_x = 0.0;
inline constexpr double Ki_x = 0.0;
inline constexpr double Kd_x = 0.0;

inline constexpr double Kp_y = 0.0;
inline constexpr double Ki_y = 0.0;
inline constexpr double Kd_y = 0.0;

inline constexpr double Kp_z = 0.0;
inline constexpr double Ki_z = 0.0;
inline constexpr double Kd_z = 0.0;
// } region PID value


// region Map params {

inline const Eigen::Vector3d kinect_pos{0.0, 0.0, 0.0};
inline const Eigen::Vector3d kinect_rot{0.0, 0.0, 0.0};


// }

}  // namespace Params
