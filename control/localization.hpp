#pragma once

#include "unit.hpp"

#include <opencv/cv.hpp>

#include <array>

namespace Control
{

namespace Localization
{

std::array<Length, 2> estimatePosWithKinect(cv::Mat rgb, cv::Mat depth, Length z);


}  // namespace Localization

}  // namespace Control
