#pragma once

#include "unit.hpp"

#include <opencv/cv.hpp>

#include <array>

namespace Control
{

namespace Localization
{
double bilinear(cv::Mat depth, int x_rgb, int y_rgb);
std::array<Length, 2> get2Dpos(cv::Mat rgb, cv::Mat depth, Length z);


}  // namespace Localization

}  // namespace Control
