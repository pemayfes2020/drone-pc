#include "localization.hpp"

namespace Control
{

namespace Localization
{

std::array<Length, 2> get2Dpos(cv::Mat rgb, cv::Mat depth, Length z)
{
    // TODO RGB画像とDepth画像、既知のzからxy平面内の座標を推定する

    return std::array<Length, 2>{0.0_mm, 0.0_mm};
}

}  // namespace Localization

}  // namespace Control
