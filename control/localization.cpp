#include "localization.hpp"
#include "circle.hpp"

namespace Control
{

namespace Localization
{

std::array<Length, 2> get2Dpos(cv::Mat rgb, cv::Mat depth, Length z)
{
    // TODO RGB画像とDepth画像、既知のzからxy平面内の座標を推定する

    //impl/circle.cpp内のdetectCircleから検出した円のkinect画面内座標を受け取る。
    auto [x,y] = circleSpace::detectCircle(rgb,1);

    int center_x = rgb.rows / 2; 
    int center_y = rgb.cols / 2; 


    return std::array<Length, 2>{0.0_mm, 0.0_mm};
}

}  // namespace Localization

}  // namespace Control
