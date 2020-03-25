#pragma once

namespace Params
{

namespace CircleDetection
{

constexpr int SATURATION_LOWER_LIMIT = 65;  //saturation（彩度）の下限
constexpr int VALUE_LOWER_LIMIT = 50;       //value（明度）の下限

// parameters for the blue ball
constexpr int HUE_LOWER_LIMIT = 95;
constexpr int HUE_UPPER_LIMIT = 105;

constexpr double CIRCULARITY_THRESHOLD = 0.0;

}  // namespace CircleDetection

}  // namespace Params
