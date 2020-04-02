#pragma once

#include "unit.hpp"

#include <Eigen/Core>

#include <cmath>

namespace Params::Environment
{

inline const Eigen::Vector3d initial_pos{0.0, 0.0, 500.0};
inline const Eigen::Vector3d initial_rot{0.0, 0.0, 0.5 * M_PI};

inline const Eigen::Vector3d kinect_pos{0.0, 0.0, 0.0};
inline const Eigen::Vector3d kinect_rot{0.0, 0.0, 0.0};


inline const Length ball_offset = 80.0_mm;
inline const Length ball_radius = 40.0_mm;

inline const Length wall_height = 4000.0_mm;
inline const Length wall_width = 6000.0_mm;

}  // namespace Params::Environment
