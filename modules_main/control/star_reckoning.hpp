#pragma once

#include "unit.hpp"

#include <opencv/cv.hpp>

#include <array>

namespace Control
{

namespace Localization
{
struct depst {
    double val;
    int rel;
};

double bilinear(cv::Mat depth, int x_rgb, int y_rgb);
std::array<Length, 2> get2Dpos(cv::Mat image_rgb, cv::Mat image_depth, Length z, depst& depth);


}  // namespace Localization

}  // namespace Control
