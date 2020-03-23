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
    double x = x_rgb * width_depth / double(width);
    double y = y_rgb * height_depth / double(height);

    return (f(x) + 1 - x) * ((f(y) + 1 - y) * depth.at<float>(f(x), f(y)) + (y - f(y)) * depth.at<float>(f(x), f(y) + 1)) + (x - f(x)) * ((f(y) + 1 - y) * depth.at<float>(f(x) + 1, f(y)) + (y - f(y)) * depth.at<float>(f(x) + 1, f(y) + 1));
}

std::array<Length, 2> get2Dpos(cv::Mat rgb, cv::Mat depth, Length z)
{
    // TODO RGB画像とDepth画像、既知のzからxy平面内の座標を推定する

    //impl/circle.cpp内のdetectCircleから検出した円のkinect画面内座標を受け取る。
    auto [x, y] = circleSpace::detectCircle(rgb, 1);

    //中心
    int center_x = (width - 1) / 2;
    int center_y = (height - 1) / 2;

    double theta = vertical * (y - center_y) / height;
    double phi = horizontal * (x - center_x) / width;

    double dp = bilinear(depth, x, y);  //中心8近傍の平均?

    Eigen::Matrix<double, 3, 1> ang{1.0, -1.0 * tan(phi), -1.0 * tan(theta) / cos(phi)};

    Eigen::Matrix<double, 3, 1> vec_to_drone = dp * ang + kinect_r;

    std::cout << vec_to_drone(0) << ' ' << vec_to_drone(1) << ' ' << vec_to_drone(2) << std::endl;

    return std::array<Length, 2>{0.0_mm, 0.0_mm};
}

}  // namespace Localization

}  // namespace Control
