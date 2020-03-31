#pragma once

#include "unit.hpp"

#include <opencv/cv.hpp>

#include <array>


namespace PositionInit
{

Eigen::Vector3d InitDronePosition(cv::Mat rgb, cv::Mat depth);

}
