#include "control/impl/circle_detection.hpp"
#include "control/localization.hpp"

#include "params/control_params.hpp"
#include "params/kinect_params.hpp"

#include <Eigen/Core>

#include <iostream>

using namespace Params::Kinect;

constexpr double width_ratio = (double)Depth::width / RGB::width;
constexpr double height_ratio = (double)Depth::height / RGB::height;

//参考
//https://www.jstage.jst.go.jp/article/isciesci/58/8/58_KJ00009469648/_pdf/-char/ja

namespace Control::Localization
{

double bilinear(cv::Mat depth, int x_rgb, int y_rgb)
{
    double x = x_rgb * width_ratio;
    double y = y_rgb * height_ratio;
    //補間https://imagingsolution.blog.fc2.com/blog-entry-142.html

    double _x = std::floor(x);
    double _y = std::floor(y);

    Eigen::Matrix<double, 1, 2> A;
    Eigen::Matrix2d B;
    Eigen::Vector2d C;
    A << _y + 1 - y, y - _y;
    try {
        B << depth.at<float>(_x, _y), depth.at<float>(_x + 1, _y),
            depth.at<float>(_x, _y + 1), depth.at<float>(_x + 1, _y);
    } catch (...) {
        std::cerr << "out of bound error" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    C << _x + 1 - x, x - _x;

    return A * B * C;
}

std::array<Length, 2> get2Dpos(cv::Mat image_rgb, cv::Mat image_depth, Length z, depst& depth)
{
    // TODO RGB画像とDepth画像、既知のzからxy平面内の座標を推定する

    //impl/circle.cpp内のdetectCircleから検出した円のkinect画面内座標を受け取る。
    Eigen::Vector2i circle_pos = StarReckoning::detectCircle(image_rgb, Params::CircleDetection::blue, 1);

    //中心
    int center_x = (RGB::width - 1) / 2;
    int center_y = (RGB::height - 1) / 2;

    double theta = ViewAngle::vertical * (circle_pos(1) - center_y) / RGB::height;
    double phi = ViewAngle::horizontal * (circle_pos(0) - center_x) / RGB::width;

    double _depth = bilinear(image_depth, circle_pos(0), circle_pos(1));

    Eigen::Vector3d angle{
        1.0,
        -tan(phi),
        -tan(theta) / cos(phi)};


    if (_depth) {
        depth.val = _depth;
        depth.rel = 1;
    } else {
        depth.rel = 0;
    }

    Eigen::Vector3d vec_to_drone = depth.val * angle + Params::kinect_pos;

    std::cout << "depth: " << depth.val << std::endl;
    std::cout << "Drone Pos(x, y, z): " << std::endl
              << vec_to_drone << std::endl;


    return std::array<Length, 2>{0.0_mm, 0.0_mm};
}

}  // namespace Control::Localization
