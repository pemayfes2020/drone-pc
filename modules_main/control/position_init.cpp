#include "params/control_params.hpp"
#include "unit.hpp"

#include <Eigen/Core>
#include <array>
#include <opencv/cv.hpp>


namespace PositionInit
{
Eigen::Vector3d InitDronePosition(cv::Mat rgb, cv::Mat depth)
{
    //2つのballの(x,y,z(既知))を求める

    //  Eigen::Vector2i circle_pos = 0;

    //
    return Eigen::Vector3d{0.0, 0.0, 0.0};
}

}  // namespace PositionInit
