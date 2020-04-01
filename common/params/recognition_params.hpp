#pragma once

namespace Params
{

namespace CircleDetection
{

constexpr int SATURATION_LOWER_LIMIT = 65;  //saturation（彩度）の下限
constexpr int VALUE_LOWER_LIMIT = 50;       //value（明度）の下限

// parameters for the blue ball

constexpr int blue = 0;
constexpr int red = 1;
constexpr int yellow = 2;

constexpr int HUE_LOW[] = {95, 162, 22};
constexpr int HUE_UPP[] = {105, 180, 32};

constexpr double CIRCULARITY_THRESHOLD = 0.0;

}  // namespace CircleDetection

}  // namespace Params
