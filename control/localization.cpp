#include "localization.hpp"
#include "circle.hpp"
#include <Eigen/Core>
#include <iostream>
//rgb画像の大きさ
constexpr int width = 1920;
constexpr int height = 1080;

//depth画像の大きさ
constexpr int width_depth = 512;
constexpr int height_depth = 424;

//    視野角について
//    http://neareal.com/629/
//    (水平)84.1 度 (垂直) 53.8 度
//    らしい
//
constexpr double horizontal = 84.1 * M_PI / 180.0;
constexpr double vertical = 53.8 * M_PI / 180.0;

Eigen::Matrix<double, 3, 1> kinect_r = {0, 0, 0};

#define f(a) std::floor(a)
//参考
//https://www.jstage.jst.go.jp/article/isciesci/58/8/58_KJ00009469648/_pdf/-char/ja

namespace Control
{

namespace Localization
{
double bilinear(cv::Mat depth, int x_rgb, int y_rgb)
{
    //補間https://imagingsolution.blog.fc2.com/blog-entry-142.html

    double x = x_rgb * width_depth / double(width);
    double y = y_rgb * height_depth / double(height);

    double _x = std::floor(x);
    double _y = std::floor(y);

    using dep = depth.at<float>;

    Eigen::Matrix<double, 1, 2> A;
    Eigen::Matrix2d B;
    Eigen::Vector2d C;
    A << _y + 1 - y, y - _y;
    B << dep(_x, _y), dep(_x + 1, _y),
        dep(_x, _y + 1), dep(_x + 1, _y + 1);

    C << _x + 1 - x, x - _x;

    return A * B * C;
}

std::array<Length, 2> get2Dpos(cv::Mat image_rgb, cv::Mat image_depth, Length z)
{
    // TODO RGB画像とDepth画像、既知のzからxy平面内の座標を推定する

    //impl/circle.cpp内のdetectCircleから検出した円のkinect画面内座標を受け取る。
    auto [x, y] = circleSpace::detectCircle(image_rgb, 1);

    //中心
    int center_x = (width - 1) / 2;
    int center_y = (height - 1) / 2;

    double theta = vertical * (y - center_y) / height;
    double phi = horizontal * (x - center_x) / width;

    double depth = bilinear(image_depth, x, y);  //中心8近傍の平均?

    Eigen::Vector3d angle{
        1.0,
        -tan(phi),
        -tan(theta) / cos(phi)};

    Eigen::Vector3d vec_to_drone = depth * angle + kinect_r;

    std::cout << "Drone Pos(x, y, z): " << vec_to_drone << std::endl;

    return std::array<Length, 2>{0.0_mm, 0.0_mm};
}

}  // namespace Localization

}  // namespace Control
